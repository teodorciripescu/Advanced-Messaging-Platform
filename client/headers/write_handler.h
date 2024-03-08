//
// Created by Teodor Ciripescu on 04/01/2020.
//

#ifndef CHATCLIENT_WRITE_HANDLER_H
#define CHATCLIENT_WRITE_HANDLER_H


class write_handler {
private:
    static void handleWrite();
public:
    static void *writeToServer(void *arg);
    void sendMsgToServer(int command_id, const char *msg);
};


#endif //CHATCLIENT_WRITE_HANDLER_H
