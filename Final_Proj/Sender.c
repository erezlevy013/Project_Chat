#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Limits.h"
#include "Sender.h"

#define INDX_TOTAL_MSG 0
#define INDX_LEN_NICK 1
#define TWO_JUMP_BYTES 2
#define THREE_JUMP_BYTES 3

static int MsgPack(char _nickname[], char _msg[], char _buffer[]);
static void Encrypt(char _buffer[]);

int Sender(char _IP[], int _port, char _nickName[])
{
    int sock, optval = 1, readBytes, sinLen, sentBytes, sendBufferLen;
    char sendToBuffer[BUFFER_SIZE_LIMIT], msgBuffer[MESSAGE_SIZE_LIMIT];
    FILE *pidFile = NULL;
    struct sockaddr_in sin;
    system("echo -n '\033]10;rgb:ce/7a/79\a\033]11;rgb:fd/fa/fa\a'");
    sinLen = sizeof(sin);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        return FAIL;
    }
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(_IP);
    sin.sin_port = htons(_port);
    sinLen = sizeof(sin);
    while (1)
    {
        printf("\x1b[1mtype here:\033[m ");
        fgets(msgBuffer, MESSAGE_SIZE_LIMIT, stdin);
        sendBufferLen = MsgPack(_nickName, msgBuffer, sendToBuffer);
        Encrypt(sendToBuffer);
        sentBytes = sendto(sock, sendToBuffer, sendBufferLen, 0, (struct sockaddr *)&sin, sizeof(sin));
        if (sentBytes < 0)
        {
            return FAIL;
        }
    }
    return SUCCESS;
}

int main(int _argc, char *_argv[])
{
    char *ip, *nickName;
    int pID, port;
    FILE *pidFile = NULL;
    if (_argc != 4)
    {
        return 0;
    }
    ip = _argv[1];
    port = atoi(_argv[2]);
    nickName = _argv[3];
    pID = getpid();
    if ((pidFile = fopen("SenderPID.txt", "w")) == NULL)
    {
        return 0;
    }
    fprintf(pidFile, "%d\n", pID);
    fclose(pidFile);
    if (Sender(ip, port, nickName)== FAIL)
    {
        return FAIL;
    }
    return 0;
}

static int MsgPack(char _nickname[], char _msg[], char _buffer[])
{
    unsigned char nickNameLen, msgLen;
    nickNameLen = strlen(_nickname);
    msgLen = strlen(_msg);
    _buffer[INDX_TOTAL_MSG] = nickNameLen + msgLen + THREE_JUMP_BYTES;
    _buffer[INDX_LEN_NICK] = nickNameLen;
    strcpy(&_buffer[TWO_JUMP_BYTES], _nickname);
    _buffer[nickNameLen + TWO_JUMP_BYTES] = msgLen;
    strcpy(&_buffer[nickNameLen + THREE_JUMP_BYTES], _msg);
    return _buffer[INDX_TOTAL_MSG];
}

static void Encrypt(char _buffer[])
{
    int i = 0;
    while(_buffer[i] != '\0')
    {
        _buffer[i] = ~_buffer[i];
        i++;
    }
}