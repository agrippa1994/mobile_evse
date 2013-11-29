#include <evse/network/tcp/http/reply.hpp>
#include <evse/network/tcp/http/script.hpp>
#include <evse/network/tcp/http/client.hpp>


evse::network::tcp::http::reply::reply(http_client *cl, request& req, const string& www) : m_www(www), m_request(req), m_client(cl)
{
    
}

string evse::network::tcp::http::reply::createReply()
{
    string path = m_www + m_request.getRequestedFile();
    string content;
    
    script scr(this, path);
    if(scr.was_successfull()){
        content = scr.getContent();
        
        return makeReply(200, content.length(), m_request.getMIMEType(), content);
    }

    content = "ERROR 404 File not found";
    return makeReply(400, content.length(), m_request.getMIMEType(), content);
}

string evse::network::tcp::http::reply::makeReply(unsigned int status_code, size_t length, const string& mime, const string& content)
{
    stringstream ss;
    ss << statusCodeForHTTPHeaderToString(status_code) << endl;
    ss << "Server: EVSE 1.0 (Unix)" << endl;
    ss << "Content-Length: " << length << endl;
    ss << "Connection: keep-alive" << endl;
    ss << "Content-Type: " << mime << "; charset=utf-8" << endl << endl;
    ss << content;
    
    return ss.str();
}

string evse::network::tcp::http::reply::statusCodeForHTTPHeaderToString(unsigned int status_code)
{
    switch(status_code)
    {
    case 200:
        return string("HTTP/1.0 200 OK");
    case 201:
        return string("HTTP/1.0 201 Created");
    case 202:
        return string("HTTP/1.0 202 Accepted");
    case 204:
        return string("HTTP/1.0 204 No Content");
            
    case 300:
        return string("HTTP/1.0 300 Multiple Choices");
    case 301:
        return string("HTTP/1.0 301 Moved Permanently");
    case 302:
        return string("HTTP/1.0 302 Moved Temporarily");
    case 304:
        return string("HTTP/1.0 304 Not Modified");
            
    case 400:
        return string("HTTP/1.0 400 Bad Request");
    case 401:
        return string("HTTP/1.0 401 Unauthorized");
    case 403:
        return string("HTTP/1.0 403 Forbidden");
    case 404:
        return string("HTTP/1.0 404 Not Found");
            
    case 500:
        return string("HTTP/1.0 500 Internal Server Error");
    case 501:
        return string("HTTP/1.0 501 Not Implemented");
    case 502:
        return string("HTTP/1.0 502 Bad Gateway");
    case 503:
        return string("HTTP/1.0 503 Service Unavailable");
            
    default:
        return string("HTTP/1.0 503 Service Unavailable");
    };
}
