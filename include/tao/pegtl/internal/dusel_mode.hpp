// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_DUSEL_MODE_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_DUSEL_MODE_HPP

#include "../config.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      enum class dusel_mode : char
      {
         NOTHING = 0,
         CONTROL = 1,
         CONTROL_AND_APPLY = 2,
         CONTROL_AND_APPLY0 = 3
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
