#ifndef __LISTENER_H__
#define __LISTENER_H__

/**
 * @brief opens a new window where group chat is display,and create a multicasting UDP to each group. In addition, save the new window PID to file.
 * @param _IP : the IP address of the group, recieved from server.
 * @param _port : port num of the group,  recieved from server.
 * @returns Fail if return to recieve the message from user, 0 if load PID to file failed, or SUCCESS if successful.
 */
int Listener(char _IP[], int _port);

#endif /* __LISTENER_H__*/
