#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__

#include <sys/types.h> /* ssize_t */
#include <unistd.h> /* ssize_t */
#include "ServerNet.h"

typedef struct ServerManager ServerManager;

/** 
 * @brief Creates a server manager
 *
 * @returns a pointer to the created server manager, NULL on failure
 */
ServerManager* CreateServerManager(void);

/** 
 * @brief Runs the given server manager
 *
 * @param _serverManager : A pointer to previously created server manager returned via CreateServerManager
 * @returns SUCCESS on success, FAIL on failure
 */
int RunServerManager(ServerManager *_serverManager);

/** 
 * @brief Destroys the server manager
 * @details Destroys the server manager completely 
 *
 * @param _serverManager : A pointer to previously created server manager returned via CreateServerManager
 */
void DestroyServerManager(ServerManager *_serverManager);


#endif /* __SERVERMANAGER_H__ */