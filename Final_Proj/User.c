#include <stdlib.h> /* malloc, free */
#include <string.h> /* strncpy, strncmp */
#include "User.h"
#include "Limits.h"
#include "GenList.h"
#include "ListItr.h"
#include "ListFunc.h"
#define MAGIC_NUM 474715

static int FindGroupName(void *_element, void *_context);

struct User
{
    char m_userName[USERNAME_SIZE_LIMIT];
    char m_password[PASSWORD_SIZE_LIMIT];
    int m_isActive;
    List *m_groupsList;
    int m_magicNum;
};

User* CreateUser(char _userName[], char _password[])
{
    User *pUser = NULL;
    List *pList = NULL;
    if(_userName == NULL || _password == NULL)
    {
        return NULL;
    }
    pUser = (User*)malloc(sizeof(User));
    if(pUser == NULL)
    {
        return NULL;
    }
    pList = ListCreate();
    if(pList == NULL)
    {
        free(pUser);
        return NULL;
    }
    strncpy(pUser->m_userName, _userName, USERNAME_SIZE_LIMIT);
    strncpy(pUser->m_password, _password, PASSWORD_SIZE_LIMIT);
    pUser->m_isActive = 0;
    pUser->m_groupsList = pList;
    pUser->m_magicNum = MAGIC_NUM;
    return pUser;
}

int ActivateUser(User *_user)
{
    if(_user == NULL)
    {
        return FAIL;
    }
    _user->m_isActive = 1;
    return SUCCESS;
}

int InactivateUser(User *_user)
{
    ListItr pNode = NULL, pEnd = NULL, pTemp = NULL;
    void *pGroupName = NULL;
    if(_user == NULL)
    {
        return FAIL;
    }
    pNode = ListItrBegin(_user->m_groupsList);
    pEnd = ListItrEnd(_user->m_groupsList);
    while(pNode != pEnd)
    {
        pTemp = pNode;
        pNode = ListItrNext(pNode);
        pGroupName = ListItrRemove(pTemp);
        free(pGroupName);
    }
    _user->m_isActive = 0;
    return SUCCESS;
}

int AddUserToGroup(User *_user, char _groupName[])
{
    ListResult result;
    char *pGroupName = NULL;
    ListItr pBegin = NULL, pEnd = NULL, pGroupToAdd = NULL;
    if(_user == NULL || _groupName == NULL)
    {
        return FAIL;
    }
    pBegin = ListItrBegin(_user->m_groupsList);
    pEnd = ListItrEnd(_user->m_groupsList);
    pGroupToAdd = ListItrFindFirst(pBegin, pEnd, FindGroupName, _groupName);
    if(pGroupToAdd != pEnd)
    {
        return DUPLICATE;
    }
    pGroupName = (char*)(malloc(GROUP_NAME_SIZE_LIMIT));
    if(pGroupName == NULL)
    {
        return FAIL;
    }
    strncpy(pGroupName, _groupName, GROUP_NAME_SIZE_LIMIT);
    result = ListPushTail(_user->m_groupsList, pGroupName);
    if(result != LIST_SUCCESS)
    {
        return FAIL;
    }
    return SUCCESS;
}

int RemoveUserFromGroup(User *_user, char _groupName[])
{
    char groupName[GROUP_NAME_SIZE_LIMIT];
    ListItr pBegin = NULL, pEnd = NULL, pGroupToRemove = NULL;
    void *pTemp = NULL;
    if(_user == NULL || _groupName == NULL)
    {
        return FAIL;
    }
    pBegin = ListItrBegin(_user->m_groupsList);
    pEnd = ListItrEnd(_user->m_groupsList);
    pGroupToRemove = ListItrFindFirst(pBegin, pEnd, FindGroupName, _groupName);
    if(pGroupToRemove == pEnd)
    {
        return NOT_FOUND;
    }
    pTemp = ListItrRemove(pGroupToRemove);
    strncpy(groupName, (char*)pTemp, GROUP_NAME_SIZE_LIMIT);
    if(strcmp(_groupName, groupName) != 0)
    {
        ListPushTail(_user->m_groupsList, groupName);
        return FAIL;
    }
    free(pTemp);
    return SUCCESS;
}

List* UserGroups(User *_user)
{
    if(_user == NULL)
    {
        return NULL;
    }
    return _user->m_groupsList;
}

char* GetUserPassword(User *_user)
{
    if(_user == NULL)
    {
        return NULL;
    }
    return _user->m_password; 
}

void DestroyUser(User* _user)
{
    if(_user != NULL && _user->m_magicNum == MAGIC_NUM)
    {
        _user->m_magicNum = 0;
        ListDestroy(&_user->m_groupsList, NULL);
        free(_user);
    }
}

static int FindGroupName(void *_element, void *_context)
{
    if(strcmp((char*)_element, (char*)_context) == 0)
    {
        return 1;
    }
    return 0;
}