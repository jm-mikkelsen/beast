//
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

#ifndef BOOST_BEAST_HTTP_IMPL_PARSER_IPP
#define BOOST_BEAST_HTTP_IMPL_PARSER_IPP

#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace boost {
namespace beast {
namespace http {

template<bool isRequest, class Body, class Allocator, class Protocol>
parser<isRequest, Body, Allocator, Protocol>::
parser()
    : rd_(m_.base(), m_.body())
{
}

template<bool isRequest, class Body, class Allocator, class Protocol>
template<class Arg1, class... ArgN, class>
parser<isRequest, Body, Allocator, Protocol>::
parser(Arg1&& arg1, ArgN&&... argn)
    : m_(
        std::forward<Arg1>(arg1),
        std::forward<ArgN>(argn)...)
    , rd_(m_.base(), m_.body())
{
    m_.clear();
}

template<bool isRequest, class Body, class Allocator, class Protocol>
template<class OtherBody, class... Args, class>
parser<isRequest, Body, Allocator, Protocol>::
parser(
    parser<isRequest, OtherBody, Allocator>&& other,
    Args&&... args)
    : base_type(std::move(other))
    , m_(other.release(), std::forward<Args>(args)...)
    , rd_(m_.base(), m_.body())
{
    if(other.rd_inited_)
        BOOST_THROW_EXCEPTION(std::invalid_argument{
            "moved-from parser has a body"});
}

} // http
} // beast
} // boost

#endif
