#pragma once
//========================================================================
// Templates.h : Some useful templates
//
// Part of the GameCode4 Application
//
// GameCode4 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 4th Edition" by Mike McShaffry and David
// "Rez" Graham, published by Charles River Media. 
// ISBN-10: 1133776574 | ISBN-13: 978-1133776574
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the authors a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1133776574/ref=olp_product_details?ie=UTF8&me=&seller=
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code: 
//    http://code.google.com/p/gamecode4/
//
// (c) Copyright 2012 Michael L. McShaffry and David Graham
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser GPL v3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
// http://www.gnu.org/licenses/lgpl-3.0.txt for more details.
//
// You should have received a copy of the GNU Lesser GPL v3
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

//========================================================================
//
//  Original Code written at Compulsive Development
//
//========================================================================

//========================================================================
//  Content References in Game Coding Complete 3rd Edition
// 
//	class singleton -			Chapter 3
//  class optional -			Chapter 3
//
//========================================================================


//---------------------------------------------------------------------------------------------------------------------
// singleton template manages setting/resettting global variables.
//---------------------------------------------------------------------------------------------------------------------

template <class T>
class singleton
{
	T m_OldValue;
	T* m_pGlobalValue;

public:
	singleton(T newValue, T* globalValue)
	{ 
		m_pGlobalValue = globalValue;
		m_OldValue = *globalValue; 
		*m_pGlobalValue = newValue;
	}

	virtual ~singleton() { *m_pGlobalValue = m_OldValue; }
};


//---------------------------------------------------------------------------------------------------------------------
// [rez] Tr1 / Boost smart pointers make me sad.  In order to dereference a weak_ptr, you have to cast it to a 
// shared_ptr first.  You still have to check to see if the pointer is dead dead by calling expired() on the weak_ptr, 
// so why not just allow the weak_ptr to be dereferenceable?  It doesn't buy anything to force this extra step because 
// you can still cast a dead weak_ptr to a shared_ptr and crash.  Nice.  Anyway, this function takes some of that 
// headache away.
//---------------------------------------------------------------------------------------------------------------------
template <class Type>
shared_ptr<Type> MakeStrongPtr(weak_ptr<Type> pWeakPtr)
{
    if (!pWeakPtr.expired())
        return shared_ptr<Type>(pWeakPtr);
    else
        return shared_ptr<Type>();
}


//////////////////////////////////////////////////////////////////////////////
// optional.h
//
// An isolation point for optionality, provides a way to define
// objects having to provide a special "null" state.
//
// In short:
//
// struct optional<T>
// {
//     bool m_bValid;
//
//	   T	m_data;
// };
//
//

#include <new>

class optional_empty { };

template <unsigned long size>
class optional_base
{
public:
    // Default - invalid.

    optional_base() : m_bValid(false) { }

    optional_base & operator = (optional_base const & t)
    {
		m_bValid = t.m_bValid;
		return * this;
    }

	//Copy constructor
    optional_base(optional_base const & other)
		: m_bValid(other.m_bValid)  { }

	//utility functions
	bool const valid() const		{ return m_bValid; }
	bool const invalid() const		{ return !m_bValid; }

protected:
    bool m_bValid;
    char m_data[size];  // storage space for T
};


template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction(void) { return new SubType; }

template <class BaseClass, class IdType>
class GenericObjectFactory
{
    typedef BaseClass* (*ObjectCreationFunction)(void);
    std::map<IdType, ObjectCreationFunction> m_creationFunctions;

public:
    template <class SubClass>
    bool Register(IdType id)
    {
        auto findIt = m_creationFunctions.find(id);
        if (findIt == m_creationFunctions.end())
        {
            m_creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;  // insert() is giving me compiler errors
            return true;
        }

        return false;
    }

    BaseClass* Create(IdType id)
    {
        auto findIt = m_creationFunctions.find(id);
        if (findIt != m_creationFunctions.end())
        {
            ObjectCreationFunction pFunc = findIt->second;
            return pFunc();
        }

        return NULL;
    }
};

