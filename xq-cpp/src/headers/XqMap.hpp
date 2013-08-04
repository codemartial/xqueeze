/*
 * Author: Tahir Hashmi
 * 
 * Copyright (C) 2004, Xqueeze Developers
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

/** \file xqmap.hpp
 * \brief Xqueeze two-way lookup container for string-symbol mapping
 */
#ifndef XQML_XQMAP_HPP
#define XQML_XQMAP_HPP

#include <algorithm>
#include <utility>
#include <vector>

using std::vector;
using std::lower_bound;
using std::pair;

namespace xqML
{
  template <typename T1, typename T2, class Compare = std::less<T1> >
  struct CompareT1
  {
    bool operator()(const pair<T1, T2> & a, const pair<T1, T2> & b)
    {
      Compare compare;
      return compare(a.first, b.first);
    }
  };
  
  template <typename T1, typename T2, class Compare = std::less<T2> >
  struct CompareT2
  {
    bool operator()(const pair<T1, T2> & a, const pair<T1, T2> & b)
    {
      Compare compare;
      return compare(a.second, b.second);
    }
  };
  
  /** \class OrderedPairMap XqMap.hpp xqml/XqMap.hpp
   *  \brief Two-way lookup container for ordered pairs
   */
  template < typename T1,
	     typename T2,
	     typename CompareFirst = xqML::CompareT1<T1, T2>,
	     typename CompareSecond = xqML::CompareT2<T1, T2>
  >
  class OrderedPairMap
  {
  public:
    typedef vector< pair<T1, T2> > Container;
    typedef typename vector< pair<T1, T2> >::iterator iterator;

    void insert(const pair<T1, T2> & item)
    {
      container.insert(
	 lower_bound(container.begin(), container.end(), item, compare_first),
	 item);
    }
    
    void insert(const T1 & a_t1, const T2 & a_t2)
    {
      insert(pair<T1, T2>(a_t1, a_t2));
    }
    
    iterator find(const T1 & a_t1)
    {
      iterator i = lower_bound(container.begin(), container.end(), pair<T1, T2>(a_t1, T2()), compare_first);
      return i == container.end() || compare_first(pair<T1, T2>(a_t1, T2()), *i) ? container.end() : i;
    }
    
    iterator find(const T2 & a_t2)
    {
      iterator i = lower_bound(container.begin(), container.end(), pair<T1, T2>(T1(), a_t2), compare_second);
      return i == container.end() || compare_second(pair<T1, T2>(T1(), a_t2), *i) ? container.end() : i;
    }
    
    iterator begin()
    {
      return container.begin();
    }
    
    iterator end()
    {
      return container.end();
    }
    
    T2 & operator[](const T1 & a_t1)
    {
      iterator i = find(a_t1);
      if (i != container.end()) return const_cast<T2 &>(i->second);
      T2 a_t2;
      insert(a_t1, a_t2);
      return this->operator[](a_t1);
    }
    
    T1 & operator[](const T2 & a_t2)
    {
      iterator i = find(a_t2);
      if (i != container.end()) return const_cast<T1 &>(i->first);
      T1 a_t1;
      insert(a_t1, a_t2);
      return this->operator[](a_t2);
    }
    
  private:
    Container container;
    CompareFirst compare_first;
    CompareSecond compare_second;
  };

}

// End of Include Guard
#endif
