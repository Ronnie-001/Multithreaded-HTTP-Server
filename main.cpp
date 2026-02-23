#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <iterator>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MY_PORT "3490"
#define BACKLOG 10

int main()
{ int status;
    // Specifies the criteria for the returned socket addresses.
    struct addrinfo hints; 
    // Collect the result here.
    struct addrinfo* servinfo;
    // Store the incoming connections
    struct sockaddr pending_connections;
    
    // Make sure that the struct is empty
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, MY_PORT, &hints, &servinfo)) != 0) {
        std::cout << "gai error: ";
        std::cout << gai_strerror(status);
        exit(1);
    }

    // Creation of a socket
    int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    int bindfd = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);

    // int connfd = connect(bindfd, servinfo->ai_addr, servinfo->ai_addrlen);

    int listenfd = listen(bindfd, BACKLOG);
    socklen_t peer_address_size = sizeof(pending_connections);
    int acceptfd = accept(sockfd, &pending_connections, &peer_address_size);

    // Free the memory
    freeaddrinfo(servinfo);

    return 0;
}
