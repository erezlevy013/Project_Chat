#include <stdlib.h> /* malloc */
#include <string.h> /* strcpy */
#include <stdio.h> /* sprintf */
#include "Group.h"
#include "Limits.h"

#define MAGIC_NUM 685741

struct Group
{
    char m_IP[IP_ADDR_SIZE_LIMIT];
    char m_groupName[GROUP_NAME_SIZE_LIMIT];
    int m_magicNumber;
    int m_userCount;
    char m_port[PORT_NUM_SIZE];
};

Group* CreateGroup(char _ip[],  char _grName[])
{
    Group *group;
    char port[PORT_NUM_SIZE];
    if(_ip == NULL || _grName == NULL)
    {
        return NULL;
    }
    if((group = (Group*)malloc( sizeof(Group) )) == NULL )
    {
        return NULL;
    }
    group->m_userCount = 1;
    strcpy(group->m_IP, _ip);
    strcpy(group->m_groupName, _grName);
    group->m_magicNumber = MAGIC_NUM;
    sprintf(port, "%d", SERVER_PORT_NUM);
    strcpy(group->m_port, port);
    return group;
}

void DestroyGroup(Group *_group)
{
    if(_group == NULL || _group->m_magicNumber != MAGIC_NUM)
    {
        return;
    }
    _group->m_magicNumber = 0;
    free(_group);
}

int GroupAddUser(Group *_group)
{
    if(_group == NULL)
    {
        return FAIL; 
    }
    _group->m_userCount++;
    return SUCCESS;
}

int GroupRemoveUser(Group *_group)
{
    if( _group == NULL || _group->m_userCount == 0)
    {
        return FAIL; 
    }
    _group->m_userCount--;
    return SUCCESS;
}

int GroupUsersCount(Group *_group)
{
    if(_group == NULL)
    {
        return FAIL;
    }
    return _group->m_userCount;
}

char* GroupIP(Group *_group)
{
    if(_group == NULL)
    {
        return NULL;
    }
    return _group->m_IP;
}

char* GroupPort(Group *_group)
{
    if(_group == NULL)
    {
        return NULL;
    }
    return _group->m_port;
}





