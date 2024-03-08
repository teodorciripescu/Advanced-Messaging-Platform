//
// Created by Teodor Ciripescu on 09/12/2019.
//

#include "../headers/database.h"
#include <sqlite3.h>
#include <stdio.h>
#include <iostream>
using namespace std;
int database::createConnection() {
    int rc;
    rc = sqlite3_open("retele.db", &db);
    if( rc ){
        printf( "Can’t open database: %s\n", sqlite3_errmsg(db));
        return -1;

    }else{

        printf( "Opened database successfully...\n");
    }
}

int database::executeSQLstatement(const char* statement,int (*callback)(void *, int, char **, char **)){
    int response = 0;
    char* messageError;
    response = sqlite3_exec(db, statement, callback, nullptr, &messageError);
    if (response != SQLITE_OK) {
        cout << "Error executing query!" << endl << messageError;
        sqlite3_free(messageError);
        return 0;
    }
    else{
        cout << "Query executed successfully! " << endl;
        return 1;
    }

}



int database::execution_callback(void *data, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

void database::closeConnection() {
    sqlite3_close(db);
}
