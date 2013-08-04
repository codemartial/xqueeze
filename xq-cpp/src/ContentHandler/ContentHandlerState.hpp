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

/** \file ContentHandlerState.hpp
 * \brief Implementation of states of a content handler
 */
#ifndef XQML_CONTENT_HANDLER_STATE_H
#define XQML_CONTENT_HANDLER_STATE_H

#include <vector>
#include <xqml/ContentHandler.hpp>
#include <xqml/xqtypes.hpp>

namespace xqML
{
  
  struct CHContext;
  
  /** \class ContentHandlerState
      \brief Represents a behavioural state of ContentHandler
   */
  class ContentHandlerState : public ContentHandler
  {
  public:
    ContentHandlerState(CHContext & ctx)
      : ch_context(ctx)
    {}
    
    virtual void characters (const string & characters)
    { throwError(); }

    virtual void endDocument ()
    { throwError(); }

    virtual void endElement (const string & uri, const string & name, const string & q_name)
    { throwError(); }

    virtual void endPrefixMapping(const string & prefix)
    { throwError(); }
    
    virtual void ignorableWhitespace(const string & spaces)
    {}
    
    virtual void processingInstruction (const string & target, const string & data)
    { throwError(); }

    virtual void setDocumentLocator(Locator & locator)
    {}

    virtual void skippedEntity (const string & name)
    { throwError(); }

    virtual void startDocument()
    { throwError(); }

    virtual void startDocument(const string & encoding)
    { throwError(); }

    virtual void startElement (const string &, const string & name, const string & q_name, const Attributes & atts)
    { throwError(); }

    virtual void startPrefixMapping(const string & prefix, const string & uri)
    { throwError(); }

    virtual ~ContentHandlerState()
    {}
  protected:
    CHContext & ch_context;
    ContentHandlerState();
  private:
    void throwError();
  };
  
  //! Enumerates concrete derivatives of ContentHandlerState
  enum CHStateName {Decl,
		    Prolog,
		    PrefixMapping,
		    StartElement,
		    Content,
		    EndElement,
		    Epilog,
		    Default // Always enumerate at last. See CHStateBox::CHStateBox()
  };
  
  /** \class CHStateBox
      \brief Container for behavioural states of a ContentHandler
  */
  class CHStateBox
  {
    typedef std::vector<ContentHandlerState *> state_map;
    state_map statebox; //!< Contains one object of each concrete state
  public:
    //! Build a collection of states and set the context data pointer of each
    explicit  CHStateBox(CHContext & ctx);

    //! Return pointer to the state requested by name
    ContentHandlerState * getState(CHStateName state)
    {
      return statebox[state];
    }

    ~CHStateBox();
  };

}
// End of Include Guard
#endif
