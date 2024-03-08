//
// Created by Teodor Ciripescu on 10/12/2019.
//

#include "../headers/database_commands.h"
#include "sqlite3.h"
#include<string>
#include<iostream>

using namespace std;
int *p;

bool database_commands::Register(const char *username, const char *password) {
    string sqlstatement = "INSERT INTO users(username, password) VALUES('"+(string)username+"', '"+(string)password+"')";
    return d->executeSQLstatement(sqlstatement.c_str(), &register_callback);
}

int database_commands::register_callback(void *data, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

bool database_commands::login(const char *username, const char *password, int* idUser, int* descriptor) {
     string sqlstatement = "SELECT id FROM users WHERE username='"+(string)username+"' AND password='"+(string)password+"'";
     p=idUser;
     d->executeSQLstatement(sqlstatement.c_str(), &login_callback);
    return *idUser > 0;
}

int database_commands::login_callback(void *data, int argc, char **argv, char **azColName) {
    if(argv[0])
        *p=atoi(argv[0]);
    return 0;
}

bool database_commands::sendFriendRequest(const char *username, int sender_id) {
    string sqlstatement = "INSERT INTO friend_lists(owner, friend, accepted) VALUES('" + Stringify(sender_id) + "', (SELECT id FROM users WHERE username='" + (string)username + "'),'1'), " + "((SELECT id FROM users WHERE username='" + (string)username + "'),'" + Stringify (sender_id) + "','0')";
    return d->executeSQLstatement(sqlstatement.c_str(), &sendFriendRequest_callback);
}

int database_commands::sendFriendRequest_callback(void *data, int argc, char **argv, char **azColName) {
    return 0;
}

char *f_list;
char * database_commands::getFriendList(int sender_id) {
    f_list = nullptr;
    char *list = new char[256 * 32];
    list[0]='\0';
    f_list = list;
    string sqlstatement = "SELECT username FROM users WHERE id IN (SELECT owner FROM friend_lists WHERE friend='" + Stringify(sender_id) +"' AND accepted=1)";
    bool exec = d->executeSQLstatement(sqlstatement.c_str(), &getFriendList_callback);
    if(exec && strlen(list)>0)
        return list;
    else{
        delete[] list;
        return nullptr;
    }
}

int database_commands::getFriendList_callback(void *data, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        strcat(f_list, argv[i]);
        strcat(f_list, "\n");
    }
    return 0;
}
char *fpm_list;
char *database_commands::getFriendListPendingMessages(int sender_id) {
    fpm_list = nullptr;
    char *list = new char[512 * 32];
    list[0]='\0';
    fpm_list = list;
    string sqlstatement, where_condition, count_messages;
    sqlstatement = "SELECT (SELECT username FROM users WHERE id=sender), COUNT(sender) FROM (SELECT sender FROM messages WHERE recipient='" + Stringify(sender_id) +"' AND seen=0) GROUP BY sender";
    bool exec = d->executeSQLstatement(sqlstatement.c_str(), &getFriendListPendingMessages_callback);
    cout << list << endl;
    if(exec && strlen(list)>0)
        return list;
    else{
        delete[] list;
        return nullptr;
    }
}

int database_commands::getFriendListPendingMessages_callback(void *data, int argc, char **argv, char **azColName) {
    strcat(fpm_list, argv[0]);
    strcat(fpm_list, "[");
    strcat(fpm_list, argv[1]);
    strcat(fpm_list, "] ");
    return 0;
}

char *fr_list;
char *database_commands::getFriendRequestList(int sender_id) {
    fr_list = nullptr;
    char *list = new char[256 * 32];
    list[0]='\0';
    fr_list = list;
    string sqlstatement = "SELECT username FROM users WHERE id IN (SELECT friend FROM friend_lists WHERE owner='" + Stringify(sender_id) +"' AND accepted=0)";
    bool exec = d->executeSQLstatement(sqlstatement.c_str(), &getFriendRequestList_callback);
    if(exec && strlen(list)>0)
        return list;
    else{
        delete[] list;
        return nullptr;
    }
}

int database_commands::getFriendRequestList_callback(void *data, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        strcat(fr_list, argv[i]);
        strcat(fr_list, "\n");
    }
    return 0;
}

bool database_commands::acceptFriendRequest(const char *username, int sender_id) {
    string sqlstatement = "UPDATE friend_lists SET accepted=1 WHERE owner='" + Stringify(sender_id) + "' AND  friend=(SELECT id FROM users WHERE username='" + (string)username + "'); ";
    return d->executeSQLstatement(sqlstatement.c_str(), &acceptFriendRequest_callback);
}

int database_commands::acceptFriendRequest_callback(void *data, int argc, char **argv, char **azColName) {
    return 0;
}

bool database_commands::rejectFriendRequest(const char *username, int sender_id) {
    string sqlstatement = "DELETE FROM friend_lists WHERE owner='" + Stringify(sender_id) + "' AND  friend=(SELECT id FROM users WHERE username='" + (string)username + "')";
    return d->executeSQLstatement(sqlstatement.c_str(), &rejectFriendRequest_callback);
}

int database_commands::rejectFriendRequest_callback(void *data, int argc, char **argv, char **azColName) {
    return 0;
}

bool friends;
char *msg;
char * database_commands::sendMessage(const char *username, const char *message, int sender_id) {
    friends = false;
    string sqlstatement;
    //check if the recipient is friends with the sender
    sqlstatement = "SELECT id FROM friend_lists WHERE owner=(SELECT id FROM users WHERE username='" + (string)username + "') AND friend='" + Stringify(sender_id) +"' AND accepted=1";
    d->executeSQLstatement(sqlstatement.c_str(), &sendMessage_callback1);
    if(friends){
        msg = new char[1024];
        sqlstatement = "INSERT INTO messages(sender, recipient, content) VALUES('" + Stringify(sender_id) + "', (SELECT id FROM users WHERE username='" + (string)username + "'),'" + (string)message + "'); SELECT id, time FROM messages WHERE sender='" + Stringify(sender_id) + "' AND recipient=(SELECT id FROM users WHERE username='" + (string)username + "') ORDER BY id DESC LIMIT 1";
        d->executeSQLstatement(sqlstatement.c_str(), &sendMessage_callback2);
        return msg;
    }
    else
        return nullptr;
}

int database_commands::sendMessage_callback1(void *data, int argc, char **argv, char **azColName) {
    friends = true;
    return 0;
}
int database_commands::sendMessage_callback2(void *data, int argc, char **argv, char **azColName) {
    //message id
    msg[0] = strlen(argv[0]);
    msg[1]='\0';
    strcat(msg, argv[0]);
    //message timestamp
    msg[strlen(msg)] = strlen(argv[1]);
    msg[strlen(msg)]='\0';
    strcat(msg, argv[1]);

    return 0;
}
char *sender;
char *database_commands::seen(const char *msg_id, int sender_id) {
    char *id = new char[256];
    sender = id;
    string sqlstatement;
    sqlstatement = "UPDATE messages SET seen=1 WHERE id='" + (string)msg_id + "'; ";
    sqlstatement += "SELECT sender FROM messages WHERE id='" + (string)msg_id + "';";
    bool resp = d->executeSQLstatement(sqlstatement.c_str(), &seen_callback);
    if(resp){
        cout<<"ID FROM DB "<<id<<endl;
        return id;
    }
    else return nullptr;
}

int database_commands::seen_callback(void *data, int argc, char **argv, char **azColName) {
    sender[0]=strlen(argv[0]);
    sender[1]='\0';
    strcat(sender, argv[0]);
    return 0;
}

bool friends_r;
char *msg_r;
char *database_commands::replyMessage(const char *username, const char *message, const char *reply, int sender_id) {//12
    friends_r = false;
    string sqlstatement;
    //check if the recipient is friends with the sender
    sqlstatement = "SELECT id FROM friend_lists WHERE owner=(SELECT id FROM users WHERE username='" + (string)username + "') AND friend='" + Stringify(sender_id) +"' AND accepted=1";
    d->executeSQLstatement(sqlstatement.c_str(), &replyMessage_callback1);
    if(friends_r){
        msg_r = new char[1024];
        sqlstatement = "INSERT INTO messages(sender, recipient, content, reply_to) VALUES('" + Stringify(sender_id) + "', (SELECT id FROM users WHERE username='" + (string)username + "'),'" + (string)message + "','" + (string)reply + "'); SELECT id, time FROM messages WHERE sender='" + Stringify(sender_id) + "' AND recipient=(SELECT id FROM users WHERE username='" + (string)username + "') ORDER BY id DESC LIMIT 1";
        d->executeSQLstatement(sqlstatement.c_str(), &replyMessage_callback2);
        return msg_r;
    }
    else
        return nullptr;
}


int database_commands::replyMessage_callback1(void *data, int argc, char **argv, char **azColName) {
    friends_r = true;
    return 0;
}

int database_commands::replyMessage_callback2(void *data, int argc, char **argv, char **azColName) {
    //message id
    msg_r[0] = strlen(argv[0]);
    msg_r[1]='\0';
    strcat(msg_r, argv[0]);
    //message timestamp
    msg_r[strlen(msg_r)] = strlen(argv[1]);
    msg_r[strlen(msg_r)]='\0';
    strcat(msg_r, argv[1]);
    return 0;
}
bool deleted;
bool database_commands::deleteMessage(const char *msg_id, int sender_id) {
    deleted = true;
    string sqlstatement;
    sqlstatement = "DELETE FROM messages WHERE sender='"+Stringify(sender_id)+"' AND id='"+(string)(msg_id)+"';";
    sqlstatement += "SELECT id FROM messages WHERE id='" + (string)(msg_id) + "';";
    d->executeSQLstatement(sqlstatement.c_str(), &deleteMessage_callback);
    return deleted;
}

int database_commands::deleteMessage_callback(void *data, int argc, char **argv, char **azColName) {
    deleted = false;
    return 0;
}

char *messages_p;
char *database_commands::getMessages(const char *username, int sender_id, int amount, int total_amount, int offset) {
    messages_p = new char[512 * amount];
    strcpy(messages_p, "");
    string sqlstatement, where_condition, sender_username;
    where_condition = "(sender=(SELECT id FROM users WHERE username='" + (string)username + "') AND recipient='"+Stringify(sender_id)+"')";
    where_condition += " OR (recipient=(SELECT id FROM users WHERE username='" + (string)username + "') AND sender='"+Stringify(sender_id)+"')";
    sender_username = "(SELECT username FROM users WHERE id=sender)";
    sqlstatement = "SELECT id, time, " + sender_username + ", content, reply_to, seen  FROM (SELECT * FROM messages WHERE "+ where_condition +" ORDER BY id DESC LIMIT "+Stringify(total_amount)+" - "+Stringify(offset)+" , "+Stringify(amount)+") ORDER BY id;";

    d->executeSQLstatement(sqlstatement.c_str(), &getMessages_callback);
    if(strlen( messages_p))
        return messages_p;
    else
        return nullptr;
}

int database_commands::getMessages_callback(void *data, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        char c[2]="1";
        c[0]=(char) strlen(argv[i]);
        strcat(messages_p, c);
        strcat(messages_p, argv[i]);
    }
    return 0;
}
char *messages_unseen;
char *database_commands::getUnseenMessages(const char *username, int sender_id, int amount, int total_amount, int offset) {
    messages_unseen = new char[512 * amount];
    strcpy(messages_unseen, "");
    string sqlstatement, where_condition, count_messages;
    where_condition = "(sender=(SELECT id FROM users WHERE username='" + (string)username + "') AND recipient='"+Stringify(sender_id)+"' AND seen=0)";
    count_messages="SELECT COUNT(id) FROM messages WHERE " + where_condition;
    sqlstatement = "SELECT id, time, '" + (string)username + "', content, reply_to, seen  FROM (SELECT * FROM messages WHERE "+ where_condition +" ORDER BY id DESC LIMIT "+Stringify(total_amount)+" - "+Stringify(offset)+" , "+Stringify(amount)+") ORDER BY id;";

    d->executeSQLstatement(sqlstatement.c_str(), &getUnseenMessages_callback);
    return messages_unseen;
}

int database_commands::getUnseenMessages_callback(void *data, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        char c[2]="1";
        c[0]=(char) strlen(argv[i]);
        strcat(messages_unseen, c);
        strcat(messages_unseen, argv[i]);
    }
    return 0;
}

char *count_pending;
int database_commands::getNumberOfPendingMessages(const char *username, int sender_id) {
    char *count = new char[256];
    count_pending = count;
    string sqlstatement;
    sqlstatement += "SELECT COUNT(id) FROM messages WHERE recipient='" + Stringify(sender_id) + "' AND sender=(SELECT id FROM users WHERE username='"+(string)username+"') AND seen=0;";
    bool resp = d->executeSQLstatement(sqlstatement.c_str(), &getNumberOfPendingMessages_callback);
    if(resp){
        return atoi(count);
    }
    else return 0;
}

int database_commands::getNumberOfPendingMessages_callback(void *data, int argc, char **argv, char **azColName) {
    strcpy(count_pending, argv[0]);
    return 0;
}

std::string database_commands::Stringify(int integer) {
    char buffer[33];
    snprintf(buffer, sizeof(buffer), "%d", integer);
    return (string)buffer;
}