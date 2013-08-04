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

/** \file writer.C
 * \brief Implementation of xqML::Writer and xqMLWriter::WriterFramework.
 */
#include "writer_impl.h"
#include <xqml-build/exceptions.h>
#include <xqml-build/writer.h>

namespace xqML
{
/** \class Writer writer.h xqml/writer.h */

  using std::ostream;
  using std::string;
  using namespace xqMLWriter;

/** Constructor requests a \c new WriterFramework to create a
 * Writer object. The default arguments are std::cin and std::cout
 * respectively.
 * \param mapfile The xqA specification against which the xqML doc is
 * to be generated
 * \param outfile The stream to which the xqML document should be
 * written
 */
  Writer::Writer(istream & mapfile, ostream & outfile)
  {
    writer_ = new WriterFramework(mapfile, outfile);
  }

/** Write an attribute whose name and value are taken from the first
 * and second arguments respectively. The third argument must be
 * true if the attribute has a predefined value.
 * 
 * \param name Name of the attribute
 * \param value Value of the attribute
 * \param hasPredefValue whether the attribute value is known in the
 * xqA
 *
 * \requires Document context should be "start tag" or "empty-element
 * start tag"
 * 
 * Throws exception IllegalContext if attribute is not allowed at
 * the point of invocation in the document.
 */
  void Writer::attribute (const string & name, const string & value, bool hasPredefValue) 
  {
    if(writer_->context == start_tag || writer_->context == ee_start_tag)
      if(hasPredefValue) {
	writer_->stream<<xqMLSymbol(24)
		       <<writer_->mapping.GetAttributeWPV(name)
		       <<writer_->mapping.GetAttributeValue(value);
      }
      else
	writer_->stream<<xqMLSymbol(22)
		       <<writer_->mapping.GetAttribute(name)<<value
		       <<xqMLSymbol(30)<<xqMLSymbol(22);
    else throw IllegalContext(writer_->contextName(), name, "attribute");
  }

/** \requires Document context should be "element", "start tag" or
 * "empty-element start tag".
 * 
 * \sideeffects 
 *    - Calls xqMLWriter::writerFramework::closePendingElements
 *      before writing CDATA.
 *    - Document context is set to "element" if not already so.
 * 
 * Throws exception IllegalContext if CDATA is not allowed at
 * the point of invocation in the document.
 */
  void Writer::cdataSection (const string & cdata)
  {
    if(writer_->context == start_tag || writer_->context == ee_start_tag)
      writer_->context = element;
    if(writer_->context == element) {
      writer_->closePendingElements();
      writer_->stream<<xqMLSymbol(30)<<xqMLSymbol(40)<<cdata<<xqMLSymbol(30);
    }
    else throw IllegalContext(writer_->contextName(), cdata, "cdata");
  }

/** \requires Document context should be "element" or "open
 * attribute" (redundant).
 * 
 * \sideeffects Sets Document context to "element" if earlier it was
 * "start tag" or "empty-element start tag".
 *   
 * Throws exception IllegalContext if character reference is not allowed at
 * the point of invocation in the document.
 */
  void Writer::characterReference (int character_code) 
  {
    if(writer_->context == start_tag || writer_->context == ee_start_tag)
      writer_->context = element;
    if(writer_->context == element || writer_->context == open_attribute) {
      writer_->stream<<xqMLSymbol(30)<<xqMLSymbol(38)<<character_code<<xqMLSymbol(30);
    }
    else throw IllegalContext(writer_->contextName(), " ", "character reference");
  }

/** \sideeffects Calls xqMLWriter::writerFramework::closePendingElements
 *  before writing the comment. 
 *  
 * Throws exception IllegalContext if comment is not allowed at
 * the point of invocation in the document.
 */
  void Writer::comment (const string & comment)
  {
    if(writer_->context == start_tag || writer_->context == ee_start_tag)
      writer_->context = element;
    else if(writer_->context != open_attribute && writer_->context != attribute_w_predef_value) {
      writer_->closePendingElements();
      writer_->stream<<xqMLSymbol(30)<<xqMLSymbol(34)<<comment<<xqMLSymbol(30);
    }
    else throw IllegalContext(writer_->contextName(), comment, "comment"); // Redundant
  }

/**\requires Document context should be "open attribute"
 *  
 * \sideeffects Sets context to "start tag"
 *
 * Throws exception IllegalContext if closing an attribute is not allowed at
 * the point of invocation in the document or IllegalIdentifier if
 * identifier look-up fails.
 */
  void Writer::endAttribute()
  {
    if(writer_->context != open_attribute) 
      throw IllegalContext(writer_->contextName(), "attribute end-marker", "reserved symbol");
    writer_->stream<<xqMLSymbol(30)<<xqMLSymbol(22);
    writer_->context = start_tag;
  }

/** \requires The calls to endElement must match those of
 * startElement and this function should never be called for empty
 * elements.
 *
 * Guarantees that no more than 255 elements are left pending, by
 * calling xqMLWriter::writerFramework::closePendingElements
 * when the count reaches 255 before incrementing.
 *
 * Throws exception IllegalContext if closing an element is not allowed at
 * the point of invocation in the document or IllegalIdentifier if
 * identifier look-up fails.
 */
  void Writer::endElement (const string & element_to_close) 
  {
    if(writer_->context == open_attribute || writer_->context == attribute_w_predef_value)
      throw IllegalContext(writer_->contextName(), element_to_close, "closing tag");
    else {
      if(writer_->elements_to_close == 255) writer_->closePendingElements();
      //std::cerr<<"Incrementing elements_to_close from "<<writer_->elements_to_close;
      ++writer_->elements_to_close;
      //std::cerr<<" to "<<writer_->elements_to_close<<'\n';
    }
  }

/** \requires Document context should be "start tag", "empty-element
 * start tag", "open attribute" or "element".
 * 
 * \sideeffects Sets Document context to "element" if not already so.
 * 
 * Throws exception IllegalContext if entity reference is not allowed at
 * the point of invocation in the document or IllegalIdentifier if
 * identifier look-up fails.
 */
  void Writer::entityReference (const string & name)
  {
    if(writer_->context == start_tag || writer_->context == ee_start_tag)
      writer_->context = element;
    if(writer_->context == element || writer_->context == open_attribute) {
      writer_->closePendingElements();
      writer_->stream<<xqMLSymbol(30)<<xqMLSymbol(36)
		     <<writer_->mapping.GetEntityReference(name);
    }
    else throw IllegalContext(writer_->contextName(), name, "entity reference");
  }

/** \param target The PI Target
 *  \param data PI Data
 *  \requires context should not be "open attribute"
 *  \sideeffects Sets context to "element" if it is not "preamble"
 */
  void Writer::processingInstruction (const string & target, const string & data)
  {
    if(writer_->context == open_attribute)
      throw IllegalContext(writer_->contextName(), target, "processing instruction");
    writer_->context = element;
    writer_->stream<<xqMLSymbol(30)<<xqMLSymbol(32)
		   <<target<<xqMLSymbol(32)<<data<<xqMLSymbol(32);
  }

/** Writes a prolog, declaring xqml version number 
 * (version number should be determined by the function itself). It
 * also declares the name provided as the argument to be the doctype.
 */
  void Writer::prolog(const string & doctype)
  {
    writer_->stream<<xqMLSymbol(30)<<xqMLSymbol(2)<<"0.3"
		   <<xqMLSymbol(30)<<xqMLSymbol(44)<<doctype;
  }
  
/** Starts an attribute whose name is supplied as the argument. This
 * method should not be called for attributes with enumerated values
 * otherwise Writer would throw IllegalIdentifier exceptions.
 *
 * \requires Context should be "start tag" or "empty-element start
 * tag"
 * 
 * \sideeffects Sets context to "open attribute"
 *
 * Throws exception IllegalContext if attribute is not allowed at the
 * point of invocation in the document or IllegalIdentifier if
 * identifier look-up fails.
 */
  void Writer::startAttribute (const string & name)
  {
    if(writer_->context == start_tag || writer_->context == ee_start_tag) {
      writer_->stream<<xqMLSymbol(22)<<writer_->mapping.GetAttribute(name);
      writer_->context = open_attribute;
    }
    else throw IllegalContext(writer_->contextName(), name, "attribute");
  }
    
/** Starts an element whose name is supplied as the first
 * argument. The optional second argument must be set to \c true
 * if the element is empty. It defaults to \c false.
 *
 * \requires Document context should not be "open attribute"
 * 
 * \sideeffects
 *    - Calls xqMLWriter::writerFramework::closePendingElements
 *	 element before writing out anything
 *    - Sets Document context to "start tag" if the second argument
 *	 is not supplied or is false, otherwise sets Document context
 *	 to "empty element start tag".
 * 
 * Throws exception IllegalContext if new element is not allowed at
 * the point of invocation in the document or IllegalIdentifier if
 * identifier look-up fails.
 */
  void Writer::startElement (const string & name, bool isEmpty)
  {
    if(writer_->context != open_attribute) {
      writer_->closePendingElements();
      if(isEmpty) {
	writer_->stream<<xqMLSymbol(30)<<xqMLSymbol(42)
		       <<writer_->mapping.GetEmptyElement(name);
	writer_->context = ee_start_tag;
      }
      else {
	writer_->stream<<xqMLSymbol(30)<<writer_->mapping.GetElement(name);
	writer_->context = start_tag;
      }
    }
    else throw IllegalContext(writer_->contextName(), name, "start tag");
  }

  Writer::~Writer()  { delete writer_; }

  WriterFramework::WriterFramework(istream & mapfile, ostream & outfile)
    : mapping(mapfile), stream(outfile), elements_to_close(0) {
  }
  
/** Returns a name for the current value of WriterFramework.context */
 string WriterFramework::contextName() {
    switch (context) {
    case preamble:
      return "preamble";
    case start_tag:
      return "start tag";
    case ee_start_tag:
      return "empty-elment start tag";
    case element:
      return "element";
    case open_attribute:
      return "open attribute";
    case attribute_w_predef_value:
      return "open attribute with predefined value";
    }
    return "unknown context";
  }

/** Requires that no more than 255 elements be left pending
 *
 * \sideeffects
 *    - Document context is set to "element"
 *    - elements_to_close is set to 0 (zero)
 */
  void WriterFramework::closePendingElements()
  {
    if (elements_to_close) {
      stream<<xqMLSymbol(30)<<xqMLSymbol(62)
	    <<xqMLSymbol(elements_to_close);
      elements_to_close = 0;
    }
    context = element;
  }

  WriterFramework::~WriterFramework() 
  {
    try {
      closePendingElements();
    }
    catch (...) {
      std::cerr<<"Writer: Could not close output stream cleanly. It may have inconsistencies";
    }
  }
} // Closed Namespace xqML
