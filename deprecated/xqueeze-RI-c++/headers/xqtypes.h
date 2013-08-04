/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2003 Xqueeze Developers
 * 
 * This file is part of Xqueeze Reference Implementation.
 * 
 * Xqueeze Reference Implementation is free software; you can
 * redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * Xqueeze Reference Implementation is distributed in the hope that
 * it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Xqueeze Reference Implementation; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA
 */

/** \file xqtypes.h
 *  \brief Utility data types for Xqueeze library
 */

#ifndef XQML_XQTYPES_H
#  define XQML_XQTYPES_H

  // We'll try to use the C99 standard header stdint.h to access
  // integral types of known size.

# ifdef XQ_HAVE_INTTYPES_H
#   include <stdint.h>
# else
  // I'm putting in some 32-bit architecture specific stuff here,
  // that too as defined by GCC. This may need modification for
  // different architectures and/or compilers.

  typedef short int		int16_t;  //!< 16-bit signed int
  typedef int			int32_t;  //!< 32-bit signed int

  typedef unsigned short int	uint16_t; //!< 16-bit unsigned int
  typedef unsigned int		uint32_t; //!< 32-bit unsigned int

# endif //Close check for availability of stdint.h
#endif // End of include guard
