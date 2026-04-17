#include "parser.h"

HttpParser::HttpParser(int fd, const std::string request) : _complete(false), _conn_fd(fd), _request(request)
{}

HttpParser::~HttpParser() { /*Nothing here!!!*/ } 

bool HttpParser::isRequestComplete() const { return _complete; }

void HttpParser::appendData(const char* buffer, int bytes)
{
    _request.append(buffer, bytes);
}

std::string HttpParser:: extractStartLine()
{
    // Look for first instance of CRLF
    std::string::size_type first_clrf = _request.find("\r\n");
    std::string start_line = _request.substr(0, first_clrf);  
    
    return start_line;
}
