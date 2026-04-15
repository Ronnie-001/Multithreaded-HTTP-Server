#include "parser.h"

HttpParser::HttpParser(int fd) : _complete(false), _conn_fd(fd)
{};

HttpParser::~HttpParser() {}; 

bool HttpParser::isRequestComplete() const { return _complete; }

void HttpParser::appendData(const char* buffer, int bytes)
{
    _request.append(buffer, bytes);
}

std::string HttpParser:: extractStartLine()
{

}
