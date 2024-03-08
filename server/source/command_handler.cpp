//
// Created by Teodor Ciripescu on 05/01/2020.
//

#include "../headers/command_handler.h"
#include "../headers/server_boilerplate.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include<string>

#include "../headers/database_commands.h"

extern int errno;
extern database d;

char *command_handler::handleCommand(int command_id, char *command, thData *thread_data) {
    if(command_id == 1){
        login(thread_data, command);
    }
    else if(command_id == 2){
        Register(thread_data->descriptor, command);
    }
    else if(thread_data->idUser!=-1){
        switch(command_id){
            case 3: //getFriendList()
                getFriendList(thread_data);
                break;
            case 4: //sendFriendRequest(username)
                sendFriendRequest(thread_data, command);
                break;
            case 5: //getFriendRequestList()
                getFriendRequestList(thread_data);
                break;
            case 6: //acceptFriendRequest(request id)
                acceptFriendRequest(thread_data, command);
                break;
            case 7: //rejectFriendRequest(request id)
                rejectFriendRequest(thread_data, command);
                break;
            case 8: //getConversations()
                //getConversations(thread_data->descriptor, command);
                break;
            case 9: //getMessages(id conversation, messages amount)
                getMessages(thread_data, command);
                break;
            case 10: //getUnseenMessages(params: id coversation)
                getUnseenMessages(thread_data, command);
                break;
            case 11: //sendMessage(id conversation, content)
                sendMessage(thread_data, command);
                break;
            case 12: //replyMessage
                replyMessage(thread_data, command);
                break;
            case 13: //deleteMessage(id message, for me/for all )
                deleteMessage(thread_data, command);
                break;
            case 14: //logout()
                logout(thread_data);
                break;
            case 15: //seen()
                seen(thread_data, command);
                break;
            case 18: //
                getFriendListPendingMessages(thread_data);
                break;
            default:
                break;
        }
    }
    else if(thread_data->idUser==-1 && command_id > 2){
        error(thread_data, "You need to login to access this command.");
    }
    std::cout<<"command_id: "<<command_id<<std::endl;
    return nullptr;
}
void command_handler::login(thData *thread_data, char *command){ //1
    database_commands dc(&d);
    int username_len = (int)command[0];
    char *username = new char [username_len];
    memcpy(username, &command[2], username_len + 2);
    username[username_len] = '\0';
    int password_len = (int)command[1];
    char *password = new char [password_len];
    memcpy(password, &command[username_len + 2], username_len + password_len + 2);
    password[password_len] = '\0';
    printf("username: .%s.\n",username);
    printf("password: .%s.\n",password);
    bool authenticated = dc.login(username, password, &thread_data->idUser, &thread_data->descriptor);
    if(authenticated){
        strcpy(thread_data->username, username);
        server_boilerplate::addThreadData(thread_data);
        server_boilerplate::sendMsgToClient(1, thread_data->descriptor, "Auth successful!");
        std::cout<<"Authenticated! userId = "<<thread_data->idUser<<std::endl;
        getFriendListPendingMessages(thread_data);
    }
    else{
        server_boilerplate::sendMsgToClient(19, thread_data->descriptor, "Auth unsuccessful!");
    }
}
void command_handler::Register(int sd, char *command) { // 2
    database_commands dc(&d);
    int username_len = (int)command[0];
    char *username = new char [username_len];
    memcpy(username, &command[2], username_len + 2);
    username[username_len] = '\0';
    int password_len = (int)command[1];
    char *password = new char [password_len];
    printf("passlen: %d\n",password_len);
    memcpy(password, &command[username_len + 2], username_len + password_len + 2);
    password[password_len] = '\0';
    printf("username: .%s.\n",username);
    printf("password: .%s.\n",password);
    bool registered = dc.Register(username, password);
    if(registered)
        server_boilerplate::sendMsgToClient(2, sd, "You registered successfully!");
    else
        server_boilerplate::sendMsgToClient(2, sd, "Registration unsuccessful.");
}
void command_handler::getFriendList(thData *thread_data){//3
    database_commands dc(&d);
    char *resp = dc.getFriendList(thread_data->idUser);
    if(resp){
        server_boilerplate::sendMsgToClient(3, thread_data->descriptor, resp);
        delete[] resp;
    }
    else{
        server_boilerplate::sendMsgToClient(3, thread_data->descriptor, "No friend list received.");
    }

}
void command_handler::sendFriendRequest(thData *thread_data, char *command){//4
    database_commands dc(&d);
    int username_len = (int)command[0];
    char *username = new char [username_len];
    memcpy(username, &command[1], username_len + 1);
    username[username_len] = '\0';
    printf("username: .%s.\n",username);
    bool sent = dc.sendFriendRequest(username, thread_data->idUser);
    if(sent)
        server_boilerplate::sendMsgToClient(4, thread_data->descriptor, "Friend request sent!");
    else
        server_boilerplate::sendMsgToClient(4, thread_data->descriptor, "Friend request not sent.");
}
void command_handler::getFriendRequestList(thData *thread_data){ //5
    database_commands dc(&d);
    char *resp = dc.getFriendRequestList(thread_data->idUser);
    if(resp){
        server_boilerplate::sendMsgToClient(5, thread_data->descriptor, resp);
        delete[] resp;
    }
    else{
        server_boilerplate::sendMsgToClient(5, thread_data->descriptor, "No friend request list received.");
    }
}
void command_handler::acceptFriendRequest(thData *thread_data, char *command){//6
    database_commands dc(&d);
    int username_len = (int)command[0];
    char *username = new char [username_len];
    memcpy(username, &command[1], username_len + 1);
    username[username_len] = '\0';
    printf("username: .%s.\n",username);
    bool sent = dc.acceptFriendRequest(username, thread_data->idUser);
    if(sent)
        server_boilerplate::sendMsgToClient(6, thread_data->descriptor, "Friend request accepted!");
    else
        server_boilerplate::sendMsgToClient(6, thread_data->descriptor, "Friend request could not be accepted.");
    delete[] username;
}
void command_handler::rejectFriendRequest(thData *thread_data, char *command){//7
    database_commands dc(&d);
    int username_len = (int)command[0];
    char *username = new char [username_len];
    memcpy(username, &command[1], username_len + 1);
    username[username_len] = '\0';
    printf("username: .%s.\n",username);
    bool sent = dc.rejectFriendRequest(username, thread_data->idUser);
    if(sent)
        server_boilerplate::sendMsgToClient(6, thread_data->descriptor, "Friend request declined!");
    else
        server_boilerplate::sendMsgToClient(6, thread_data->descriptor, "Friend request could not be declined.");
    delete[] username;
}
void command_handler::getConversations(){//8

}
void command_handler::getMessages(thData *thread_data, char *command){//9
    database_commands dc(&d);
    const int MESSAGES_PER_PACKET = 10;
    int username_len = (int)command[0];
    char *username = new char [username_len];
    memcpy(username, &command[2], username_len + 2);
    username[username_len] = '\0';
    int amount_len = (int)command[1];
    char *amount_c = new char [amount_len];
    memcpy(amount_c, &command[username_len + 2], username_len + amount_len + 2);
    amount_c[amount_len] = '\0';
    int amount = atoi(amount_c);
    int total_amount = amount;
    printf("recipient .%s.\n",username);
    printf("amount: .%d.\n",amount);
    char* messages;
    if (amount < MESSAGES_PER_PACKET){
        messages = dc.getMessages(username, thread_data->idUser, amount, total_amount, amount);
        if(messages)
            server_boilerplate::sendMsgToClient(9, thread_data->descriptor, messages);
        else
            server_boilerplate::sendMsgToClient(19, thread_data->descriptor, "Error retrieving messages.");
        delete[] messages;
    }
    else{
        while(amount > MESSAGES_PER_PACKET - 1){
            amount -= MESSAGES_PER_PACKET;
            messages = dc.getMessages(username, thread_data->idUser, MESSAGES_PER_PACKET, total_amount, total_amount - amount);
            if(messages)
                server_boilerplate::sendMsgToClient(9, thread_data->descriptor, messages);
            else{
                server_boilerplate::sendMsgToClient(19, thread_data->descriptor, "Error retrieving messages.");
                break;
            }
            delete[] messages;
        }
        if(amount>0){
            messages = dc.getMessages(username, thread_data->idUser, amount, amount, amount);
            if(messages)
                server_boilerplate::sendMsgToClient(9, thread_data->descriptor, messages);
            else
                server_boilerplate::sendMsgToClient(19, thread_data->descriptor, "Error retrieving messages.");
            delete[] messages;
        }
    }
    delete[] username;
    delete[] amount_c;
}
void command_handler::getUnseenMessages(thData *thread_data, char *command){//10
    database_commands dc(&d);
    const int MESSAGES_PER_PACKET = 10;
    int username_len = (int)command[0];
    char *username = new char [username_len];
    memcpy(username, &command[1], username_len + 2);
    username[username_len] = '\0';

    int amount = dc.getNumberOfPendingMessages(username, thread_data->idUser);
    std::cout<< amount<<",----";
    if(amount==0)return;
    int total_amount = amount;
    printf("recipient .%s.\n",username);
    printf("amount: .%d.\n",amount);
    char* messages;
    if (amount < MESSAGES_PER_PACKET){
        messages = dc.getUnseenMessages(username, thread_data->idUser, amount, total_amount, amount);
        if(messages)
            server_boilerplate::sendMsgToClient(10, thread_data->descriptor, messages);
        else
            server_boilerplate::sendMsgToClient(19, thread_data->descriptor, "Error retrieving messages.");
        delete[] messages;
    }
    else{
        while(amount > MESSAGES_PER_PACKET - 1){
            amount -= MESSAGES_PER_PACKET;
            messages = dc.getUnseenMessages(username, thread_data->idUser, MESSAGES_PER_PACKET, total_amount, total_amount - amount);
            if(messages)
                server_boilerplate::sendMsgToClient(10, thread_data->descriptor, messages);
            else{
                server_boilerplate::sendMsgToClient(19, thread_data->descriptor, "Error retrieving messages.");
                break;
            }
            delete[] messages;
        }
        if(amount>0){
            messages = dc.getUnseenMessages(username, thread_data->idUser, amount, amount, amount);
            if(messages)
            server_boilerplate::sendMsgToClient(10, thread_data->descriptor, messages);
            else
            server_boilerplate::sendMsgToClient(19, thread_data->descriptor, "Error retrieving messages.");
            delete[] messages;
        }
    }
    delete[] username;
}
void command_handler::sendMessage(thData *thread_data, char *command){//11
    database_commands dc(&d);
    int username_len = (int)command[0];
    char *username = new char [username_len];
    memcpy(username, &command[2], username_len + 2);
    username[username_len] = '\0';
    int message_len = (int)command[1];
    char *message = new char [message_len + 512];
    memcpy(message, &command[username_len + 2], username_len + message_len + 2);
    message[message_len] = '\0';
    printf("recipient .%s.\n",username);
    printf("message: .%s.\n",message);
    char* msg = dc.sendMessage(username, message, thread_data->idUser);
    if(msg){
        //who sent the message
        int l = strlen(msg);
        msg[l] = strlen(thread_data->username);
        msg[l+1]='\0';
        strcat(msg, thread_data->username);
        //message
        l = strlen(msg);
        msg[l] = message_len;
        msg[l+1]='\0';
        strcat(msg, message);
        printf("%s\n", msg);
        int desc = server_boilerplate::getDescriptorFromOnlineUser(username);
        if(desc != 0){
            server_boilerplate::sendMsgToClient(16, desc, msg);
            server_boilerplate::sendMsgToClient(11, thread_data->descriptor, "Message sent!");
        } else{
            server_boilerplate::sendMsgToClient(11, thread_data->descriptor, "Message sent! User is offline right now.");
        }
    }
    else
        server_boilerplate::sendMsgToClient(11, thread_data->descriptor, "Message could not be sent.");
    delete[] username;
    delete[] message;
    delete[] msg;
}
void command_handler::replyMessage(thData *thread_data, char *command){//12
    database_commands dc(&d);
    int username_len = (int)command[0];
    char *username = new char [username_len];
    memcpy(username, &command[3], username_len + 3);
    username[username_len] = '\0';
    int message_len = (int)command[1];
    char *message = new char [message_len + 768];
    memcpy(message, &command[username_len + 3], username_len + message_len + 3);
    message[message_len] = '\0';
    int reply_len = (int)command[2];
    char *reply = new char [reply_len];
    memcpy(reply, &command[username_len + message_len + 3], username_len + message_len + reply_len + 3);
    reply[reply_len] = '\0';
    printf("recipient .%s.\n",username);
    printf("message: .%s.\n",message);
    printf("reply: .%s.\n",reply);
    char* msg = dc.replyMessage(username, message, reply, thread_data->idUser);
    if(msg){
        //who sent the message
        int l = strlen(msg);
        msg[l] = strlen(thread_data->username);
        msg[l+1]='\0';
        strcat(msg, thread_data->username);
        //message
        l = strlen(msg);
        msg[l] = message_len;
        msg[l+1]='\0';
        strcat(msg, message);
        //reply id
        l = strlen(msg);
        msg[l] = reply_len;
        msg[l+1] = '\0';
        strcat(msg, reply);
        printf("%s\n", msg);
        int desc = server_boilerplate::getDescriptorFromOnlineUser(username);
        if(desc != 0){
            server_boilerplate::sendMsgToClient(17, desc, msg);
            server_boilerplate::sendMsgToClient(12, thread_data->descriptor, "Message reply sent!");
        } else{

            server_boilerplate::sendMsgToClient(12, thread_data->descriptor, "Message reply sent! User is offline right now.");
        }
    }
    else
        server_boilerplate::sendMsgToClient(12, thread_data->descriptor, "Message reply could not be sent.");
    delete[] username;
    delete[] message;
    delete[] msg;
    delete[] reply;
}
void command_handler::deleteMessage(thData *thread_data, char *command){//13
    database_commands dc(&d);
    char msg_id[32]="";
    strcat(msg_id, &command[1]);
    bool deleted = dc.deleteMessage(msg_id, thread_data->idUser);
    if(deleted){
        server_boilerplate::sendMsgToClient(13, thread_data->descriptor, "Message deleted!");
    } else{
        server_boilerplate::sendMsgToClient(13, thread_data->descriptor, "Message could not be deleted!");
    }
}
void command_handler::logout(thData *thread_data){//14

}
void command_handler::seen(thData *thread_data, char *command){//15
    database_commands dc(&d);
    int msg_id_len = command[0];
    char msg_id[msg_id_len+1];
    msg_id[0]='\0';
    strcat(msg_id, &command[1]);
    //printf("msg id to be seen: %s\n", msg_id);
    char* resp = dc.seen(msg_id, thread_data->idUser);
    if(resp){
        int sender_id = atoi(&resp[1]);
        int desc = server_boilerplate::getDescriptorFromOnlineUser(sender_id);
        //printf("sender id: %d\n", sender_id);
        if(desc != 0){
            std::string m = "Message " + (std::string)(msg_id) + " was seen.";
            server_boilerplate::sendMsgToClient(15, desc, m.c_str());
        }
    }
    delete[] resp;
}
void command_handler::getFriendListPendingMessages(thData *thread_data){//18
    database_commands dc(&d);
    char *resp = dc.getFriendListPendingMessages(thread_data->idUser);
    if(resp){
        server_boilerplate::sendMsgToClient(18, thread_data->descriptor, resp);
        delete[] resp;
    }
    else{
        server_boilerplate::sendMsgToClient(18, thread_data->descriptor, "No pending messages.");
    }
}

void command_handler::error(thData *thread_data, const char *error_message) {
    server_boilerplate::sendMsgToClient(19, thread_data->descriptor, error_message);
}
