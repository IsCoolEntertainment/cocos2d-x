/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2017 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "base/RefTrace.h"
#include "base/ccMacros.h"

#include <algorithm>
#include <iostream>
#include <unordered_map>

NS_CC_BEGIN

RefTrace* RefTrace::s_singleInstance = nullptr;

RefTrace* RefTrace::getInstance()
{
    if (s_singleInstance == nullptr)
        s_singleInstance = new (std::nothrow) RefTrace();

    return s_singleInstance;
}

RefTrace::RefTrace()
    : _enabled( false )
{

}

void RefTrace::setEnabled( bool enabled )
{
    if ( !enabled )
        _objects.clear();

    _enabled = enabled;
}

bool RefTrace::isEnabled() const
{
    return _enabled;
}

void RefTrace::insert( Ref* obj )
{
    if ( !_enabled )
        return;
    
    _objects.insert( obj );
}

void RefTrace::erase( Ref* obj )
{
    if ( !_enabled )
        return;

    _objects.erase( obj );
}

void RefTrace::dump( std::ostream& os ) const
{
    std::unordered_map< std::string, std::size_t > countByType;
    
    for (const auto &obj : _objects)
        ++countByType[ typeid( *obj ).name() ];

    const auto begin( countByType.begin() );
    const auto end( countByType.end() );
    
    const std::size_t totalCount( _objects.size() );

    os << totalCount << " objects.\n"
       << "--------\n"
       << "type\t#instances\n";
    
    for ( const auto& entry : countByType )
        os << entry.first << '\t' << entry.second << '\n';
}

NS_CC_END
