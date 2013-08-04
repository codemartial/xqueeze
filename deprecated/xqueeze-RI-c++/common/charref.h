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

/** \file charref.h
 *  \brief Interface for CharRef Unit
 */

#ifndef XQML_CHARREF_H
#define XQML_CHARREF_H

#include <common/reference.h>

namespace xqMLImpl
{
  //! A Unit of type Character Reference
  class CharRef: public xqMLImpl::Reference
  {
  public:
    //! Create a character reference from a CodePoint
    CharRef(xqMLImpl::CodePoint);
    
    //! Copy constructor
    CharRef(const CharRef &);
    
    //! Assignment operator
    CharRef operator=(const CharRef &);

  private:
    xqMLImpl::CodePoint codepoint_;
  };  
}
#endif
