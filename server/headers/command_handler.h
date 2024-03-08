//
// Created by Teodor Ciripescu on 05/01/2020.
//

#ifndef RETELECHATSERVER_COMMAND_HANDLER_H
#define RETELECHATSERVER_COMMAND_HANDLER_H

#include "../headers/server_boilerplate.h"

class command_handler {
    void login(thData *thread_data, char *command);
    void Register(int sd, char *command);
    void getFriendList(thData *thread_data);
    void sendFriendRequest(thData *thread_data, char *command);
    void getFriendRequestList(thData *thread_data);
    void acceptFriendRequest(thData *thread_data, char *command);
    void rejectFriendRequest(thData *thread_data, char *command);
    void getConversations();
    void getMessages(thData *thread_data, char *command);
    void getUnseenMessages(thData *thread_data, char *command);
    void sendMessage(thData *thread_data, char *command);
    void replyMessage(thData *thread_data, char *command);
    void deleteMessage(thData *thread_data, char *command);
    void logout(thData *thread_data);
    void seen(thData *thread_data, char *command);
    void getFriendListPendingMessages(thData *thread_data);
    void error(thData *thread_data, const char *error_message);
public:
    char *handleCommand(int command_id, char *command, thData *thread_data);
};


#endif //RETELECHATSERVER_COMMAND_HANDLER_H
