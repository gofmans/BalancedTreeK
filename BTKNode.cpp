//
// Created by Sgofman on 08-Jan-18.
//
#include "BTKNode.h"
#include<iostream>
#include <cstddef>
#include "BTKNode.h"
using namespace std;


BTKNode::~BTKNode()
{
    for( int i =0; i < 2*k -1; i++)
        delete Children[i];
    if(leaf)
    {
        delete key;
        if(val != NULL)
            delete val;
    }
};


Value* BTKNode::Search(const Key* k)
{
    if (this->leaf)
        if(!(*key < *k) && !(*k < *(key)))
            return this->val;
        else
            return NULL;
    int i = 0;
    while ((i < Cnum) && !(*k < *(Children[i]->key)) && !(!(*(Children[i]->key) < *k) && !(*k < *(Children[i]->key)))) // Find the first key greater than or equal to k
        i++;
    return Children[i]->Search(k);  // Go to the appropriate child
}


BTKNode* BTKNode::SearchLeaf(const Key* k)
{
    if (this->leaf)
        if(!(*key < *k) && !(*k < *(key)))
            return this;
        else
            return NULL;
    int i = 0;
    while ((i < Cnum) && !(*k < *(Children[i]->key)) && !(!(*(Children[i]->key) < *k) && !(*k < *(Children[i]->key)))) // Find the first key greater than or equal to k
        i++;
    return Children[i]->SearchLeaf(k);  // Go to the appropriate child
}


BTKNode* BTKNode::SearchRighty(const Key* k)
{
    if (this->leaf)
        if(!(*key < *k))
            return this;
        else
            return NULL;
    int i = 0;
    while ((i < Cnum) && !(*k < *(Children[i]->key)) && !(!(*(Children[i]->key) < *k) && !(*k < *(Children[i]->key)))) // Find the first key greater than or equal to k
        i++;
    return Children[i]->SearchRighty(k);  // Go to the appropriate child

}


void BTKNode::UpdateKey()
{
    key = Children[Cnum-1]->key;
}


void BTKNode::UpdateVal()
{
    val = NULL;
    for(int i =0; i < Cnum; i++)
    {
        if(Children[i]->val == NULL) continue;//do nothing for sentinals
        if(val == NULL || *val < *(Children[i]->val))
            val = Children[i]->val;
    }
}


void BTKNode::SetChildren(BTKNode **children)
{
    for(int i = 0; i < Cnum; i++)
    {
        Children[i] = children[i];
        Children[i]->parent=this;
    }
    for(int j = Cnum; j < 2*k -1; j ++)
        Children[j] = NULL;
    this->UpdateKey();
    this->UpdateVal();
}


BTKNode* BTKNode::InsertAndSplit(BTKNode *z)
{
    BTKNode* tempChildren[Cnum+1];//temp array of children
    int i = 0;
    while(!(*(z->key) < *(Children[i]->key)))//insert z's left brother's
    {
        tempChildren[i] = Children[i];
        i++;
        if(i == Cnum) break;
    }
    tempChildren[i] = z; // insert the new node z
    for(;i <Cnum; i++)// insert rest of z's right brothers
        tempChildren[i+1] = Children[i];
    if (Cnum < (2*k - 1)) //No split is required
    {
        Cnum++;
        totaLeafs++;//update leaf counter
        SetChildren(tempChildren);
        return NULL;
    }
    BTKNode* y = new BTKNode();//split is required, create new uncle "y"
    y->parent =parent;
    BTKNode* firstHalf[k];//create temp array to store x's(this) new children
    unsigned firstHalfLeafs = 0;//create leaf counter for x's(this) new children
    BTKNode* secondHalf[k];//create temp array to store y children
    unsigned secondHalfLeafs = 0;//create leaf counter for y children
    for(int i =0; i < k; i++)//move all temp children to appropriate arrays
    {
        firstHalf[i] = tempChildren[i];
        firstHalfLeafs += tempChildren[i]->totaLeafs;
        secondHalf[i] = tempChildren[k+i];
        secondHalfLeafs += tempChildren[k+i]->totaLeafs;
    }
    Cnum=k;//update new children number of x (this)
    totaLeafs = firstHalfLeafs;//update new totalLeaf number of x (this)
    SetChildren(firstHalf);
    y->Cnum=k; //update new children number of y
    y->totaLeafs = secondHalfLeafs; //update new totalLeaf number of y
    y->SetChildren(secondHalf);
    return y;
}


BTKNode* BTKNode::BorrowMerge()
{
    BTKNode* z = parent;
    int index = 0;
    while(z->Children[index] != this) //find the index of y(this) in z (y parent)
        index++;
    bool right = false;
    if (index +1 != z->Cnum)//y have right brother
        right = true;
    bool left = false;
    if (index > 0) //y have left brother
        left = true;
    if(right)//y have right brother
    {
        BTKNode* x =z->Children[index+1]; //Right brother of y(this)
        if(x->Cnum > k) //x can give 1 node
        {
            //-------update y(this) children---------
            Children[Cnum] = x->Children[0];//add 1 missing child to y
            Cnum = Cnum +1;//update Children counter
            unsigned tempYLeafs =0;
            for(int l =0; l < Cnum; l++) //calculate Y's total number of leafs after children update
                tempYLeafs += Children[l]->totaLeafs;
            this->SetChildren(Children);
            totaLeafs = tempYLeafs;
            //-------update x(right brother) children---------
            BTKNode* newXchildren[x->Cnum-1];
            unsigned tempXLeafs = 0;
            for (int i = 0; i< x->Cnum -1; i ++) //copy the rest of x's children's
            {
                newXchildren[i] = x->Children[i+1];
                tempXLeafs += x->Children[i+1]->totaLeafs; //calculate X total number of leafs after children update
            }
            x->Cnum = x->Cnum -1;//update children counter
            x->SetChildren(newXchildren);
            x->totaLeafs = tempXLeafs;
            return z;
        }
        else //x can merge with y(this)
        {
            //-------merge y with x(right brother)---------
            BTKNode* newXchildren[2*k -1];
            unsigned newTotal = totaLeafs;
            for(int i =0; i< k-1; i ++)
                newXchildren[i] = Children[i]; //at first, take y children (exactly k-1 children)
            for (int i = 0; i< k; i ++) //copy the rest of x's children's
            {
                newXchildren[i+k -1] = x->Children[i];
                newTotal += x->Children[i]->totaLeafs; //add new leafs for new total
            }
            x->Cnum = 2*k -1; //x now have maximum number of children
            x->SetChildren(newXchildren);
            x->totaLeafs = newTotal;
            //-------update z(parent) children---------
            BTKNode* tempBrothers[z->Cnum-1];
            int j =0;
            for (int i = 0; i< z->Cnum; i ++)
            {
                if (i == index) continue;// y is no longer child of z
                tempBrothers[j] = z->Children[i];
                j++;
            }
            z->Cnum = z->Cnum -1; //update number of z children
            z->SetChildren(tempBrothers);
            for(int l = 0; l < 2* k -1; l++)
                Children[l] = NULL;
            key = NULL;
            delete this;
            return z;
        }
    }
    else if(left) //y have left brother
    {
        BTKNode* w =z->Children[index-1]; //Left brother of y(this)
        if(w->Cnum > k) //w can give 1 node
        {
            //-------update y children---------
            BTKNode* newYchildren[Cnum+1];
            unsigned tempYleafs = 0;
            newYchildren[0] = w->Children[w->Cnum -1];
            tempYleafs += newYchildren[0]->totaLeafs;
            for (int i = 0;i < Cnum; i ++) //copy the rest of y's children's
            {
                newYchildren[i+1] =Children[i];
                tempYleafs +=Children[i]->totaLeafs; // calculate new leafs number
            }
            Cnum = Cnum +1;//update Children counter
            this->SetChildren(newYchildren);
            totaLeafs = tempYleafs;
            //-------update w(left brother) children---------
            w->Children[w->Cnum -1] = NULL; //that child is now y's child
            w->Cnum = w->Cnum -1;//update children counter
            w->SetChildren(w->Children);//update children after loosing right most child
            w->totaLeafs -= newYchildren[0]->totaLeafs; //subtract the missing child leafs from total
            return z;
        }
        else //w can merge with y(this)
        {
            //-------merge y with w(left brother)---------
            BTKNode* newWchildren[2*k -1];
            unsigned newTotal = w->totaLeafs;
            for(int i =0; i< k; i ++)
                newWchildren[i] = w->Children[i]; //at first, take w's children (exactly k children)
            for (int i = 0; i< k-1; i ++) //copy the rest of y's children (exactly k-1 children)
            {
                newWchildren[i+k] = Children[i];
                newTotal +=Children[i]->totaLeafs; //add new leafs for new total
            }
            w->Cnum = 2*k -1; //w now have maximum number of children
            w->SetChildren(newWchildren);
            w->totaLeafs = newTotal; //update total
            //-------update z(parent) children---------
            BTKNode* tempBrothers[z->Cnum-1];
            int j = 0;
            for (int i = 0; i< z->Cnum; i ++)
            {
                if (i == index) continue;// y is no longer child of z
                tempBrothers[j] = z->Children[i];
                j++;
            }
            z->Cnum = z->Cnum -1; //update number of z children
            z->SetChildren(tempBrothers);
            for(int l = 0; l < 2* k -1; l++)
                Children[l] = NULL;
            key = NULL;
            delete this;
            return z;
        }
    }
}


BTKNode* BTKNode::SelectRec(unsigned index)
{
    if (totaLeafs < index)
        return NULL;
    if(leaf)
        return this;
    unsigned temp = 0;
    unsigned tempLeafs = 0;
    while(index > tempLeafs) //find the first child with suitable leaf number
    {
        tempLeafs += Children[temp]->totaLeafs; //update total leaf number
        temp ++;
    }
    if(temp > 0) //suitable child is not left child
    {
        tempLeafs -= Children[temp-1]->totaLeafs; //subtract the current number of leafs from total
        return Children[temp-1]->SelectRec(index - tempLeafs);
    }
    return Children[0]->SelectRec(index); //suitable child is left most child, no index update required
}