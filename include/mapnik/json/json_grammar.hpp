/*****************************************************************************
 * 
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2011 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#ifndef MAPNIK_JSON_GRAMMAR_HPP
#define MAPNIK_JSON_GRAMMAR_HPP

#include <boost/assert.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
// spirit::qi
#include <boost/spirit/include/qi.hpp>
// spirit::phoenix
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
// mapnik
#include <mapnik/geometry.hpp>

#define BOOST_SPIRIT_DEBUG

namespace mapnik { namespace json {

using namespace boost::spirit;
using namespace boost::fusion;
using namespace boost::phoenix;

struct push_vertex
{
    template <typename T0,typename T1, typename T2, typename T3>
    struct result
    {
        typedef void type;
    };
    
    template <typename T0,typename T1, typename T2, typename T3>
    void operator() (T0 c, T1 path, T2 x, T3 y) const
    {
        BOOST_ASSERT( path!=0 );
        path->push_vertex(x,y,c);        
    }
};

struct cleanup
{
    template <typename T0>
    struct result
    {
        typedef void type;
    };
    
    template <typename T0>
    void operator() (T0 & path) const
    {        
        if (path)  delete path,path=0;
    }
};

template <typename Iterator>
struct json_grammar : qi::grammar<Iterator,  boost::ptr_vector<mapnik::geometry_type>() , ascii::space_type>
{
    json_grammar()
        : json_grammar::base_type(geometry_tagged_text)
    {       
        using qi::no_case;   
        using boost::phoenix::push_back;
        using ascii::char_;
        using qi::lexeme;
        
        geometry_tagged_text = point_tagged_text
            ;
        
        quoted_string = lexeme['"' >> +(char_ - '"') >> '"'];
        
         point_tagged_text = '{' >> no_case[
               '"' >> lit("type") >> '"' >> ':' 
               >> '"' >> lit("Point") >> '"'
               >> ',' >> '"' >> lit("coordinates")
               >> '"' >> ':'
               ] [ _a = new_<geometry_type>(Point) ]
            >> ( point_text(_a) [push_back(_val,_a)] >> '}'
                 | eps[cleanup_(_a)][_pass = false])
            ;
       
        // <point text> ::= <empty set> | <left paren> <point> <right paren>
        point_text = (lit('[') >> point(SEG_MOVETO,_r1) >> lit(']')) 
            | empty_set
            ;

        empty_set = no_case[lit("null")];

        // point 
        point = (double_ >> lit(',') >> double_) [push_vertex_(_r1,_r2,_1,_2)];

    }

    // start
    qi::rule<Iterator,boost::ptr_vector<geometry_type>(),ascii::space_type> geometry_tagged_text;
    
    qi::rule<Iterator,qi::locals<geometry_type*>,boost::ptr_vector<geometry_type>(),ascii::space_type> point_tagged_text;
    qi::rule<Iterator,void(geometry_type*),ascii::space_type> point_text;
    qi::rule<Iterator,void(CommandType,geometry_type*),ascii::space_type> point;
    qi::rule<Iterator,ascii::space_type> empty_set;
    boost::phoenix::function<push_vertex> push_vertex_;
    boost::phoenix::function<cleanup> cleanup_;
    qi::rule< Iterator,std::string(),ascii::space_type > quoted_string;
};

}}

#endif // MAPNIK_JSON_GRAMMAR_HPP
