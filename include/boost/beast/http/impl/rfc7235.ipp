#ifndef BOOST_BEAST_HTTP_IMPL_RFC7235_IPP
#define BOOST_BEAST_HTTP_IMPL_RFC7235_IPP

#include <boost/beast/http/detail/rfc7230.hpp>
#include <boost/beast/http/detail/rfc7235.hpp>
#include <iterator>

namespace boost {
namespace beast {
namespace http {

class auth_param_list::const_iterator
{
public:
    using value_type = auth_param_list::value_type;

private:
    using iter_type = string_view::const_iterator;

    std::string s_;
    value_type v_;
    iter_type it_;
    iter_type last_;
    iter_type first_;

public:
    using pointer = value_type const*;
    using reference = value_type const&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    const_iterator() = default;
    const_iterator(const_iterator const& o) :
	s_(o.s_), v_(o.v_), it_(o.it_), last_(o.last_), first_(o.first_)
    {
	if (!s_.empty()) v_.second = { s_.data(), s_.size() };
    }

    const_iterator(const_iterator&& o) :
	s_(std::move(o.s_)), v_(o.v_), it_(o.it_), last_(o.last_),
	first_(o.first_)
    {
	if (!s_.empty()) v_.second = { s_.data(), s_.size() };
    }

    const_iterator& operator=(const_iterator&& o)
    {
	s_ = std::move(o.s_);
       	v_ = o.v_;
	it_ = o.it_;
	last_ = o.last_;
	first_ = o.first_;
	if (!s_.empty()) v_.second = { s_.data(), s_.size() };
	return *this;
    }

    const_iterator& operator=(const_iterator const& o)
    {
	s_ = o.s_;
       	v_ = o.v_;
	it_ = o.it_;
	last_ = o.last_;
	first_ = o.first_;
	if (!s_.empty()) v_.second = { s_.data(), s_.size() };
	return *this;
    }

    bool
    operator==(const_iterator const& other) const
    {
        return
            other.it_ == it_ &&
            other.last_ == last_ &&
            other.first_ == first_;
    }

    bool
    operator!=(const_iterator const& other) const
    {
        return !(*this == other);
    }

    reference
    operator*() const
    {
        return v_;
    }

    pointer
    operator->() const
    {
        return &*(*this);
    }

    const_iterator&
    operator++()
    {
        increment();
        return *this;
    }

    const_iterator
    operator++(int)
    {
        auto temp = *this;
        ++(*this);
        return temp;
    }

private:
    friend class auth_param_list;
    friend class auth_list;

    const_iterator(iter_type first, iter_type last)
    {
        it_ = first;
        last_ = last;
        first_ = first;
        increment();
    }

    template<class = void>
    bool
    increment(bool do_unquote = true);
};

inline
auto
auth_param_list::
begin() const ->
    const_iterator
{
    return const_iterator{s_.begin(), s_.end()};
}

inline
auto
auth_param_list::
end() const ->
    const_iterator
{
    return const_iterator{s_.end(), s_.end()};
}

inline
auto
auth_param_list::
cbegin() const ->
    const_iterator
{
    return const_iterator{s_.begin(), s_.end()};
}

inline
auto
auth_param_list::
cend() const ->
    const_iterator
{
    return const_iterator{s_.end(), s_.end()};
}

template<class T>
auto
auth_param_list::
find(T const& s) const ->
    const_iterator
{
    return std::find_if(begin(), end(),
        [&s](value_type const& v)
        {
            return iequals(s, v.first);
        });
}

template<class>
bool
auth_param_list::const_iterator::
increment(bool do_unquote)
{
    s_.clear();
    v_.first = {};
    v_.second = {};

    auto const err =
        [&]
        {
            it_ = last_;
            first_ = last_;
	    return false;
        };

    bool need_comma = it_ != first_;
    detail::skip_ows(it_, last_);
    if (it_ == last_)
	return err();
    if(need_comma) {
	if(*it_ != ',')
	    return err();
	detail::skip_commas_and_ows(++it_, last_);
	if (it_ == last_)
	    return err();
    }
    if (!detail::is_token_char(*it_))
	return err();
    auto const p0 = it_;
    detail::skip_token(++it_, last_);
    v_.first = { &*p0, static_cast<std::size_t>(it_ - p0) };
    detail::skip_ows(it_, last_);
    if (it_ == last_ || *it_ != '=')
	return err();
    detail::skip_ows(++it_, last_);
    if (it_ == last_)
	return err();
    if(*it_ == '"') {
        // quoted-string
        auto const p1 = it_;
        ++it_;
        for(;;)
        {
            if(it_ == last_)
                return err();
            auto c = *it_++;
            if(c == '"')
                break;
            if(detail::is_qdchar(c))
                continue;
            if(c != '\\')
                return err();
            if(it_ == last_)
                return err();
            c = *it_++;
            if(! detail::is_qpchar(c))
                return err();
        }
	v_.second = { &*p1, static_cast<std::size_t>(it_ - p1) };

	if(do_unquote) {
	    s_ = detail::unquote(v_.second);
	    v_.second = { s_.data(), s_.size() };
	}
    }
    else
    {
	if (!detail::is_token_char(*it_))
	    return err();
	auto const p1 = it_;
	detail::skip_token(++it_, last_);
        v_.second = { &*p1, static_cast<std::size_t>(it_ - p1) };
    }

    return true;
}

class auth_list::const_iterator
{
    auth_list::value_type v_;
    iter_type it_;
    iter_type first_;
    iter_type last_;

public:
    using value_type = auth_list::value_type;
    using pointer = value_type const*;
    using reference = value_type const&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    const_iterator() = default;

    bool
    operator==(const_iterator const& other) const
    {
        return
            other.it_ == it_ &&
            other.first_ == first_ &&
            other.last_ == last_;
    }

    bool
    operator!=(const_iterator const& other) const
    {
        return !(*this == other);
    }

    reference
    operator*() const
    {
        return v_;
    }

    pointer
    operator->() const
    {
        return &*(*this);
    }

    const_iterator&
    operator++()
    {
        increment();
        return *this;
    }

    const_iterator
    operator++(int)
    {
        auto temp = *this;
        ++(*this);
        return temp;
    }

private:
    friend class auth_list;

    const_iterator(iter_type begin, iter_type end)
    {
        it_ = begin;
        first_ = begin;
        last_ = end;
        increment();
    }

    template<class = void>
    void
    increment();
};

inline
auto
auth_list::
begin() const ->
    const_iterator
{
    return const_iterator{s_.begin(), s_.end()};
}

inline
auto
auth_list::
end() const ->
    const_iterator
{
    return const_iterator{s_.end(), s_.end()};
}

inline
auto
auth_list::
cbegin() const ->
    const_iterator
{
    return const_iterator{s_.begin(), s_.end()};
}

inline
auto
auth_list::
cend() const ->
    const_iterator
{
    return const_iterator{s_.end(), s_.end()};
}

template<class T>
auto
auth_list::
find(T const& s) ->
    const_iterator
{
    return std::find_if(begin(), end(),
        [&s](value_type const& v)
        {
            return iequals(s, std::get<0>(v));
        });
}

template<class T>
bool
auth_list::
exists(T const& s)
{
    return find(s) != end();
}

template<class>
void
auth_list::const_iterator::
increment()
{
    /*
	Authorization = credentials
	WWW-Authenticate = *( "," OWS ) challenge *( OWS "," [ OWS challenge ] )
	auth-param = token BWS "=" BWS ( token / quoted-string )
	auth-scheme = token
	challenge = auth-scheme
	    [ 1*SP
		( token68 /
		  [ ( "," / auth-param ) *( OWS "," [ OWS auth-param ] ) ]
	        )
	    ]
	credentials = auth-scheme [ 1*SP ( token68 / [ ( "," / auth-param )
		*( OWS "," [ OWS auth-param ] ) ] ) ]
	token68 = 1*( ALPHA / DIGIT / "-" / "." / "_" / "~" / "+" / "/" ) *"="
     */
    auto const err =
        [&]
        {
            it_ = last_;
            first_ = last_;
        };

    std::get<0>(v_) = {};
    std::get<1>(v_) = {};
    std::get<2>(v_) = auth_param_list{string_view{}};

    if (it_ == last_)
	return err();

    if(first_ != it_) {
	detail::skip_ows(it_, last_);
	if(it_ == last_ || *it_ != ',')
	    return err();
	++it_;
    }

    detail::skip_commas_and_ows(it_, last_);

    if(it_ == last_ || !detail::is_token_char(*it_))
	return err();
    auto const p0 = it_;
    detail::skip_token(++it_, last_);
    std::get<0>(v_) = string_view{&*p0, static_cast<std::size_t>(it_ - p0)};
    
    if (it_ == last_ || *it_ != ' ')
	return;

    do {
	++it_;
    } while (it_ != last_ && *it_ == ' ');

    if (it_ == last_)
	return;

    if (detail::is_token68_char(*it_)) {
	auto const p1 = it_;

	do {
	    ++it_;
	} while (it_ != last_ && detail::is_token68_char(*it_));

	while (it_ != last_ && *it_ == '=')
	    ++it_;

	auto const p2 = it_;

	detail::skip_ows(it_, last_);

	// This is a token68 if we are at the end of the string or if the
	// next character is a comma. Otherwise we need to backtrack.

	if (it_ == last_ || *it_ == ',') {
	    std::get<1>(v_) = string_view{&*p1,
		    static_cast<std::size_t>(p2 - p1)};
	    return;
	}

	// Not a match, backtrack.
	it_ = p1;
    } else if(*it_ == ',') {
	detail::skip_commas_and_ows(++it_, last_);
	if (it_ == last_)
	    return err();
    }

    if(!detail::is_token_char(*it_))
	return err();

    auth_param_list::const_iterator i;
    i.it_ = it_;
    i.first_ = it_;
    i.last_ = last_;
    auto p2 = it_;
    while(i.increment(false))
	p2 = i.it_;
    if (p2 == it_)
	return err();
    std::get<2>(v_) = auth_param_list{string_view{&*it_,
	static_cast<std::size_t>(p2 - it_)}};
    it_ = p2;
}

} // http
} // beast
} // boost

#endif
