#ifndef BOOST_BEAST_HTTP_DETAIL_RFC7235_HPP
#define BOOST_BEAST_HTTP_DETAIL_RFC7235_HPP

#include <boost/beast/core/string.hpp>
#include <iterator>
#include <utility>

namespace boost {
namespace beast {
namespace http {
namespace detail {

inline
char
is_token68_char(char c)
{
    /*
       token68 = 1*( ALPHA / DIGIT / "-" / "." / "_" / "~" / "+" / "/" ) *"="
       
       This test excludes the '=' character because they can only trail.
   */

    static char constexpr tab[] = {
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 0
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 16
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 1,  0, 1, 1, 1, // 32
        1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 0, 0,  0, 0, 0, 0, // 48
        0, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1, // 64
        1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 0,  0, 0, 0, 1, // 80
        0, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1, // 96
        1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 0,  0, 0, 1, 0, // 112
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 128
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 144
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 160
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 176
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 192
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 208
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 224
        0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0  // 240
    };
    BOOST_STATIC_ASSERT(sizeof(tab) == 256);
    return tab[static_cast<unsigned char>(c)];
}

template<class FwdIt>
inline
void
skip_commas_and_ows(FwdIt& it, FwdIt const& end)
{
    while(it != end)
    {
        if(*it != ' ' && *it != '\t' && *it != ',')
            break;
        ++it;
    }
}

} // detail
} // http
} // beast
} // boost

#endif
