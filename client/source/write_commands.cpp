//
// Created by Teodor Ciripescu on 04/01/2020.
//

#include "../headers/write_commands.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../headers/write_handler.h"
#define MAX_COMMAND_SIZE 256

using namespace std;

extern int errno;

extern int sd;

extern bool alive;

extern bool authenticated;
write_handler wh;
extern char myUsername[256];

int write_commands::getCommandId(const char* command){
    if(strcmp(command, "login")==0){
        return 1;
    }
    else if(strcmp(command, "register")==0){
        return 2;
    }
    else if(strcmp(command, "getfriendlist")==0){
        return 3;
    }
    else if(strcmp(command, "sendfriendrequest")==0){
        return 4;
    }
    else if(strcmp(command, "getfriendrequestlist")==0){
        return 5;
    }
    else if(strcmp(command, "acceptfriendrequest")==0){
        return 6;
    }
    else if(strcmp(command, "rejectfriendrequest")==0){
        return 7;
    }
    else if(strcmp(command, "getconversations")==0){
        return 8;
    }
    else if(strcmp(command, "getmessages")==0){
        return 9;
    }
    else if(strcmp(command, "getunseenmessages")==0){
        return 10;
    }
    else if(strcmp(command, "sendmessage")==0){
        return 11;
    }
    else if(strcmp(command, "replymessage")==0){
        return 12;
    }
    else if(strcmp(command, "deletemessage")==0){
        return 13;
    }
    else if(strcmp(command, "logout")==0){
        return 14;
    }
    else if(strcmp(command, "seen")==0){
        return 15;
    }
    else if(strcmp(command, "getmessage")==0){
        return 16;
    }
    else if(strcmp(command, "getmessagereply")==0){
        return 17;
    }
    else if(strcmp(command, "getfriendlistpendingmessages")==0){
        return 18;
    }
    return -1;
}

void write_commands::login(){
    if(!authenticated){
        char username [MAX_COMMAND_SIZE], password[MAX_COMMAND_SIZE];
        cout << "Username: ";
        fgets(username, MAX_COMMAND_SIZE, stdin);
        cout << "Password: ";
        fgets(password, MAX_COMMAND_SIZE, stdin);

        char response[2 * MAX_COMMAND_SIZE];
        username[strlen(username)-1]='\0';
        password[strlen(password)-1]='\0';
        response[0] = strlen(username);
        response[1] = strlen(password);
        response[2]='\0';
        strcat(response, username);
        strcat(response, password);
        wh.sendMsgToServer(1, response);
        strcpy(myUsername, username);
    }
    else{
        cout << "You are already logged in!\n";
    }
}
void write_commands::Register(){
    char username [MAX_COMMAND_SIZE], password[MAX_COMMAND_SIZE], repeat_password[MAX_COMMAND_SIZE];
    cout << "Username: ";
    fgets(username, MAX_COMMAND_SIZE, stdin);
    cout << "Password: ";
    fgets(password, MAX_COMMAND_SIZE, stdin);
    cout << "Repeat Password: ";
    fgets(repeat_password, MAX_COMMAND_SIZE, stdin);
    if(strcmp(password, repeat_password) != 0){
        cout << "The passwords do not match!\n";
        return;
    }
    char response[3 * MAX_COMMAND_SIZE];
    username[strlen(username)-1]='\0';
    password[strlen(password)-1]='\0';
    response[0] = strlen(username);
    response[1] = strlen(password);
    response[2]='\0';
    strcat(response, username);
    strcat(response, password);
    wh.sendMsgToServer(2, response);
}
void write_commands::getFriendList(){
    char response[MAX_COMMAND_SIZE]="0";
    wh.sendMsgToServer(3, response);
}
void write_commands::sendFriendRequest(){
    char username[MAX_COMMAND_SIZE];
    cout << "Username to send friend request: ";
    fgets(username, MAX_COMMAND_SIZE, stdin);
    username[strlen(username)-1]='\0';
    char response[MAX_COMMAND_SIZE + 1];
    response[0] = strlen(username);
    response[1] = '\0';
    strcat(response, username);
    wh.sendMsgToServer(4, response);
}
void write_commands::getFriendRequestList(){
    char response[MAX_COMMAND_SIZE]="0";
    wh.sendMsgToServer(5, response);
}
void write_commands::acceptFriendRequest(){
    char username[MAX_COMMAND_SIZE];
    cout << "Username to accept friend request: ";
    fgets(username, MAX_COMMAND_SIZE, stdin);
    username[strlen(username)-1]='\0';
    char response[MAX_COMMAND_SIZE + 1];
    response[0] = strlen(username);
    response[1] = '\0';
    strcat(response, username);
    wh.sendMsgToServer(6, response);
}
void write_commands::rejectFriendRequest(){
    char username[MAX_COMMAND_SIZE];
    cout << "Username to reject friend request: ";
    fgets(username, MAX_COMMAND_SIZE, stdin);
    username[strlen(username)-1]='\0';
    char response[MAX_COMMAND_SIZE + 1];
    response[0] = strlen(username);
    response[1] = '\0';
    strcat(response, username);
    wh.sendMsgToServer(7, response);
}
void write_commands::getConversations(){

}
void write_commands::getMessages(){
    char username[MAX_COMMAND_SIZE];
    cout << "Username to get message: ";
    fgets(username, MAX_COMMAND_SIZE, stdin);
    username[strlen(username)-1]='\0';
    char message_count[MAX_COMMAND_SIZE];
    cout << "Number of messages: ";
    fgets(message_count, MAX_COMMAND_SIZE, stdin);
    message_count[strlen(message_count)-1]='\0';
    char response[MAX_COMMAND_SIZE * 2 + 1];
    response[0] = strlen(username);
    response[1] = strlen(message_count);
    response[2] = '\0';
    strcat(response, username);
    strcat(response, message_count);
    wh.sendMsgToServer(9, response);
}
void write_commands::getUnseenMessages(){
    char username[MAX_COMMAND_SIZE];
    cout << "Username to get message: ";
    fgets(username, MAX_COMMAND_SIZE, stdin);
    username[strlen(username)-1]='\0';
    char response[MAX_COMMAND_SIZE + 1];
    response[0] = strlen(username);
    response[1] = '\0';
    strcat(response, username);
    wh.sendMsgToServer(10, response);
}
void write_commands::sendMessage(){
    char username[MAX_COMMAND_SIZE];
    cout << "Username to send message: ";
    fgets(username, MAX_COMMAND_SIZE, stdin);
    username[strlen(username)-1]='\0';
    char message[MAX_COMMAND_SIZE];
    cout << "Your message: ";
    fgets(message, MAX_COMMAND_SIZE, stdin);
    message[strlen(message)-1]='\0';
    char response[MAX_COMMAND_SIZE * 2 + 1];
    response[0] = strlen(username);
    response[1] = strlen(message);
    response[2] = '\0';
    strcat(response, username);
    strcat(response, message);
    wh.sendMsgToServer(11, response);
}
void write_commands::replyMessage(){
    char username[MAX_COMMAND_SIZE];
    cout << "Username to send message: ";
    fgets(username, MAX_COMMAND_SIZE, stdin);
    username[strlen(username)-1]='\0';
    char message[MAX_COMMAND_SIZE];
    cout << "Your message: ";
    fgets(message, MAX_COMMAND_SIZE, stdin);
    message[strlen(message)-1]='\0';
    char reply_Id[MAX_COMMAND_SIZE];
    cout << "Message to reply: ";
    fgets(reply_Id, MAX_COMMAND_SIZE, stdin);
    reply_Id[strlen(reply_Id)-1]='\0';
    char response[MAX_COMMAND_SIZE * 2 + 1];
    response[0] = strlen(username);
    response[1] = strlen(message);
    response[2] = strlen(reply_Id);
    response[3] = '\0';
    strcat(response, username);
    strcat(response, message);
    strcat(response, reply_Id);
    wh.sendMsgToServer(12, response);
}
void write_commands::deleteMessage(){
    char msg_id[32];
    cout << "Message (ID) to delete: ";
    fgets(msg_id, MAX_COMMAND_SIZE, stdin);
    msg_id[strlen(msg_id)-1]='\0';
    char response[32];
    response[0] = strlen(msg_id);
    response[1] = '\0';
    strcat(response, msg_id);
    wh.sendMsgToServer(13, response);
}
void write_commands::logout(){
    char response[MAX_COMMAND_SIZE]="0";
    wh.sendMsgToServer(14, response);
    alive = false;
}
void write_commands::seen(const char* msg_id){
    char response[MAX_COMMAND_SIZE];
    response[0] = strlen(msg_id);
    response[1] = '\0';
    strcat(response, msg_id);
    wh.sendMsgToServer(15, response);
}
void write_commands::getMessage(){

}
void write_commands::getMessageReply(){

}
void write_commands::getFriendListPendingMessages(){
    char response[MAX_COMMAND_SIZE]="0";
    wh.sendMsgToServer(18, response);
}