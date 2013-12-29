#include <evse/network/tcp/http/reply.hpp>
#include <evse/network/tcp/http/script.hpp>
#include <evse/network/tcp/http/client.hpp>
#include <evse/network/tcp/http/request.hpp>

#include <sstream>

evse::network::tcp::http::reply::reply(client *cl, request& req, const std::string& www) : m_www(www), m_request(req), m_client(cl)
{
    
}

std::string evse::network::tcp::http::reply::createReply()
{
    std::string path = m_www + m_request.getRequestedFile();
    std::string content;
    
    script scr(this, path);
    if(scr.was_successfull()){
        content = scr.getContent();

        return makeReply(200, content.length(), m_request.getMIMEType(), content);;
    }

    content = "ERROR 404 File not found";
    return makeReply(400, content.length(), m_request.getMIMEType(), content);
}

std::string evse::network::tcp::http::reply::makeReply(unsigned int status_code, size_t length, const std::string& mime, const std::string& content)
{
    std::stringstream ss;
    ss << statusCodeForHTTPHeaderToString(status_code) << std::endl;
    ss << "Server: EVSE 1.0 (Unix)" << std::endl;
    ss << "Content-Length: " << length << std::endl;
    ss << "Connection: keep-alive" << std::endl;
    ss << "Content-Type: " << mime << "; charset=utf-8" << std::endl << std::endl;
    ss << content;
    
    return ss.str();
}

std::string evse::network::tcp::http::reply::statusCodeForHTTPHeaderToString(unsigned int status_code)
{
    switch(status_code)
    {
    case 200:
        return std::string("HTTP/1.0 200 OK");
    case 201:
        return std::string("HTTP/1.0 201 Created");
    case 202:
        return std::string("HTTP/1.0 202 Accepted");
    case 204:
        return std::string("HTTP/1.0 204 No Content");
            
    case 300:
        return std::string("HTTP/1.0 300 Multiple Choices");
    case 301:
        return std::string("HTTP/1.0 301 Moved Permanently");
    case 302:
        return std::string("HTTP/1.0 302 Moved Temporarily");
    case 304:
        return std::string("HTTP/1.0 304 Not Modified");
            
    case 400:
        return std::string("HTTP/1.0 400 Bad Request");
    case 401:
        return std::string("HTTP/1.0 401 Unauthorized");
    case 403:
        return std::string("HTTP/1.0 403 Forbidden");
    case 404:
        return std::string("HTTP/1.0 404 Not Found");
            
    case 500:
        return std::string("HTTP/1.0 500 Internal Server Error");
    case 501:
        return std::string("HTTP/1.0 501 Not Implemented");
    case 502:
        return std::string("HTTP/1.0 502 Bad Gateway");
    case 503:
        return std::string("HTTP/1.0 503 Service Unavailable");
            
    default:
        return std::string("HTTP/1.0 503 Service Unavailable");
    };
}
