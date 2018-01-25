//
// Created by Sgofman on 08-Jan-18.
//

#ifndef FINALALGO_BALANCEDTREEK_H
#define FINALALGO_BALANCEDTREEK_H

#include <cstddef>
#include "Value.h"
#include "Key.h"
#include "ParameterK.h"
#include "BTKNode.h"


class  BalancedTreeK {

private:

    BTKNode* root; //pointer default NULL
    int _k; //Minimum degree
    Key* _min ;//pointer default NULL
    Key* _max ;//pointer default NULL

public:

    BalancedTreeK(const Key* min, const Key* max)
    {
        root = new BTKNode();
        _min =min->clone(),_max =max->clone();
        root->Children[0] = new BTKNode();
        root->Children[0]->parent = root;
        root->Children[0]->leaf = true;
        root->Children[0]->sentinal = true;
        root->Children[0]->key = _min;
        root->Children[1] = new BTKNode();
        root->Children[1]->parent = root;
        root->Children[1]->leaf = true;
        root->Children[1]->sentinal = true;
        root->Children[1]->key = _max;
        root->key = _max;
        root->Cnum = 2;
        _k= K;
    }

    ~BalancedTreeK();

    void Insert(const Key* nkey, const Value* nval);

    void Delete(const Key* dkey);

    Value* Search(const Key* key) const;

    BTKNode* SearchLeaf(const Key* key) const ;

    unsigned Rank(const Key* key) const;

    const Key* Select(unsigned index) const;

    const Value* GetMaxValue(const Key* key1, const Key* key2) const;

    BTKNode* Successor(BTKNode* node) const;

    BTKNode* Predeccessor(BTKNode* node) const;

};


#endif //FINALALGO_BALANCEDTREEK_H
