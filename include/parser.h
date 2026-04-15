#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "request.h"

#define BUFFER_SIZE 1024

// Used for parsing incoming HTTP requests accepter from the TCP listener.
class HttpParser 
{
private:
    // Used to check if the entire HTTP request has been recieved.
    bool _complete;
    int _conn_fd;
    std::string _request;
    std::string _start_line;

    // The final request to be constructed.
    Request _parsed_request;
public:
    // Constructor 
    HttpParser(int fd);

    // Destructor
    ~HttpParser();
    
    // Returns the state of _request.
    bool isRequestComplete() const; 
    /*
     * Used for appending data from the recv() system call
     * and appending it to _request.
    */
    void appendData(const char* buffer, int bytes);
    
    /* Used for extracting the start line from the HTTP request, which
     * contains the method, resource path and HTTP version,
     */
    std::string extractStartLine();
    
    // Used for extracting the method used, i.e. GET, POST, PUT, etc.
    std::string extractMethod(); 
    
    // Used for extracting the path to the resource.
    std::string extractResourcePath();
    
    // Used for extracting the HTTP version being used.
    std::string extractVersion();
};

#endif // ! PARSER_H
