/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2003, Xqueeze Developers
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

/** \file xqmlsymbol.h
 * \brief Interface specifications for xqML::xqMLSymbol
 */
#ifndef XQML_XQMLSYMBOL_H
#define XQML_XQMLSYMBOL_H
#include <iostream>
#include <xqml/xqtypes.h>

namespace xqML {

  using std::istream;
  using std::ostream;
  using xqML::uint32_t;

  /** \ingroup utils @{ */
  /** \class xqMLSymbol xqmlsymbol.h xqml/xqmlsymbol.h 
   *  \brief Provides a data type for xqML Symbols and I/O facilities.
   */
  class xqMLSymbol
  {
  public:
    //! Constructor sets the value of the Symbol to the (optional) argument or zero.
    xqMLSymbol(uint32_t symbol = 0)
      : code(symbol) {}

    //! Enables assignment from 32-bit unsigned integers to symbols
    xqMLSymbol & operator=(const uint32_t);

    //!Enables comparison of symbols to 32-bit unsigned integers
    bool operator==(const uint32_t) const;

    //!Allows casting an %xqML Symbol to a 32-bit unsigned integer
    operator uint32_t () const;

    //! Overloads operator << for xqML Symbol output.
    friend ostream & operator<<(ostream &, xqMLSymbol);

    //! Overloads operator >> for xqML Symbol input.
    friend istream & operator>>(istream &, xqMLSymbol &);

    //! Returns numeric value of the Symbol
    uint32_t  getCode() const { return code; }
  private:
    uint32_t code; //!< Holds the numeric value of the Symbol.
  };
  /** @} */
}

//End of Include Guard
#endif
