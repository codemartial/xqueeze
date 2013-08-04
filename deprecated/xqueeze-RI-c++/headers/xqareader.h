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

/** \file xqareader.h
 * \brief Definition of xqML::xqAReader for reading xqA specification format
 */
#ifndef XQML_XQAREADER_H
#define XQML_XQAREADER_H
#include <iostream>
#include <string>
#include <xqml/xqmlsymbol.h>

namespace xqML
{
  using std::string;
  using std::istream;
  using xqML::xqMLSymbol;

  //! Reads an Xqueeze Association specification to help build a mapping
  class xqAReader
  {
  public:
    xqAReader();
    
    //! Creates a reader for the supplied xqA specification document stream
    xqAReader(istream &);

    //! Sets source stream for xqA specification to be read from
    void setSource(istream &);

    //! Supplies the next symbol entry that is found
    bool getNextSymbol(xqMLSymbol & symbol, string & identifier);

    //! Starts reading the xqA specification
    void startReading();

    ~xqAReader();
  };

} //Closed Namespace xqML
#endif 
