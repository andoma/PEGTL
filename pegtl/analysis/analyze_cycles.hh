// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_ANALYZE_CYCLES_HH
#define PEGTL_ANALYSIS_ANALYZE_CYCLES_HH

#include <cassert>

#include <map>
#include <set>

#include <utility>
#include <iostream>

#include "rule_dummy.hh"
#include "insert_impl.hh"
#include "grammar_info.hh"
#include "insert_guard.hh"

namespace pegtl
{
   namespace analysis
   {
      class analyze_cycles
      {
      public:
         template< typename Rule >
         explicit
         analyze_cycles( const rule_dummy< Rule > dummy )
               : m_verbose( dummy.verbose ),
                 m_problems( 0 )
         {
            insert_rule< Rule >( m_info, static_cast< const typename Rule::internal_t * >( nullptr ) );
         }

         std::size_t problems()
         {
            for ( auto i = m_info.map.begin(); i != m_info.map.end(); ++i ) {
               const bool consumes = work( i, false );
               m_map[ i->first ] = consumes;
            }
            return m_problems;
         }

         template< typename Rule >
         bool consumes() const
         {
            const auto i = m_map.find( internal::demangle< Rule >() );
            assert( i != m_map.end() );
            return i->second;
         }

      private:
         const bool m_verbose;
         unsigned m_problems;
         grammar_info m_info;
         std::set< std::string > m_stack;
         std::set< std::string > m_until;
         std::set< std::string > m_rules;
         std::map< std::string, bool > m_map;  // For unit tests.

         const std::map< std::string, rule_info >::const_iterator find( const std::string & name ) const
         {
            const auto iter = m_info.map.find( name );
            assert( iter != m_info.map.end() );
            return iter;
         }

         bool work( const std::map< std::string, rule_info >::const_iterator & start, const bool accum )
         {
            const auto g = make_insert_guard( m_stack, start->first );

            if ( g ) {
               switch ( start->second.type ) {
                  case rule_type::CONJUNCTION:
                  {
                     bool a = false;
                     for ( std::size_t i = 0; i < start->second.rules.size(); ++i ) {
                        a |= work( find( start->second.rules[ i ] ), accum || a );
                     }
                     return a;
                  }
                  case rule_type::DISJUNCTION:
                  {
                     bool a = work( find( start->second.rules[ 0 ] ), accum );
                     for ( std::size_t i = 1; i < start->second.rules.size(); ++i ) {
                        a &= work( find( start->second.rules[ i ] ), accum );
                     }
                     return a;
                  }
                  case rule_type::ALWAYS_CONSUMES_WHEN_SUCCEEDS:
                  {
                     return true;
                  }
                  case rule_type::CAN_SUCCEED_WITHOUT_CONSUME:
                  {
                     bool a = false;
                     for ( std::size_t i = 0; i < start->second.rules.size(); ++i ) {
                        work( find( start->second.rules[ i ] ), accum || a );
                     }
                     return false;
                  }
                  case rule_type::RULE_UNTIL:
                  {
                     bool a = false;
                     for ( std::size_t i = 1; i < start->second.rules.size(); ++i ) {
                        a |= work( find( start->second.rules[ i ] ), accum || a );
                     }
                     if ( ! a ) {
                        if ( m_until.insert( start->first ).second ) {
                           ++m_problems;
                           if ( m_verbose ) {
                              std::cout << "problem: rules without progress detected in until " << start->first << std::endl;
                           }
                        }
                     }
                     return work( find( start->second.rules[ 0 ] ), accum );
                  }
                  case rule_type::RULE_IF_THEN_ELSE:
                  case rule_type::RULE_IF_MUST_ELSE:
                  {
                     assert( start->second.rules.size() == 3 );
                     const bool c = work( find( start->second.rules[ 0 ] ), accum );  // Cond
                     const bool t = work( find( start->second.rules[ 1 ] ), accum || c );  // Rule
                     const bool e = work( find( start->second.rules[ 2 ] ), accum );  // Else
                     return ( c || t ) && e;
                  }
               }
            }
            else if ( ! accum ) {
               if ( m_rules.insert( start->first ).second ) {
                  ++m_problems;
                  if ( m_verbose ) {
                     std::cout << "problem: cycle without progress detected at class " << start->first << std::endl;
                  }
               }
               return false;
            }
            return accum;  // We have detected a cycle with progress.
         }
      };

   } // analysis

} // pegtl

#endif