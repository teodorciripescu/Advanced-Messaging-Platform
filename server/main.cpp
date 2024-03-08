#include <iostream>
#include "headers/server_boilerplate.h"
#include "headers/database.h"
int main() {
    server_boilerplate server;
    server.createServer(2908);
    return 0;
}