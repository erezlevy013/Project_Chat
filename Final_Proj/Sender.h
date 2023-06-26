#ifndef __SENDER_H__
#define __SENDER_H__

/**
 * @brief opens a new window that used as a keyBoard, to each group chat. In addition, save the new window PID to file.
 * @param _IP : the IP address of the group, recieved from server.
 * @param _port : port num of the group,  recieved from server.
 * @param _nickkName : username that user chhose ehwn registered to chat.
 * @returns Fail if return to send the message to the chat, 0 if load PID to file failed, or SUCCESS if successful.
 */
int Sender(char _IP[], int _port, char _nickName[]);

#endif /* __SENDER_H__*/
