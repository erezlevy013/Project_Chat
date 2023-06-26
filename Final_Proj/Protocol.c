#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <stddef.h> /* NULL */
#include <string.h> /* strncpy */
#include <sys/types.h> /* ssize_t*/
#include "Protocol.h"
#include "Limits.h"
#include "GenVector.h"

#define INDX_TAG_MSG 0
#define INDX_LEN_MSG 1
#define INDX_LEN_DATA 2
#define THREE_BYTES_JUMP 3
#define FOUR_BYTES_JUMP 4

static void PackTwoParameters(char _str1[], char _str2[], char _buffer[]);

static void PackParameters(char _str[], char _buffer[]);

/* REGISTRATION */

size_t RegReqPack(char _usName[], char _pass[], char _buffer[])
{
    PackTwoParameters(_usName,_pass, _buffer);
    _buffer[INDX_TAG_MSG] = REGISTRATION_REQUEST;
    return _buffer[INDX_LEN_MSG];
}

int RegReqUnPack(char _buffer[], char _usName[], char _pass[], ssize_t _bufferSize)  
{
    if(_bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_usName, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    strncpy(_pass, &_buffer[_buffer[INDX_LEN_DATA] + FOUR_BYTES_JUMP], _buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP]);
    _usName[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    _pass[(int)(_buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP])] = '\0';
    return SUCCESS;
}

size_t RegRepPack(char _reply[], char _buffer[])
{
    _buffer[INDX_TAG_MSG] = REGISTRATION_REPLY;
    PackParameters(_reply,_buffer);
    return _buffer[INDX_LEN_MSG]; 
}

int RegRepUnPack(char _buffer[], char _reply[], ssize_t _bufferSize) 
{
    if(_buffer[INDX_TAG_MSG] != REGISTRATION_REPLY || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_reply, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    _reply[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    return SUCCESS;
}

/* LOGIN */

size_t LogInReqPack(char _usName[], char _pass[] , char _buffer[])
{
    PackTwoParameters(_usName,_pass , _buffer);
    _buffer[INDX_TAG_MSG] = LOGIN_REQUEST;
    return  _buffer[INDX_LEN_MSG];
}

int LogInReqUnPack(char _buffer[], char _usName[], char _pass[], ssize_t _bufferSize)  
{
    if(_buffer[INDX_TAG_MSG] != LOGIN_REQUEST || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_usName, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    strncpy(_pass, &_buffer[_buffer[INDX_LEN_DATA] + FOUR_BYTES_JUMP], _buffer[_buffer[INDX_LEN_DATA]+ THREE_BYTES_JUMP]);
    _usName[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    _pass[(int)(_buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP])] = '\0';
    return SUCCESS;
}

size_t LogInRepPack(char _reply[], char _buffer[])
{
    _buffer[INDX_TAG_MSG] = LOGIN_REPLY;
    PackParameters(_reply,_buffer);
    return  _buffer[INDX_LEN_MSG]; 
}

int LogInRepUnPack(char _buffer[], char _reply[], ssize_t _bufferSize)  
{
    if(_buffer[INDX_TAG_MSG] != LOGIN_REPLY || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_reply, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    _reply[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    return SUCCESS;
}

/* LOGOUT */

size_t LogOutReqPack(char _usName[],  char _buffer[])
{
    _buffer[INDX_TAG_MSG] = LOGOUT_REQUEST;
    PackParameters(_usName,_buffer);
    return  _buffer[INDX_LEN_MSG]; 
}

int LogOutReqUnPack(char _buffer[], char _usName[], ssize_t _bufferSize)  
{
    if( _buffer[INDX_TAG_MSG] != LOGOUT_REQUEST || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_usName, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    _usName[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    return SUCCESS;
}

size_t LogOutRepPack(char _reply[], char _buffer[])
{
    _buffer[INDX_TAG_MSG] = LOGOUT_REPLY;
    PackParameters(_reply,_buffer);
    return _buffer[INDX_LEN_MSG]; 
}

int LogOutRepUnPack(char _buffer[], char _reply[], ssize_t _bufferSize)  
{
    if(_buffer[INDX_TAG_MSG] != LOGOUT_REPLY || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_reply, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    _reply[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    return SUCCESS;
}

/* CREATE GROUP */

size_t CreateGroupReqPack(char _usName[], char _groName[],  char _buffer[])
{
    _buffer[INDX_TAG_MSG] = CREATE_GROUP_REQUEST;
    PackTwoParameters(_usName,_groName , _buffer);
    return _buffer[INDX_LEN_MSG]; 
}

int CreateGroupReqUnPack(char _buffer[], char _usName[], char _groName[], ssize_t _bufferSize)  
{
    if(_buffer[INDX_TAG_MSG] != CREATE_GROUP_REQUEST || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_usName, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    strncpy(_groName, &_buffer[_buffer[INDX_LEN_DATA] + FOUR_BYTES_JUMP], _buffer[_buffer[INDX_LEN_DATA]+ THREE_BYTES_JUMP]);
    _usName[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    _groName[(int)(_buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP])] = '\0';
    return SUCCESS;
}

size_t CreateGroupRepPack(char _IPAddr[], char _port[], char _buffer[]) 
{
    _buffer[INDX_TAG_MSG] = CREATE_GROUP_REPLY;
    PackTwoParameters(_IPAddr, _port , _buffer);
    return _buffer[INDX_LEN_MSG]; 
}

int CreateGroupRepUnPack(char _buffer[], char _IPAddr[], char _port[], ssize_t _bufferSize)  
{
    if(_buffer[INDX_TAG_MSG] != CREATE_GROUP_REPLY || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_IPAddr, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    strncpy(_port, &_buffer[_buffer[INDX_LEN_DATA] + FOUR_BYTES_JUMP], _buffer[_buffer[INDX_LEN_DATA]+ THREE_BYTES_JUMP]);
    _IPAddr[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    _port[(int)(_buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP])] = '\0';
    return SUCCESS;
}

/* JOIN GROUP */

size_t JoinGroupReqPack(char _usName[], char _groName[],  char _buffer[])
{
    _buffer[INDX_TAG_MSG] = JOIN_GROUP_REQUEST;
    PackTwoParameters(_usName, _groName, _buffer);
    return _buffer[INDX_LEN_MSG]; 
}

int JoinGroupReqUnPack(char _buffer[], char _usName[], char _groName[], ssize_t _bufferSize)  
{
    if(_buffer[INDX_TAG_MSG] != JOIN_GROUP_REQUEST || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_usName, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    strncpy(_groName, &_buffer[_buffer[INDX_LEN_DATA] + FOUR_BYTES_JUMP], _buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP]);
    _usName[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    _groName[(int)(_buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP])] = '\0';
    return SUCCESS;
}

size_t JoinGroupRepPack(char _IPAddr[], char _port[], char _buffer[]) 
{
    _buffer[INDX_TAG_MSG] = JOIN_GROUP_REPLY;
    PackTwoParameters(_IPAddr, _port, _buffer);
    return _buffer[INDX_LEN_MSG]; 
}

int JoinGroupRepUnPack(char _buffer[], char _IPAddr[], char _port[], ssize_t _bufferSize)  
{
    if(_buffer[INDX_TAG_MSG] != JOIN_GROUP_REPLY || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_IPAddr, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    strncpy(_port, &_buffer[_buffer[INDX_LEN_DATA] + FOUR_BYTES_JUMP], _buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP]);
    _IPAddr[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    _port[(int)(_buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP])] = '\0';
    return SUCCESS;
}

/* LEAVE GROUP */

size_t LeaveGroupReqPack(char _usName[], char _groName[],  char _buffer[])
{
    _buffer[INDX_TAG_MSG] = LEAVE_GROUP_REQUEST;
    PackTwoParameters(_usName, _groName , _buffer);
    return  _buffer[INDX_LEN_MSG]; 
}

int LeaveGroupReqUnPack(char _buffer[], char _usName[], char _groName[], ssize_t _bufferSize)  
{
    if(_buffer[INDX_TAG_MSG] != LEAVE_GROUP_REQUEST || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_usName, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    strncpy(_groName, &_buffer[_buffer[INDX_LEN_DATA] + FOUR_BYTES_JUMP], _buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP]);
    _usName[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    _groName[(int)(_buffer[_buffer[INDX_LEN_DATA] + THREE_BYTES_JUMP])] = '\0';
    return SUCCESS;
}

size_t LeaveGroupRepPack(char _reply[], char _buffer[])
{
    _buffer[INDX_TAG_MSG] = LEAVE_GROUP_REPLY;
    PackParameters(_reply ,_buffer);
    return _buffer[INDX_LEN_MSG]; 
}

int LeaveGroupRepUnPack(char _buffer[], char _reply[], ssize_t _bufferSize)  
{
    if(_buffer[INDX_TAG_MSG] != LEAVE_GROUP_REPLY || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_reply, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    _reply[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    return SUCCESS;
}

/* GET GROUPS */

size_t GetGroupsReqPack(char _buffer[])
{
    _buffer[INDX_TAG_MSG] = GET_GROUPS_REQUEST;
    return INDX_LEN_MSG; 
}

size_t GetGroupsRepPack(Vector *_vec,  char _buffer[])         /* TDB: need to add code that handles a msg that is too long to pass once */
{
    int count = 3, sizeLen;
    char *val;
    _buffer[INDX_TAG_MSG] = GET_GROUPS_REPLY;
    _buffer[INDX_LEN_DATA] = VectorSize(_vec);
    _buffer[INDX_LEN_MSG] = count;
    while(VectorSize(_vec) != 0)
    {   
        VectorRemove(_vec, (void**)&val);
        sizeLen = strlen(val);
        _buffer[count] = sizeLen;
        strncpy(&_buffer[count + 1], val, sizeLen);
        count += sizeLen + 1;
    }
    _buffer[INDX_LEN_MSG] = count;
    _buffer[count] = '\0';
    return _buffer[INDX_LEN_MSG];
}

int GetGroupsRepUnPack( char _buffer[], Vector *_vec, ssize_t _bufferSize)    /* TDB: need to add code that handles a msg that is too long to pass once */
{
    int size = _buffer[INDX_LEN_DATA], count = 4, i = 0;
    char *val[NUM_OF_GROUPS_LIMIT];
    if(_buffer[INDX_LEN_MSG] != _bufferSize)
    {
        return FAIL;
    }
    while(size > 0)
    {
        if((val[i] = (char*)malloc(GROUP_NAME_SIZE_LIMIT + 1)) == NULL)
        {
            return FAIL;
        }
        strncpy(val[i], &_buffer[count], _buffer[count - 1]);
        val[i][(int)(_buffer[count - 1])] = '\0';
        VectorAppend(_vec, val[i]);
        count += _buffer[count - 1] + 1;
        i++;
        size--;
    }
    return SUCCESS;
}

size_t GetGroupsRepFailPack(char _reply[], char _buffer[])
{
    _buffer[INDX_TAG_MSG] = FAIL_REPLY;
    PackParameters(_reply,_buffer);
    return _buffer[INDX_LEN_MSG]; 
}

int GetGroupsRepFailUnpack(char _buffer[], char _reply[], ssize_t _bufferSize)
{
    if( _buffer[INDX_TAG_MSG] != FAIL_REPLY || _bufferSize != _buffer[INDX_LEN_MSG])
    {
        return FAIL;
    }
    strncpy(_reply, &_buffer[THREE_BYTES_JUMP], _buffer[INDX_LEN_DATA]);
    _reply[(int)(_buffer[INDX_LEN_DATA])] = '\0';
    return SUCCESS;
}

/* TAG TYPE */

ProReqType TagReqType(char _s[])
{
    return _s[INDX_TAG_MSG];
}

ProRepType TagRepType(char _s[])
{
    return _s[INDX_TAG_MSG];
}

/* PARAMETERS CHECK */

static void PackTwoParameters(char _str1[], char _str2[], char _buffer[])
{
    unsigned char nameLen ,passLen;
    nameLen = strlen(_str1);
    passLen = strlen(_str2);
    _buffer[INDX_LEN_MSG] = nameLen + passLen + FOUR_BYTES_JUMP;
    _buffer[INDX_LEN_DATA] = nameLen;
    strcpy(&_buffer[THREE_BYTES_JUMP], _str1);
    _buffer[nameLen + THREE_BYTES_JUMP] = passLen;
    strcpy(&_buffer[nameLen + FOUR_BYTES_JUMP], _str2);
}

static void PackParameters(char _str[],  char _buffer[])
{
    _buffer[INDX_LEN_MSG] = strlen(_str) + THREE_BYTES_JUMP;
    _buffer[INDX_LEN_DATA] = strlen(_str);
    strcpy(&_buffer[THREE_BYTES_JUMP], _str);
}