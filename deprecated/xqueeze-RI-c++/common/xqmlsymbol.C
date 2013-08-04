/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2002 2003, Xqueeze Developers
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

/** \file xqmlsymbol.C
 * \brief Implementation of xqML::xqMLSymbol
 */
#include <iostream>
#include <xqml/xqtypes.h>
#include <xqml/xqmlsymbol.h>

namespace xqML {
  using xqML::uint32_t;

  /** Assigns the value of a 32-bit unsigned integer to an %xqML
      Symbol. Does not check whether the xqML Symbol has a valid
      structure (i.e. uses lsb as a continuation flag)
   */
  xqMLSymbol & xqMLSymbol::operator=(const uint32_t num)
  {
    this->code = num;
    return *this;
  }

  bool xqMLSymbol::operator==(const uint32_t num) const
  {
    return this->code == num;
  }
  
  xqMLSymbol::operator uint32_t () const
  {
    return this->code;
  }

  /** This operator is capable of writing any 32-bit unsigned integer in
   * network byte order since it doesn't check whether the xqMLSymbol is
   * indeed valid (i.e. properly uses the lsb as a continuation flag). 
   * It would also write the null byte '\\0'.
   * 
   * \requires The symbol should have no longer than 32-bit unsigned
   * integral code value.
   */
  ostream & operator<<(ostream & stream, xqMLSymbol symbol)
  {
    char octets[4];
    short int i = 0;
    octets[i] = static_cast<unsigned char> (symbol.code%256);
    symbol.code /= 256;
    for(i = 1; symbol.code; ++i) {
      octets[i] = static_cast<unsigned char> (symbol.code%256);
      symbol.code /= 256;
    }
    for(--i; i >= 0; --i)
      stream.put(octets[i]);
    return stream;
  }

  /** \promises The %xqML Symbol read will be structurally valid
   *  (i.e. properly uses the lsb as a continuation flag).
   *
   *  \requires The symbol in the input stream should not have a value
   *  greater than 2^32. If it is, then the most significant octets will
   *  be discarded.
   */
  istream & operator>>(istream & stream, xqMLSymbol & symbol)
  {
    symbol.code = 0;
    char octet;
    stream.get(octet); // Atleast one octet is to be uncoditionally read
    symbol.code = static_cast<unsigned char>(octet);
    while (symbol.code%2 && stream.get(octet)) {
      symbol.code <<= 8;
      symbol.code |= static_cast<unsigned char>(octet);
    }
    return stream;
  }

}
