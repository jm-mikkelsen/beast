#ifndef BOOST_BEAST_HTTP_RFC7235_HPP
#define BOOST_BEAST_HTTP_RFC7235_HPP

#include <boost/beast/core/detail/config.hpp>
#include <boost/beast/http/detail/rfc7230.hpp>
#include <boost/beast/http/detail/basic_parsed_list.hpp>
#include <tuple>

namespace boost {
namespace beast {
namespace http {

/** A list of authentication parameters in an HTTP authentication field value.

    This container allows iteration of the parameter list in an HTTP
    extension. The parameter list is a series of name/value pairs
    with each pair starting with a semicolon. The value is optional.

    If a parsing error is encountered while iterating the string,
    the behavior of the container will be as if a string containing
    only characters up to but excluding the first invalid character
    was used to construct the list.

    @par BNF
    @code
	Authorization = credentials
	WWW-Authenticate = *( "," OWS ) challenge *( OWS "," [ OWS challenge ] )
	auth-param = token BWS "=" BWS ( token / quoted-string )
	auth-scheme = token
	challenge = auth-scheme [ 1*SP ( token68 / [ ( "," / auth-param ) *(
		OWS "," [ OWS auth-param ] ) ] ) ]
	credentials = auth-scheme [ 1*SP ( token68 / [ ( "," / auth-param )
		*( OWS "," [ OWS auth-param ] ) ] ) ]
	token68 = 1*( ALPHA / DIGIT / "-" / "." / "_" / "~" / "+" / "/" ) *"="
    @endcode

    To use this class, construct with the string to be parsed and
    then use @ref begin and @ref end, or range-for to iterate each
    item:

    @par Example
    @code
    for(auto const& param : param_list{"basic abc==, digest md5, nonce=\"abc\"")
    {
        std::cout << ";" << param.first;
        if(! param.second.empty())
            std::cout << "=" << param.second;
        std::cout << "\n";
    }
    @endcode
*/
class auth_param_list
{
    string_view s_;

public:
    /** The type of each element in the list.

        The first string in the pair is the name of the parameter,
        and the second string in the pair is its value (which may
        be empty).
    */
    using value_type =
        std::pair<string_view, string_view>;

    /// A constant iterator to the list
#if BOOST_BEAST_DOXYGEN
    using const_iterator = __implementation_defined__;
#else
    class const_iterator;
#endif

    /// Default constructor.
    auth_param_list() = default;

    /** Construct a list.

        @param s A string containing the list contents. The string
        must remain valid for the lifetime of the container.
    */
    explicit
    auth_param_list(string_view s)
        : s_(s)
    {
    }

    /// Return a const iterator to the beginning of the list
    const_iterator begin() const;

    /// Return a const iterator to the end of the list
    const_iterator end() const;

    /// Return a const iterator to the beginning of the list
    const_iterator cbegin() const;

    /// Return a const iterator to the end of the list
    const_iterator cend() const;

    /** Find a token in the list.

        @param s The token to find. A case-insensitive comparison is used.

        @return An iterator to the matching token, or `end()` if no
        token exists.
    */
    template<class T>
    const_iterator
    find(T const& s) const;
};

//------------------------------------------------------------------------------

/** A list of extensions in a comma separated HTTP field value.

    This container allows iteration of the extensions in an HTTP
    field value. The extension list is a comma separated list of
    token parameter list pairs.

    If a parsing error is encountered while iterating the string,
    the behavior of the container will be as if a string containing
    only characters up to but excluding the first invalid character
    was used to construct the list.

    @par BNF
    @code
        ext-list    = *( "," OWS ) ext *( OWS "," [ OWS ext ] )
        ext         = token param-list
        param-list  = *( OWS ";" OWS param )
        param       = token OWS [ "=" OWS ( token / quoted-string ) ]
    @endcode

    To use this class, construct with the string to be parsed and
    then use @ref begin and @ref end, or range-for to iterate each
    item:

    @par Example
    @code
    for(auto const& ext : ext_list{"none, 7z;level=9, zip;no_context_takeover;bits=15"})
    {
        std::cout << ext.first << "\n";
        for(auto const& param : ext.second)
        {
            std::cout << ";" << param.first;
            if(! param.second.empty())
                std::cout << "=" << param.second;
            std::cout << "\n";
        }
    }
    @endcode
*/
class auth_list
{
    using iter_type = string_view::const_iterator;

    string_view s_;

public:
    /** The type of each element in the list.

	The first element of the tuple is the authentication scheme, the second
	element of the tuple, and the third element is an iterable container
	holding the name/value parameters for the challenge.
    */
    using value_type = std::tuple<string_view, string_view, auth_param_list>;

    /// A constant iterator to the list
#if BOOST_BEAST_DOXYGEN
    using const_iterator = __implementation_defined__;
#else
    class const_iterator;
#endif

    /** Construct a list.

        @param s A string containing the list contents. The string
        must remain valid for the lifetime of the container.
    */
    explicit
    auth_list(string_view s)
        : s_(s)
    {
    }

    /// Return a const iterator to the beginning of the list
    const_iterator begin() const;

    /// Return a const iterator to the end of the list
    const_iterator end() const;

    /// Return a const iterator to the beginning of the list
    const_iterator cbegin() const;

    /// Return a const iterator to the end of the list
    const_iterator cend() const;

    /** Find a token in the list.

        @param s The token to find. A case-insensitive comparison is used.

        @return An iterator to the matching token, or `end()` if no
        token exists.
    */
    template<class T>
    const_iterator
    find(T const& s);

    /** Return `true` if a token is present in the list.

        @param s The token to find. A case-insensitive comparison is used.
    */
    template<class T>
    bool
    exists(T const& s);
};

} // http
} // beast
} // boost

#include <boost/beast/http/impl/rfc7235.ipp>

#endif
