//
// Created by Teodor Ciripescu on 04/01/2020.
//

#include "../headers/read_commands.h"
#include <errno.h>
#include <string.h>
#include <iostream>
#include "../headers/write_commands.h"

#define MAX_COMMAND_SIZE 256

using namespace std;

extern int errno;

extern int sd;

extern bool alive;

extern bool authenticated;

extern char myUsername[256];
extern write_commands wc;
void read_commands::loginResponse(const char *response){
    if(strcmp(response, "Auth successful!") == 0)
        authenticated = true;
    cout << response << '\n';
}
void read_commands::RegisterResponse(const char *response){
    cout<<response<<'\n';
}
void read_commands::getFriendListResponse(const char *response){
    cout<<"Your friend list:\n" << response;
}
void read_commands::sendFriendRequestResponse(const char *response){
    cout<<response<<'\n';
}
void read_commands::getFriendRequestListResponse(const char *response){
    cout<<"Your friend requests list: " << response << "\n";
}
void read_commands::acceptFriendRequestResponse(const char *response){
    cout<<response<<'\n';
}
void read_commands::rejectFriendRequestResponse(const char *response){
    cout<<response<<'\n';
}
void read_commands::getConversationsResponse(const char *response){
    cout<<response<<'\n';
}
void read_commands::getMessagesResponse(const char *response){
    int msg_id_len, time_len, username_len, msg_len, reply_len, seen_len;
    int msg_id_pos, time_pos, username_pos, msg_pos, reply_pos , seen_pos;

    string s = response;
    for(int i = 0; i < strlen(response); i = seen_len + seen_pos){
        msg_id_len = s[i];
        msg_id_pos = i + 1;
        time_len = s[msg_id_pos + msg_id_len];
        time_pos = msg_id_pos + msg_id_len + 1;
        username_len = s[time_pos + time_len];
        username_pos = time_pos + time_len + 1;
        msg_len = s[username_pos + username_len];
        msg_pos = username_pos + username_len + 1;
        reply_len = s[msg_pos + msg_len];
        reply_pos = msg_pos + msg_len + 1;
        seen_len = s[reply_pos + reply_len];
        seen_pos = reply_pos + reply_len + 1;
        cout << s.substr(username_pos, username_len) << ": " << s.substr(msg_pos, msg_len) << "\t";
        cout << "Date: " << s.substr(time_pos, time_len) << "\t Msg ID: " << s.substr(msg_id_pos, msg_id_len) <<"\t";
        if(s.compare(reply_pos, reply_len,"0"))cout << "Reply to: " << s.substr(reply_pos, reply_len);
        if(s.compare(seen_pos, seen_len,"1")){
            cout << " [NOT SEEN] \n";
            if(s.compare(username_pos, username_len,myUsername))
                wc.seen(s.substr(msg_id_pos, msg_id_len).c_str());
        } else{
            cout<<endl;
        }

    }
}
void read_commands::getUnseenMessagesResponse(const char *response){
    int msg_id_len, time_len, username_len, msg_len, reply_len, seen_len;
    int msg_id_pos, time_pos, username_pos, msg_pos, reply_pos , seen_pos;

    string s = response;
    for(int i = 0; i < strlen(response); i = seen_len + seen_pos){
        msg_id_len = s[i];
        msg_id_pos = i + 1;
        time_len = s[msg_id_pos + msg_id_len];
        time_pos = msg_id_pos + msg_id_len + 1;
        username_len = s[time_pos + time_len];
        username_pos = time_pos + time_len + 1;
        msg_len = s[username_pos + username_len];
        msg_pos = username_pos + username_len + 1;
        reply_len = s[msg_pos + msg_len];
        reply_pos = msg_pos + msg_len + 1;
        seen_len = s[reply_pos + reply_len];
        seen_pos = reply_pos + reply_len + 1;
        cout << s.substr(username_pos, username_len) << ": " << s.substr(msg_pos, msg_len) << "\t";
        cout << "Date: " << s.substr(time_pos, time_len) << "\t Msg ID: " << s.substr(msg_id_pos, msg_id_len) <<"\t";
        if(s.compare(reply_pos, reply_len,"0"))cout << "Reply to: " << s.substr(reply_pos, reply_len);
        if(s.compare(seen_pos, seen_len,"1")){
            cout << " [NOT SEEN] \n";
            if(s.compare(username_pos, username_len,myUsername))
                wc.seen(s.substr(msg_id_pos, msg_id_len).c_str());
        } else{
            cout<<endl;
        }
    }
}
void read_commands::sendMessageResponse(const char *response){
    cout<<response<<"\n";
}
void read_commands::replyMessageResponse(const char *response){
    cout<<response<<'\n';
}
void read_commands::deleteMessageResponse(const char *response){
    cout<<response<<'\n';
}
void read_commands::logoutResponse(const char *response){
    cout<<response<<'\n';
}
void read_commands::seenResponse(const char *response){
    cout<<response<<"\n";
}
void read_commands::getMessageResponse(const char *response){
    int msg_id_len, time_len, username_len, msg_len;
    int msg_id_pos, time_pos, username_pos, msg_pos;
    string s = response;
    msg_id_len = s[0];
    msg_id_pos = 1;
    time_len = s[msg_id_pos + msg_id_len];
    time_pos = msg_id_pos + msg_id_len + 1;
    username_len = s[time_pos + time_len];
    username_pos = time_pos + time_len + 1;
    msg_len = s[username_pos + username_len];
    msg_pos = username_pos + username_len + 1;
    cout << s.substr(username_pos, username_len) << ": " << s.substr(msg_pos, msg_len) << "\t";
    cout << "Date: " << s.substr(time_pos, time_len) << "\t Msg ID: " << s.substr(msg_id_pos, msg_id_len) << endl;
    wc.seen(s.substr(msg_id_pos, msg_id_len).c_str());

}
void read_commands::getMessageReplyResponse(const char *response){
    int msg_id_len, time_len, username_len, msg_len, reply_len;
    int msg_id_pos, time_pos, username_pos, msg_pos, reply_pos;
    string s = response;
    msg_id_len = s[0];
    msg_id_pos = 1;
    time_len = s[msg_id_pos + msg_id_len];
    time_pos = msg_id_pos + msg_id_len + 1;
    username_len = s[time_pos + time_len];
    username_pos = time_pos + time_len + 1;
    msg_len = s[username_pos + username_len];
    msg_pos = username_pos + username_len + 1;
    reply_len = s[msg_pos + msg_len];
    reply_pos = msg_pos + msg_len + 1;

    cout << s.substr(username_pos, username_len) << ": " << s.substr(msg_pos, msg_len) << "\t";
    cout << "Date: " << s.substr(time_pos, time_len) << "\tMsg ID: " << s.substr(msg_id_pos, msg_id_len) << "\t";
    cout << "Reply to: " << s.substr(reply_pos, reply_len) << endl;

    wc.seen(s.substr(msg_id_pos, msg_id_len).c_str());

}
void read_commands::getFriendListPendingMessages(const char *response){
    cout << "Pending messages: " << response << "\n";
}

void read_commands::error(const char *response) {
    cout << response << "\n";
}

