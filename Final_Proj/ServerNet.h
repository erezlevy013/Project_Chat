#ifndef __SERVERNET_H__
#define __SERVERNET_H__

#include <sys/types.h> /* ssize_t */
#include <unistd.h> /* ssize_t */

typedef enum
{
	SERVER_SUCCESS = 0,
	SERVER_ERR_NOT_INITIALIZED,
    SERVER_ERR_INVALID_PORT,
    SERVER_ERR_INVALID_BACKLOG,
    SERVER_ERR_INVALID_SOCKET,
    SERVER_ERR_INVALID_DATALEN,
	SERVER_ERR_SOCKET_FAILED,
	SERVER_ERR_SETSOCKET_FAILED,
	SERVER_ERR_BIND_FAILED,
	SERVER_ERR_LISTEN_FAILED,
	SERVER_ERR_ACCEPT_FAILED,
    SERVER_ERR_ALLOCATION_FAILED,
	SERVER_ERR_LIST_ALLOCATION_FAILED,
	SERVER_ERR_MAX_CLIENTS_REACHED,
	SERVER_ERR_SOCKET_CLOSED,
	SERVER_ERR_SELECT_FAILED,
    SERVER_ERR_RECV_FAILED,
    SERVER_ERR_SEND_FAILED
} ServerResult;

typedef struct Server Server;

/** 
 * @brief Application function to be called when a new client joins the server
 *
 * @param _clientID : the ID of the new client
 */
typedef int (*NewClientFunc)(int _clientID);

/** 
 * @brief Application function to be called when a client message was recieved
 *
 * @param _clientID : the ID of the client who sent the message
 * @param _buffer : the message of the client
 * @param _bufferLen : the length of the message of the client
 * @param _context : Application context to be used (can be NULL)
 * @return 1 on success, 0 otherwise
 */
typedef int (*ClientMsgFunc)(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context);

/** 
 * @brief Application function to be called when a client disconnected from the server
 *
 * @param _clientID : the ID of the client who disconnected
 */
typedef void (*CloseClientFunc)(int _clientID);

/** 
 * @brief Application function to be called when an error occurred
 *
 * @param _serverError : the enum error that occurred
 * @param _clientID : the client ID of the client that experienced errors
 * @warning _clientID can be -1 if it is not relevant to the error
 */
typedef int (*FailFunc)(ServerResult _error, int _clientID);

/** 
 * @brief Creates a server
 * 
 * @param _portNum : The port number to which the server will be binded
 * @param _backLog : The maximum number of clients that can wait in the accept queue
 * @param _newClientFunc : Pointer to an application function to be called when a new client joins the server (can be NULL)
 * @param _clientMsgFunc : Pointer to an a aplication function to be called when a client message was recieved (cannot be NULL)
 * @param _closeClientFunc : Pointer to an application function to be called when a client disconnected (can be NULL)
 * @param _clientFailFunc : Pointer to an application function to be called when an error occured (can be NULL)
 * @param _context : Pointer to context to be used upon calling _clientMsgFunc (can be NULL)
 * @param _status : will contain SERVER_SUCCESS if the function was successful, or the enum error upon failure (cannot be NULL)
 * @returns a pointer to the created server.
 * @retval NULL on failure, _status will contain the relevant error (SERVER_ERR_NOT_INITIALIZED, SERVER_ERR_INVALID_PORT, SERVER_ERR_INVALID_BACKLOG,
 *                                                                   SERVER_ERR_SOCKET_FAILED, SERVER_ERR_SETSOCKET_FAILED, SERVER_ERR_BIND_FAILED,
 *                                                                   SERVER_ERR_LISTEN_FAILED, SERVER_ERR_ALLOCATION_FAILED, SERVER_ERR_LIST_ALLOCATION_FAILED)
 */
Server* CreateServer(int _portNum, int _backLog, NewClientFunc _newClientFunc, ClientMsgFunc _clientMsgFunc, CloseClientFunc _closeClientFunc, FailFunc _clientFailFunc, void * _context, ServerResult *_status);

/** 
 * @brief Runs the given server
 *
 * @param _server : A pointer to previously created server returned via CreateServer
 * @returns SERVER_SUCCESS on success, errors on failure
 * @retval SERVER_ERR_NOT_INITIALIZED if server pointer is NULL
 * @retval SERVER_ERR_SELECT_FAILED if select failed
 */
ServerResult RunServer(Server *_server);

/** 
 * @brief Sends a message to the given client
 *
 * @param _clientID : The ID of the client to which the message will be sent
 * @param _data : The message to be sent
 * @param _dataLen : The length of the message to be sent
 * @param _server : A pointer to previously created server returned via CreateServer
 * @returns SERVER_SUCCESS on success, FAIL on failure, will print the reason for it
 */
ServerResult SendToClient(int _clientID, char _data[], size_t _dataLen, Server *_server);

/** 
 * @brief Stops the given running server
 *
 * @param _server : A pointer to previously created server returned via CreateServer
 * @returns SERVER_SUCCES on success, SERVER_ERR_NOT_INITIALIZED if given pointer was NULL
 */
ServerResult StopServer(Server *_server);

/** 
 * @brief Destroys server
 * @details Destroys the server completely 
 *
 * @param _server : A pointer to previously created server returned via CreateServer
 */
void DestroyServer(Server *_server);

#endif /* __SERVERNET_H__ */