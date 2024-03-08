//
// Created by Teodor Ciripescu on 04/01/2020.
//

#ifndef CHATCLIENT_READ_HANDLER_H
#define CHATCLIENT_READ_HANDLER_H


class read_handler {
    static void handleRead();
public:
    static void *readFromServer(void *arg);

};


#endif //CHATCLIENT_READ_HANDLER_H
