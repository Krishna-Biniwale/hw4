#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif
#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int height(Node* root) {
    if (root == nullptr) {
        return 0;
    } else {
        return 1 + max(height(root->left), height(root->right));
    }
}
bool equalPaths(Node * root)
{
  if (root == nullptr) {
    return true;
  }
  if (root->left == nullptr) {
    return equalPaths(root->right);
  }
  if (root->right == nullptr) {
    return equalPaths(root->left);
  }
    return height(root->left) == height(root->right);

}

