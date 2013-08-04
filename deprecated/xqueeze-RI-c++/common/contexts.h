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

/** \file contexts.h
    \brief Provides an enumeration for document contexts (internal)
*/

#ifndef XQML_CONTEXTS_H
#define XQML_CONTEXTS_H

#include <string>

namespace xqMLImpl {
  /** \ingroup impl @{ */
  
  /** Enumeration of structural contexts of an xqML document to
      facilitate generation/parsing.
  */
  enum Context {prolog,         //!< Document prolog
		start_tag,      //!< (element) start tag
		ee_start_tag,   //!< empty element start tag
		content,        //!< element content
		open_attribute, //!< open attribute
		ready,          //!< ready to process document
		//not_ready,      //!< not ready to process document
		end,            //!< end of document reached
		unknown         // SHOULDN'T EVER BE NEEDED
  };

  std::string xqMLImpl::contextName(Context state) 
  {
    switch (state) {
    case prolog:
      return "prolog";
    case start_tag:
      return "start tag";
    case ee_start_tag:
      return "empty-elment start tag";
    case content:
      return "element content";
    case open_attribute:
      return "open attribute";
    case ready:
      return "ready to process document";
      //case not_ready:
      //return "not ready to process document";
    case end:
      return "end of document";
    case unknown:
      return "unknown context (shouldn't have been this bad)";
    }
    return "unknown context";
  }

/** @} */
}
#endif 
