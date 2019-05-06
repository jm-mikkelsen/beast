#ifndef BOOST_BEAST_HTTP_PROTOCOL_HPP
#define BOOST_BEAST_HTTP_PROTOCOL_HPP


#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/core/string.hpp>
#include <boost/beast/http/field.hpp>

namespace boost {
namespace beast {
namespace http {

class protocol
{
public:
    static constexpr string_view name()
    {
	return string_view("HTTP");
    }

    static constexpr size_t name_length()
    {
	return name().size();
    }

    static bool check_version(int v)
    {
	return v >= 10 && v <= 11;
    }

    static bool use_http11_keepalive(int v)
    {
	return v >= 11;
    }

    static int default_version()
    {
	return 11;
    }

    static string_view remap_field(string_view name)
    {
	return name;
    }

    static string_view name_to_string(field name)
    {
	return to_string(name);
    }

    static bool constexpr allow_chunked(int version)
    {
	return version >= 11;
    }

    static bool constexpr accept_chunked()
    {
	return false;
    }

    static bool constexpr content_length_required()
    {
	return false;
    }

    static bool constexpr override_content_length()
    {
	return false;
    }

    static constexpr
    std::uint64_t
    default_body_limit(std::true_type)
    {
        // limit for requests
        return 1 * 1024 * 1024; // 1MB
    }

    static constexpr
    std::uint64_t
    default_body_limit(std::false_type)
    {
        // limit for responses
        return 8 * 1024 * 1024; // 8MB
    }
};

} // http
} // beast
} // boost

#endif

