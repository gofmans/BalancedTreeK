//
// Created by Sgofman on 08-Jan-18.
//

#ifndef FINALALGO_BTKNODE_H
#define FINALALGO_BTKNODE_H

#include <cstddef>
#include "Key.h"
#include "Value.h"
#include "ParameterK.h"
using namespace std;


class BTKNode {
    friend class BalancedTreeK;
private:
    BTKNode* parent;   //Pointer to parent node
    int k;                  // Minimum degree (defines the range for number of keys)
    BTKNode* Children[2*K-1];     // An array of child pointers, initially contain NULL
    unsigned Cnum;      // Current number of children
    bool leaf;      // Is true when node is leaf. Otherwise false
    unsigned totaLeafs; // total number of leafs roted at this
    Key* key;        //key if leaf - maximum key in sub tree if not
    Value* val;      //value of leaf

public:

    BTKNode()
    {
        parent = NULL;
        k = K;
        for(int i =0; i < 2*k -1; i ++)
            Children[i] = NULL;
        Cnum = 0;
        leaf = false;
        totaLeafs = 0;
        key = NULL;
        val = NULL;

    }

    BTKNode(bool _leaf,Value* _val,Key* _key,BTKNode* newParent)  //Normal Constructor
    {
        parent = newParent;
        k = K;
        for(int i =0; i < 2*k -1; i ++)
            Children[i] = NULL;
        Cnum = 0;
        if (_leaf)
            totaLeafs = 1;
        else
            totaLeafs = 0;
        leaf = _leaf;
        key=_key;
        val=_val;
    };


    BTKNode& operator = (const BTKNode& other)
    {
        if(this != &other) // not the same entity
        {
            parent = other.parent;
            k = other.k;
            //Children = new BTKNode *[2*k -1];
            for(int i =0; i < 2*k -1; i++)
                Children[i] = other.Children[i];
            Cnum = other.Cnum;
            leaf = other.leaf;
            totaLeafs = other.totaLeafs;
            key = other.key->clone();
            val = other.val->clone();
        }
        return *this;
    };

    ~BTKNode();

    BTKNode* SelectRec(unsigned index);

    Value* Search(const Key* k);

    BTKNode* SearchLeaf(const Key* k);

    BTKNode* SearchRighty(const Key* k);

    void UpdateKey();

    void UpdateVal();

    void SetChildren(BTKNode **children);

    BTKNode* InsertAndSplit(BTKNode *z);

    BTKNode* BorrowMerge();

};

#endif //FINALALGO_BTKNODE_H
