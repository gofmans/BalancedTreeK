//
// Created by Sgofman on 08-Jan-18.
//

#include "BalancedTreeK.h"
#include "BTKNode.h"
#include "Value.h"
#include "Key.h"
#include "ParameterK.h"
#include <cstddef>
#include<iostream>
using namespace std;


BalancedTreeK::~BalancedTreeK()
{
    delete root;
}

Value* BalancedTreeK::Search(const Key* key) const
{
    return root->Search(key);
}

BTKNode* BalancedTreeK::SearchLeaf(const Key* key) const
{
    return root->SearchLeaf(key);
}
//sasads
void BalancedTreeK::Insert(const Key* nkey, const Value* nval)
{
    if(*(nkey) < *(_min) || *(_max) < *(nkey))//check for valid input
        return;
    BTKNode* y=root;
    while(y->Children[0]->leaf != true)//go down to leaf level
    {
        int i=0;
        while(!(*nkey < *(y->Children[i]->key)))
            i++;
        y=y->Children[i];
    }
    BTKNode* x=y;//parent of new node to be inserted

    Key* tempk =nkey->clone();
    Value* tempv =nval->clone();
    BTKNode* nNode =new BTKNode(true,tempv,tempk,x);
    nNode=x->InsertAndSplit(nNode);
    while(x!=root)
    {
        x=x->parent;
        if(nNode!=NULL)
            nNode=x->InsertAndSplit(nNode);
        else
        {
            x->totaLeafs++;
            x->UpdateKey();
            x->UpdateVal();
        }
    }
    if(nNode!=NULL)//new root will be created
    {
        BTKNode* tempChildren[2];
        if(*(root->key) < *(nNode->key))//old root is smaller then new node
        {
            tempChildren[0]=root;//update left child
            tempChildren[1]=nNode;//update right child
        } else{ //old root is bigger then new node
            tempChildren[0]=nNode;//update left child
            tempChildren[1]=root;//update right child
        }
        BTKNode* nRoot=new BTKNode(false,NULL,NULL,NULL);
        nRoot->totaLeafs = root->totaLeafs + nNode->totaLeafs; //new root now hold total number of leafs
        nRoot->Cnum=2; //new root have only 2 children
        nRoot->SetChildren(tempChildren);
        root=nRoot; //update the root
    }
}


void BalancedTreeK::Delete(const Key* dkey)
{
    if(*(dkey) < *(_min) || *(_max) < *(dkey))//check for valid input
        return;
    BTKNode* Leaf;
    Leaf = SearchLeaf(dkey); // find the leaf we want to delete
    if(Leaf == NULL)
        return;
    BTKNode* y=Leaf->parent;
    int index=0;
    while(!(!(*(y->Children[index]->key) < *dkey) && !(*dkey < *(y->Children[index]->key)))) // find the index of the unwanted leaf
        index++;
    BTKNode* newYchildren[y->Cnum-1]; // the new children of the leaf's parent
    int j = 0;
    for(int i=0 ;i < y->Cnum;i++)
    {
        if(i!=index)
        {
            newYchildren[j]=y->Children[i]; // updating new children for the leaf's parent
            j++;
        }
        else continue;
    }
    y->Cnum=y->Cnum-1;
    y->SetChildren(newYchildren);
    Leaf->parent = NULL;//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    delete Leaf;
    while(y!=NULL)  //after deleting we have to update the tree
    {
        if(y->parent != NULL) //regular node
        {
            if(y->Cnum<_k)// invalid number of children after deletion (<k)
                y=y->BorrowMerge();
            else //valid number of children after deletion
            {
                y->totaLeafs --;// decrease number of total leafs after deletion@@@@@@@@@@
                y->UpdateKey();
                y->UpdateVal();
                y = y->parent;
            }
        }
        else if(y->Cnum < 2) // there is only one child thus we update the root
        {
            y->Children[0]->parent=NULL;
            y->Children[0]->UpdateKey();
            y->Children[0]->UpdateVal();
            root=y->Children[0]; //update the root
            for(int l = 0; l < 2* _k -1; l++) //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                y->Children[l] = NULL;
            y->key = NULL;
            delete y;
            return;
        }
        else// no need to update the root, only decrease total leaf num
        {
            y->totaLeafs --;
            y->UpdateKey();
            y->UpdateVal();
            return;
        }
    }
}


unsigned BalancedTreeK::Rank(const Key* key) const
{
    BTKNode* x;
    x=SearchLeaf(key);
    if(x == NULL)
        return 0;
    BTKNode* y;
    y=x->parent;
    unsigned rank=1;
    while(y != NULL)
    {
        int index = 0;
        while(y->Children[index] != x) //find the index of x in y children
            index++;
        for(int i =0; i < index ; i++) //add number of leafs inside x's left brothers
            rank = rank + y->Children[i]->totaLeafs;
        x = y;           //move up in the tree
        y = y->parent;  //move up in the tree
    }
    return rank;
}


const Key* BalancedTreeK::Select(unsigned index) const
{
    BTKNode* x;
    x = root->SelectRec(index);
    if (x != NULL)
        return x->key;
    return NULL;
}


BTKNode* BalancedTreeK::Successor(BTKNode* node) const
{
    BTKNode* z = node->parent;
    while (!(*(node->key) < *(z->Children[z->Cnum-1]->key)) && !(*(z->Children[z->Cnum-1]->key) < *(node->key)))
    {
        node = z;
        z = z->parent;
    }
    int i = 0;
    while (!(!(*(node->key) < *(z->Children[i]->key)) && !(*(z->Children[i]->key) < *(node->key))))
        i++;
    BTKNode* y = z->Children[i+1];
    while(!(y->leaf))
        y = y->Children[0];
    if (*(y->key) < *(_max))
        return y;
    else
        return NULL;
}

BTKNode* BalancedTreeK::Predeccessor(BTKNode* node) const
{
    BTKNode* z = node->parent;
    while (!(*(node->key) < *(z->Children[0]->key)) && !(*(z->Children[0]->key) < *(node->key)))
    {
        node = z;
        z = z->parent;
    }
    int i = 0;
    while (!(!(*(node->key) < *(z->Children[i]->key)) && !(*(z->Children[i]->key) < *(node->key))))
        i++;
    BTKNode* y = z->Children[i-1];
    while(!(y->leaf))
        y = y->Children[y->Cnum-1];
    if (!(*(y->key) < *_min))
        return y;
    else
        return NULL;
}

const Value *BalancedTreeK::GetMaxValue(const Key *key1, const Key *key2) const
{
    if(root == NULL || key1 == NULL || key2 == NULL)
        return NULL;
    BTKNode* leftKey;
    leftKey=SearchLeaf(key1); //find left boundary
    if(leftKey == NULL)//no key to match in date structure
        leftKey = root->SearchRighty(key1);// find left key
    BTKNode* rightKey;
    rightKey=SearchLeaf(key2);//find right boundary
    if(rightKey == NULL)
    {
        rightKey = root->SearchRighty(key2);
        rightKey = Predeccessor(rightKey);
    }

    if(!(*(leftKey->key) < *(rightKey->key)))
        return NULL;
    Value* maxVal = *(leftKey->val) < *(rightKey->val) ?  rightKey->val : leftKey->val;
    //loop inside each parent
    while(leftKey->parent != rightKey->parent)
    {
        BTKNode* leftParent = leftKey->parent;
        int leftIndex = 0;
        while(leftParent->Children[leftIndex] != leftKey)//find the index of the left key
            leftIndex++;
        leftIndex ++;//move right
        //check for every brother right of left boundary in the same branch
        while(leftIndex < leftParent->Cnum)
        {
            if(*maxVal < *(leftParent->Children[leftIndex]->val))
                maxVal = leftParent->Children[leftIndex]->val;
            leftIndex ++;//check for max val
        }
        //-------------------------------------------------------------------------//
        BTKNode* rightParent = rightKey->parent;
        int rightIndex = 0;
        //check for every brother left of right boundary in the same branch
        while(rightParent->Children[rightIndex] != rightKey)
        {
            if(*maxVal < *(rightParent->Children[rightIndex]->val))
                maxVal = rightParent->Children[rightIndex]->val;
            rightIndex++;//check for max val
        }
        leftKey = leftKey->parent;//move up the tree
        rightKey = rightKey->parent;//move up the tree
    }
    //we reached the first common father
    BTKNode* firstParent = leftKey->parent;
    int tempIndex = 0;
    int leftIndex = 0;
    int rightIndex = 0;
    while(firstParent->Children[tempIndex] != rightKey)
    {
        if(firstParent->Children[tempIndex] == leftKey)
            leftIndex = tempIndex;//find index of left boundary
        tempIndex ++;
    }
    rightIndex = tempIndex; //find index of left boundary
    leftIndex = leftIndex +1;//move index right
    rightIndex = rightIndex -1;//move index left
    while(leftIndex <= rightIndex)//compare last level
    {
        if(*maxVal < *(firstParent->Children[leftIndex]->val))
            maxVal = firstParent->Children[leftIndex]->val;
        leftIndex++;
    }
    return maxVal;
}