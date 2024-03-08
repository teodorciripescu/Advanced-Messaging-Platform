//
// Created by Teodor Ciripescu on 04/01/2020.
//

#ifndef CHATCLIENT_WRITE_COMMANDS_H
#define CHATCLIENT_WRITE_COMMANDS_H


class write_commands {
public:
    int getCommandId(const char* command);
    void login();
    void Register();
    void getFriendList();
    void sendFriendRequest();
    void getFriendRequestList();
    void acceptFriendRequest();
    void rejectFriendRequest();
    void getConversations();
    void getMessages();
    void getUnseenMessages();
    void sendMessage();
    void replyMessage();
    void deleteMessage();
    void logout();
    void seen(const char* msg_id);
    void getMessage();
    void getMessageReply();
    void getFriendListPendingMessages();
};


#endif //CHATCLIENT_WRITE_COMMANDS_H
