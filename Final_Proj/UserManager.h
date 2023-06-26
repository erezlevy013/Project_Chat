#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__

#include "GenList.h"

typedef enum
{
    USER_MANAGER_SUCCESS = 0,
    USER_MANAGER_NOT_INITIALIZED,
    USER_MANAGER_ERR_ALLOCATION_FAILED,
    USER_MANAGER_HASH_ERR_ALLOCATION_FAILED,
    USER_MANAGER_ERR_DUPLICATE_USERNAME,
    USER_MANAGER_ERR_INVALID_USERNAME,
    USER_MANAGER_ERR_INVALID_PASSWORD,
    USER_MANAGER_ERR_LOGIN_FAILED,
    USER_MANAGER_ERR_LOGOUT_FAILED,
    USER_MANAGER_ERR_JOIN_GROUP_DUPLICATE,
    USER_MANAGER_ERR_JOIN_GROUP_FAILED,
    USER_MANAGER_ERR_LEAVE_GROUP_NOT_FOUND,
    USER_MANAGER_ERR_LEAVE_GROUP_FAILED,
    USER_MANAGER_ERR_USERS_OVERFLOW,
    USER_MANAGER_ERR_USER_FILE
} UserManagerResult;

typedef struct UserManager UserManager;

/**
 * @brief Creates a user manager
 * 
 * @returns a pointer to the created user manager, NULL on failure
 */
UserManager* CreateUserManager(void);

/** 
 * @brief Registers a user
 * 
 * @param _userManager : A pointer to previously created user manager returned via CreateUserManager
 * @param _userName : the name of the user
 * @param _password : the password of the user
 * @returns USER_MANAGER_SUCCESS on success, errors on failure
 * @retval USER_MANAGER_NOT_INITIALIZED if either given pointer is null
 * @retval USER_MANAGER_ERR_ALLOCATION_FAILED if failed to allocate memory
 * @retval USER_MANAGER_DUPLICATE_USERNAME if username is already taken
 * @retval USER_MANAGER_HASH_ERR_ALLOCATION_FAILED if hash map failed to allocated memory
 * @retval USER_MANAGER_ERR_USER_FILE if failed to save the info of the user to a file
 */
UserManagerResult RegisterUser(UserManager *_userManager, char _userName[], char _password[]);

/** 
 * @brief Logs a user in
 * 
 * @param _userManager : A pointer to previously created user manager returned via CreateUserManager
 * @param _userName : the name of the user
 * @param _password : the password of the user
 * @returns USER_MANAGER_SUCCESS on success, errors on failure
 * @retval USER_MANAGER_NOT_INITIALIZED if either given pointer is null
 * @retval USER_MANAGER_ERR_INVALID_USERNAME if username is incorrect
 * @retval USER_MANAGER_ERR_INVALID_PASSWORD if password is incorrect
 * @retval USER_MANAGER_ERR_LOGIN_FAILED if failed to log the user in
 */
UserManagerResult LoginUser(UserManager *_userManager, char _userName[], char _password[]);

/** 
 * @brief Logs a user out
 * 
 * @param _userManager : A pointer to previously created user manager returned via CreateUserManager
 * @param _userName : the name of the user
 * @returns USER_MANAGER_SUCCESS on success, errors on failure
 * @retval USER_MANAGER_NOT_INITIALIZED if either given pointer is null
 * @retval USER_MANAGER_ERR_INVALID_USERNAME if username is incorrect
 * @retval USER_MANAGER_ERR_LOGOUT_FAILED if failed to log the user out
 */
UserManagerResult LogoutUser(UserManager *_userManager, char _userName[]);

/** 
 * @brief Adds the user into the chosen group
 * 
 * @param _userManager : A pointer to previously created user manager returned via CreateUserManager
 * @param _userName : the name of the user
 * @param _groupName : the name of the group the user will join
 * @returns USER_MANAGER_SUCCESS on success, errors on failure
 * @retval USER_MANAGER_NOT_INITIALIZED if either given pointer is null
 * @retval USER_MANAGER_ERR_INVALID_USERNAME if username is incorrect
 * @retval USER_MANAGER_ERR_JOIN_GROUP_DUPLICATE if user is already a part of the chosen group
 * @retval USER_MANAGER_ERR_JOIN_GROUP_FAILED if failed to add the user to the chosen group
 */
UserManagerResult JoinGroupUser(UserManager *_userManager, char _userName[], char _groupName[]);

/** 
 * @brief Removes the user from the given group
 * 
 * @param _userManager : A pointer to previously created user manager returned via CreateUserManager
 * @param _userName : the name of the user
 * @param _groupName : the name of the group from which the user will be removed
 * @returns USER_MANAGER_SUCCESS on success, errors on failure
 * @retval USER_MANAGER_NOT_INITIALIZED if either given pointer is null
 * @retval USER_MANAGER_ERR_INVALID_USERNAME if username is incorrect
 * @retval USER_MANAGER_ERR_LEAVE_GROUP_NOT_FOUND if user is not a part of the given group
 * @retval USER_MANAGER_ERR_LEAVE_GROUP_FAILED if failed to remove the user to the given group
 */
UserManagerResult LeaveGroupUser(UserManager *_userManager, char _userName[], char _groupName[]);

/** 
 * @brief Returns a list containing all the group names in which the user takes part
 * 
 * @param _userManager : A pointer to previously created user manager returned via CreateUserManager
 * @param _userName : the name of the user
 * @returns pointer to the list on success, NULL on failure
 */
List* GetGroupsUser(UserManager *_userManager, char _userName[]);

/** 
 * @brief Destroys the user manager
 * @details Destroys the user manager completely 
 *
 * @param _userManager : A pointer to previously created user manager returned via CreateUserManager
 */
void DestroyUserManager(UserManager *_userManager);

#endif /* __USERMANAGER_H__ */