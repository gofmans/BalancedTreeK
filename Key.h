//
// Created by Sgofman on 08-Jan-18.
//

#ifndef FINALALGO_KEY_H
#define FINALALGO_KEY_H
#include <cstddef>

class Key
{
public:
    virtual ~Key(){};
    virtual Key* clone() const = 0;
    virtual bool operator<(const Key &rhs) const = 0;
};
#endif //FINALALGO_KEY_H
