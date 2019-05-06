#ifndef BOOST_BEAST_SIP_MESSAGE_HPP
#define BOOST_BEAST_SIP_MESSAGE_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/sip/fields.hpp>

namespace boost {
namespace beast {
namespace sip {

template<class Body, class Fields = udp_fields>
using udp_request = http::message<true, Body, Fields>;

template<class Body, class Fields = udp_fields>
using udp_response = http::message<false, Body, Fields>;

template<class Body, class Fields = tcp_fields>
using tcp_request = http::message<true, Body, Fields>;

template<class Body, class Fields = tcp_fields>
using tcp_response = http::message<false, Body, Fields>;

} // sip
} // beast
} // boost

#endif
