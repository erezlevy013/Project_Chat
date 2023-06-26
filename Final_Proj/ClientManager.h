#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

typedef struct ClientManager ClientManager;

typedef struct Group Group;

/**
 * @brief Creates a client manager to manage a single client interface.
 * @returns pointer to the created client manager, NULL on failure.
 */
ClientManager *CreateClientManager(void);

/**
 * @brief Handles the client's requests through contact with the server (possible requests: register, login, logout, create groups, join groups, leave groups)
 * @param _clientManager : A pointer to previously created client manager returned via CreateClientManager
 * @returns SUCCESS on success, FAIL on failure.
 */
int RunClientManager(ClientManager *_clientManager);

/**
 * @brief Destroys the client manager
 * @details Destroys the client manager completely 
 * @param _clientManager : A pointer to previously created client manager returned via CreateClientManager
 */
void DestroyClientManager(ClientManager *_clientManager);

#endif /* __CLIENTMANAGER_H__ */