/*****************************************************************************
 * 
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2006 Artem Pavlenko
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

#include <mapnik/stroke.hpp>

static const char * line_cap_strings[] = {
    "butt",
    "square",
    "round",
    ""
};

IMPLEMENT_ENUM( mapnik::line_cap_e, line_cap_strings );

static const char * line_join_strings[] = {
    "miter",
    "miter_revert",
    "round",
    "bevel",
    ""
};

IMPLEMENT_ENUM( mapnik::line_join_e, line_join_strings );

