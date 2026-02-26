#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

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

    // Store the incoming connections
    struct sockaddr pending_connections;

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
    
    std::cout << "Waiting for new connections"; 


    return 0;
}
