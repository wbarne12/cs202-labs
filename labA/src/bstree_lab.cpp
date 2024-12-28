/* Stanley Barnes
 * wbarne12
 * 000-65-7926
 *  
 * Lab Write Up:
 *   https://web.eecs.utk.edu/~jplank/plank/classes/cs202/Labs/LabA/*/
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <cmath>
#include "bstree.hpp"
using namespace std;
using CS202::BSTree;
using CS202::BSTNode;

int BSTree::Depth(const string &key) const
{
  // Similar to find function. Start at root and add +1 for each node traversed. 
  BSTNode *current = sentinel->right;
  int depth = 0;

  while (current != sentinel) {
    if (key == current->key) {
      return depth;
    } else if (key < current->key) {
      current = current->left;
    } else {
      current = current->right;
    }
    ++depth;
  }

  return -1; 
}
           
int BSTree::Height() const
{
  return recursive_find_height(sentinel->right) + 1;
}

vector <string> BSTree::Ordered_Keys() const
{
  vector <string> rv;

  make_key_vector(sentinel->right, rv);

  return rv;
}
    
BSTree::BSTree(const BSTree &t)        
{
  // Make a sentinel and copy over the values from t
  this->sentinel = new BSTNode;
  this->sentinel->left   = this->sentinel;
  this->sentinel->right  = this->sentinel;
  this->sentinel->parent = this->sentinel;
  this->size = 0;

  // Recursivly copy and set root parent pointer to sentinel.
  if (t.sentinel->right != t.sentinel) {
    this->sentinel->right = 
        make_balanced_tree(t.Ordered_Keys(), t.Ordered_Vals(), 0, t.size);
  }
  if (this->sentinel->right) {
    this->sentinel->right->parent = this->sentinel;
  }

  this->size = t.size;
}

BSTree &BSTree::operator=(const BSTree &t) 
{
  // Clear the current tree, copy over from make balanced tree, and then set the parent. 
  if (this != &t) {
    Clear();

    if (t.sentinel->right != t.sentinel) {
      this->sentinel->right = 
          make_balanced_tree(t.Ordered_Keys(), t.Ordered_Vals(), 0, t.size);

      if (this->sentinel->right) this->sentinel->right->parent = this->sentinel;
    }
    this->size = t.size;
  }
  return *this;
}

int BSTree::recursive_find_height(const BSTNode *n) const
{
  // Copy from the Hacker rank we did in lab.
  if (n == sentinel) return -1;

  int left_h = recursive_find_height(n->left);
  int right_h = recursive_find_height(n->right);

  return max(left_h, right_h) + 1;
}

void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const
{
  // In order traversal. 
  if (n == sentinel) return;

  make_key_vector(n->left, v);
  v.push_back(n->key);
  make_key_vector(n->right, v);
}

BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys, 
                            const vector<void *> &vals,
                            size_t first_index,
                            size_t num_indices) const
{
  // Make a copy of the tree, but it has to be balanced. .
  if (num_indices == 0) return sentinel;

  size_t mid = num_indices / 2 + first_index;

  BSTNode *node = new BSTNode;
  node->key = sorted_keys[mid];
  node->val = vals[mid];
  node->left  = make_balanced_tree(sorted_keys, vals, first_index, mid - first_index);
  node->right = make_balanced_tree(sorted_keys, vals, mid + 1, num_indices - (mid - first_index) - 1);

  if (node->left != sentinel) {
    node->left->parent = node;
  } 
  if (node->right != sentinel) {
    node->right->parent = node;
  }

  return node;
}
