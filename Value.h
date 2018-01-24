//
// Created by Sgofman on 08-Jan-18.
//

#ifndef FINALALGO_VALUE_H
#define FINALALGO_VALUE_H
#include <cstddef>

class Value
{
public:
    virtual ~Value(){};
    virtual Value* clone() const = 0;
    virtual bool operator<(const Value &rhs) const = 0;
};
#endif //FINALALGO_VALUE_H
