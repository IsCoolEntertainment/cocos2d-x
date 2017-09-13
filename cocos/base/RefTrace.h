#ifndef __REFTRACE_H__
#define __REFTRACE_H__

#include "base/ccMacros.h"

#include <iosfwd>
#include <unordered_set>

NS_CC_BEGIN

class Ref;

class RefTrace
{
public:
    static RefTrace* getInstance();

    RefTrace();
    
    void setEnabled( bool enabled );
    bool isEnabled() const;
    
    void insert( Ref* obj );
    void erase( Ref* obj );
    
    void dump( std::ostream& os ) const;
    
private:
    std::unordered_set< Ref* > _objects;
    bool _enabled;

    static RefTrace* s_singleInstance;
};

NS_CC_END

#endif
