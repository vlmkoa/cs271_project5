#include "btree.h"

/*
NOTE: Please follow logic from CLRSv4 directly. Additionally, in cases 3a and 3b please check for an immediate right sibling first.
*/
/**
 * Created by Stacey Truex, Ryan Vo, Benjamin Nelson, and Nathan Hydukovich
 * This file contains the remove function and its helper functions for the BTree class.
 */

/**
 * remove(k): remove the key k from the tree
 *
 * Pre-condition: None
 * Post-condition: If k is in the tree, it is removed.
 */
using namespace std;
void BTree::remove(int k)
{
    remove(root, k, true);
}

/**
 * remove(x, k, x_root): delete the key k from the btree rooted at x
 *
 * Pre-condition: if x_root is true, then x must have at least one key
 *                if x_root is false, then x must have at least t keys
 * Post-condition: If k is in the subtree rooted at x, it is removed.
 */
void BTree::remove(Node *x, int k, bool x_root)
{
    if (x == nullptr)
    {
        cout << "node is null" << endl;
        return;
    }
    int nearest = find_k(x, k);

    if(x->leaf &&  (nearest == x->n || x->keys[nearest] != k)){ //special case. not in leaf and not in tree
        cout<< "key is not in node" << endl;
        return; 
    }
    if (nearest < x->n && x->keys[nearest] == k)
    {
        // case 1: key is in leaf node
        if (x->leaf)
        {
            remove_leaf_key(x, nearest);
        }
        else // case 2: key is in internal node
        {
            // if child at nearest or nearest+1 has >= t keys
            // case 2a
            if (x->c[nearest]->n >= t)
            {
                remove_internal_key(x, nearest, nearest);
            }
            else if (x->c[nearest + 1]->n >= t) // case 2b
            {
                remove_internal_key(x, nearest, nearest + 1);
            }
            else // case 2c: both children have t-1 keys
            {
                merge_left(x->c[nearest], x->c[nearest + 1], x->keys[nearest]);
                for (int i = nearest; i < (x->n - 1); i++)
                {
                    x->keys[i] = x->keys[i + 1];
                    x->c[i + 1] = x->c[i + 2];
                }

                x->n--;
                if (x_root && x->n == 0) // Root case
                {
                    root = x->c[0];
                    delete x;
                    remove(k); // recursive call
                }
                else
                {
                    remove(x->c[nearest], k); // recursive call
                }
            }
        }
    }
    else // case 3: key is not in node
    {
        if (x->c[nearest]->n == t - 1)
        {
            // case 3a: if either sibling of the nearest child has at least t keys
            if (nearest < x->n && x->c[nearest + 1]->n >= t)
            {
                swap_right(x, x->c[nearest], x->c[nearest + 1], nearest);
            }
            else if (nearest >= 1 && x->c[nearest - 1]->n >= t)
            {
                swap_left(x, x->c[nearest], x->c[nearest - 1], nearest - 1);
            }

            else // case 3b: if siblings of the nearest child has t-1 keys
            {
                if (nearest < x->n) // nearest isn't the last one
                {
                    merge_right(x->c[nearest + 1], x->c[nearest], x->keys[nearest]);
                    for (int i = nearest; i < (x->n - 1); i++)
                    {
                        x->keys[i] = x->keys[i + 1];
                    }
                    for (int i = nearest; i < (x->n); i++)
                    {
                        x->c[i] = x->c[i + 1];
                    }
                }
                else // nearest is the last one
                {
                    merge_left(x->c[nearest - 1], x->c[nearest], x->keys[nearest-1]);
                    nearest--;
                }
                x->n--;
            }
        }
        // if root is empty after merge
        if (x_root && x->n == 0) // Root case
        {
            root = x->c[0];
            delete x;
            remove(k); // recursive call
        }
        else
        {
            remove(x->c[nearest], k); // recursive call
        }
    }
}

/**
 * find_k(x, k): return the index i of the first key in the btree node x where k <= x.keys[i]
 * if i = x.n then no such key exists
 *
 * Pre-condition: x is not nullptr
 * Post-condition: returns the index i of the first key in the btree node x where k <= x.keys[i]
 */
int BTree::find_k(Node *x, int k)
{
    // fast check for removing max key
    if (k == x->keys[x->n - 1])
    {
        return x->n - 1;
    }
    int i = 0; // i is location
    while (i < (x->n) && x->keys[i] < k)
    {
        i++;
    }
    return i;
}

/**
 * remove_leaf_key(x, i): remove the key at index i from a btree leaf node x
 *
 * Pre-condition: x is a leaf node and 0 <= i < x.n
 * Post-condition: the key at index i is removed from the btree leaf node x
 */
void BTree::remove_leaf_key(Node *x, int i)
{
    x->n--;
    for (int j = i; j < x->n; j++)
    {
        x->keys[j] = x->keys[j + 1];
    }
    // if root is empty after removing
    if (root == x && x->n == 0)
    {
        delete x;
        root = nullptr;
    }
    return;
}

/**
 * remove_internal_key(x, i, j): remove the key at index i from a btree internal node x by
 * replacing it with either its predecessor or successor at child index j and continue to
 * delete predecessor/successor from child j.
 *
 * Pre-condition: x is an internal node, 0 <= i < x.n, and j = i or j = i+1
 * Post-condition: the key at index i becomes its predecessor/successor from child j and
 * the predecessor/successor is removed from child j.
 */
void BTree::remove_internal_key(Node *x, int i, int j)
{
    int replacment = 0;
    if (i == j)
    {
        replacment = max_key(x->c[j]);
    }
    else
    {
        replacment = min_key(x->c[j]);
    }
    x->keys[i] = replacment;
    remove(x->c[j], replacment);
}

/**
 * max_key(x): return the max key in the btree rooted at node x
 *
 * Pre-condition: x is not nullptr
 * Post-condition: returns the max key in the btree rooted at node x
 */
int BTree::max_key(Node *x)
{
    return x->keys[x->n - 1];
}

/**
 * min_key(x): return the min key in the btree rooted at node x
 *
 * Pre-condition: x is not nullptr
 * Post-condition: returns the min key in the btree rooted at node x
 */
int BTree::min_key(Node *x)
{
    return x->keys[0];
}

/**
 * merge_left(x, y, k): merge key k and all keys and children from y into y's LEFT sibling x
 *
 * Pre-condition: x and y are siblings, x is to the left of y, and both have t-1 keys
 * Post-condition: key k and all keys and children from y are merged into x, and y is deleted
 */
void BTree::merge_left(Node *x, Node *y, int k)
{
    // copying all keys
    x->keys[t - 1] = k;
    for (int i = 0; i < t - 1; i++)
    {
        x->keys[t + i] = y->keys[i];
    }
    x->n = (2 * t) - 1;
    if (!x->leaf)
    { // if not leaf getting kids
        for (int i = 0; i < t; i++)
        {
            x->c[t + i] = y->c[i];
        }
    }
    delete y;
}

/**
 * merge_right(x, y, k): merge key k and all keys and children from y into y's RIGHT sibling x
 *
 * Pre-condition: x and y are siblings, y is to the left of x, and both have t-1 keys
 * Post-condition: key k and all keys and children from y are merged into x, and y is deleted
 */
void BTree::merge_right(Node *x, Node *y, int k)
{
    // copying all keys
    x->keys[t - 1] = k;
    for (int i = 0; i < t - 1; i++)
    {
        x->keys[t + i] = x->keys[i]; // move keys at i over
        x->keys[i] = y->keys[i];     // copy key's from y at i to x
    }
    x->n = (2 * t) - 1;
    if (!x->leaf)
    { // if not leaf getting kids
        for (int i = 0; i < t; i++)
        {
            x->c[t + i] = x->c[i]; // move child at i over
            x->c[i] = y->c[i];     // grab y's child at i to replace x at i
        }
    }
    delete y;
}

/**
 * swap_left(x, y, z, i): Give y an extra key by moving a key from its parent x down into y
 *                        Move rightmost key from y's LEFT sibling z up into x
 *                        Move rightmost child pointer from z into y leftmost position
 *                        i is the index of the key dividing y and z in x
 * Pre-condition: x is the parent of y and z, z is the LEFT sibling of y, and z has at least t keys
 * Post-condition: y has one more key, z has one less key, and the keys and children are appropriately moved
 */
void BTree::swap_left(Node *x, Node *y, Node *z, int i)
{
    // z = x->c[i] (left sibling), y = x->c[i+1]

    // Shift y's keys one position to the right
    for (int j = y->n - 1; j >= 0; --j)
    {
        y->keys[j + 1] = y->keys[j];
    }

    // If y is not a leaf, shift its child pointers to the right
    if (!y->leaf)
    {
        for (int j = y->n; j >= 0; --j)
        {
            y->c[j + 1] = y->c[j];
        }
    }

    // Move the dividing key from parent x down into y (front)
    y->keys[0] = x->keys[i];

    // If z is not a leaf, move its rightmost child over to be y's leftmost child
    if (!z->leaf)
    {
        y->c[0] = z->c[z->n]; // last child pointer of z
        z->c[z->n] = nullptr;
    }

    // Move z's rightmost key up into x
    x->keys[i] = z->keys[z->n - 1];

    y->n++;
    z->n--;
}

/**
 * swap_right(x, y, z, i): Give y an extra key by moving a key from its parent x down into y
 *                         Move leftmost key from y's RIGHT sibling z up into x
 *                         Move leftmost child pointer from z into y rightmost position
 *                         i is the index of the key dividing y and z in x
 * Pre-condition: x is the parent of y and z, z is the RIGHT sibling of y, and z has at least t keys
 * Post-condition: y has one more key, z has one less key, and the keys and children are appropriately moved
 */
void BTree::swap_right(Node *x, Node *y, Node *z, int i)
{
    // y = x->c[i], z = x->c[i+1] (right sibling)

    // Move the dividing key from parent x down into y as y's rightmost key
    y->keys[y->n] = x->keys[i];

    // If y is not a leaf, move z's leftmost child to be y's rightmost+1 child
    if (!y->leaf)
    {
        y->c[y->n + 1] = z->c[0];
    }

    // Move z's leftmost key up into x
    x->keys[i] = z->keys[0];

    // Shift z's keys left by one
    for (int j = 1; j < z->n; ++j)
    {
        z->keys[j - 1] = z->keys[j];
    }

    // If z is not a leaf, shift its child pointers left by one
    if (!z->leaf)
    {
        for (int j = 1; j <= z->n; ++j)
        {
            z->c[j - 1] = z->c[j];
        }
        z->c[z->n] = nullptr;
    }

    y->n++;
    z->n--;
}
