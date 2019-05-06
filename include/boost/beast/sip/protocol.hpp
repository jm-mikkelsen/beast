#ifndef BOOST_BEAST_SIP_PROTOCOL_HPP
#define BOOST_BEAST_SIP_PROTOCOL_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/core/string.hpp>

namespace boost {
namespace beast {
namespace sip {

class protocol_base
{
public:
    static string_view constexpr name()
    {
	return "SIP";
    }

    static http::field string_to_field(string_view name)
    {
	if (name.size() == 1)
	{
	    // Compact forms from RFC 3261, section 20.
	    switch (detail::ascii_tolower(name[0])) {
		case 'i': return http::field::call_id;
		case 'm': return http::field::contact;
		case 'e': return http::field::content_encoding;
		case 'l': return http::field::content_length;
		case 'c': return http::field::content_type;
		case 'f': return http::field::from;
		case 's': return http::field::subject;
		case 'k': return http::field::supported;
		case 't': return http::field::to;
		case 'v': return http::field::via;
		default: break;
	    }
	}
	return http::default_string_to_field(name);
    }

    static bool constexpr allow_chunked(int version)
    {
	// From RFC 3261, section 7.4.2:
	//
	// The "chunked" transfer encoding of HTTP/1.1 MUST NOT
	// be used for SIP.

	boost::ignore_unused(version);

	return false;
    }

    static bool constexpr accept_chunked()
    {
	return false;
    }

    static bool check_version(int v)
    {
	return v == 20;
    }

    static constexpr int default_version()
    {
	return 20;
    }
};

class stream_protocol : public protocol_base
{
public:
    static string_view field_to_compact(http::field name)
    {
	return to_string(name);
    }

    static constexpr
    std::uint64_t
    default_body_limit(std::false_type)
    {
        // limit for responses
        return 1 * 1024 * 1024;
    }

    static constexpr
    std::uint64_t
    default_body_limit(std::true_type)
    {
        // limit for requests
        return 1 * 1024 * 1024;
    }

    static bool constexpr content_length_required()
    {
	// RFC 2161, section 20.14:
	//
	// If a stream-based protocol (such as TCP) is used as
	// transport, the header field MUST be used.

	return true;
    }

    static bool constexpr override_content_length()
    {
	// Not used because content_length_required() return true

	return false;
    }

    static bool constexpr use_http11_keepalive(int)
    {
	return true;
    }
};

class datagram_protocol : public protocol_base
{
public:
    static string_view field_to_compact(http::field name)
    {
	// Compact forms used when transmitting over from RFC 3261, section 20.
	// There is an argument for doing this consistently across stream
	// and datagram transports.

	switch (name) {
	    case http::field::call_id: return "i";
	    case http::field::contact: return "m";
	    case http::field::content_encoding: return "e";
	    case http::field::content_length: return "l";
	    case http::field::content_type: return "c";
	    case http::field::from: return "f";
	    case http::field::subject: return "s";
	    case http::field::supported: return "k";
	    case http::field::to: return "t";
	    case http::field::via: return "v";
	    default: return to_string(name);
	}
    }

    static string_view name_to_compact(string_view sname)
    {
	auto name = string_to_field(sname);
	return name == http::field::unknown ? sname : field_to_compact(name);
    }

    // Body size details
    //
    //    From RFC 3261, section 18.1.1:
    //
    //    If a request is within 200 bytes of the path MTU, or if it is larger
    //    than 1300 bytes and the path MTU is unknown, the request MUST be
    //    sent using an RFC 2914 [43] congestion controlled transport
    //    protocol, such as TCP. If this causes a change in the transport
    //    protocol from the one indicated in the top Via, the value in the top
    //    Via MUST be changed.  This prevents fragmentation of messages over
    //    UDP and provides congestion control for larger messages.  However,
    //    implementations MUST be able to handle messages up to the maximum
    //    datagram packet size.  For UDP, this size is 65,535 bytes, including
    //    IP and UDP headers.
    //
    // Approach here is to set the body size limits for requests and replies
    // to (65535 - 200). This ignores the fact that the limits here are just
    // for the body, and not for the overall datagram. There is an argument
    // for setting the values a little bit lower.
    //

    static constexpr
    std::uint64_t
    default_body_limit(std::false_type)
    {
        // limit for responses
	return 65535 - 200;
    }

    static constexpr
    std::uint64_t
    default_body_limit(std::true_type)
    {
        // limit for requests
        return 65535 - 200;
    }

    // Content-Length handling
    //
    // Do not require a Content-Length header in datagram message,
    // as we do for a stream based transport. If the Content-Length
    // header is absent do not assume zero as with, for example, RTSP.
    //
    // From RFC 3261, section 18.3:
    //
    //    In the case of message-oriented transports (such as UDP), if the
    //    message has a Content-Length header field, the message body is
    //    assumed to contain that many bytes.  If there are additional bytes
    //    in the transport packet beyond the end of the body, they MUST be
    //    discarded.  If the transport packet ends before the end of the
    //    message body, this is considered an error.  If the message is a
    //    response, it MUST be discarded.  If the message is a request, the
    //    element SHOULD generate a 400 (Bad Request) response.  If the
    //    message has no Content-Length header field, the message body is
    //    assumed to end at the end of the transport packet.

    static bool constexpr content_length_required()
    {
	return false;
    }

    static bool constexpr override_content_length()
    {
	return false;
    }

    static bool constexpr use_http11_keepalive(int)
    {
	return false;
    }
};

using tcp_protocol = stream_protocol;
using udp_protocol = datagram_protocol;

} // sip
} // beast
} // boost

#endif
