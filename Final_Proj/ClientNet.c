#include <netinet/in.h> /* sockaddr_in, sockaddr */
#include <string.h> /* strcpy, strcat */
#include <unistd.h> /* close */
#include <arpa/inet.h> /* inet_addr */
#include <sys/types.h> /* ssize_t */
#include <sys/socket.h> /* recv, send, connect, socket */
#include "Limits.h"
#include "ClientNet.h"

#define MIN_SOCK_LIMIT 3
#define MAX_SOCK_LIMIT 1024
#define SOCKET_CLOSED 0

static int Connection(int _sock);

static ClientResult SendParameterCheck(int _sock, char _data[], size_t _dataLength);

static ClientResult RecvParameterCheck(int _sock, char _buffer[]);

int CreateClient(void)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        return CLIENT_INVALID_SOCK;
    }
    if (Connection(sock) == CLIENT_CONNECTION_FAILED)
    {
        return FAIL;
    }
    return sock;
}

ClientResult SendToServer(int _sock, char _data[], size_t _dataLength)
{
    ClientResult result;
    ssize_t sentBytes;
    if ((result = SendParameterCheck(_sock, _data, _dataLength)) != CLIENT_SUCCESS)
    {
        return result;
    }
    sentBytes = send(_sock, _data, _dataLength, 0);
    if (sentBytes < 0)
    {
        close(_sock);
        return CLIENT_SEND_FAILED;
    }
    return CLIENT_SUCCESS;
}

ssize_t GotServerMsg(int _sock, char _buffer[])
{
    ssize_t readBytes;
    ClientResult result;
    if ((result = RecvParameterCheck(_sock, _buffer)) != CLIENT_SUCCESS)
    {
        return FAIL;
    }
    readBytes = recv(_sock, _buffer, BUFFER_SIZE_LIMIT, 0);
    if (readBytes < 0)
    {
        close(_sock);
        return FAIL;
    }
    else if (readBytes == 0)
    {
        close(_sock);
        return SOCKET_CLOSED;
    }
    return readBytes;
}

void CloseClient(int _sock)
{
    if (_sock < MIN_SOCK_LIMIT || _sock > MAX_SOCK_LIMIT)
    {
        return;
    }
    close(_sock);
}

static int Connection(int _sock)
{
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(SERVER_IP);
    sin.sin_port = htons(SERVER_PORT_NUM);
    if (connect(_sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        close(_sock);
        return CLIENT_CONNECTION_FAILED;
    }
    return CLIENT_SUCCESS;
}

static ClientResult SendParameterCheck(int _sock, char _data[], size_t _dataLength)
{

    if (_sock < MIN_SOCK_LIMIT || _sock > MAX_SOCK_LIMIT)
    {
        return CLIENT_INVALID_SOCK;
    }
    if (_data == NULL)
    {
        return CLIENT_INITIZLIZATION_ERROR;
    }

    if (_dataLength == 0)
    {
        return CLIENT_INVALID_DATA;
    }
    return CLIENT_SUCCESS;
}

static ClientResult RecvParameterCheck(int _sock, char _buffer[])
{

    if (_sock < MIN_SOCK_LIMIT || _sock > MAX_SOCK_LIMIT)
    {
        return CLIENT_INVALID_SOCK;
    }
    if (_buffer == NULL)
    {
        return CLIENT_INITIZLIZATION_ERROR;
    }
    return CLIENT_SUCCESS;
}
