#ifndef BOOST_BEAST_SIP_FIELDS_HPP
#define BOOST_BEAST_SIP_FIELDS_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/http/fields.hpp>
#include <boost/beast/sip/protocol.hpp>

namespace boost {
namespace beast {
namespace sip {

using tcp_fields = http::basic_fields<std::allocator<char>, tcp_protocol>;
using udp_fields = http::basic_fields<std::allocator<char>, udp_protocol>;

} // sip
} // beast
} // boost

#endif
