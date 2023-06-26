#ifndef __CHAT_H__
#define __CHAT_H__

/**
 * @brief opens 2 new windows when user joins a group: a keyboard window, and a multi-users chat screen.
 * @param _clientManager : A pointer to previously created client manager returned via CreateClientManager
 * @param _clientManager : A pointer to previously created group returned when user created a new group or joined exixting group.
 */
int OpenGroup(char _userName[], char _groupIP[], char _groupPort[], char _groupName[], int *_chatPID, int *_keyBoradPID);

/**
 * @brief exits chat window and keyboard window of a single group.
 * @param _group : A pointer to previously created  group that the use is now pare of it. 
 */
void CloseGroupWindows(int _chatPID, int _keyBoradPID);

#endif /* __CHAT_H__ */