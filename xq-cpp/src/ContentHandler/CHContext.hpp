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

/** \file CHContext.hpp
 * \brief Header for xqML::CHContext
 */
#ifndef XQML_CHCONTEXT_HPP
#define XQML_CHCONTEXT_HPP

#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <utility>
#include <xqml/xqtypes.hpp>
#include <xqml/XqMap.hpp>
#include <xqml/XqaHandler.hpp>
#include "ContentHandlerState.hpp"

using std::ostream;
using std::string;
using std::map;
using std::pair;
using std::stack;

namespace xqML
{
  
  /** \class CHContext
      \brief Encapsulates the context data of a TranslatingContentHandler
  */
  struct CHContext
  {
    typedef map<string, xqML::XqA *> URIMap;
    typedef map<string, string> PrefixURIMap;
    typedef map<string, uint32_t> PrefixCodeMap;
    typedef map<string, uint32_t> DynAssoc;
    typedef map<string, DynAssoc> DynMap;
    typedef stack<pair<string, string> > OpenELStack;
    
    //! Constructor sets the XqA handler and output stream
    CHContext(XqaHandler & handler, ostream & stream, bool dynamic = false);
    
    //! Look up an identifier in the XqA corresponding to the given uri
    uint32_t lookup(const string & uri, const string & id);
    
    //! Look up an identifier in the dynamic map corresponding to the given uri
    uint32_t dynamicLookup(const string & uri, const string & name);
    
    CHStateBox state_box;                 //!< container for all states
    ContentHandlerState * ch_state;       //!< currently enabled state
    XqaHandler & xqa_handler;             //!< XqA handler
    ostream & out;                        //!< output stream
    bool do_dynamic_maps;                 //!< Whether to build Dynamic Maps or not
    uint32_t current_prefix;              //!< namespace prefix in scope
    string current_uri;                   //!< namespace uri in scope
    PrefixURIMap prefix_uri_map;          //!< Prefix to URI mapping
    URIMap uri_map;                       //!< URI to XqA * mapping
    PrefixCodeMap prefix_codes;           //!< URI to Prefix code mapping
    DynMap dyn_maps;                      //!< URI to Dynamic Association table map
    OpenELStack open_el_stack;            //!< Open element stack
  };
}

// End of Include Guard
#endif
