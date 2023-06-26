#include <stdlib.h>    /* malloc, free */
#include <unistd.h>    /* close */
#include <stdio.h>     /* printf, scanf */
#include <string.h>    /* strcmp */
#include <sys/types.h> /* ssize_t */

#include "ClientManager.h"
#include "ClientNet.h"
#include "Limits.h"
#include "Protocol.h"
#include "UI.h"
#include "GenList.h"
#include "ListItr.h"
#include "ListFunc.h"
#include "GenVector.h"
#include "Listener.h"
#include "Sender.h"
#include "Chat.h"

#define MAGIC_NUM 696354
#define REGISTER 1
#define LOGIN 2
#define EXIT 3
#define CREATE 1
#define JOIN 2
#define LEAVE 3
#define LOG_OUT 4

struct ClientManager
{
    int m_clientSocket;
    char m_userName[USERNAME_SIZE_LIMIT];
    List *m_groupsList;
    int m_magicNum;
};

struct Group
{
    char m_GroupName[GROUP_NAME_SIZE_LIMIT];
    char m_GroupIP[IP_ADDR_SIZE_LIMIT];
    char m_groupPort[PORT_NUM_SIZE];
    int m_KeyBoardPID;
    int m_ChatPID;
};

static int Logout(ClientManager *_clientManager);
static int FreeGroupsInList(List *_groupList);
static void PrintUserGroups(ClientManager *_clientManager);
static int PrintGroupNamesFromServer(ClientManager *_clientManager);
static void DestroyVectorElements(void *_group);
static int RegisterMode(ClientManager *_clientManager, int *_mode, int *_sock);
static int LoginMode(ClientManager *_clientManager, int *_mode);
static int Register(ClientManager *_clientManager);
static int Login(ClientManager *_clientManager);
static int CreateGroup(ClientManager *_clientManager);
static int JoinGroup(ClientManager *_clientManager);
static int FindGroupByName(void *_element, void *_context);
static int LeaveGroup(ClientManager *_clientManager);
static int AddGroupToList(ClientManager *_clientManager, Group *_group);
static ssize_t SendAndRecv(int _clientSocket, char _reqBuffer[], size_t _ReqLen, char _repBuffer[]);

ClientManager *CreateClientManager(void)
{
    ClientManager *pClientManager = NULL;
    List *list = NULL;
    int clientSocket;
    clientSocket = CreateClient();
    if (clientSocket < 0)
    {
        return NULL;
    }
    pClientManager = (ClientManager*)malloc(sizeof(ClientManager));
    if (pClientManager == NULL)
    {
        close(clientSocket);
        return NULL;
    }
    if ((list = ListCreate()) == NULL)
    {
        close(clientSocket);
        free(pClientManager);
        return NULL;
    }
    pClientManager->m_clientSocket = clientSocket;
    pClientManager->m_groupsList = list;
    pClientManager->m_magicNum = MAGIC_NUM;
    return pClientManager;
}

int RunClientManager(ClientManager *_clientManager)
{
    int mode = FALSE, sock = TRUE;
    if(_clientManager == NULL)
    {
        return FAIL;
    }
    SetAppColor();
    while(sock)
    {
        if(!mode)
        {
            RegisterMode(_clientManager, &mode, &sock);
        }
        else if(mode)
        {
            LoginMode(_clientManager, &mode);
        }
    }
    DestroyClientManager(_clientManager);
    return SUCCESS;
}

void DestroyClientManager(ClientManager *_clientManager)
{
    if (_clientManager != NULL && _clientManager->m_magicNum == MAGIC_NUM)
    {
        _clientManager->m_magicNum = 0;
        CloseClient(_clientManager->m_clientSocket);
        ListDestroy(&_clientManager->m_groupsList, NULL);
        free(_clientManager);
    }
}

static int RegisterMode(ClientManager *_clientManager, int *_mode, int *_sock)
{
    int option;
    WelcomePrint("");
    option = PrintLoginMenu();
    switch (option)
    {
    case REGISTER:
        Register(_clientManager);
        break;
    case LOGIN:
        if (Login(_clientManager) == SUCCESS)
        {
            *_mode = TRUE;
        }
        break;
    case EXIT:
        *_sock = FALSE;
        break;
    }
    return SUCCESS;
}

static int LoginMode(ClientManager *_clientManager, int *_mode)
{
    int option;
    WelcomePrint(_clientManager->m_userName);
    option = PrintMainMenu();
    switch (option)
    {
        case CREATE:
            CreateGroup(_clientManager);
        break;
        case JOIN:
            JoinGroup(_clientManager);
            break;
        case LEAVE:
            LeaveGroup(_clientManager);
            break;
        case LOG_OUT:
            if (Logout(_clientManager) == SUCCESS)
            {
                *_mode = FALSE;
            }
            break;
    }
    return SUCCESS;
}

static int Register(ClientManager *_clientManager)
{
    char userName[USERNAME_SIZE_LIMIT], password[PASSWORD_SIZE_LIMIT], regBuffer[BUFFER_SIZE_LIMIT], regReqBuffer[BUFFER_SIZE_LIMIT], regRepBuffer[REPLY_SIZE_LIMIT];
    size_t regReqLen;
    ssize_t ReplyLen;
    int result;
    RegisterUserDetails(userName, password);
    regReqLen = RegReqPack(userName, password, regReqBuffer);
    ReplyLen = SendAndRecv(_clientManager->m_clientSocket, regReqBuffer, regReqLen, regRepBuffer);
    if (ReplyLen <= 0)
    {
        return FAIL;
    }
    result = RegRepUnPack(regRepBuffer, regBuffer, ReplyLen);
    if (result != SUCCESS)
    {
        return FAIL;
    }
    result = PrintRegisterReply(regBuffer);
    if (result != SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

static int Login(ClientManager *_clientManager)
{
    char password[PASSWORD_SIZE_LIMIT], logBuffer[BUFFER_SIZE_LIMIT], logReqBuffer[BUFFER_SIZE_LIMIT], logRepBuffer[REPLY_SIZE_LIMIT];
    size_t logReqLen;
    ssize_t ReplyLen;
    int result;
    InsertUserDetails(_clientManager->m_userName, password);
    logReqLen = LogInReqPack(_clientManager->m_userName, password, logReqBuffer);
    ReplyLen = SendAndRecv(_clientManager->m_clientSocket, logReqBuffer, logReqLen, logRepBuffer);
    if (ReplyLen <= 0)
    {
        return FAIL;
    }
    result = LogInRepUnPack(logRepBuffer, logBuffer, ReplyLen);
    if (result != SUCCESS)
    {
        return FAIL;
    }
    result = PrintLoginReply(logBuffer);
    if (result != SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

static int CreateGroup(ClientManager *_clientManager)
{
    char createReqBuffer[BUFFER_SIZE_LIMIT], createRepBuffer[REPLY_SIZE_LIMIT], groupName[GROUP_NAME_SIZE_LIMIT], IPAddr[IP_ADDR_SIZE_LIMIT], portNum[PORT_NUM_SIZE];
    Group *group = NULL;
    size_t createReqLen;
    ssize_t ReplyLen;
    int result, chatPID, keyBoaredPID;
    if(_clientManager == NULL)
    {
        return FAIL;
    }
    PrintCreateGroup(groupName);
    createReqLen = CreateGroupReqPack(_clientManager->m_userName, groupName, createReqBuffer);
    ReplyLen = SendAndRecv(_clientManager->m_clientSocket, createReqBuffer, createReqLen, createRepBuffer);
    if(ReplyLen <= 0)
    {
        return FAIL;
    }
    result = CreateGroupRepUnPack(createRepBuffer, IPAddr, portNum, ReplyLen);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    result = PrintCreateReply(IPAddr);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    if((group = (Group*)malloc(sizeof(Group))) == NULL)
    {
        return FAIL;
    }
    strcpy(group->m_GroupName, groupName);
    strcpy(group->m_GroupIP, IPAddr);
    strcpy(group->m_groupPort, portNum);
    if(AddGroupToList(_clientManager, group) != SUCCESS)
    {
        free(group);
        return FAIL;
    }
    OpenGroup(_clientManager->m_userName, group->m_GroupIP, group->m_groupPort, group->m_GroupName, &chatPID, &keyBoaredPID);
    group->m_ChatPID = chatPID;
    group->m_KeyBoardPID = keyBoaredPID;
    return SUCCESS;
}


static int AddGroupToList(ClientManager *_clientManager, Group *_group)
{
    if (ListPushHead(_clientManager->m_groupsList, _group) != LIST_SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

static int JoinGroup(ClientManager *_clientManager)
{
    char joinReqBuffer[BUFFER_SIZE_LIMIT], joinRepBuffer[REPLY_SIZE_LIMIT], groupName[GROUP_NAME_SIZE_LIMIT], IPAddr[IP_ADDR_SIZE_LIMIT], portNum[PORT_NUM_SIZE];
    Group *group = NULL;
    int chatPID, keyBoaredPID;
    size_t joinReqLen;
    ssize_t ReplyLen;
    int result;
    if (PrintGroupNamesFromServer(_clientManager) != SUCCESS)
    {
        return FAIL;
    }
    ChooseGroupToJoin(groupName);
    joinReqLen = JoinGroupReqPack(_clientManager->m_userName, groupName, joinReqBuffer);
    ReplyLen = SendAndRecv(_clientManager->m_clientSocket, joinReqBuffer, joinReqLen, joinRepBuffer);
    if(ReplyLen <= 0)
    {
        return FAIL;
    }
    result = JoinGroupRepUnPack(joinRepBuffer, IPAddr, portNum, ReplyLen);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    result = PrintJoinReply(IPAddr);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    if((group = (Group*)malloc(sizeof(Group))) == NULL)
    {
        return FAIL;
    }
    strcpy(group->m_GroupName, groupName);
    strcpy(group->m_GroupIP, IPAddr);
    strcpy(group->m_groupPort, portNum);
    if(AddGroupToList(_clientManager, group) != SUCCESS)
    {
        free(group);
        return FAIL;
    }
    OpenGroup(_clientManager->m_userName, group->m_GroupIP, group->m_groupPort, group->m_GroupName, &chatPID, &keyBoaredPID);
    group->m_ChatPID = chatPID;
    group->m_KeyBoardPID = keyBoaredPID;
    return SUCCESS;
}

static int LeaveGroup(ClientManager *_clientManager)
{
    char leaveReqBuffer[BUFFER_SIZE_LIMIT], leaveRepBuffer[REPLY_SIZE_LIMIT], groupName[GROUP_NAME_SIZE_LIMIT], leaveBuffer[BUFFER_SIZE_LIMIT];
    ListItr begin = NULL, end = NULL, groupToLeave = NULL;
    Group *group = NULL;
    size_t leaveReqLen;
    ssize_t ReplyLen;
    int result;
    PrintTitleGroupNames();
    PrintUserGroups(_clientManager);
    ChooseGroupToLeave(groupName);
    leaveReqLen = LeaveGroupReqPack(_clientManager->m_userName, groupName, leaveReqBuffer);
    ReplyLen = SendAndRecv(_clientManager->m_clientSocket, leaveReqBuffer, leaveReqLen, leaveRepBuffer);
    if(ReplyLen <= 0)
    {
        return FAIL;
    }
    result = LeaveGroupRepUnPack(leaveRepBuffer, leaveBuffer, ReplyLen);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    result = PrintLeaveReply(leaveBuffer);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    begin = ListItrBegin(_clientManager->m_groupsList);
    end = ListItrEnd(_clientManager->m_groupsList);
    groupToLeave = ListItrFindFirst(begin, end, FindGroupByName, groupName);
    if(groupToLeave == end)
    {
        return FAIL;
    }
    group = (Group*)ListItrRemove(groupToLeave);
    CloseGroupWindows(group->m_ChatPID, group->m_KeyBoardPID);
    free(group);
    return SUCCESS;
}

static void PrintUserGroups(ClientManager *_clientManager)
{
    ListItr begin, end;
    begin = ListItrBegin(_clientManager->m_groupsList);
    end = ListItrEnd(_clientManager->m_groupsList);
    ListItrForEach(begin, end, PrintList, NULL);
}

static int FindGroupByName(void *_element, void *_context)
{
    if (strcmp(((Group*)_element)->m_GroupName, (char*)_context) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

static int PrintGroupNamesFromServer(ClientManager *_clientManager)
{
    Vector* vector = NULL;
    char PrintReqBuffer[1], PrintrepBuffer[BUFFER_SIZE_LIMIT], PrintFailedBuffer[REPLY_SIZE_LIMIT];
    size_t printReqLen;
    ssize_t ReplyLen;
    int result;
    printReqLen = GetGroupsReqPack(PrintReqBuffer);
    ReplyLen = SendAndRecv(_clientManager->m_clientSocket, PrintReqBuffer, printReqLen, PrintrepBuffer);
    if (ReplyLen <= 0)
    {
        return FAIL;
    }
    if (TagRepType(PrintrepBuffer) == FAIL_REPLY)
    {
        GetGroupsRepFailUnpack(PrintrepBuffer, PrintFailedBuffer, ReplyLen);
        PrintGroupsFailReply(PrintFailedBuffer);
        return FAIL;
    }
    if ((vector = VectorCreate(NUM_OF_GROUPS_LIMIT, 0)) == NULL)
    {
        return FAIL;
    }
    result = GetGroupsRepUnPack(PrintrepBuffer, vector, ReplyLen);
    if (result != SUCCESS)
    {
        VectorDestroy(&vector, DestroyVectorElements);
        return FAIL;
    }
    PrintTitleGroupNames();
    VectorForEach(vector, PrintArrayOfGroupNames, NULL);
    VectorDestroy(&vector, DestroyVectorElements);
    return SUCCESS;
}

static void DestroyVectorElements(void *_group)
{
    free(_group);
}

static ssize_t SendAndRecv(int _clientSocket, char _reqBuffer[], size_t _ReqLen, char _repBuffer[])
{
    ClientResult clientResult;
    size_t ReplyLen;
    clientResult = SendToServer(_clientSocket, _reqBuffer, _ReqLen);
    if (clientResult != CLIENT_SUCCESS)
    {
        return FAIL;
    }
    ReplyLen = GotServerMsg(_clientSocket, _repBuffer);
    return ReplyLen;
}

static int Logout(ClientManager *_clientManager)
{
    char LogoutReqBuffer[BUFFER_SIZE_LIMIT], userName[USERNAME_SIZE_LIMIT], logoutRepBuffer[REPLY_SIZE_LIMIT], logoutBuffer[BUFFER_SIZE_LIMIT];
    size_t printReqLen;
    ssize_t ReplyLen;
    int result;
    strcpy(userName, _clientManager->m_userName);
    printReqLen = LogOutReqPack(userName, LogoutReqBuffer);
    ReplyLen = SendAndRecv(_clientManager->m_clientSocket, LogoutReqBuffer, printReqLen, logoutRepBuffer);
    if (ReplyLen <= 0)
    {
        return FAIL;
    }
    result = LogOutRepUnPack(logoutRepBuffer, logoutBuffer, ReplyLen);
    if (result != SUCCESS)
    {
        return FAIL;
    }
    if (LogoutFailReply(logoutBuffer) != SUCCESS)
    {
        return FAIL;
    }
    if (FreeGroupsInList(_clientManager->m_groupsList) != SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

static int FreeGroupsInList(List *_groupList)
{
    ListItr begin, end, node;
    Group *group;
    begin = ListItrBegin(_groupList);
    end = ListItrEnd(_groupList);
    while (begin != end)
    {
        group = (Group *)ListItrGet(begin);
        free(group);
        node = begin;
        begin = ListItrNext(begin);
        ListItrRemove(node);
    }
    if (begin != end)
    {
        return FAIL;
    }
    return SUCCESS;
}

int main()
{
    ClientManager *clientManager = NULL;
    clientManager = CreateClientManager();
    if (clientManager != NULL)
    {
        RunClientManager(clientManager);
    }
    DestroyClientManager(clientManager);
    return 0;
}

