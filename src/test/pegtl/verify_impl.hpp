// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_TEST_VERIFY_IMPL_HPP
#define TAOCPP_PEGTL_INCLUDE_TEST_VERIFY_IMPL_HPP

#include <cstddef>
#include <string>

#include <tao/pegtl/memory_input.hpp>

#include "test_failed.hpp"
#include "verify_help.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Rule, typename Eol >
      void verify_impl( const std::size_t line, const char* file, const std::string& data, const result_type expected, const std::size_t remain )
      {
         basic_memory_input< Eol > i( { 0, line, 0, data.data() }, data.data() + data.size(), file );

         const result_type received = verify_help< Rule >( i );

         if( ( received == expected ) && ( ( received == result_type::GLOBAL_FAILURE ) || ( i.size( 999999999 ) == remain ) ) ) {
            return;
         }
         TAOCPP_PEGTL_TEST_FAILED( "input data [ '" << data << "' ] result received/expected [ " << received << " / " << expected << " ] remain received/expected [ " << i.size( 999999999 ) << " / " << remain << " ]" );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
