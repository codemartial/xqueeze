/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2002, 2003 Xqueeze Developers
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

/** \file exceptions.h
 * \brief Interface specifications for xqML exceptions
 */
#ifndef XQML_EXCEPTIONS_HPP
#define XQML_EXCEPTIONS_HPP
#include <string>
#include <memory>
#include <xqml/xqmlsymbol.hpp>

namespace xqML
{
  using std::string;
  
  struct xqException {
    virtual string what() const = 0;
    virtual ~xqException()
    {}
  };
  
  /** \struct IllegalIdentifier exceptions.h xqml/exceptions.h */

  /** \struct IllegalContext exceptions.h xqml/exceptions.h
   * This exception is thrown by Writer methods if the request is not possible
   * to be serviced since it violates the correct %xqML document structure. The
   * Parser throws this exception when it encounters malformed %xqML structure.
   */

  /** \defgroup exceptions Exceptions
   *  These are the exceptions thrown by the modules in Xqueeze.
   *  @{
   */
  //! Exception thrown when lookup for an identifier or symbol fails.
  struct IllegalIdentifier : public xqException
  {
    string uri_; //!< URI of the identifier
    string name_; //!< indentifier that could not be found
    //! Gives URI and name of identifier for which a matching symbol
    //! was not found
    /** \param uri URI of the identifier for which a symbol could not
	be found
	\param name Name of such identifier
    */
    IllegalIdentifier(const string & uri, const string & name);

    //! Gives URI and decimal value of xqML Symbol (as string) that could not
    //! be translated
    /** \param uri URI of the symbol for which an identifier could not be found
	\param code Value of %xqML Symbol that could not be translated
     */
    IllegalIdentifier(const string & uri, const uint32_t code);

    string what() const;
  };

  //! Exception thrown due to bad document structure.
  struct IllegalContext : public xqException
  {
    string context; //!< name of the current context of the document
    string uri_; //!< URI of the identifier
    string name_; //!< name of the offending identifier
    //! Gives context, URI of identifier and name of identifier where
    //! bad structure was found
    /** \param ill_context Name of the document context where
	exception occured
	\param uri URI of the identifier that caused the exception
	\param name Name of the identifier that caused the
	exception
    */
    IllegalContext(const string & ill_context, const string & uri, const string & name);
    
    //! Gives context and decimal value (as string) of %xqML Symbol
    //! that caused bad structure
    /** \param ill_context Name of the document context where
	exception occured
	\param uri URI of the %xqML Symbol that caused the exception
	\param code Value of %xqML Symbol that caused the exception
    */
    IllegalContext(const string & ill_context, const string & uri, const unsigned int code);
    
    string what() const;
  };

  struct SAXException
  {
    virtual string what() const = 0;
    virtual ~SAXException() {}
  };
  
  struct SAXParseException : public SAXException
  {
    std::auto_ptr<xqException> const exc;

    SAXParseException(IllegalContext & e);
    
    SAXParseException(IllegalIdentifier & e);
    
    string what() const;
  };
  
  struct SAXNotSupported : public SAXException
  {
    string msg;
    
    SAXNotSupported(const string & message);

    string what() const;
  };
  
  struct SAXNotRecognized : public SAXException
  {
    string msg;
    
    SAXNotRecognized(const string & message);

    string what() const;
  };
  
  //! Exception thrown when the XqA for a URI could not be obtained
  struct XqaNotAvailable
  {
    string uri_;

    XqaNotAvailable(const string & uri);
    
    string what() const;
  };
  /** @} */
}
#endif
