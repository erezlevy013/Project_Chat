#include <stdio.h> /* sprintf */
#include <stdlib.h> /* malloc */
#include <string.h> /* strcpy */
#include "GroupManager.h"
#include "Group.h"
#include "GenQueue.h"
#include "HashMap.h"
#include "GenVector.h"
#include "Limits.h"

#define MAGIC_NUM 852456
#define IP_MULTICAST "224.0.0."

static size_t HashFunc(const void *_key);
static int EqualityFunc(void *_firstKey, void *_secondKey);
static int AddMultiCastIP(Queue *_queue);
static void DestroyIPAddr(void *_elem);
static void DestroyGroupName(void *_elem);
static void CalculateIP(char _ip[], int _num);
static int GetGroupName(const void *_key, void *_value, void *_context);
static GroupManagerResult CheckParameter(GroupManager *_groMan, char _grName[]);
static GroupManagerResult CheckParameterGetGroupInfo(GroupManager *_groMan, char _grName[],char _IPAddr[], char _port[]);

struct GroupManager
{
    Queue *m_IPQueue;
    HashMap* m_groupsHashMap;
    int m_groupsCount;
    int m_magicNumber;
};

GroupManager* CreateGroupManager(void)
{
    GroupManager* groMan;
    if((groMan = (GroupManager*)malloc(sizeof(GroupManager))) == NULL)
    {
        return NULL;
    }
    if((groMan->m_IPQueue = QueueCreate(NUM_OF_IP_ADDR)) == NULL)
    {
        free(groMan);
        return NULL;
    } 
    if((groMan->m_groupsHashMap = HashMap_Create(NUM_OF_GROUPS_LIMIT, HashFunc, EqualityFunc)) == NULL)
    {
        free(groMan->m_IPQueue);
        free(groMan);
        return NULL;
    } 
    groMan->m_magicNumber = MAGIC_NUM;
    groMan->m_groupsCount = 0;
    AddMultiCastIP(groMan->m_IPQueue);
    return groMan;
}

void DestroyGroupManager(GroupManager *_groMan)
{
    if(_groMan  == NULL || _groMan->m_magicNumber != MAGIC_NUM)
    {
        return;
    }
    QueueDestroy(&_groMan->m_IPQueue, DestroyIPAddr);
    HashMap_Destroy(&_groMan->m_groupsHashMap, DestroyGroupName, (void(*)(void*))DestroyGroup);
    _groMan->m_magicNumber = 0;
    free(_groMan);
}

GroupManagerResult AddNewGroup(GroupManager *_groMan, char _grName[])
{
    Group *group;
    char *IPAddr, *groupName;
    Map_Result res;
    if(CheckParameter(_groMan, _grName) != GROUP_MANAGER_SUCCESS)
    {
        return GROUP_MANAGER_NOT_INITIALIZED;
    }
    if(_groMan->m_groupsCount >= NUM_OF_IP_ADDR)
    {
        return GROUP_MANAGER_GROUPS_OVERFLOW;
    }
    if(QueueRemove(_groMan->m_IPQueue, (void**)&IPAddr) != QUEUE_SUCCESS)
    {
        return GROUP_MANAGER_QUEUE_ERROR;
    }
    if((group = CreateGroup(IPAddr, _grName)) == NULL)
    {
        return GROUP_MANAGER_ERR_ALLOCATION_FAILED;
    }
    groupName = (char*)malloc(strlen(_grName) + 1);
    strcpy(groupName, _grName);
    if((res = HashMap_Insert(_groMan->m_groupsHashMap, groupName, group)) == MAP_KEY_DUPLICATE_ERROR)
    {
        QueueInsert(_groMan->m_IPQueue, &IPAddr);
        free(groupName);
        free(group);
        return GROUP_MANAGER_DUPLICATE_GROUPNAME;
    }
    else if( res != MAP_SUCCESS)
    {
        free(groupName);
        free(group);
        return GROUP_MANAGER_HASH_ERR_ALLOCATION_FAILED;
    }
    _groMan->m_groupsCount++;
    return GROUP_MANAGER_SUCCESS;
}

GroupManagerResult GroManRemoveUser(GroupManager *_groMan, char _grName[])
{
    char *groupName;
    Group *group;
    if(CheckParameter(_groMan, _grName) != GROUP_MANAGER_SUCCESS)
    {
        return GROUP_MANAGER_NOT_INITIALIZED;
    }
    if(HashMap_Find(_groMan->m_groupsHashMap, _grName, (void **)&group) == MAP_KEY_NOT_FOUND_ERROR)
    {
        return GROUP_MANAGER_INVALID_GROUPNAME;
    }
    if(GroupRemoveUser(group) == FAIL)
    {
        return GROUP_MANAGER_REMOVE_USER_FAILED;
    }
    if(GroupUsersCount(group) == 0)
    {
        QueueInsert(_groMan->m_IPQueue, GroupIP(group));
        HashMap_Remove(_groMan->m_groupsHashMap, _grName, (void**)&groupName, (void**)&group);
        DestroyGroupName(groupName);
        DestroyGroup(group);
        _groMan->m_groupsCount--;
        return GROUP_MANAGER_GROUP_REMOVED;
    }
    return GROUP_MANAGER_SUCCESS;
}

GroupManagerResult GroManAddUser(GroupManager *_groMan, char _grName[])
{
    Group *group;
    if(CheckParameter(_groMan, _grName) != GROUP_MANAGER_SUCCESS)
    {
        return GROUP_MANAGER_NOT_INITIALIZED;
    }
    if(HashMap_Find(_groMan->m_groupsHashMap, _grName, (void **)&group) == MAP_KEY_NOT_FOUND_ERROR)
    {
        return GROUP_MANAGER_INVALID_GROUPNAME;
    }
    if(GroupAddUser(group) == FAIL)
    {
        return GROUP_MANAGER_ADD_USER_FAILED;
    }
    return GROUP_MANAGER_SUCCESS;
}

GroupManagerResult GroupManagerGetGroupInfo(GroupManager *_groMan, char _grName[], char _IPAddr[], char _port[])
{
    Group *group;
    if(CheckParameterGetGroupInfo(_groMan, _grName, _IPAddr, _port) != GROUP_MANAGER_SUCCESS)
    {
        return GROUP_MANAGER_NOT_INITIALIZED;
    }
    if(HashMap_Find(_groMan->m_groupsHashMap, _grName, (void **)&group) == MAP_KEY_NOT_FOUND_ERROR)
    {
        return GROUP_MANAGER_INVALID_GROUPNAME;
    }
    strcpy(_IPAddr, GroupIP(group));
    strcpy(_port, GroupPort(group));
    return GROUP_MANAGER_SUCCESS; 
}

GroupManagerResult GroupManagerGetGroupsNames(GroupManager *_groMan, Vector *_vector)
{
    size_t result;
    if(_groMan == NULL || _vector == NULL)
    {
        return GROUP_MANAGER_NOT_INITIALIZED;
    }
    if(_groMan->m_groupsCount == 0)
    {
        return GROUP_MANAGER_EMPTY;
    }
    result = HashMap_ForEach(_groMan->m_groupsHashMap, GetGroupName, _vector);
    if(result != HashMap_Size(_groMan->m_groupsHashMap))
    {
        return GROUP_MANAGER_GET_GROUPS_NAMES_FAILED;
    }
    return GROUP_MANAGER_SUCCESS;
}

static size_t HashFunc(const void *_key)
{
    char groupName[GROUP_NAME_SIZE_LIMIT];
    int i = 0, count = 0;
    strncpy(groupName, (char*)_key, GROUP_NAME_SIZE_LIMIT);
    while(groupName[i] != '\0')
    {
        count += groupName[i] * (i + 1);
        ++i;
    }
    return count;
}

static int EqualityFunc(void *_firstKey, void *_secondKey)
{
    if(strcmp((char*)_firstKey, (char*)_secondKey) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

static void DestroyIPAddr(void *_elem)
{
    free(_elem);
}

static void DestroyGroupName(void *_elem)
{
    free(_elem);
}

static int AddMultiCastIP(Queue *_queue)
{
    int i;
    char ipName[] = IP_MULTICAST;
    char *IPAddr[NUM_OF_IP_ADDR];
    for(i=0; i<NUM_OF_IP_ADDR; i++)
    {
        if((IPAddr[i] = (char*)malloc(IP_ADDR_SIZE_LIMIT)) == NULL)
        {
            return FAIL;
        }
        strcpy(IPAddr[i], ipName);
        CalculateIP(IPAddr[i], i);
        QueueInsert(_queue, IPAddr[i]);
    }
    return SUCCESS;
}

static void CalculateIP(char _ip[], int _num)
{
    sprintf(&_ip[8], "%d", _num);
}

static int GetGroupName(const void *_key, void *_value, void *_context)
{
    VectorResult result;
    result = VectorAppend((Vector*)_context, (char*)_key);
    if(result != VECTOR_SUCCESS)
    {
        return 0;
    }
    return 1;
}

static GroupManagerResult CheckParameter(GroupManager *_groMan, char _grName[])
{
    if(_groMan == NULL || _grName == NULL)
    {
        return GROUP_MANAGER_NOT_INITIALIZED;
    }
    return GROUP_MANAGER_SUCCESS;
}

static GroupManagerResult CheckParameterGetGroupInfo(GroupManager *_groMan, char _grName[],char _IPAddr[], char _port[])
{
    if(_groMan == NULL || _grName == NULL || _IPAddr == NULL || _port == NULL)
    {
        return GROUP_MANAGER_NOT_INITIALIZED;
    }
    return GROUP_MANAGER_SUCCESS;
}
