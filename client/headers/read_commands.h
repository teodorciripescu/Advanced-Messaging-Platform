//
// Created by Teodor Ciripescu on 04/01/2020.
//

#ifndef CHATCLIENT_READ_COMMANDS_H
#define CHATCLIENT_READ_COMMANDS_H


class read_commands {
public:
    void loginResponse(const char *response);
    void RegisterResponse(const char *response);
    void getFriendListResponse(const char *response);
    void sendFriendRequestResponse(const char *response);
    void getFriendRequestListResponse(const char *response);
    void acceptFriendRequestResponse(const char *response);
    void rejectFriendRequestResponse(const char *response);
    void getConversationsResponse(const char *response);
    void getMessagesResponse(const char *response);
    void getUnseenMessagesResponse(const char *response);
    void sendMessageResponse(const char *response);
    void replyMessageResponse(const char *response);
    void deleteMessageResponse(const char *response);
    void logoutResponse(const char *response);
    void seenResponse(const char *response);
    void getMessageResponse(const char *response);
    void getMessageReplyResponse(const char *response);
    void getFriendListPendingMessages(const char *response);
    void error(const char *response);
};


#endif //CHATCLIENT_READ_COMMANDS_H
