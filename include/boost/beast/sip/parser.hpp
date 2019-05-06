#ifndef BOOST_BEAST_SIP_PARSER_HPP
#define BOOST_BEAST_SIP_PARSER_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/http/fields.hpp>
#include <boost/beast/sip/protocol.hpp>

namespace boost {
namespace beast {
namespace sip {

template<
    bool isRequest,
    class Body,
    class Allocator = std::allocator<char>,
    class Protocol = udp_protocol>
using udp_parser = http::parser<isRequest, Body, Allocator, Protocol>;

template<
    bool isRequest,
    class Body,
    class Allocator = std::allocator<char>,
    class Protocol = tcp_protocol>
using tcp_parser = http::parser<isRequest, Body, Allocator, Protocol>;

} // sip
} // beast
} // boost

#endif
