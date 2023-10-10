#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <pthread.h>

using namespace std;
#define IP "127.0.0.1"
#define maxline 1024
struct pack {
    int a;
    struct node {
        char id[256];
        char ip[256];
        int port;
    } node1;
    char chat[256];
};
int query(string id) {
    struct pack s1;
	s1.a = 2;
	strcpy(s1.node1.id, id.c_str());
    int sockfd;
	struct sockaddr_in	 servaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8000);
	servaddr.sin_addr.s_addr = inet_addr(IP);

	
	sendto(sockfd, &s1, sizeof(s1), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	struct pack s2;
		socklen_t len;
	len = sizeof(servaddr);
	recvfrom(sockfd, &s2, sizeof(struct pack), MSG_WAITALL, ( struct sockaddr *) &servaddr, &len);
	close(sockfd);
	return s2.node1.port;
}
void serv(string id) {
    int p = query(id);
    while (1) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr, clientaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(p);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));
    
    socklen_t len = sizeof(clientaddr);

   
    struct pack s1;
    int n = recvfrom(sockfd, &s1, sizeof(s1), MSG_WAITALL, (struct sockaddr*)&clientaddr, &len);
    
    cout<<s1.chat<<endl;
    close(sockfd);
    }
}



void send_msg(struct pack s1) {
     int sockfd;
    sockfd =  socket(AF_INET, SOCK_DGRAM, 0); 
    struct sockaddr_in servaddr, clientaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8000);
    servaddr.sin_addr.s_addr = inet_addr(IP);

    
    socklen_t len;
    len =sizeof(clientaddr);
    sendto(sockfd, &s1, sizeof(s1),MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));
    /*if (b == 1) {
    char buffer[maxline];
    int n = recvfrom(sockfd, (char *)buffer, maxline, MSG_WAITALL, (struct sockaddr*)&clientaddr, &len);
    buffer[n] = '\0';
    cout<<buffer<<endl;
    } else if (b == 2){
        struct pack s2;
        recvfrom(sockfd, &s2, sizeof(s2), MSG_WAITALL, (struct sockaddr*)&clientaddr, &len);
        if (s2.node1.port == 0) cout<<"user does'nt exits"<<endl;
        else {
            cout<<"the ip is ->"<<s2.node1.ip<<" "<<"the port no is ->"<<s2.node1.port<<endl;
        }
    }*/
   close(sockfd);
}

void input(string id) {
while (1) {
struct pack s1;
strcpy(s1.node1.id, id.c_str());
s1.a = 3;
string ch;
//cout<<"for reg press 1"<<endl;
//cout<<"for retrieve press 2"<<endl;
getline(cin, ch);
strcpy(s1.chat, ch.c_str());
cout<<"broadcasting req"<<endl;
send_msg(s1);
  
}

}
void reg_user(string id) {
    int sockfd;
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8000);
    servaddr.sin_addr.s_addr = inet_addr(IP);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    struct pack s;
    s.a = 1;
    strcpy(s.node1.id, id.c_str());
    
    sendto(sockfd, &s, sizeof(s), MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    close(sockfd);
}
int main(int argc, char **argv) {
 string id = "";
 for (int i = 1; i < argc; i++) {
    id = id + argv[i];
 }
 cout<<id<<endl;
 reg_user(id); 
 thread th1(serv, id);
 thread th2(input, id);
 th1.join();
 th2.join();

    return 1;

}