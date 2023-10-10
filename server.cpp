#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;
struct pack {
    int a;
    struct node {
        char id[256];
        char ip[256];
        int port;
    } node1;
     char chat[256];
};
#define maxline 1024
int main() {
    vector<struct pack>v;
    int sockfd;
    struct sockaddr_in seraddr, clientaddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(8000);
    seraddr.sin_addr.s_addr = INADDR_ANY;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (const struct sockaddr*)&seraddr, sizeof(seraddr));
    while(1) {
    socklen_t len;
    len = sizeof(clientaddr);
    struct pack s1;
    int n = recvfrom(sockfd, &s1, sizeof(s1), MSG_WAITALL, (struct sockaddr*)&clientaddr, &len);
    bool flag = false;

    if (s1.a == 1) {
        for (int i = 0; i < v.size(); i++) {
            if (strcmp(v[i].node1.id, s1.node1.id) == 0) {
                flag = true;
                strcpy(v[i].node1.ip, inet_ntoa(clientaddr.sin_addr));
                v[i].node1.port = clientaddr.sin_port;
                break;
            }
        }
        if (flag == true) {
            cout<<s1.node1.id<<" "<<"user updated succesfully";
            flag = false;
        } else {
            cout<<s1.node1.id<<" "<<"new user registered succesfully";
            strcpy(s1.node1.ip, inet_ntoa(clientaddr.sin_addr));
            s1.node1.port = clientaddr.sin_port;
            v.push_back(s1);
            //sendto(sockfd, (char *)rep2, strlen(rep2), MSG_CONFIRM, (const struct sockaddr*)&clientaddr, len);
        }
    } else if (s1.a == 2) {
        for (int i = 0; i < v.size(); i++) {			
				if (strcmp(s1.node1.id, v[i].node1.id) == 0) {
                    cout<<v[i].node1.id<<" "<<v[i].node1.port<<endl;				
					strcpy(s1.node1.ip, v[i].node1.ip);
					s1.node1.port = v[i].node1.port;
					break;
				}
        }
        sendto(sockfd, &s1, sizeof(s1), MSG_CONFIRM, (const struct sockaddr*)&clientaddr, len);
    } else if (s1.a == 3) {
        cout<<"broadcasting req"<<endl;
        
        for (int i = 0; i < v.size(); i++) {
            struct sockaddr_in dest;
            if (strcmp(v[i].node1.id, s1.node1.id) != 0) {
                dest.sin_family = AF_INET;
                dest.sin_port = htons(v[i].node1.port);
                dest.sin_addr.s_addr = inet_addr(v[i].node1.ip);

                sendto(sockfd, &s1, sizeof(s1), MSG_CONFIRM, (const struct sockaddr*)&dest, sizeof(dest));
            }
        }
        
        
    }
    }
   
    close(sockfd);
    
    return 1;

}