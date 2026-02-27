#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define MY_PORT "3490"
#define BACKLOG 10


int main()
{ 
    int status;
    // Specifies the criteria for the returned socket addresses.
    struct addrinfo hints; 
    // Collect the result here, and have a pointer to servinfo
    struct addrinfo* servinfo;
    struct addrinfo* ptr;

    int bindfd, sockfd;

    // Make sure that the struct is empty before filling it in with data
    std::memset(&hints, 0, sizeof(hints));

    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, MY_PORT, &hints, &servinfo)) != 0) {
        std::cout << "gai error: ";
        std::cout << gai_strerror(status);
        exit(1);
    }
    
    // Loop through all of the found server addresses.
    for (ptr = servinfo; ptr != NULL; ptr = ptr->ai_next) {
        
        sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (sockfd == -1) {
            perror("socket error");
            continue;
        }
        
        bindfd = bind(sockfd, ptr->ai_addr, ptr->ai_addrlen);

        if (bindfd == -1) {
            perror("bind error");
            continue;
        }
        
        break;
    } 
    
    freeaddrinfo(servinfo);

    if (ptr == NULL) {
        // Flush any pending output and exit
        std::cerr << "No avaliable connections"; 
        exit(1);
    }

    int listenfd = listen(bindfd, BACKLOG);

    if (listenfd == -1) {
        perror("listen error");
        exit(1);
    } 
    
    std::cout << "Waiting for new connections...\n"; 
    
    bool server_running = true;
    // Store the recieved connection
    struct sockaddr recieved_connection;

    while (server_running) {
        socklen_t connection_size = sizeof(recieved_connection);
        int new_conn_fd = accept(sockfd, &recieved_connection, &connection_size);
        
        if (new_conn_fd == -1) {
            perror("accept: error accepting a new connection");
            continue;
        }
        
        // Get the name of the IP address that is connecting
        std::string conn_ip;
        inet_ntop(AF_INET, &recieved_connection, conn_ip.data(), sizeof(conn_ip));
        std::cout << "server: Recived connection from" << conn_ip << '\n'; 
        
        close(new_conn_fd);
    }
}
