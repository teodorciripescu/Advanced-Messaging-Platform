//
// Created by Teodor Ciripescu on 04/01/2020.
//

#include "../headers/read_handler.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include "../headers/read_commands.h"
#define MAX_COMMAND_SIZE 256

using namespace std;

extern int errno;

extern int sd;

extern bool alive;
read_commands rc;
void *read_handler::readFromServer(void *arg){
    fflush (stdout);
    pthread_detach(pthread_self());
    handleRead();
    close ((intptr_t)arg);
    return(NULL);
}
void read_handler::handleRead(){

    while(alive) {
        short int msg_len =0;
        int command_id;

        command_id = -1;
        if (read(sd, &command_id, sizeof(int)) <= 0) {
            perror("Error at reading msg length from server.\n");
        }
        if (read(sd, &msg_len, sizeof(short int)) <= 0) {
            perror("Error at reading msg length from server.\n");
        }
        char *response = new char[msg_len];
        if (read(sd, response, msg_len) <= 0) {
            perror("Error at reading msg from server.\n");
            alive = false;
        }
        response[msg_len]='\0';
        //cout << "Received -> Content: " << response << "\tCommand_id: " << command_id << "\tLen: " << msg_len << '\n';
        switch(command_id){
            case 1: //login
                rc.loginResponse(response);
                break;
            case 2: //register
                rc.RegisterResponse(response);
                break;
            case 3: //getFriendList()
                rc.getFriendListResponse(response);
                break;
            case 4: //sendFriendRequest(username)
                rc.sendFriendRequestResponse(response);
                break;
            case 5: //getFriendRequestList()
                rc.getFriendRequestListResponse(response);
                break;
            case 6: //acceptFriendRequest(request id)
                rc.acceptFriendRequestResponse(response);
                break;
            case 7: //rejectFriendRequest(request id)
                rc.rejectFriendRequestResponse(response);
                break;
            case 8: //getConversations()
                rc.getConversationsResponse(response);
                break;
            case 9: //getMessages(id conversation, messages amount)
                rc.getMessagesResponse(response);
                break;
            case 10: //getUnseenMessages
                rc.getUnseenMessagesResponse(response);
                break;
            case 11: //sendMessage(id conversation, content)
                rc.sendMessageResponse(response);
                break;
            case 12: //replyMessage
                rc.replyMessageResponse(response);
                break;
            case 13: //deleteMessage(id message, for me/for all )
                rc.deleteMessageResponse(response);
                break;
            case 14: //logout()
                rc.logoutResponse(response);
                break;
            case 15: //seen()
                rc.seenResponse(response);
                break;
            case 16: //getMessage()
                rc.getMessageResponse(response);
                break;
            case 17: //getMessageReply()
                rc.getMessageReplyResponse(response);
                break;
            case 18:
                rc.getFriendListPendingMessages(response);
                break;
            case 19:
                rc.error(response);
                break;
            default:
                break;
        }
        delete[] response;
    }
}