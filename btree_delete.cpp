#include "btree.h"

/*
NOTE: Please follow logic from CLRSv4 directly. Additionally, in cases 3a and 3b please check for an immediate right sibling first.
*/

// delete the key k from the btree
void BTree::remove(int k)
{
    if (root == nullptr)
    {
        return;    
    }
    
    remove(root, k, true);
}

// delete the key k from the btree rooted at x
void BTree::remove(Node *x, int k, bool x_root)
{
    int nearest = find_k(k);
    if (x->keys[nearest] == k)
    {
        //case 1
        if(x->leaf)
        {
           remove_leaf_key(x, nearest);
        }
        else
        {   
            //if child at nearest or nearest+1 has >= t keys
            //case 2
            if (x->c[nearest]->n >= t)
            {
                remove_internal_key(x, nearest, nearest);
            }
            else if (x->c[nearest+1]-> >= t)
            {
                remove_internal_key(x, nearest, nearest+1);
            }
            else  
            {
                merge_left(x->c[nearest], x->c[nearest+1],k);
                x->n--;
                if (x_root && x->n == 0)
                {
                    
                }
                
            }
            
            //j = successor/predecessor index 
            // nearest/nearest+1
            
            /* code */
            
            //case 3
            //else
            
        }
        
        
    }
    else
    {
        if(x->c[nearest]->n)
        {

        }
        remove(x->c[nearest], int k, false);
    }
    
}

// return the index i of the first key in the btree node x where k <= x.keys[i]
// if i = x.n then no such key exists
int BTree::find_k(Node *x, int k)
{
    
}

// remove the key at index i from a btree leaf node x
void BTree::remove_leaf_key(Node *x, int i)
{
    x->n--;
    x->keys[nearest] = 
}

// remove the key at index i and child at index j from a btree internal node x
void BTree::remove_internal_key(Node *x, int i, int j)
{
}

// return the max key in the btree rooted at node x
int BTree::max_key(Node *x)
{
}

// return the min key in the btree rooted at node x
int BTree::min_key(Node *x)
{
}

// merge key k and all keys and children from y into y's LEFT sibling x
void BTree::merge_left(Node *x, Node *y, int k)
{
}

// merge key k and all keys and children from y into y's RIGHT sibling x
void BTree::merge_right(Node *x, Node *y, int k)
{
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's LEFT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_left(Node *x, Node *y, Node *z, int i)
{
}

// Give y an extra key by moving a key from its parent x down into y
// Move a key from y's RIGHT sibling z up into x
// Move appropriate child pointer from z into y
// Let i be the index of the key dividing y and z in x
void BTree::swap_right(Node *x, Node *y, Node *z, int i)
{
}