#ifndef IO_SERVICE_HPP
#define IO_SERVICE_HPP

namespace boost { namespace asio { class io_service; } }

namespace evse{
    namespace network{
        extern class boost::asio::io_service io_service;
    }
}

#endif
