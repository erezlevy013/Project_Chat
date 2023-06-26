#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include "Listener.h"
#include "Limits.h"

/*
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
*/
#define INDX_TOTAL_MSG 0
#define INDX_LEN_NICK 1
#define TWO_JUMP_BYTES 2
#define THREE_JUMP_BYTES 3

static int MsgUnPack(char _buffer[], char _nickname[], char _msg[], int _bufferSize);
static void UnEncrypt(char _buffer[]);

int Listener(char _IP[], int _port)
{
    int sock, optval = 1, readBytes,t;
    char recvMsg[BUFFER_SIZE_LIMIT], nickName[USERNAME_SIZE_LIMIT], msg[MESSAGE_SIZE_LIMIT];
    unsigned int sinLen;
    struct sockaddr_in sin;
    struct ip_mreq ipReq;
    system("echo -n '\033]11;rgb:ce/7a/79\a'");
    sinLen = sizeof(sin);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        return FAIL;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        close(sock);
        return FAIL;
    }
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr =inet_addr(_IP);
    sin.sin_port = htons(_port);
    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        return FAIL;
    }
    ipReq.imr_multiaddr.s_addr = inet_addr(_IP);
    ipReq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&ipReq, sizeof(ipReq)) < 0)
    {
        return FAIL;
    }

    while (TRUE)
    {
        readBytes = recvfrom(sock, recvMsg, BUFFER_SIZE_LIMIT, 0, (struct sockaddr *)&sin, &sinLen);
        if (readBytes < 0)
        {
            return FAIL;
        }
        UnEncrypt(recvMsg);
        MsgUnPack(recvMsg, nickName, msg, readBytes);
        printf("\x1b[1m%s:\033[m %s", nickName, msg);
    }
    return SUCCESS;
}

int main(int argc, char *argv[])
{
    char ip[12];
    int pID, port;
    FILE *PIDFile = NULL;
    if (argc != 3)
    {
        return 0;
    }
    strcpy(ip, argv[1]);
    port = atoi(argv[2]);
    pID = getpid();
    if ((PIDFile = fopen("ListenerPID.txt", "w")) == NULL)
    {
        return 0;
    }
    fprintf(PIDFile, "%d\n", pID);
    fclose(PIDFile);
    if (Listener(ip, port)== FAIL)
    {
        return FAIL;
    }
    return 0;
}

static int MsgUnPack(char _buffer[], char _nickname[], char _msg[], int _bufferSize)
{
    if (_bufferSize != _buffer[0])
    {
        return FAIL;
    }
    strncpy(_nickname, &_buffer[2], _buffer[INDX_LEN_NICK]);
    strncpy(_msg, &_buffer[_buffer[INDX_LEN_NICK] + THREE_JUMP_BYTES], _buffer[_buffer[INDX_LEN_NICK] + TWO_JUMP_BYTES]);
    _nickname[(int)(_buffer[INDX_LEN_NICK])] = '\0';
    _msg[(int)(_buffer[_buffer[INDX_LEN_NICK] + TWO_JUMP_BYTES])] = '\0';
    return SUCCESS;
}

static void UnEncrypt(char _buffer[])
{
    int i = 0;
    while(_buffer[i] != '\0')
    {
        _buffer[i] = ~_buffer[i]; 
        i++;
    }
}




