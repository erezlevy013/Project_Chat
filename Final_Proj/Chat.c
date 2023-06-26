#include <stdio.h> /* FILE */
#include <stdlib.h> /* free */

#include <string.h> /* strncat,strlen,strncpy */

#include "Limits.h"

int OpenGroup(char _userName[], char _groupIP[], char _groupPort[], char _groupName[], int *_chatPID, int *_keyBoradPID)
{
    static int location = 0;
    /*InitListencommand[] = "15x15+0+0 ", InitSendercommand[] = "15x5+0+340 ";*/
    char listener[] = " -- ./Listener.out", Sender[] = " -- ./Sender.out", keyBoard[] = "KeyBoard ";
    char IPAddr[IP_ADDR_SIZE_LIMIT], port[PORT_NUM_SIZE], nickName[USERNAME_SIZE_LIMIT], listenerBuffer[BUFFER_SIZE_LIMIT], senderBuffer[BUFFER_SIZE_LIMIT], groupName[GROUP_NAME_SIZE_LIMIT];
    FILE *PIDfile = NULL;
    int end;
    end = strlen(_userName);
    strncpy(nickName, _userName, end);
    nickName[end] = '\0';
    end = strlen(_groupIP);
    strncpy(IPAddr, _groupIP, end);
    IPAddr[end] = '\0';
    end = strlen(_groupPort);
    strncpy(port, _groupPort, end);
    port[end] = '\0';
    end = strlen(_groupName);
    strncpy(groupName, _groupName, end);
    groupName[end] = '\0';
    sprintf(listenerBuffer, "gnome-terminal --geometry 15x15+%d+0 --title=%s %s %s %s", location, groupName, listener, IPAddr, port);
    sprintf(senderBuffer, "gnome-terminal --geometry 15x5+%d+340 --title=%s %s %s %s %s", location, keyBoard, Sender, IPAddr, port, nickName);
    if (system(listenerBuffer) == FAIL)
    {
        return FAIL;
    }
    if (system(senderBuffer) == FAIL)
    {
        return FAIL;
    }
    if ((PIDfile = fopen("ListenerPID.txt", "r")) != NULL)
    {
        while (!feof(PIDfile))
        {
            fscanf(PIDfile, "%d", _chatPID);
        }
        fclose(PIDfile);
    }
    if ((PIDfile = fopen("SenderPID.txt", "r")) != NULL)
    {
        while (!feof(PIDfile))
        {
            fscanf(PIDfile, "%d", _keyBoradPID);
        }
        fclose(PIDfile);
    }
    location += 400;
    return SUCCESS;
}
void CloseGroupWindows(int _chatPID, int _keyBoradPID)
{
    char killChatCommand[BUFFER_SIZE_LIMIT], killKeyBoardCommand[BUFFER_SIZE_LIMIT];
    sprintf(killChatCommand, "kill -9 %d", _chatPID);
    sprintf(killKeyBoardCommand, "kill -9 %d", _keyBoradPID);
    system(killChatCommand);
    system(killKeyBoardCommand);
}