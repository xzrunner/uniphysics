#pragma once

#ifndef NO_BOOST
#include "uniphysics/noncopyable.h"
#endif // NO_BOOST

namespace up
{
#ifdef NO_BOOST

// from boost's noncopyable.hpp

//  Private copy constructor and copy assignment ensure classes derived from
//  class noncopyable cannot be copied.

//  Contributed by Dave Abrahams

namespace noncopyable_  // protection from unintended ADL
{
#ifndef BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED
#define BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED

// noncopyable derives from base_token to enable Type Traits to detect
// whether a type derives from noncopyable without needing the definition
// of noncopyable itself.
//
// The definition of base_token is macro-guarded so that Type Trais can
// define it locally without including this header, to avoid a dependency
// on Core.

  struct base_token {};

#endif // #ifndef BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED

  class noncopyable: base_token
  {
  protected:
      noncopyable() = default;
      ~noncopyable() = default;
      noncopyable( const noncopyable& ) = delete;
      noncopyable& operator=( const noncopyable& ) = delete;
  };
}

typedef noncopyable_::noncopyable noncopyable;

#else

using noncopyable = noncopyable

#endif NO_BOOST
}