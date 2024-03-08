//
// Created by Teodor Ciripescu on 10/12/2019.
//

#ifndef RETELECHATSERVER_DATABASE_COMMANDS_H
#define RETELECHATSERVER_DATABASE_COMMANDS_H


#include "database.h"
#include "server_boilerplate.h"
#include<string>

class database_commands {
    database *d;
    std::string Stringify(int integer);
public:
    database_commands(database *dd){
        d = dd;
        //d.createConnection();
    }
    bool login(const char *username, const char *password, int* idUser, int* descriptor);
    static int login_callback(void* data, int argc, char** argv, char** azColName);

    bool Register(const char *username, const char *password);
    static int register_callback(void *data, int argc, char **argv, char **azColName);


    char * getFriendList(int sender_id);
    static int getFriendList_callback(void *data, int argc, char **argv, char **azColName);

    char * getFriendListPendingMessages(int sender_id);// no command ID, function called after a successful authentication
    static int getFriendListPendingMessages_callback(void *data, int argc, char **argv, char **azColName);

    bool sendFriendRequest(const char *username, int sender_id);
    static int sendFriendRequest_callback(void *data, int argc, char **argv, char **azColName);

    char * getFriendRequestList(int sender_id);
    static int getFriendRequestList_callback(void *data, int argc, char **argv, char **azColName);

    bool acceptFriendRequest(const char *username, int sender_id);
    static int acceptFriendRequest_callback(void *data, int argc, char **argv, char **azColName);

    bool rejectFriendRequest(const char *username, int sender_id);
    static int rejectFriendRequest_callback(void *data, int argc, char **argv, char **azColName);

    char * getMessages(const char *username, int sender_id, int amount, int total_amount, int offset);
    static int getMessages_callback(void *data, int argc, char **argv, char **azColName);

    char * getUnseenMessages(const char *username, int sender_id, int amount, int total_amount, int offset);
    static int getUnseenMessages_callback(void *data, int argc, char **argv, char **azColName);

    char * sendMessage(const char *username, const char *message, int sender_id);
    static int sendMessage_callback1(void *data, int argc, char **argv, char **azColName);
    static int sendMessage_callback2(void *data, int argc, char **argv, char **azColName);

    char * replyMessage(const char *username, const char *message, const char *reply, int sender_id);
    static int replyMessage_callback1(void *data, int argc, char **argv, char **azColName);
    static int replyMessage_callback2(void *data, int argc, char **argv, char **azColName);

    bool deleteMessage(const char *msg_id, int sender_id);
    static int deleteMessage_callback(void *data, int argc, char **argv, char **azColName);


    char * seen(const char* msg_id, int sender_id);
    static int seen_callback(void *data, int argc, char **argv, char **azColName);

    int getNumberOfPendingMessages(const char *username, int sender_id);
    static int getNumberOfPendingMessages_callback(void *data, int argc, char **argv, char **azColName);
};


#endif //RETELECHATSERVER_DATABASE_COMMANDS_H
