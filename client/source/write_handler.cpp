//
// Created by Teodor Ciripescu on 04/01/2020.
//

#include "../headers/write_handler.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include "../headers/write_commands.h"
#define MAX_COMMAND_SIZE 256

using namespace std;

extern int errno;

extern int sd;

extern bool alive;
write_commands wc;
void *write_handler::writeToServer(void *arg){
    fflush (stdout);
    pthread_detach(pthread_self());
    handleWrite();
    close ((intptr_t)arg);
    return(NULL);
}
void write_handler::handleWrite(){
    char command[MAX_COMMAND_SIZE];
    int command_id;
    while (alive){
        fgets(command, MAX_COMMAND_SIZE, stdin);
        command[strlen(command)-1]='\0';
        command_id = wc.getCommandId(command);
        if(command_id == -1 ){
            cout<<"This command doesn't exist.\n";
            continue;
        }
        if(command_id == 1){ //login
            wc.login();
        }
        else if(command_id == 2){ //register
            wc.Register();
        }
        else if(command_id == 3){ //getFriendList()
            wc.getFriendList();
        }
        else if(command_id == 4){ //sendFriendRequest(username)
            wc.sendFriendRequest();
        }
        else if(command_id == 5){ //getFriendRequestList()
            wc.getFriendRequestList();
        }
        else if(command_id == 6){ //acceptFriendRequest(request id)
            wc.acceptFriendRequest();
        }
        else if(command_id == 7){ //rejectFriendRequest(request id)
            wc.rejectFriendRequest();
        }
        else if(command_id == 8){ //getConversations()
            wc.getConversations();
        }
        else if(command_id == 9){ //getMessages(id conversation, messages amount)
            wc.getMessages();
        }
        else if(command_id == 10){ //getUnseenMessages(params: id coversation)
            wc.getUnseenMessages();
        }
        else if(command_id == 11){ //sendMessage(id conversation, content)
            wc.sendMessage();
        }
        else if(command_id == 12){ //replyMessage
            wc.replyMessage();
        }
        else if(command_id == 13){ //deleteMessage(id message, for me/for all )
            wc.deleteMessage();
        }
        else if(command_id == 14){ //logout()
            wc.logout();
        }
        else if(command_id == 18){ //getFriendListPendingMessages()
            wc.getFriendListPendingMessages();
        }
    }
}
void write_handler::sendMsgToServer(int command_id, const char *msg) {
    int len = strlen(msg);
    if (write (sd, &command_id, sizeof(int)) <= 0)
    {
        perror ("Error at writing command_id length to server.\n");
    }
    if (write (sd, &len, sizeof(short int)) <= 0)
    {
        perror ("Error at writing msg length to server.\n");
    }
    if (write (sd, msg, len) <= 0)
    {
        perror ("Error at writing msg to server.\n");
        alive = false;
    }
    cout << "Command sent.\n";
}
