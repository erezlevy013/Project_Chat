#ifndef __CLIENTNET_H__
#define __CLIENTNET_H__

#include <sys/types.h> /* ssize_t */

typedef enum ClientResult
{
    CLIENT_SUCCESS = 0,
    CLIENT_INITIZLIZATION_ERROR,
    CLIENT_INVALID_SOCK,
    CLIENT_INVALID_DATA,
    CLIENT_SEND_FAILED,
    CLIENT_RECV_FAILED,
    CLIENT_CONNECTION_FAILED
} ClientResult;

/**
 * @brief create a new socket for client and connect the socket to the server.
 * @retval new socket value, error on failure.
 * @returns CLIENT_INVALID_SOCK if socket() failed.
 * @returns CLIENT_CONNECTION_FAILED if the connection to server failed.
 */
int CreateClient(void);

/**
 * @brief sends a message from user to server
 *
 * @param _sock : the client's socket the meassage will sent  through.
 * @param _data : the message that will be sent.
 * @param _dataLength : length of message that will be sent.
 * @retval CLIENT_SUCCESS on success, error on failure.
 * @returns CLIENT_INVALID_SOCK if sock value is bigger then 1023 or smaller then 3.
 * @returns CLIENT_INITIZLIZATION_ERROR if _data has no value.
 * @returns CLIENT_SEND_FAILED if send() fails.
 */
ClientResult SendToServer(int _sock, char _data[], size_t _dataLength);

/**
 * @brief recieve a message from server.
 * @param _sock : the client's socket the meassage will recieve through.
 * @param _buffer : the buffer that holds the meassage will recieve from server
 * @retval length of the meassage that was recieved from server, 0 or -1 if error occoure.
 * @retval -1 if _sock value is invalid, buffer is NULL or recv() failed.
 * @retval 0 if socket was cllosed by server.
 */
ssize_t GotServerMsg(int _sock, char _buffer[]);

/**
 * @brief close a socket.
 * @param _sock : the client's socket that should be closed.
 * @retval return if socket value is from 0 to 2, that should not allowed to be closed.
 */
void CloseClient(int _sock);

#endif /* __CLIENTNET_H__*/