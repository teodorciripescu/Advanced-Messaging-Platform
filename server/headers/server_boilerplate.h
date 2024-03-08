//
// Created by Teodor Ciripescu on 09/12/2019.
//

#ifndef RETELECHATSERVER_SERVER_BOILERPLATE_H
#define RETELECHATSERVER_SERVER_BOILERPLATE_H

#include "database_commands.h"

struct thData{
    int idUser; //user's id extracted from the DB at authentication
    int descriptor; //descriptor returned by accept
    char username[256];
};

class server_boilerplate {

    static void *treatClient(void *arg);
    static void receiveFromClient(void *arg);

    static void deleteThreadData(thData *thread_data);
    static void printThreadData();
public:
    int createServer(int PORT);
    static void addThreadData(void *arg);
    static void sendMsgToClient(int command_id, int sd, const char *msg);
    static int getDescriptorFromOnlineUser(const char *username);
    static int getDescriptorFromOnlineUser(int id);
};


#endif //RETELECHATSERVER_SERVER_BOILERPLATE_H
