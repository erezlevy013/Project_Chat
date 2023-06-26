#include <stdlib.h> /* MALLOC, FREE */
#include <unistd.h> /* close */
#include <netinet/in.h> /* sockaddr_in, socket, setsocketopt, listen, bind */
#include <fcntl.h> /* fcntl */
#include <string.h> /* strncpy */
#include <sys/select.h> /* fd_set */
#include <sys/types.h> /* ssize_t */
#include "GenList.h"
#include "ListItr.h"
#include "ListFunc.h"
#include "ServerNet.h"
#include "Limits.h"
#define MAGIC_NUM 111475
#define MIN_PORT_NUM 1024
#define MAX_PORT_NUM 65535
#define CLIENTS_LIMIT 1000
#define MAX_SELECT_CLIENTS 1024
#define MIN_CLIENT_SOCKET 3
#define MAX_CLIENT_SOCKET 1023

static ServerResult CreateCheckParameters(int _portNum, int _backLog, ClientMsgFunc _clientMsgFunc);

static int CreateServerSocket(int _portNum, int _backLog, ServerResult *_status);

static int* CreateClient(int _clientSocket);

static ServerResult ConnectWithNewClient(Server *_server);

static ServerResult ClientsForEach(Server *_server);

static ServerResult CheckCurrentClient(int _clientSocket, Server *_server);

static ServerResult SendCheckParameters(int _cliendID, void *_data, size_t _dataLen);

void DestroyClientSocket(void *_clientSocket);

struct Server
{
    int m_listeningSocket;
    List* m_list;
    int m_numOfClients;
    fd_set m_masterClientSockets;
    fd_set m_clientSocketsInNeed;
    int m_clientActivity;
    NewClientFunc m_newClientFunction;
    ClientMsgFunc m_clientMsgFunction;
    CloseClientFunc m_closeClientFunction;
    FailFunc m_failFunction;
    void *m_applicationContext;
    int m_stopFlag;
    int m_magicNum;
};

Server* CreateServer(int _portNum, int _backLog, NewClientFunc _newClientFunc, ClientMsgFunc _clientMsgFunc, CloseClientFunc _closeClientFunc, FailFunc _failFunc, void *_context, ServerResult *_status)
{
    Server *pServer = NULL;
    List *pList = NULL;
	int serverSocket;
    ServerResult result;
    if(_status == NULL)
    {
        return NULL;
    }
    result = CreateCheckParameters(_portNum, _backLog, _clientMsgFunc);
    if(result != SERVER_SUCCESS)
    {
        *_status = result;
        return NULL;
    }
    serverSocket = CreateServerSocket(_portNum, _backLog, _status);
    if(serverSocket == FAIL)
    {
        return NULL;
    }
    pServer = (Server*)malloc(sizeof(Server));
    if(pServer == NULL)
    {
        close(serverSocket);
        *_status = SERVER_ERR_ALLOCATION_FAILED;
        return NULL;
    }
    pList = ListCreate();
    if(pList == NULL)
    {
        close(serverSocket);
        free(pServer);
        *_status = SERVER_ERR_LIST_ALLOCATION_FAILED;
        return NULL;
    }
    pServer->m_listeningSocket = serverSocket;
    pServer->m_list = pList;
    pServer->m_numOfClients = 0;
    FD_ZERO(&pServer->m_masterClientSockets);
    FD_SET(serverSocket, &pServer->m_masterClientSockets);
    pServer->m_newClientFunction = _newClientFunc;
    pServer->m_clientMsgFunction = _clientMsgFunc;
    pServer->m_closeClientFunction = _closeClientFunc;
    pServer->m_failFunction = _failFunc;
    pServer->m_applicationContext = _context;
    pServer->m_stopFlag = 0;
    pServer->m_magicNum = MAGIC_NUM;
    *_status = SERVER_SUCCESS;
	return pServer;
}

ServerResult RunServer(Server *_server)
{
    int clientsActivity;
    ServerResult result;
    if(_server == NULL)
    {
        return SERVER_ERR_NOT_INITIALIZED;
    }
    while(!_server->m_stopFlag)
    {
        _server->m_clientSocketsInNeed = _server->m_masterClientSockets;
        clientsActivity = select(MAX_SELECT_CLIENTS, &_server->m_clientSocketsInNeed, NULL, NULL, NULL);
        if(clientsActivity < 0)
        {
            if(_server->m_failFunction != NULL)
                {
                    result = SERVER_ERR_SELECT_FAILED;
                    (*_server->m_failFunction)(result, -1);
                }
            return SERVER_ERR_SELECT_FAILED;
        }
        else if(clientsActivity > 0)
        {
            _server->m_clientActivity = clientsActivity;
            if(FD_ISSET(_server->m_listeningSocket, &_server->m_clientSocketsInNeed))
            {
                result = ConnectWithNewClient(_server);
                if(result != SERVER_SUCCESS && _server->m_failFunction != NULL)
                {
                    (*_server->m_failFunction)(result, -1);
                }
                if(result == SERVER_ERR_ACCEPT_FAILED)
                {
                    return SERVER_ERR_ACCEPT_FAILED;
                }
                _server->m_clientActivity -= 1;
            }
            if(_server->m_clientActivity > 0)
            {
                ClientsForEach(_server);
            }
        }
    }
    return SERVER_SUCCESS;
}

ServerResult SendToClient(int _clientID, char _data[], size_t _dataLen, Server *_server)
{
	ServerResult result;
    ssize_t sentBytes;
    result = SendCheckParameters(_clientID, _data, _dataLen);
    if(result != SERVER_SUCCESS)
    {
        return result;
    }
	sentBytes = send(_clientID, _data, _dataLen, 0);
	if(sentBytes < 0)
	{
        /* RemoveClient(_server, _clientID) */
        return SERVER_ERR_SEND_FAILED;
	}
	return SERVER_SUCCESS;
}

ServerResult StopServer(Server *_server)
{
    if(_server == NULL)
    {
        return SERVER_ERR_NOT_INITIALIZED;
    }
    _server->m_stopFlag = 1;
    return SERVER_SUCCESS;
}

void DestroyServer(Server *_server)
{
    List *pList = NULL;
    if(_server != NULL && _server->m_magicNum == MAGIC_NUM)
    {
        _server->m_magicNum = 0;
        pList = _server->m_list;
        ListDestroy(&pList, DestroyClientSocket);
        close(_server->m_listeningSocket);
        free(_server);
    }
}

static ServerResult CreateCheckParameters(int _portNum, int _backLog, ClientMsgFunc _clientMsgFunc)
{
    if(_clientMsgFunc == NULL)
    {
        return SERVER_ERR_NOT_INITIALIZED;
    }
    if(_portNum < MIN_PORT_NUM || _portNum > MAX_PORT_NUM)
    {
        return SERVER_ERR_INVALID_PORT;
    }
    if(_backLog <= 0 || _backLog > BACKLOG_LIMIT)
    {
        return SERVER_ERR_INVALID_BACKLOG;
    }
    return SERVER_SUCCESS;
}

static int CreateServerSocket(int _portNum, int _backLog, ServerResult *_status)
{
    struct sockaddr_in serverSin;
    int result, serverSocket = socket(AF_INET, SOCK_STREAM, 0), optVal = 1;
	if(serverSocket < 0)
	{
		*_status = SERVER_ERR_SOCKET_FAILED;
        return FAIL;
	}
    result = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));
    if(result < 0)
    {
        close(serverSocket);
        *_status = SERVER_ERR_SETSOCKET_FAILED;
        return FAIL;
    }
	memset(&serverSin, 0, sizeof(serverSin));
	serverSin.sin_family = AF_INET;
	serverSin.sin_addr.s_addr = INADDR_ANY;
	serverSin.sin_port = htons(_portNum);
	result = bind(serverSocket, (struct sockaddr *)&serverSin, sizeof(serverSin));
	if(result < 0)
	{
        close(serverSocket);
        *_status = SERVER_ERR_BIND_FAILED;
		return FAIL;
	}
    result = listen(serverSocket, _backLog);
    if(result < 0)
    {
        close(serverSocket);
        *_status = SERVER_ERR_LISTEN_FAILED;
        return FAIL;
    }
    return serverSocket;
}

static int* CreateClient(int _clientSocket)
{
    int *pClient = NULL;
    pClient = (int*)malloc(sizeof(int));
    if(pClient == NULL)
    {
        return NULL;
    }
    *pClient = _clientSocket;
    return pClient;
}

static ServerResult ConnectWithNewClient(Server *_server)
{
    struct sockaddr_in clientSin;
    int *pClient = NULL;
    unsigned int addrLen = sizeof(clientSin);
    int clientSocket;
    ListResult listResult;
    if(_server == NULL)
    {
        return SERVER_ERR_NOT_INITIALIZED;
    }
    clientSocket = accept(_server->m_listeningSocket, (struct sockaddr *)&clientSin, &addrLen);
    if(clientSocket < 0)
    {
        return SERVER_ERR_ACCEPT_FAILED;
    }
    if(_server->m_numOfClients >= CLIENTS_LIMIT)
    {
        close(clientSocket);
        return SERVER_ERR_MAX_CLIENTS_REACHED;
    }
    pClient = CreateClient(clientSocket);
    if(pClient == NULL)
    {
        close(clientSocket);
        return SERVER_ERR_ALLOCATION_FAILED;
    }
    listResult = ListPushHead(_server->m_list, pClient);
    if(listResult != LIST_SUCCESS)
    {
        close(clientSocket);
        free(pClient);
        return SERVER_ERR_LIST_ALLOCATION_FAILED;
    }
    _server->m_numOfClients += 1;
    FD_SET(clientSocket, &_server->m_masterClientSockets);
    return SERVER_SUCCESS;
}

static ServerResult ClientsForEach(Server *_server)
{
    ListItr pNode = NULL, pEnd = NULL, pNodeToRemove = NULL;
    int *pClientSocket = NULL;
    ServerResult result;
    pNode = ListItrBegin(_server->m_list);
    pEnd = ListItrEnd(_server->m_list);
    while(pNode != pEnd && _server->m_clientActivity > 0)
    {
        pClientSocket = (int*)ListItrGet(pNode);
        if(FD_ISSET(*pClientSocket, &_server->m_clientSocketsInNeed))
        {
            _server->m_clientActivity -= 1;
            result = CheckCurrentClient(*pClientSocket, _server);
            if(result != SERVER_SUCCESS)
            {
                close(*pClientSocket);
                if(_server->m_closeClientFunction != NULL)
                {
                    (*_server->m_closeClientFunction)(*pClientSocket);
                }
                if(_server->m_failFunction != NULL && result != SERVER_ERR_SOCKET_CLOSED)
                {
                    (*_server->m_failFunction)(result, *pClientSocket);
                }
                FD_CLR(*pClientSocket, &_server->m_masterClientSockets);
                free(pClientSocket);
                pNodeToRemove = pNode;
                pNode = ListItrNext(pNode);
                ListItrRemove(pNodeToRemove);
                _server->m_numOfClients -= 1;
                continue;
            }
        }
        pNode = ListItrNext(pNode);
    }
    return SERVER_SUCCESS;
}

static ServerResult CheckCurrentClient(int _clientSocket, Server *_server)
{
	char buffer[BUFFER_SIZE_LIMIT];
    ssize_t readBytes;
    readBytes = recv(_clientSocket, buffer, sizeof(buffer), 0);
	if(readBytes < 0)
	{
        return SERVER_ERR_RECV_FAILED;
	}
    else if(readBytes == 0)
        {
            return SERVER_ERR_SOCKET_CLOSED;
        }
    (*_server->m_clientMsgFunction)(_clientSocket, buffer, readBytes, _server->m_applicationContext);
    return SERVER_SUCCESS;
}

static ServerResult SendCheckParameters(int _cliendID, void *_data, size_t _dataLen)
{
    if(_data == NULL)
    {
        return SERVER_ERR_NOT_INITIALIZED;
    }
    if(_cliendID < MIN_CLIENT_SOCKET || _cliendID > MAX_CLIENT_SOCKET)
    {
        return SERVER_ERR_INVALID_SOCKET;
    }
    if(_dataLen == 0)
    {
        return SERVER_ERR_INVALID_DATALEN;
    }
    return SERVER_SUCCESS;
}

void DestroyClientSocket(void *_clientSocket)
{
    close(*(int*)_clientSocket);
    free(_clientSocket);
}