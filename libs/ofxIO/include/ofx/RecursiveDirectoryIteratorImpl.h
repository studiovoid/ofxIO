// From: http://sourceforge.net/p/poco/feature-requests/168/
//
// RecursiveDirectoryIteratorImpl.h
//
// $Id$
//
// Library: Foundation
// Package: Filesystem
// Module:  RecursiveDirectoryIterator
//
// Definition of the RecursiveDirectoryIteratorImpl class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_RecursiveDirectoryIteratorImpl_INCLUDE
#define Foundation_RecursiveDirectoryIteratorImpl_INCLUDE


#include "Poco/Foundation.h"
#include "ofx/RecursiveDirectoryIteratorStategies.h"
#include <stack>
#include <functional>


namespace ofx {


    class ChildrenFirstTraverse;
    class SiblingsFirstTraverse;


    template <class TTraverseStrategy = ChildrenFirstTraverse>
    class RecursiveDirectoryIteratorImpl
    {
    public:
        enum { D_INFINITE = 0 };
        /// Constant for infinite traverse depth.

        RecursiveDirectoryIteratorImpl(
                                       const std::string& path, Poco::UInt16 maxDepth = D_INFINITE);
        ~RecursiveDirectoryIteratorImpl();

        void duplicate();
        void release();
        Poco::UInt16 depth() const;
        Poco::UInt16 maxDepth() const;

        const std::string& get() const;
        const std::string& next();

    private:
        typedef std::stack<Poco::DirectoryIterator> Stack;

        static Poco::UInt16 depthFun(const Stack& stack);
		/// Function which implements the logic of determining
		/// recursion depth.

        Poco::UInt16 _maxDepth;
        TTraverseStrategy _traverseStrategy;

        bool _isFinished;

        Stack _itStack;
        std::string _current;
        int _rc;
    };


    //
    // inlines
    //
    template <class TTraverseStrategy>
    inline void
    RecursiveDirectoryIteratorImpl<TTraverseStrategy>::duplicate()
    {
        ++_rc;
    }


    template <class TTraverseStrategy>
    inline void
    RecursiveDirectoryIteratorImpl<TTraverseStrategy>::release()
    {
        if (--_rc == 0)
            delete this;
    }


    template <class TTraverseStrategy>
    inline Poco::UInt16
    RecursiveDirectoryIteratorImpl<TTraverseStrategy>::depth() const
    {
        return depthFun(_itStack);
    }


    template <class TTraverseStrategy>
    inline Poco::UInt16
    RecursiveDirectoryIteratorImpl<TTraverseStrategy>::maxDepth() const
    {
        return _maxDepth;
    }


    template <class TTraverseStrategy>
    inline const std::string&
    RecursiveDirectoryIteratorImpl<TTraverseStrategy>::get() const
    {
        return _current;
    }


    template <class TTraverseStrategy>
    inline Poco::UInt16
    RecursiveDirectoryIteratorImpl<TTraverseStrategy>::depthFun(const Stack& stack)
    {
        return stack.size();
    }


    //
    // not inlines
    //
    template <class TTraverseStrategy>
    RecursiveDirectoryIteratorImpl<TTraverseStrategy>
    ::RecursiveDirectoryIteratorImpl(const std::string& path, Poco::UInt16 maxDepth)
	: _maxDepth(maxDepth),
    _traverseStrategy([](const Stack& stack) { return depthFun(stack); }, _maxDepth),
	_isFinished(false)
    {
        _itStack.push(Poco::DirectoryIterator(path));
        _current = _itStack.top()->path();
    }
    
    
    template <class TTraverseStrategy>
    RecursiveDirectoryIteratorImpl<TTraverseStrategy>
	::~RecursiveDirectoryIteratorImpl()
    {
        
    }
    
    
    template <class TTraverseStrategy>
    const std::string&
    RecursiveDirectoryIteratorImpl<TTraverseStrategy>::next()
    {
        if (_isFinished)
            return _current;
        
        _current = _traverseStrategy.next(&_itStack, &_isFinished);
        
        return _current;
    }
    
    
} // namespace Poco


#endif // Foundation_RecursiveDirectoryIteratorImpl_INCLUDE