#include <stdio.h>  /* printf, scanf */
#include <string.h> /* strlen */
#include <stdlib.h> /* atoi */
#include "Limits.h"
#include "UI.h"
#include "ClientManager.h"

#define MIN_MENU_OPTION 0
#define MAX_LOGIN_OPTION 4
#define MAX_MAIN_MENU_OPTION 5
#define BOLD "\x1b[1m"
#define UNBOLD "\033[m"

void SetAppColor()
{
    system("echo -n '\033]10;rgb:dd/d7/d7\a\033]11;rgb:30/2a/2a\a'");
}

int PrintLoginMenu()
{
    char buffer[BUFFER_SIZE_LIMIT];
    int choice;
    while (TRUE)
    {
        printf("%sPlease choose one of the options below:\n1. Registration\n2. Login\n3. Exit\nYour choice:%s ", BOLD, UNBOLD);
        scanf("%255s", buffer);
        choice = atoi(buffer);
        if (choice > MIN_MENU_OPTION && choice < MAX_LOGIN_OPTION)
        {
            break;
        }
        printf("%s\t\t\tInvalid choice, please try again%s\n", BOLD, UNBOLD);
    }
    return choice;
}

void WelcomePrint(char _userName[])
{
    int userNameLen = 0;
    userNameLen = strlen(_userName);
    if (userNameLen > 0)
    {
        printf("%s\t\t\t\tWelcome, %s!%s\n", BOLD, _userName, UNBOLD);
    }
    else
    {
        printf("%s\t\t\t\tWelcome, guest!%s\n", BOLD, UNBOLD);
    }
}

void RegisterUserDetails(char _userName[], char _password[])
{
    char bufferUserName[BUFFER_SIZE_LIMIT], bufferPassword[BUFFER_SIZE_LIMIT];
    printf("%sPlease choose a username (up to 10 characters):%s ", BOLD, UNBOLD);
    scanf("%255s", bufferUserName);
    while (strlen(bufferUserName) > USERNAME_SIZE_LIMIT)
    {
        printf("%sUsername is too long. Please choose a different one:%s ", BOLD, UNBOLD);
        scanf("%255s", bufferUserName);
    }
    printf("%sPlease choose a password (up to 10 characters):%s ", BOLD, UNBOLD);
    scanf("%255s", bufferPassword);
    while (strlen(bufferPassword) > PASSWORD_SIZE_LIMIT)
    {
        printf("%sPassword is too long. Please choose a different one:%s ", BOLD, UNBOLD);
        scanf("%255s", bufferPassword);
    }
    strncpy(_userName, bufferUserName, USERNAME_SIZE_LIMIT);
    strncpy(_password, bufferPassword, PASSWORD_SIZE_LIMIT);
}

int PrintRegisterReply(char _regRepBuffer[])
{
    if (strcmp(_regRepBuffer, "Duplicate") == 0)
    {
        printf("%s\t\tUsername is taken, please choose a different one%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    else if (strcmp(_regRepBuffer, "Fail") == 0)
    {
        printf("%s\t\t\tRegistration failed, please try again%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    else
    {
        printf("%s\t\t\tRegistration successful, please login!%s\n", BOLD, UNBOLD);
    }
    return SUCCESS;
}

void InsertUserDetails(char _userName[], char _password[])
{
    char bufferUserName[BUFFER_SIZE_LIMIT], bufferPassword[BUFFER_SIZE_LIMIT];
    printf("%sPlese enter your username:%s ", BOLD, UNBOLD);
    scanf("%255s", bufferUserName);
    while (strlen(bufferUserName) > USERNAME_SIZE_LIMIT)
    {
        printf("%sUsername is too long. Please enter your username:%s ", BOLD, UNBOLD);
        scanf("%255s", bufferUserName);
    }
    printf("%sPlese enter your password:%s ", BOLD, UNBOLD);
    scanf("%255s", bufferPassword);
    while (strlen(bufferPassword) > PASSWORD_SIZE_LIMIT)
    {
        printf("%sPassword is too long. Please enter your password: %s", BOLD, UNBOLD);
        scanf("%255s", bufferPassword);
    }
    strncpy(_userName, bufferUserName, USERNAME_SIZE_LIMIT);
    strncpy(_password, bufferPassword, PASSWORD_SIZE_LIMIT);
}

int PrintLoginReply(char _logRepBuffer[])
{
    if (strcmp(_logRepBuffer, "Success") == 0)
    {
        printf("%s\t\t\t\tLogin successful!%s\n", BOLD, UNBOLD);
        return SUCCESS;
    }
    else if (strcmp(_logRepBuffer, "Fail") == 0)
    {
        printf("%s\t\tInvalid username or password, please try again%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    return SUCCESS;
}

int PrintMainMenu()
{
    char buffer[BUFFER_SIZE_LIMIT];
    int choice;
    while (TRUE)
    {
        printf("%sPlease choose one of the options below:\n1. Create group\n2. Join group\n3. Leave group\n4. Logout\nYour choice:%s ", BOLD, UNBOLD);
        scanf("%255s", buffer);
        choice = atoi(buffer);
        if (choice > MIN_MENU_OPTION && choice < MAX_MAIN_MENU_OPTION)
        {
            break;
        }
        printf("%sInvalid choice, please try again%s\n", BOLD, UNBOLD);
    }
    return choice;
}

void PrintCreateGroup(char _groupName[])
{
    char bufferGroupName[BUFFER_SIZE_LIMIT];
    printf("%sPlease choose a group name (up to 10 characters):%s ", BOLD, UNBOLD);
    scanf("%255s", bufferGroupName);
    while (strlen(bufferGroupName) > GROUP_NAME_SIZE_LIMIT)
    {
        printf("%sGroup name is too long. Please choose a different one:%s ", BOLD, UNBOLD);
        scanf("%255s", bufferGroupName);
    }
    strncpy(_groupName, bufferGroupName, GROUP_NAME_SIZE_LIMIT);
}

int PrintCreateReply(char _createRepBuffer[])
{
    if (strcmp(_createRepBuffer, "Duplicate") == 0)
    {
        printf("%s\tGroup name is already taken :( Please create one with a different name!%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    else if(strcmp(_createRepBuffer, "Fail") == 0)
    {
        printf("%s\t\tAn error occurred while creating the group, please try again%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    printf("%s\t\t\tThe group was created successfully!%s\n", BOLD, UNBOLD);
    return SUCCESS;
}

void ChooseGroupToJoin(char _groupName[])
{
    char bufferGroupName[BUFFER_SIZE_LIMIT];
    printf("%sPlease choose a group to join (enter group's name):%s ", BOLD, UNBOLD);
    scanf("%255s", bufferGroupName);
    while (strlen(bufferGroupName) > GROUP_NAME_SIZE_LIMIT)
    {
        printf("%sGroup name is invalid. Please choose a group to join (enter group's name):%s ", BOLD, UNBOLD);
        scanf("%255s", bufferGroupName);
    }
    strncpy(_groupName, bufferGroupName, GROUP_NAME_SIZE_LIMIT);
}

int PrintJoinReply(char _joinRepBuffer[])
{
    if (strcmp(_joinRepBuffer, "Not Found") == 0)
    {
        printf("%s\tThe requested group was not found, please try to join a group with a different name!%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    else if(strcmp(_joinRepBuffer, "Duplicate") == 0)
    {
        printf("%s\t\t\tYou are already a part of this group!%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    else if(strcmp(_joinRepBuffer, "Fail") == 0)
    {
        printf("%s\tAn error occurred while trying to join the group, please try again%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    printf("%s\t\t\t\tYou have joined the group!%s\n", BOLD, UNBOLD);
    return SUCCESS;
}

void ChooseGroupToLeave(char _groupName[])
{
    char bufferGroupName[BUFFER_SIZE_LIMIT];
    printf("%sPlease choose a group to leave (enter group's name):%s ", BOLD, UNBOLD);
    scanf("%255s", bufferGroupName);
    while (strlen(bufferGroupName) > GROUP_NAME_SIZE_LIMIT)
    {
        printf("%sGroup name is invalid. Please choose a group to leave (enter group's name):%s ", BOLD, UNBOLD);
        scanf("%255s", bufferGroupName);
    }
    strncpy(_groupName, bufferGroupName, GROUP_NAME_SIZE_LIMIT);
}

int PrintLeaveReply(char _leaveRepBuffer[])
{
    if (strcmp(_leaveRepBuffer, "Not Found") == 0)
    {
        printf("%s\tYou are either not a part of that group or the group does not exist!%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    else if(strcmp(_leaveRepBuffer, "Fail") == 0)
    {
        printf("%s\tAn error occurred while trying to leave the group, please try again%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    printf("%s\t\t\t\tYou have left the group!%s\n", BOLD, UNBOLD);
    return SUCCESS;
}

void PrintGroupsFailReply(char _PrintRepBuffer[])
{
    if (strcmp(_PrintRepBuffer, "Not Found") == 0)
    {
        printf("%s\tThere are no existing groups to display, please create one :)%s\n", BOLD, UNBOLD);
    }
    else if (strcmp(_PrintRepBuffer, "Fail") == 0)
    {
        printf("%s\t\tDisplaying existing groups failed, please try again%s\n", BOLD, UNBOLD);
    }
}

void PrintTitleGroupNames(void)
{
    printf("%s\t\t\t\tThe groups are:%s\n", BOLD, UNBOLD);
}

int PrintArrayOfGroupNames(void *_element, size_t _index, void *_context)
{
    printf("\t\t\t\t%s\n", (char*)_element);
    return 1;
}

int LogoutFailReply(char _LogoutRepBuffer[])
{
    if (strcmp(_LogoutRepBuffer, "Fail") == 0)
    {
        printf("%s\t\tAn error occurred while trying to log out, please try again%s\n", BOLD, UNBOLD);
        return FAIL;
    }
    else if (strcmp(_LogoutRepBuffer, "Success") == 0)
    {
        printf("%s\t\t\t\tLogging out...%s\n", BOLD, UNBOLD);
        return SUCCESS;
    }
    return SUCCESS;
}

int PrintList(void *_element, void *_context)
{
    printf("\t\t\t\t%s\n", (char *)_element);
    return 1;
}