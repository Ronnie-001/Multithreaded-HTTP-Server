#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "parser.h"

HttpParser::HttpParser(int fd, const std::string request) : _complete(false), _conn_fd(fd), _request(request)
{}

HttpParser::~HttpParser() { /*Nothing here!!!*/ } 

bool HttpParser::isRequestComplete() const { return _complete; }

std::string HttpParser::getMethod() const { return _method; }
std::string HttpParser::getResourcePath() const { return _resource_path; }
std::string HttpParser::getVersion() const { return _version; }

void HttpParser::appendData(const char* buffer, int bytes)
{
    _request.append(buffer, bytes);
}

void HttpParser:: extractStartLine()
{
    // Look for first instance of CRLF
    std::string::size_type first_clrf = _request.find("\r\n");
    std::string start_line = _request.substr(0, first_clrf);  
    
    _start_line = start_line;
}

void HttpParser::parseStartLine()
{
    std::stringstream ss(_start_line);
    // Use a vector to store each part of the start line.
    std::vector<std::string> v;
    std::string store;
    
    while (std::getline(ss, store, ' ')) {
        v.push_back(store);
    }
        
    // Set the member variables pertaining to the start line
    _method = v[0];
    _resource_path = v[1];
    _version = v[2];
}

void HttpParser::parseHeaders() 
{
    std::string::size_type start = _request.find("\r\n");
    std::string::size_type end = _request.find("\r\n\r\n");

    // Take everything between the start line and the space.
    std::string headers = _request.substr(start, end);
    
    std::size_t parse_start = 0;
    std::size_t parse_end = headers.find("\r\n");
    
    // Iterate through manually until no CLRF
    while (parse_end != std::string::npos) {
        std::string header = headers.substr(parse_start, parse_end - parse_start);  

        // Split the header based on ": ", then add to std::map _headers
        std::stringstream ss(header);
        std::vector<std::string> v;
        std::string store;

        while (std::getline(ss, store, ':')) {
            v.push_back(store);
        }
        
        // Remove leading space from the value
        v[1] = v[1].find(' ') != std::string::npos ?  v[1].substr(1, v[1].size()) : v[1];

        std::cout << "Key: " << v[0] << '\n';
        std::cout << "Value: " << v[1] << '\n';

        std::cout << '\n';
        
        _headers.insert({v[0], v[1]});
    }
}
