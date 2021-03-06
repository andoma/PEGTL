// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      struct test_grammar : seq< string< 'a', 'b' >, discard, string< 'c', 'd' >, discard, string< 'e', 'f' >, discard, eof >
      {
      };

      void unit_test()
      {
         const char* test_data = "abcdef";
         TAOCPP_PEGTL_TEST_ASSERT( parse_cstring< test_grammar >( test_data, "test data", 2 ) );
         TAOCPP_PEGTL_TEST_ASSERT( !parse_cstring< test_grammar >( test_data, "test data", 1 ) );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
