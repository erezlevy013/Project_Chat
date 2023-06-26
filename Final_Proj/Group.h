#ifndef __GROUP_H__
#define __GROUP_H__

typedef struct Group Group;

/** 
 * @brief Creates a group
 * @param _ip : the IP address of the group
 * @param _grName : the name of the group
 * @returns a pointer to the created group, NULL on failure
 */
Group* CreateGroup(char _ip[], char _grName[]);

/** 
 * @brief Destroys the group
 * 
 * @param _group : A pointer to previously created group returned via CreateGroup
 */
void DestroyGroup(Group *_group);

/** 
 * @brief Adds the user to the given group
 * @param _group : A pointer to previously created group returned via CreateGroup
 * @returns SUCCESS on success, FAIL on failure
 */
int GroupAddUser(Group *_group);

/** 
 * @brief Removes the user from the given group
 * @param _group : A pointer to previously created group returned via CreateGroup
 * @returns SUCCESS on success, FAIL on failure
 */
int GroupRemoveUser(Group *_group);

/** 
 * @brief Returns the number of users in the given group
 * @param _group : A pointer to previously created group returned via CreateGroup
 * @returns number of users on success, -1 on failure
 */
int GroupUsersCount(Group *_group);

/** 
 * @brief returns the IP address of the given group
 * @param _group : A pointer to previously created group returned via CreateGroup
 * @returns IP address on success, NULL on failure
 */
char* GroupIP(Group *_group);

/** 
 * @brief returns the port number of the given group
 * @param _group : A pointer to previously created group returned via CreateGroup
 * @returns port on success, NULL on failure
 */
char* GroupPort(Group *_group);

#endif /* __GROUP_H__ */