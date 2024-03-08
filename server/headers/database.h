//
// Created by Teodor Ciripescu on 09/12/2019.
//

#ifndef RETELECHATSERVER_DATABASE_H
#define RETELECHATSERVER_DATABASE_H


#include <sqlite3.h>

class database {
    sqlite3 *db;

    static int execution_callback(void* data, int argc, char** argv, char** azColName);
public:
    int createConnection();
    void closeConnection();
    int executeSQLstatement(const char* statement, int (*callback)(void *data, int argc, char **argv, char **azColName));
};


#endif //RETELECHATSERVER_DATABASE_H
