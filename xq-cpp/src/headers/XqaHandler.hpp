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

/** \file XqaHandler.hpp
 * \brief Helper Class for managing string-symbol associations
 */
#ifndef XQML_XQAHANDLER_HPP
#define XQML_XQAHANDLER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include <xqml/XqMap.hpp>
#include <xqml/xqtypes.hpp>
#include <xqml/xqmlsymbol.hpp>

using std::string;
using std::istream;
using std::ifstream;

namespace xqML
{

  typedef xqML::OrderedPairMap<uint32_t, string> XqA;
  typedef std::map<string, XqA> XqACache;

  struct XqAFinder
  {
    virtual istream * operator()(const string & uri) = 0;
  };
  
  struct XqSpecFinder: public XqAFinder
  {
    inline istream * operator()(const string & uri)
    {
      return new ifstream((uri + ".xqa").c_str(), std::ios_base::binary);
    }
  };

  //! Global lookup table for xqueeze associations
  class XqaHandler 
  {
  public:
    XqaHandler(XqAFinder & xqa_finder)
      : xqa_finder(xqa_finder)
    {}

    //! Return XqA corresponding to the URI
    XqA * getXqA(const string &);
    
    bool registerXqA(const string & uri, std::auto_ptr<istream> spec_stream);
    bool registerXqA(const string & uri, istream * const spec_stream);
    
  private:
    /**  Builds an XqA map from specifications read
     *   from the istream argument
     */
    XqA buildMap(std::auto_ptr<istream> source);
    XqA buildMap(istream * const source);
    
    XqACache known_maps;      //!< Currently loaded XqA maps
    XqAFinder & xqa_finder;   //!< Functor to return an istream for the specfile
  }; 

}

// End of Include Guard
#endif
