#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <sys/types.h> /* ssize_t */
#include "GenVector.h"

typedef enum 
{
	REGISTRATION_REQUEST = 1,
    LOGIN_REQUEST,
    CREATE_GROUP_REQUEST,
    GET_GROUPS_REQUEST,
    JOIN_GROUP_REQUEST,
    LEAVE_GROUP_REQUEST,
    LOGOUT_REQUEST
} ProReqType;

typedef enum
{
    REGISTRATION_REPLY = 1,
    LOGIN_REPLY,
    CREATE_GROUP_REPLY,
    GET_GROUPS_REPLY,
    JOIN_GROUP_REPLY,
    LEAVE_GROUP_REPLY,
    LOGOUT_REPLY,
    FAIL_REPLY
} ProRepType;

/** 
 * @brief Creates a Registration Request Package with given username and password
 * @param _usName : The name of the client.
 * @param _pass : The password of the client.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t RegReqPack(char _usName[], char _pass[] , char _buffer[]);

/** 
 * @brief Unpacks a Registration Request Package
 * @param _buffer : Package -> [T,L,Ln,'e','h','y',Lp,'1','2','3']
 * @param _usName : Empty buffer to contain the username.
 * @param _pass : Empty buffer to contain the password.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int RegReqUnPack(char _buffer[], char _usName[], char _pass[], ssize_t _bufferSize);

/** 
 * @brief Creates a Registration Reply Package with given reply
 * @param _reply : Reply from server.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t RegRepPack(char _reply[], char _buffer[]);

/** 
 * @brief Unpacks a Registration Reply Package
 * @param _buffer : Package -> [tag,length,datalen,'e','h','y']
 * @param _reply : Empty buffer to contain the reply.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int RegRepUnPack(char _buffer[], char _reply[], ssize_t _bufferSize);

/** 
 * @brief Creates a Login Request Package with given username and password
 * @param _usName : The name of the client.
 * @param _pass : The password of the client.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t LogInReqPack(char _usName[], char _pass[] , char _buffer[]);

/** 
 * @brief Unpacks a Login Request Package
 * @param _buffer : Package -> [T,L,Ln,'e','h','y',Lp,'1','2','3']
 * @param _usName : Empty buffer to contain the username.
 * @param _pass : Empty buffer to contain the password.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int LogInReqUnPack(char _buffer[], char _usName[], char _pass[], ssize_t _bufferSize);

/** 
 * @brief Creates a Login Reply Package with given reply
 * @param _reply : Reply from server.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t LogInRepPack(char _reply[], char _buffer[]);

/** 
 * @brief Unpacks a Login Reply Package
 * @param _buffer : Package -> [tag,length,datalen,'e','h','y']
 * @param _reply : Empty buffer to contain the reply.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int LogInRepUnPack(char _buffer[], char _reply[], ssize_t _bufferSize);

/** 
 * @brief Creates a Logout Request Package with given username
 * @param _usName : The name of the client.
 * @param _buffer : Empty buffer to contain the username.
 * @return the length of the package.
 */
size_t LogOutReqPack(char _usName[],  char _buffer[]);

/** 
 * @brief Unpacks a Logout Request Package
 * @param _buffer : Package -> [tag,length,datalen,'e','h','y']
 * @param _usName : Empty buffer to contain the username.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int LogOutReqUnPack(char _buffer[], char _usName[], ssize_t _bufferSize);

/** 
 * @brief Creates a Logout Reply Package with given reply
 * @param _reply : Reply from server.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t LogOutRepPack(char _reply[], char _buffer[]);

/** 
 * @brief Unpacks a Logout Reply Package
 * @param _buffer : Package -> [tag,length,datalen,'e','h','y']
 * @param _reply : Empty buffer to contain the reply.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int LogOutRepUnPack(char _buffer[], char _reply[], ssize_t _bufferSize);

/** 
 * @brief Creates a Create Group Request Package with given username and group name
 * @param _usName : the name of the client.
 * @param _groName : The name of the group.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t CreateGroupReqPack(char _usName[], char _groName[],  char _buffer[]);

/** 
 * @brief Unpacks a Create Group Request Package
 * @param _buffer : Package -> [T,L,Ln,'e','h','y',Lp,'1','2','3']
 * @param _usName : Empty buffer to contain the username.
 * @param _groName : Empty buffer to contain the group name.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int CreateGroupReqUnPack(char _buffer[], char _usName[], char _groName[], ssize_t _bufferSize);

/** 
 * @brief Creates a Create Group Reply Package with given IP address and port address
 * @param _IPAddr : An IP address.
 * @param _port: A port address.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t CreateGroupRepPack(char _IPAddr[], char _port[], char _buffer[]);

/** 
 * @brief Unpacks a Create Group Reply Package
 * @param _buffer : Package -> [tag,length,datalen,'e','h','y']
 * @param _IPAddr : Empty buffer to contain the IP address.
 * @param _port : Empty buffer to contain the port address.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int CreateGroupRepUnPack(char _buffer[], char _IPAddr[], char _port[], ssize_t _bufferSize);

/** 
 * @brief Creates a Get Groups Request Package
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t GetGroupsReqPack(char _buffer[]);

/** 
 * @brief Creates a Get Groups Reply Package with given vector
 * @param _vec : a vector with group names.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t GetGroupsRepPack(Vector *_vec,  char _buffer[]);

/** 
 * @brief Unpacks a Get Groups Reply Package
 * @param _buffer : Package -> [tag,length,datalen,'e','h','y']
 * @param _vec : Empty vector to contain the group names.
 *  @param _bufferSize : The length of the package.
 * @return FAIL if failed to allocate memory for the group names' strings
 * @return SUCCESS on success.
 */
int GetGroupsRepUnPack( char _buffer[], Vector *_vec, ssize_t _bufferSize);

/** 
 * @brief Creates a Get Groups Fail Reply Package with given reply
 * @param _reply : a string signifying the cause of failure.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t GetGroupsRepFailPack(char _reply[], char _buffer[]);

/** 
 * @brief Unpacks a GetGroups Fail Reply Package
 * @param _buffer : Package -> [tag,length,datalen,'e','h','y']
 * @param _reply : Empty buffer to contain the fail reply.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int GetGroupsRepFailUnpack(char _buffer[], char _reply[], ssize_t _bufferSize);

/** 
 * @brief Creates a Join Group Request Package with given username and group name
 * @param _usName : The name of the client.
 * @param _groName : The name of the group.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t JoinGroupReqPack(char _usName[], char _groName[], char _buffer[]);

/** 
 * @brief Unpacks a Join Group Request Package
 * @param _buffer : Package -> [T,L,Ln,'e','h','y',Lp,'1','2','3']
 * @param _usName : Empty buffer to contain the username.
 * @param _groName : Empty buffer to contain the group name.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int JoinGroupReqUnPack(char _buffer[], char _usName[], char _groName[], ssize_t _bufferSize);

/** 
 * @brief Creates a Join Group Request Package with given IP address and port address
 * @param _IPAddr : An IP address.
 * @param _port : A port address.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t JoinGroupRepPack(char _IPAddr[], char _port[], char _buffer[]);

/** 
 * @brief Unpacks a Join Group Request Package
 * @param _buffer : Package -> [tag,length,datalen,'e','h','y']
 * @param _IPAddr : Empty buffer to contain the IP address.
 * @param _port : Empty buffer to contain the port address.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int JoinGroupRepUnPack(char _buffer[], char _IPAddr[], char _port[], ssize_t _bufferSize);

/** 
 * @brief Creates a Leave Group Request Package with given username and group name
 * @param _usName : The name of the client.
 * @param _groName : The name of the group.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t LeaveGroupReqPack(char _usName[], char _groName[],  char _buffer[]);

/** 
 * @brief Unpacks a Leave Group Request Package
 * @param _buffer : Package -> [T,L,Ln,'e','h','y',Lp,'1','2','3']
 * @param _usName : Empty buffer to contain the username.
 * @param _groName : Empty buffer to contain the group name.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int LeaveGroupReqUnPack(char _buffer[], char _usName[], char _groName[], ssize_t _bufferSize);

/** 
 * @brief Creates a Leave Group Reply Package with given reply
 * @param _reply : Reply from server.
 * @param _buffer : Empty buffer for the package.
 * @return the length of the package.
 */
size_t LeaveGroupRepPack(char _reply[], char _buffer[]);

/** 
 * @brief Unpacks a Leave Group Reply Package
 * @param _buffer : Package -> [tag,length,datalen,'e','h','y']
 * @param _reply : Empty buffer.
 * @param _bufferSize : The length of the package.
 * @return FAIL if _bufferSize != length of the package.
 * @return SUCCESS on success.
 */
int LeaveGroupRepUnPack(char _buffer[], char _reply[], ssize_t _bufferSize);

/** 
 * @brief Returns the tag of the given request
 * @param _s : package -> [tag,length,datalen,'e','h','y']
 * @return the tag of the given request
 */
ProReqType TagReqType(char _s[]);

/** 
 * @brief Returns the tag of the given reply
 * @param _s : Package -> [tag,length,datalen,'e','h','y']
 * @return the tag of the given reply.
 */
ProRepType TagRepType(char _s[]);

#endif /* __PROTOCOL_H__ */