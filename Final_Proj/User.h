#ifndef __USER_H__
#define __USER_H__

#include "GenList.h"

typedef struct User User;

/** 
 * @brief Creates a user
 * 
 * @param _userName : the name of the user
 * @param _password : the password of the user
 * @returns a pointer to the created user, NULL on failure
 */
User* CreateUser(char _userName[], char _password[]);

/** 
 * @brief Turns the user to active mode
 * 
 * @param _user : A pointer to previously created user returned via CreateUser
 * @returns SUCCESS on success, FAIL on failure
 */
int ActivateUser(User *_user);

/** 
 * @brief Turns the user to inactive mode
 * 
 * @param _user : A pointer to previously created user returned via CreateUser
 * @returns SUCCESS on success, FAIL on failure
 */
int InactivateUser(User *_user);

/** 
 * @brief Adds the user to a group
 * 
 * @param _user : A pointer to previously created user returned via CreateUser
 * @param _groupName : the name of the group to which the user wishes to join
 * @returns SUCCESS on success, errors on failure
 * @retval DUPLICATE if user is already a part of the given group
 * @retval FAIL if either given pointer is NULL, if failed to allocate memory, or if failed to update the list of groups the user takes part in
 */
int AddUserToGroup(User *_user, char _groupName[]);

/** 
 * @brief Removes the user from a group
 * 
 * @param _user : A pointer to previously created yser returned via CreateUser
 * @param _groupName : the name of the group the user has left
 * @returns SUCCESS on success, errors on failure
 * @retval NOT_FOUND if the user is not a member of the given group
 * @retval FAIL if either given pointer is NULL, or if failed to update the list of groups the user takes part in
 */
int RemoveUserFromGroup(User *_user, char _groupName[]);

/** 
 * @brief Returns a list containing all the group names in which the user takes part
 * 
 * @param _user : A pointer to previously created user returned via CreateUser
 * @returns pointer to a list on success, NULL on failure
 */
List* UserGroups(User *_user);

/** 
 * @brief Returns the password of the user
 * 
 * @param _user : A pointer to previously created user returned via CreateUser
 * @returns pointer to a string on success, NULL on failure
 */
char* GetUserPassword(User *_user);

/** 
 * @brief Destroys the user
 * @details Destroys the user completely 
 *
 * @param _user : A pointer to previously created user returned via CreateUser
 */
void DestroyUser(User *_user);


#endif /* __USER_H__ */