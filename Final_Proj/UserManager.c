#include <stdio.h> /* FILE, fopen, fscanf, fclose, feof */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strncpy */
#include "UserManager.h"
#include "User.h"
#include "Limits.h"
#include "HashMap.h"
#include "GenList.h"
#define MAGIC_NUM 885968
#define FILENAME "UsersInfo.txt"

size_t HashFunc(const void *_key);

int EqualityFunc(void *_firstKey, void *_secondKey);

static UserManagerResult LoadUser(UserManager *_userManager, char _userName[], char _password[]);

static void DestroyUserName(char _userName[]);

struct UserManager
{
    HashMap* m_usersHashMap;
    int m_numOfUsers;
    int m_magicNum;
};

UserManager* CreateUserManager(void)
{
    UserManager *pUserManager = NULL;
    HashMap *pHashMap = NULL;
    FILE *pFile = NULL;
    char userName[USERNAME_SIZE_LIMIT], password[PASSWORD_SIZE_LIMIT];
    pUserManager = (UserManager*)malloc(sizeof(UserManager));
    if(pUserManager == NULL)
    {
        return NULL;
    }
    pHashMap = HashMap_Create(NUM_OF_USERS_LIMIT, HashFunc, EqualityFunc);
    if(pHashMap == NULL)
    {
        free(pUserManager);
        return NULL;
    }
    pUserManager->m_usersHashMap = pHashMap;
    pUserManager->m_magicNum = MAGIC_NUM;
    pFile = fopen(FILENAME, "r");
	if(pFile != NULL)
	{
        while(!feof(pFile))
	    {
		    fscanf(pFile, "%s%s", userName, password);
		    LoadUser(pUserManager, userName, password);
	    }
        fclose(pFile);
	}
    return pUserManager;
}

UserManagerResult RegisterUser(UserManager *_userManager, char _userName[], char _password[])
{
    User *pUser = NULL;
    Map_Result result;
    char *pUserName = NULL;
    FILE *pFile = NULL;
    if(_userManager == NULL || _userName == NULL || _password == NULL)
    {
        return USER_MANAGER_NOT_INITIALIZED;
    }
    if(_userManager->m_numOfUsers >= NUM_OF_USERS_LIMIT)
    {
        return USER_MANAGER_ERR_USERS_OVERFLOW;
    }
    pUser = CreateUser(_userName, _password);
    if(pUser == NULL)
    {
        return USER_MANAGER_ERR_ALLOCATION_FAILED;
    }
    pUserName = (char*)malloc(strlen(_userName) + 1);
    strncpy(pUserName, _userName, USERNAME_SIZE_LIMIT);
    result = HashMap_Insert(_userManager->m_usersHashMap, pUserName, pUser);
    if(result == MAP_KEY_DUPLICATE_ERROR)
    {
        free(pUser);
        free(pUserName);
        return USER_MANAGER_ERR_DUPLICATE_USERNAME;
    }
    else if(result != MAP_SUCCESS)
    {
        free(pUser);
        free(pUserName);
        return USER_MANAGER_HASH_ERR_ALLOCATION_FAILED;
    }
    pFile = fopen(FILENAME, "a");
    if(pFile == NULL)
    {
        return USER_MANAGER_ERR_USER_FILE;
    }
	fprintf(pFile, "%s %s\n", _userName, _password);
	fclose(pFile);
    _userManager->m_numOfUsers += 1;
    return USER_MANAGER_SUCCESS;
}

UserManagerResult LoginUser(UserManager *_userManager, char _userName[], char _password[])
{
    Map_Result result;
    int userResult;
    User *pUser;
    char *pPassword;
    if(_userManager == NULL || _userName == NULL || _password == NULL)
    {
        return USER_MANAGER_NOT_INITIALIZED;
    }
    result = HashMap_Find(_userManager->m_usersHashMap, _userName, (void**)&pUser);
    if(result != MAP_SUCCESS)
    {
        return USER_MANAGER_ERR_INVALID_USERNAME;
    }
    pPassword = GetUserPassword(pUser);
    if(strcmp(pPassword, _password) != 0)
    {
        return USER_MANAGER_ERR_INVALID_PASSWORD;
    }
    userResult = ActivateUser(pUser);
    if(userResult != SUCCESS)
    {
        return USER_MANAGER_ERR_LOGIN_FAILED;
    }
    return USER_MANAGER_SUCCESS;
}

UserManagerResult LogoutUser(UserManager *_userManager, char _userName[])
{
    Map_Result result;
    int userResult;
    User *pUser;
    if(_userManager == NULL || _userName == NULL)
    {
        return USER_MANAGER_NOT_INITIALIZED;
    }
    result = HashMap_Find(_userManager->m_usersHashMap, _userName, (void**)&pUser);
    if(result != MAP_SUCCESS)
    {
        return USER_MANAGER_ERR_INVALID_USERNAME;
    }
    userResult = InactivateUser(pUser);
    if(userResult != SUCCESS)
    {
        return USER_MANAGER_ERR_LOGOUT_FAILED;
    }
    return USER_MANAGER_SUCCESS;
}

UserManagerResult JoinGroupUser(UserManager *_userManager, char _userName[], char _groupName[])
{
    Map_Result result;
    int userResult;
    User *pUser;
    if(_userManager == NULL || _userName == NULL || _groupName == NULL)
    {
        return USER_MANAGER_NOT_INITIALIZED;
    }
    result = HashMap_Find(_userManager->m_usersHashMap, _userName, (void**)&pUser);
    if(result != MAP_SUCCESS)
    {
        return USER_MANAGER_ERR_INVALID_USERNAME;
    }
    userResult = AddUserToGroup(pUser, _groupName);
    if(userResult == DUPLICATE)
    {
        return USER_MANAGER_ERR_JOIN_GROUP_DUPLICATE;
    }
    else if(userResult != SUCCESS)
    {
        return USER_MANAGER_ERR_JOIN_GROUP_FAILED;
    }
    return USER_MANAGER_SUCCESS;
}

UserManagerResult LeaveGroupUser(UserManager *_userManager, char _userName[], char _groupName[])
{
    Map_Result result;
    int userResult;
    User *pUser;
    if(_userManager == NULL || _userName == NULL || _groupName == NULL)
    {
        return USER_MANAGER_NOT_INITIALIZED;
    }
    result = HashMap_Find(_userManager->m_usersHashMap, _userName, (void**)&pUser);
    if(result != MAP_SUCCESS)
    {
        return USER_MANAGER_ERR_INVALID_USERNAME;
    }
    userResult = RemoveUserFromGroup(pUser, _groupName);
    if(userResult == NOT_FOUND)
    {
        return USER_MANAGER_ERR_LEAVE_GROUP_NOT_FOUND;
    }
    else if(userResult != SUCCESS)
    {
        return USER_MANAGER_ERR_LEAVE_GROUP_FAILED;
    }
    return USER_MANAGER_SUCCESS; 
}

List* GetGroupsUser(UserManager *_userManager, char _userName[])
{
    Map_Result result;
    User *pUser;
    if(_userManager == NULL || _userName == NULL)
    {
        return NULL;
    }
    result = HashMap_Find(_userManager->m_usersHashMap, _userName, (void**)&pUser);
    if(result != MAP_SUCCESS)
    {
        return NULL;
    }
    return UserGroups(pUser);
}

void DestroyUserManager(UserManager *_userManager)
{
    if(_userManager != NULL && _userManager->m_magicNum == MAGIC_NUM)
    {
        _userManager->m_magicNum = 0;
        HashMap_Destroy(&_userManager->m_usersHashMap, (void(*)(void*))DestroyUserName, (void(*)(void*))DestroyUser);
        free(_userManager);
    }
}

size_t HashFunc(const void *_key)
{
    char userName[USERNAME_SIZE_LIMIT];
    int i = 0, count = 0;
    strncpy(userName, (char*)_key, USERNAME_SIZE_LIMIT);
    while(userName[i] != '\0')
    {
        count += userName[i] * (i + 1);
        ++i;
    }
    return count;
}

int EqualityFunc(void *_firstKey, void *_secondKey)
{
        if(strcmp((char*)_firstKey, (char*)_secondKey) == 0)
        {
            return TRUE;
        }
        return FALSE;
}

static UserManagerResult LoadUser(UserManager *_userManager, char _userName[], char _password[])
{
    User *pUser = NULL;
    Map_Result result;
    char *pUserName = NULL;
    if(_userManager == NULL || _userName == NULL || _password == NULL)
    {
        return USER_MANAGER_NOT_INITIALIZED;
    }
    pUser = CreateUser(_userName, _password);
    if(pUser == NULL)
    {
        return USER_MANAGER_ERR_ALLOCATION_FAILED;
    }
    pUserName = (char*)malloc(strlen(_userName) + 1);
    strncpy(pUserName, _userName, USERNAME_SIZE_LIMIT);
    result = HashMap_Insert(_userManager->m_usersHashMap, pUserName, pUser);
    if(result == MAP_KEY_DUPLICATE_ERROR)
    {
        free(pUser);
        return USER_MANAGER_ERR_DUPLICATE_USERNAME;
    }
    else if(result != MAP_SUCCESS)
    {
        free(pUser);
        return USER_MANAGER_HASH_ERR_ALLOCATION_FAILED;
    }
    _userManager->m_numOfUsers += 1;
    return USER_MANAGER_SUCCESS;
}

static void DestroyUserName(char _userName[])
{
    free(_userName);
}