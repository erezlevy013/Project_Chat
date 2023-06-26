#include <string.h> /* strncpy */
#include <stdlib.h> /* MALLOC, FREE */
#include "ServerManager.h"
#include "ServerNet.h"
#include "Limits.h"
#include "Protocol.h"
#include "UserManager.h"
#include "GroupManager.h"
#include "GenVector.h"
#include "GenList.h"
#include "ListItr.h"
#include "ListFunc.h"
#define MAGIC_NUM 115477

struct ServerManager
{
    Server* m_serverNet;
    UserManager* m_usersManager;
    GroupManager* m_groupsManager;
    int m_magicNum;
};

int GotClientMsg(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context);

int RemoveClientFromGroups(void *_element, void *_context);

static int RegsiterNewClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context);

static int LoginClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context);

static int LogoutClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context);

static int CreateGroupClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context);

static int GetGroupsClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context);

static int JoinGroupClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context);

static int LeaveGroupClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context);

static void UpdateServerUserReply(UserManagerResult _result, char _reply[]);

static void UpdateServerGroupReply(GroupManagerResult _result, char _reply[]);

static void UpdateServerUserReplyTwoParams(UserManagerResult _result, char _IPAddr[], char _portNum[]);

static void UpdateServerGroupReplyTwoParams(GroupManagerResult _result, char _IPAddr[], char _portNum[]);

ServerManager* CreateServerManager(void)
{
    ServerManager* pServerManager = NULL;
    Server *pServer = NULL;
    UserManager *pUsersManager = NULL;
    GroupManager *pGroupsManager = NULL;
    ServerResult serverRes;
    pServerManager = (ServerManager*)malloc(sizeof(ServerManager));
    if(pServerManager == NULL)
    {
        return NULL;
    }
    pUsersManager = CreateUserManager();
    if(pUsersManager == NULL)
    {
        free(pServerManager);
        return NULL;
    }
    pGroupsManager = CreateGroupManager();
    if(pGroupsManager == NULL)
    {
        DestroyUserManager(pUsersManager);
        free(pServerManager);
        return NULL;
    }
    pServer = CreateServer(SERVER_PORT_NUM, BACKLOG_LIMIT, NULL, GotClientMsg, NULL, NULL, pServerManager, &serverRes);
    if(pServer == NULL || serverRes != SERVER_SUCCESS)
    {
        free(pServerManager);
        free(pUsersManager);
        return NULL;
    }
    pServerManager->m_serverNet = pServer;
    pServerManager->m_usersManager = pUsersManager;
    pServerManager->m_groupsManager = pGroupsManager;
    pServerManager->m_magicNum = MAGIC_NUM;
    return pServerManager;
}

int RunServerManager(ServerManager *_serverManager)
{
    ServerResult result;
    if(_serverManager == NULL)
    {
        return FAIL;
    }
    result = RunServer(_serverManager->m_serverNet);
    if(result != SERVER_SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

void DestroyServerManager(ServerManager *_serverManager)
{
    if(_serverManager != NULL && _serverManager->m_magicNum == MAGIC_NUM)
    {
        _serverManager->m_magicNum = 0;
        DestroyServer(_serverManager->m_serverNet);
        DestroyUserManager(_serverManager->m_usersManager);
        DestroyGroupManager(_serverManager->m_groupsManager);
        free(_serverManager);
    }
}

int GotClientMsg(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context)
{
    ProReqType msgType;
    int result;
    msgType = TagReqType(_buffer);
    switch(msgType)
    {
        case REGISTRATION_REQUEST:
            result = RegsiterNewClient(_clientID, _buffer, _bufferLen, _context);
            if(result != SUCCESS)
            {
                return FAIL;
            }
            break;
        case LOGIN_REQUEST:
            result = LoginClient(_clientID, _buffer, _bufferLen, _context);
            if(result != SUCCESS)
            {
                return FAIL;
            }
            break;
        case LOGOUT_REQUEST:
            result = LogoutClient(_clientID, _buffer, _bufferLen, _context);
            if(result != SUCCESS)
            {
                return FAIL;
            }
            break;
        case CREATE_GROUP_REQUEST:
            result = CreateGroupClient(_clientID, _buffer, _bufferLen, _context);
            if(result != SUCCESS)
            {
                return FAIL;
            }
            break;
        case GET_GROUPS_REQUEST:
            result = GetGroupsClient(_clientID, _buffer, _bufferLen, _context);
            if(result != SUCCESS)
            {
                return FAIL;
            }
        case JOIN_GROUP_REQUEST:
            result = JoinGroupClient(_clientID, _buffer, _bufferLen, _context);
            if(result != SUCCESS)
            {
                return FAIL;
            }
            break;
        case LEAVE_GROUP_REQUEST:
            result = LeaveGroupClient(_clientID, _buffer, _bufferLen, _context);
            if(result != SUCCESS)
            {
                return FAIL;
            }
            break;
        default:
            return FAIL;
    }
    return SUCCESS;
}

static int RegsiterNewClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context)
{
    ServerManager* pServerManager = (ServerManager*)_context;
    char userName[USERNAME_SIZE_LIMIT], password[PASSWORD_SIZE_LIMIT], reply[REPLY_SIZE_LIMIT], buffer[BUFFER_SIZE_LIMIT];
    int result;
    ServerResult serverRes;
    UserManagerResult userManagerRes;
    size_t replyLen;
    result = RegReqUnPack(_buffer, userName, password, _bufferLen);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    userManagerRes = RegisterUser(pServerManager->m_usersManager, userName, password);
    UpdateServerUserReply(userManagerRes, reply);
    replyLen = RegRepPack(reply, buffer);
    serverRes = SendToClient(_clientID, buffer, replyLen, pServerManager->m_serverNet);
    if(serverRes != SERVER_SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

static int LoginClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context)
{
    ServerManager* pServerManager = (ServerManager*)_context;
    char userName[USERNAME_SIZE_LIMIT], password[PASSWORD_SIZE_LIMIT], reply[REPLY_SIZE_LIMIT], buffer[BUFFER_SIZE_LIMIT];
    int result;
    ServerResult serverRes;
    UserManagerResult userManagerRes;
    size_t replyLen;
    result = LogInReqUnPack(_buffer, userName, password, _bufferLen);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    userManagerRes = LoginUser(pServerManager->m_usersManager, userName, password);
    UpdateServerUserReply(userManagerRes, reply);
    replyLen = LogInRepPack(reply, buffer);
    serverRes = SendToClient(_clientID, buffer, replyLen, pServerManager->m_serverNet);
    if(serverRes != SERVER_SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

static int LogoutClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context)
{
    ServerManager* pServerManager = (ServerManager*)_context;
    char userName[USERNAME_SIZE_LIMIT], reply[REPLY_SIZE_LIMIT], buffer[BUFFER_SIZE_LIMIT];
    int result;
    List *pList = NULL;
    ListItr pBegin = NULL, pEnd = NULL, pItr = NULL;
    ServerResult serverRes;
    UserManagerResult userManagerRes;
    size_t replyLen;
    result = LogOutReqUnPack(_buffer, userName, _bufferLen);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    pList = GetGroupsUser(pServerManager->m_usersManager, userName);
    if(pList == NULL)
    {
        return FAIL;
    }
    pBegin = ListItrBegin(pList);
    pEnd = ListItrEnd(pList);
    pItr = ListItrForEach(pBegin, pEnd, RemoveClientFromGroups, pServerManager->m_groupsManager);
    if(pItr != pEnd)
    {
        return FAIL;
    }
    userManagerRes = LogoutUser(pServerManager->m_usersManager, userName);
    UpdateServerUserReply(userManagerRes, reply);
    replyLen = LogOutRepPack(reply, buffer);
    serverRes = SendToClient(_clientID, buffer, replyLen, pServerManager->m_serverNet);
    if(serverRes != SERVER_SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

static int CreateGroupClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context)
{
    ServerManager* pServerManager = (ServerManager*)_context;
    char userName[USERNAME_SIZE_LIMIT], groupName[GROUP_NAME_SIZE_LIMIT], buffer[BUFFER_SIZE_LIMIT], IPAddr[IP_ADDR_SIZE_LIMIT], portNum[PORT_NUM_SIZE];
    int result;
    ServerResult serverRes;
    UserManagerResult userManagerRes;
    GroupManagerResult groupManagerRes;
    size_t replyLen;
    result = CreateGroupReqUnPack(_buffer, userName, groupName, _bufferLen);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    groupManagerRes = AddNewGroup(pServerManager->m_groupsManager, groupName);
    if(groupManagerRes != GROUP_MANAGER_SUCCESS)
    {
        UpdateServerGroupReplyTwoParams(groupManagerRes, IPAddr, portNum);
    }
    else
    {
        userManagerRes = JoinGroupUser(pServerManager->m_usersManager, userName, groupName);
        if(userManagerRes != USER_MANAGER_SUCCESS)
        {
            UpdateServerUserReplyTwoParams(userManagerRes, IPAddr, portNum);
            GroManRemoveUser(pServerManager->m_groupsManager, groupName);
        }
        else
        {
            GroupManagerGetGroupInfo(pServerManager->m_groupsManager, groupName, IPAddr, portNum);
        }
    }
    replyLen = CreateGroupRepPack(IPAddr, portNum, buffer);
    serverRes = SendToClient(_clientID, buffer, replyLen, pServerManager->m_serverNet);
    if(serverRes != SERVER_SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

static int GetGroupsClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context)
{
    ServerManager* pServerManager = (ServerManager*)_context;
    Vector *pVector = NULL;
    char buffer[BUFFER_SIZE_LIMIT], reply[REPLY_SIZE_LIMIT];
    ServerResult serverRes;
    GroupManagerResult groupManagerRes;
    size_t replyLen;
    pVector = VectorCreate(NUM_OF_GROUPS_LIMIT, 0);
    if(pVector == NULL)
    {
        return FAIL;
    }
    groupManagerRes = GroupManagerGetGroupsNames(pServerManager->m_groupsManager, pVector);
    if(groupManagerRes != GROUP_MANAGER_SUCCESS)
    {
        UpdateServerGroupReply(groupManagerRes, reply);
        replyLen = GetGroupsRepFailPack(reply, buffer);
    }
    else
    {
        replyLen = GetGroupsRepPack(pVector, buffer);
    }
    serverRes = SendToClient(_clientID, buffer, replyLen, pServerManager->m_serverNet);
    if(serverRes != SERVER_SUCCESS)
    {
        VectorDestroy(&pVector, NULL);
        return FAIL;
    }
    VectorDestroy(&pVector, NULL);
    return SUCCESS;
}

static int JoinGroupClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context)
{
    ServerManager* pServerManager = (ServerManager*)_context;
    char userName[USERNAME_SIZE_LIMIT], groupName[GROUP_NAME_SIZE_LIMIT], buffer[BUFFER_SIZE_LIMIT], IPAddr[IP_ADDR_SIZE_LIMIT], portNum[PORT_NUM_SIZE];
    int result;
    ServerResult serverRes;
    UserManagerResult userManagerRes;
    GroupManagerResult groupManagerRes;
    size_t replyLen;
    result = JoinGroupReqUnPack(_buffer, userName, groupName, _bufferLen);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    groupManagerRes = GroManAddUser(pServerManager->m_groupsManager, groupName);
    if(groupManagerRes != GROUP_MANAGER_SUCCESS)
    {
        UpdateServerGroupReplyTwoParams(groupManagerRes, IPAddr, portNum);
    }
    else
    {
        userManagerRes = JoinGroupUser(pServerManager->m_usersManager, userName, groupName);
        if(userManagerRes != USER_MANAGER_SUCCESS)
        {
            UpdateServerUserReplyTwoParams(userManagerRes, IPAddr, portNum);
            GroManRemoveUser(pServerManager->m_groupsManager, groupName);
        }
        else
        {
            GroupManagerGetGroupInfo(pServerManager->m_groupsManager, groupName, IPAddr, portNum);
        }
    }
    replyLen = JoinGroupRepPack(IPAddr, portNum, buffer);
    serverRes = SendToClient(_clientID, buffer, replyLen, pServerManager->m_serverNet);
    if(serverRes != SERVER_SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

static int LeaveGroupClient(int _clientID, char _buffer[], ssize_t _bufferLen, void *_context)
{
    ServerManager* pServerManager = (ServerManager*)_context;
    char userName[USERNAME_SIZE_LIMIT], groupName[GROUP_NAME_SIZE_LIMIT], buffer[BUFFER_SIZE_LIMIT], reply[REPLY_SIZE_LIMIT];
    int result;
    ServerResult serverRes;
    UserManagerResult userManagerRes;
    GroupManagerResult groupManagerRes;
    size_t replyLen;
    result = LeaveGroupReqUnPack(_buffer, userName, groupName, _bufferLen);
    if(result != SUCCESS)
    {
        return FAIL;
    }
    userManagerRes = LeaveGroupUser(pServerManager->m_usersManager, userName, groupName);
    if(userManagerRes != USER_MANAGER_SUCCESS)
    {
        UpdateServerUserReply(userManagerRes, reply);
    }
    else
    {
        groupManagerRes = GroManRemoveUser(pServerManager->m_groupsManager, groupName);
        UpdateServerGroupReply(groupManagerRes, reply);
    }
    replyLen = LeaveGroupRepPack(reply, buffer);
    serverRes = SendToClient(_clientID, buffer, replyLen, pServerManager->m_serverNet);
    if(serverRes != SERVER_SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

int RemoveClientFromGroups(void *_element, void *_context)
{
    GroupManagerResult groupManagerRes;
    GroupManager *pGroupManager = (GroupManager*)_context;
    groupManagerRes = GroManRemoveUser(pGroupManager, (char*)_element);
    if(groupManagerRes != GROUP_MANAGER_SUCCESS && groupManagerRes != GROUP_MANAGER_GROUP_REMOVED)
    {
        return 0;
    }
    return 1;
}

static void UpdateServerUserReply(UserManagerResult _result, char _reply[])
{
    if(_result == USER_MANAGER_ERR_DUPLICATE_USERNAME)
    {
        strncpy(_reply, "Duplicate", REPLY_SIZE_LIMIT);
    }
    else if(_result == USER_MANAGER_ERR_LEAVE_GROUP_NOT_FOUND)
    {
        strncpy(_reply, "Not Found", REPLY_SIZE_LIMIT);
    }
    else if(_result != USER_MANAGER_SUCCESS)
    {
        strncpy(_reply, "Fail", REPLY_SIZE_LIMIT);
    }
    else
    {
        strncpy(_reply, "Success", REPLY_SIZE_LIMIT);
    }
}

static void UpdateServerGroupReply(GroupManagerResult _result, char _reply[])
{
    if(_result == GROUP_MANAGER_INVALID_GROUPNAME || _result == GROUP_MANAGER_EMPTY)
    {
        strncpy(_reply, "Not Found", REPLY_SIZE_LIMIT);
    }
    else if(_result != GROUP_MANAGER_SUCCESS && _result != GROUP_MANAGER_GROUP_REMOVED)
    {
        strncpy(_reply, "Fail", REPLY_SIZE_LIMIT);
    }
    else
    {
        strncpy(_reply, "Success", REPLY_SIZE_LIMIT);
    }
}

static void UpdateServerGroupReplyTwoParams(GroupManagerResult _result, char _IPAddr[], char _portNum[])
{
    if(_result == GROUP_MANAGER_DUPLICATE_GROUPNAME)
    {
        strncpy(_IPAddr, "Duplicate", REPLY_SIZE_LIMIT);
        strncpy(_portNum, "Dupe", REPLY_SIZE_LIMIT);
    }
    else if(_result == GROUP_MANAGER_INVALID_GROUPNAME)
    {
        strncpy(_IPAddr, "Not Found", REPLY_SIZE_LIMIT);
        strncpy(_portNum, "NotF", REPLY_SIZE_LIMIT);
    }
    else
    {
        strncpy(_IPAddr, "Fail", REPLY_SIZE_LIMIT);
        strncpy(_portNum, "Fail", REPLY_SIZE_LIMIT);
    }
}

static void UpdateServerUserReplyTwoParams(UserManagerResult _result, char _IPAddr[], char _portNum[])
{
    if(_result == USER_MANAGER_ERR_JOIN_GROUP_DUPLICATE)
    {
        strncpy(_IPAddr, "Duplicate", REPLY_SIZE_LIMIT);
        strncpy(_portNum, "Dupe", REPLY_SIZE_LIMIT);
    }
    else
    {
        strncpy(_IPAddr, "Fail", REPLY_SIZE_LIMIT);
        strncpy(_portNum, "Fail", REPLY_SIZE_LIMIT);
    }
}

int main()
{
    ServerManager *pManager;
    pManager = CreateServerManager();
    RunServerManager(pManager);
    DestroyServerManager(pManager);
    return 0;
}