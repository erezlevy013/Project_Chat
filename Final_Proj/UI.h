#ifndef __UI_H__
#define __UI_H__

#include <stddef.h> /* size_t */

/**
 * @brief sets the background color of the terminal and the output color to grey varients
 */
void SetAppColor();

/**
 * @brief prints the login menu (register, login, exit) and gets the client's choice
 * @returns the user's chioce (REGISTER = 1, LOGIN = 2, EXIT = 3)
 */
int PrintLoginMenu();

/**
 * @brief prints a "welcome guest" or "welcome [username]" if username is known
 */
void WelcomePrint(char _userName[]);

/**
 * @brief asks the client to input username and password in order to register (limited to 10 characters each)
 */
void RegisterUserDetails(char _userName[], char _password[]);

/**
 * @brief prints the status of the client's registration request (i.e. successful or otherwise)
 * @returns SUCCESS if the reply means that user input is valid, or FAIL if not.
 */
int PrintRegisterReply(char _regRepBuffer[]);

/**
 * @brief asks the client to input username and password in order to log in (limited to 10 characters each)
 */
void InsertUserDetails(char _userName[], char _password[]);

/**
 * @brief prints the status of the client's login request (i.e. successful or otherwise)
 * @returns SUCCESS if the reply means that user input is valid, or FAIL if not.
 */
int PrintLoginReply(char _logRepBuffer[]);

/**
 * @brief prints the app's main menu (create, login, logout) and gets the client's choice
 * @returns the integer that represent user's chioce, from 1 to 4.
 */
int PrintMainMenu();

/**
 * @brief asks the client to input group name of the group to be created (limited to 10 characters)
 */
void PrintCreateGroup(char _groupName[]);

/**
 * @brief prints the status of the client's create group request (i.e. successful or otherwise)
 * @returns SUCCESS if the reply means that user input is valid, or FAIL if not.
 */
int PrintCreateReply(char _createRepBuffer[]);

/**
 * @brief asks the client to input group name of the group to be joined (limited to 10 characters)
 */
void ChooseGroupToJoin(char _groupName[]);

/**
 * @brief prints the status of the client's join group request (i.e. successful or otherwise)
 * @returns SUCCESS if the reply means that user input is valid, or FAIL if not.
 */
int PrintJoinReply(char _joinRepBuffer[]);

/**
 * @brief asks the client to input group name of the group to leave (limited to 10 characters)
 */
void ChooseGroupToLeave(char _groupName[]);

/**
 * @brief prints the status of the client's leave group request (i.e. successful or otherwise)
 * @returns SUCCESS if the reply means that user input is valid, or FAIL if not.
 */
int PrintLeaveReply(char _leaveRepBuffer[]);

/**
 * @brief prints a permanent title before displaying all existing group names
 */
void PrintTitleGroupNames(void);

/**
 * @brief prints all group name that exsist in server
 * @return always returns 1
 */
int PrintArrayOfGroupNames(void *_element, size_t _index, void *_context);

/**
 * @brief prints the reason of failure for the client's get groups request
 */
void PrintGroupsFailReply(char _PrintRepBuffer[]);

/**
 * @brief prints the reason of failure for the client's log out request
 * @returns return FAIL if an error occoured during the request from server to log out, or SUCCESS if request recieved successfully.
 */
int LogoutFailReply(char _LogoutRepBuffer[]);

/**
 * @brief prints all groups' names that the user is part of.
 * @return always returns 1
 */
int PrintList(void *_element, void *_context);

#endif /* __UI_H__*/