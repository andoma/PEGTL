// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace test1
      {
         struct fiz : if_must< at< one< 'a' > >, two< 'a' > >
         {
         };
         struct foo : sor< fiz, one< 'b' > >
         {
         };
         struct bar : until< eof, foo >
         {
         };

         void test_result()
         {
            TAOCPP_PEGTL_TEST_ASSERT( applied.size() == 10 );

            TAOCPP_PEGTL_TEST_ASSERT( applied[ 0 ].first == internal::demangle< one< 'b' > >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 1 ].first == internal::demangle< foo >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 2 ].first == internal::demangle< at< one< 'a' > > >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 3 ].first == internal::demangle< two< 'a' > >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 4 ].first == internal::demangle< fiz >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 5 ].first == internal::demangle< foo >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 6 ].first == internal::demangle< one< 'b' > >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 7 ].first == internal::demangle< foo >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 8 ].first == internal::demangle< eof >() );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 9 ].first == internal::demangle< bar >() );

            TAOCPP_PEGTL_TEST_ASSERT( applied[ 0 ].second == "b" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 1 ].second == "b" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 2 ].second == "" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 3 ].second == "aa" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 4 ].second == "aa" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 5 ].second == "aa" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 6 ].second == "b" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 7 ].second == "b" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 8 ].second == "" );
            TAOCPP_PEGTL_TEST_ASSERT( applied[ 9 ].second == "baab" );
         }

         struct state1
         {
            char c;

            template< typename Input >
            state1( const Input&, std::string& )
            {
            }

            template< typename Input >
            void success( const Input&, std::string& s ) const
            {
               s += c;
            }
         };

         struct fobble : sor< state< state1, alpha >, digit >
         {
         };
         struct fibble : until< eof, fobble >
         {
         };

         template< typename Rule >
         struct action1 : nothing< Rule >
         {
         };

         template<>
         struct action1< alpha >
         {
            template< typename Input >
            static void apply( const Input& in, state1& s )
            {
               assert( in.size() == 1 );
               s.c = 0 [ in.begin() ];
            }
         };

         void state_test()
         {
            std::string result;
            parse_string< fibble, action1 >( "dk41sk41xk3", __FILE__, result );
            TAOCPP_PEGTL_TEST_ASSERT( result == "dkskxk" );
         }

         template< typename Rule >
         struct action0 : nothing< Rule >
         {
         };

         static int i0 = 0;

         template<>
         struct action0< alpha >
         {
            static void apply0()
            {
               ++i0;
            }
         };

         template<>
         struct action0< digit >
         {
            static void apply0( std::string& s )
            {
               s += '0';
            }
         };

         void apply0_test()
         {
            parse_string< star< alpha >, action0 >( "abcdefgh", __FILE__ );
            TAOCPP_PEGTL_TEST_ASSERT( i0 == 8 );
            std::string s0;
            parse_string< star< digit >, action0 >( "12345678", __FILE__, s0 );
            TAOCPP_PEGTL_TEST_ASSERT( s0 == "00000000" );
         }

      }  // namespace test1

      void unit_test()
      {
         parse_string< disable< test1::bar >, test_action >( "baab", __FILE__ );

         TAOCPP_PEGTL_TEST_ASSERT( applied.size() == 1 );

         TAOCPP_PEGTL_TEST_ASSERT( applied[ 0 ].first == internal::demangle< disable< test1::bar > >() );
         TAOCPP_PEGTL_TEST_ASSERT( applied[ 0 ].second == "baab" );

         applied.clear();

         parse_string< at< action< test_action, test1::bar > > >( "baab", __FILE__ );

         TAOCPP_PEGTL_TEST_ASSERT( applied.empty() );

         applied.clear();

         parse_string< test1::bar, test_action >( "baab", __FILE__ );

         test1::test_result();

         applied.clear();

         parse_string< action< test_action, test1::bar > >( "baab", __FILE__ );

         test1::test_result();

         applied.clear();

         parse_string< disable< enable< action< test_action, test1::bar > > > >( "baab", __FILE__ );

         test1::test_result();

         test1::state_test();
         test1::apply0_test();
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#include "main.hpp"
