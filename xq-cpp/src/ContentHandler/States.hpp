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

/** \file States.hpp
 * \brief Derivatives of ContentHandlerState
 */

#include <xqml/xqmlsymbol.hpp>
#include <xqml/exceptions.hpp>
#include <xqml/TranslatingContentHandler.hpp>
#include "ContentHandlerState.hpp"
#include "CHContext.hpp"

namespace xqML
{
  
  //! Initial state of Translating ContentHandler
  class CHDecl : public ContentHandlerState
  {
  public:
    CHDecl(CHContext & ctx)
      :ContentHandlerState(ctx)
    {}
    void startDocument();
    void startDocument(const string & encoding);
  };

  //! Prolog section
  class CHProlog : public ContentHandlerState
  {
  public:
    CHProlog(CHContext & ctx)
      :ContentHandlerState(ctx)
    {}
    void processingInstruction(const string & target, const string & data);
    void startPrefixMapping(const string & prefix, const string & uri);
    void startElement(const string & uri, const string & name, const string & q_name, const Attributes & atts);
  };
  
  //! startPrefixMapping, always followed by startElement
  class CHPrefixMapping : public ContentHandlerState
  {
  public:
    CHPrefixMapping(CHContext & ctx)
      :ContentHandlerState(ctx), last_code(0)
    {}
    void startPrefixMapping(const string & prefix, const string & uri);
    void startElement(const string & uri, const string & name, const string & q_name, const Attributes & atts);
  private:
    unsigned int last_code;
  };
  
  //! Content (Element, PI, Characters, Entity Reference)
  class CHContent : public ContentHandlerState
  {
  public:
    CHContent(CHContext & ctx)
      :ContentHandlerState(ctx)
    {}
    void startElement(const string & uri, const string & name, const string & q_name, const Attributes & atts);
    void endElement(const string & uri, const string & name, const string & q_name);
    void startPrefixMapping(const string & prefix, const string & uri);
    void processingInstruction(const string & target, const string & data);
    void skippedEntity(const string & name);
    void characters(const string & characters);
  };

  //! startElement
  class CHStartElement : public ContentHandlerState
  {
  public:
    CHStartElement(CHContext & ctx)
      :ContentHandlerState(ctx), flags(0x30), queued_tag("")
    {
      real_se_call = & xqML::CHStartElement::seInit;
    }
    void startElement(const string & uri, const string & name, const string & q_name, const Attributes & atts);
    void endElement(const string & uri, const string & name, const string & q_name);
    void processingInstruction(const string & target, const string & data);
    void startPrefixMapping(const string & prefix, const string & uri);
    void skippedEntity(const string & name);
    void characters(const string & characters);
    void setCPFlag()
    {
      flags |= 0x08;
    }
    
  private:
    uint8_t flags;
    string queued_tag;
    void (xqML::CHStartElement::* real_se_call) (const string &, const string &, const string &, const Attributes &);

    void unqueueTag();
    void seInit(const string &, const string &, const string &, const Attributes &);
    void seURI(const string &, const string &, const string &, const Attributes &);
    void sePrefix(const string &, const string &, const string &, const Attributes &);
    void writeAttributePfx(const Attribute &, ostream &, ostream &);
    void writeAttributeURI(const Attribute &, ostream &, ostream &);
  };
  
  //! endElement
  class CHEndElement : public ContentHandlerState
  {
  public:
    CHEndElement(CHContext & ctx)
      :ContentHandlerState(ctx), elements_to_close(0)
    {}
    void startElement(const string & uri, const string & name, const string & q_name, const Attributes & atts);
    void endElement(const string & uri, const string & name, const string & q_name);
    void endElement(const string & uri, const string & name, const string & q_name, bool handle_ee);
    void startPrefixMapping(const string & prefix, const string & uri);
    void endPrefixMapping(const string & prefix);
    void processingInstruction(const string & target, const string & data);
    void skippedEntity(const string & name);
    void characters(const string & characters);
    void endDocument();
    
  private:
    void closePendingElements();
    void write_etag();
    void pop_elements();
    uint8_t elements_to_close;
  };

  class CHEpilog : public ContentHandlerState
  {
  public:
    CHEpilog(CHContext & ctx)
      :ContentHandlerState(ctx), done_prefix_unmapping(false)
    {}
    void endPrefixMapping(const string & prefix);
    void processingInstruction(const string & target, const string & data);
    void endDocument()
    {}
    
  private:
    bool done_prefix_unmapping;
  };
  
}
