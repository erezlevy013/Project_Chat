#ifndef __GROUP_MANAGER_H__
#define __GROUP_MANAGER_H__

#include "GenVector.h"

typedef struct GroupManager GroupManager;

typedef enum
{
    GROUP_MANAGER_SUCCESS = 0,
    GROUP_MANAGER_NOT_INITIALIZED,
    GROUP_MANAGER_ERR_ALLOCATION_FAILED,
    GROUP_MANAGER_HASH_ERR_ALLOCATION_FAILED,
    GROUP_MANAGER_DUPLICATE_GROUPNAME,
    GROUP_MANAGER_INVALID_GROUPNAME,
    GROUP_MANAGER_GROUPS_OVERFLOW,
    GROUP_MANAGER_QUEUE_EMPTY,
    GROUP_MANAGER_REMOVE_USER_FAILED,
    GROUP_MANAGER_ADD_USER_FAILED,
    GROUP_MANAGER_GET_GROUPS_NAMES_FAILED,
    GROUP_MANAGER_GROUP_REMOVED,
    GROUP_MANAGER_QUEUE_ERROR,
    GROUP_MANAGER_EMPTY
} GroupManagerResult;

/**
 * @brief Creates a group manager
 * 
 * @returns a pointer to the created group manager, NULL on failure
 */
GroupManager* CreateGroupManager(void);

/** 
 * @brief Destroys the group manager
 * @details Destroys the group manager completely 
 *
 * @param _userManager : A pointer to previously created group manager returned via CreateGroupManager
 */
void DestroyGroupManager(GroupManager *_groMan);

/** 
 * @brief Create a new group
 * 
 * @param _groMan : A pointer to previously created group manager returned via CreateGroupManager
 * @param _grName : the name of the group to be created
 * @returns GROUP_MANAGER_SUCCESS on success, errors on failure
 * @retval GROUP_MANAGER_NOT_INITIALIZED if either given pointer is null
 * @retval GROUP_MANAGER_GROUPS_OVERFLOW if the max number of groups was already reached
 * @retval GROUP_MANAGER_QUEUE_ERROR if queue is empty
 * @retval GROUP_MANAGER_ERR_ALLOCATION_FAILED if failed to allocate memory
 * @retval GROUP_MANAGER_DUPLICATE_GROUPNAME if group name is already taken
 * @retval GROUP_MANAGER_HASH_ERR_ALLOCATION_FAILED if hash map failed to allocated memory
 */
GroupManagerResult AddNewGroup(GroupManager *_groMan, char _grName[]);

/** 
 * @brief Removes user from group
 * 
 * @param _groMan : A pointer to previously created group manager returned via CreateGroupManager
 * @param _grName : the name of the group from which the user will be removed
 * @returns GROUP_MANAGER_SUCCESS on success, errors on failure
 * @retval GROUP_MANAGER_NOT_INITIALIZED if either given pointer is null
 * @retval GROUP_MANAGER_INVALID_GROUPNAME if the group does not exist
 * @retval GROUP_MANAGER_REMOVE_USER_FAILED if removeing the user from the group failed
 * @retval GROUP_MANAGER_GROUP_REMOVED if the group was closed due to having no members
 */
GroupManagerResult GroManRemoveUser(GroupManager *_groMan, char _grName[]);

/** 
 * @brief Adds a user to the group
 * 
 * @param _groMan : A pointer to previously created group manager returned via CreateGroupManager
 * @param _grName : the name of the group to which the user will be added
 * @returns GROUP_MANAGER_SUCCESS on success, errors on failure
 * @retval GROUP_MANAGER_NOT_INITIALIZED if either given pointer is null
 * @retval GROUP_MANAGER_INVALID_GROUPNAME if the group does not exist
 * @retval GROUP_MANAGER_ADD_USER_FAILED if adding the user to the group failed
 */
GroupManagerResult GroManAddUser(GroupManager *_groMan, char _grName[]);

/** 
 * @brief Returns the IP address and port number of the given group
 * 
 * @param _groMan : A pointer to previously created group manager returned via CreateGroupManager
 * @param _grName : the name of the group
 * @param _IPAddr : empty buffer to contain the IP address
 * @param _port : empty buffer to contain the port number
 * @returns GROUP_MANAGER_SUCCESS on success, errors on failure
 * @retval GROUP_MANAGER_NOT_INITIALIZED if either given pointer is null
 * @retval GROUP_MANAGER_INVALID_GROUPNAME if the group does not exist
 */
GroupManagerResult GroupManagerGetGroupInfo(GroupManager *_groMan, char _grName[], char _IPAddr[], char _port[]);

/** 
 * @brief Returns a vector with all the existing groups' names
 * 
 * @param _groMan : A pointer to previously created group manager returned via CreateGroupManager
 * @param _vector : A pointer to a vector
 * @returns GROUP_MANAGER_SUCCESS on success, errors on failure
 * @retval GROUP_MANAGER_NOT_INITIALIZED if either given pointer is null
 */
GroupManagerResult GroupManagerGetGroupsNames(GroupManager *_groMan, Vector *_vector);

#endif /* __GROUP_MANAGER_H__ */