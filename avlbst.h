#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{
}

/**
* A getter for the balance of an AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value>* node, int8_t diff);
    void fixBalance(AVLNode<Key, Value>* node);
    // Add helper functions here


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* child = node->getRight();
    node->setRight(child->getLeft());
    if (node->getRight() != nullptr) {
        node->getRight()->setParent(node);
    }
    child->setLeft(node);

    if (node == static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_)) {
        BinarySearchTree<Key, Value>::root_ = child;
        child->setParent(nullptr);
        node->setParent(child);
    } else {
        AVLNode<Key, Value>* parent = node->getParent();
        if (parent->getLeft() == node) {
            parent->setLeft(child);
        } else {
          parent->setRight(child);
        }
        child->setParent(parent);
        node->setParent(child);
    }
    //fixBalance(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_));
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {

    AVLNode<Key, Value>* child = node->getLeft();
    node->setLeft(child->getRight());
    if (node->getLeft() != nullptr) {
        node->getLeft()->setParent(node);
    } else {
    }
    child->setRight(node);

    if (node == static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_)) {
        BinarySearchTree<Key, Value>::root_ = child;
        child->setParent(nullptr);
        node->setParent(child);
    } else {
        AVLNode<Key, Value>* parent = node->getParent();
        if (parent->getLeft() == node) {
            parent->setLeft(child);
        } else {
          parent->setRight(child);
        }
        child->setParent(parent);
        node->setParent(child);
    }
    //fixBalance(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_));
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::fixBalance(AVLNode<Key, Value>* node) {
  if (node == nullptr) {
    return;
  }
  fixBalance(node->getLeft());
  fixBalance(node->getRight());
  int lHeight = BinarySearchTree<Key, Value>::height(node->getLeft());
  int rHeight = BinarySearchTree<Key, Value>::height(node->getRight());
  int balance = rHeight - lHeight;
  node->setBalance(balance);
}


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* grandparent = parent->getParent();
    if (parent == nullptr || grandparent == nullptr) {
        return;
    }

    // Left child
    if (grandparent->getLeft() == parent) {
        grandparent->updateBalance(-1);
        int8_t balance = grandparent->getBalance();
        if (balance == 0) {
            return;
        }

        if (balance == -1) {
            insertFix(grandparent, parent);
        }

        if (balance == -2) {
            if (parent->getLeft() == node) {
                rotateRight(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else {
                rotateLeft(parent);
                rotateRight(grandparent);
                if (node->getBalance() == -1) {
                    node->setBalance(0);
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                } else if (node->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                } else if (node->getBalance() == 1) {
                    node->setBalance(0);
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                }
            }
        }
    // Right child
    } else {
        grandparent->updateBalance(1);
        int8_t balance = grandparent->getBalance();
        if (balance == 0) {
            return;
        }

        if (balance == 1) {
            insertFix(grandparent, parent);
        }

        if (balance == 2) {
            if (parent->getRight() == node) {
                rotateLeft(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else {
                rotateRight(parent);
                rotateLeft(grandparent);
                if (node->getBalance() == 1) {
                    node->setBalance(0);
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                } else if (node->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                } else if (node->getBalance() == -1) {
                    node->setBalance(0);
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                }
            }
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if (BinarySearchTree<Key, Value>::root_ == nullptr) {
        BinarySearchTree<Key, Value>::root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }

    AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
    int x = 0;
    bool b = false;
    while (parent != nullptr) {
        if (parent->getKey() > new_item.first) {
            if (parent->getLeft() == nullptr) {
                x = 1;
                b = true;
            } else {
                parent = parent->getLeft();
            }
        } else if (parent->getKey() < new_item.first) {
            if (parent->getRight() == nullptr) {
                x = 2;
                b = true;
            } else {
                parent = parent->getRight();
            }
        } else {
            parent->setValue(new_item.second);
            return;
        }
        if (b) {
            break;
        }
    }

    AVLNode<Key, Value>* node = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (x == 1) {
        parent->setLeft(node);
    } else if (x == 2) {
        parent->setRight(node);
    }

    if (parent->getBalance() == -1) {
        parent->setBalance(0);
    } else if (parent->getBalance() == 1) {
        parent->setBalance(0);
    } else {
        if (x == 1) {
            parent->setBalance(-1);
        } else {
            parent->setBalance(1);
        }
    insertFix(node->getParent(), node);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int8_t diff) {
    if (node == nullptr) {
        return;
    }
    AVLNode<Key, Value>* parent = node->getParent();
    int8_t ndiff;
    if (parent != nullptr) {
        if (parent->getLeft() == node) {
            //removeFix(parent, 1);
            ndiff = 1;
        } else {
            //removeFix(parent, -1);
            ndiff = -1;
        }
    }

    if (diff == -1) {
        if (node->getBalance() + diff == -2) {
            AVLNode<Key, Value>* child;
            child = node->getLeft();
            if (child->getBalance() == -1) {
              removeFix(node->getParent(), ndiff);
                rotateRight(node);
                node->setBalance(0);
                child->setBalance(0);
            } else if (child->getBalance() == 0) {
                rotateRight(node);
                node->setBalance(-1);
                child->setBalance(1);
            } else if (child->getBalance() == 1) {
                AVLNode<Key, Value>* grandchild = child->getRight();
                removeFix(node->getParent(), ndiff);
                rotateLeft(child);
                rotateRight(node);
                if (grandchild->getBalance() == 1) {
                    grandchild->setBalance(0);
                    child->setBalance(-1);
                    node->setBalance(0);
                } else if (grandchild->getBalance() == 0) {
                    grandchild->setBalance(0);
                    child->setBalance(0);
                    node->setBalance(0);
                } else if (grandchild->getBalance() == -1) {
                    grandchild->setBalance(0);
                    child->setBalance(0);
                    node->setBalance(1);
                }
            }
        } else if (node->getBalance() + diff == -1) {
            node->setBalance(-1);
        } else if (node->getBalance() + diff == 0) {
          removeFix(node->getParent(), ndiff);
            node->setBalance(0);
        }
    } else {
        if (node->getBalance() + diff == 2) {
            AVLNode<Key, Value>* child;
            child = node->getRight();
            if (child->getBalance() == 1) {
              removeFix(node->getParent(), ndiff);
                rotateLeft(node);
                node->setBalance(0);
                child->setBalance(0);
            } else if (child->getBalance() == 0) {
                rotateLeft(node);
                node->setBalance(1);
                child->setBalance(-1);
            } else if (child->getBalance() == -1) {
              removeFix(node->getParent(), ndiff);
                AVLNode<Key, Value>* grandchild = child->getLeft();
                rotateRight(child);
                rotateLeft(node);
                if (grandchild->getBalance() == -1) {
                    grandchild->setBalance(0);
                    child->setBalance(1);
                    node->setBalance(0);
                } else if (grandchild->getBalance() == 0) {
                    grandchild->setBalance(0);
                    child->setBalance(0);
                    node->setBalance(0);
                } else if (grandchild->getBalance() == 1) {
                    grandchild->setBalance(0);
                    child->setBalance(0);
                    node->setBalance(-1);
                }
            }
        } else if (node->getBalance() + diff == 1) {
            node->setBalance(1);
        } else if (node->getBalance() + diff == 0) {
          removeFix(node->getParent(), ndiff);
            node->setBalance(0);
        }
    }
}



template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    Node<Key, Value>* temp = BinarySearchTree<Key, Value>::internalFind(key);

    if (temp == nullptr) {
        return;
    }

    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(temp);

    if (node->getLeft() != nullptr && node->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(temp));
        nodeSwap(node, pred);
    }

    AVLNode<Key, Value>* parent = node->getParent();

    int diff = 0;
    if (parent != nullptr) {
        if (parent->getLeft() == node) {
            diff = 1;
        } else {
            diff = -1;
        }
    }
    // has no successor
    if (node->getLeft() == nullptr && node->getRight() == nullptr) {

      if (parent == nullptr) {
        BinarySearchTree<Key, Value>::root_ = nullptr;
      } else if (parent->getLeft() == node) {
        parent->setLeft(nullptr);
      } else if (parent->getRight() == node) {
        parent->setRight(nullptr);
      }
      // has a successor
    } else if (node->getLeft() != nullptr || node->getRight() != nullptr) {
      AVLNode<Key, Value>* child;

      // root node
      if (parent == nullptr) {
        if (node->getLeft() != nullptr) {
          child = node->getLeft();
        } else if (node->getRight() != nullptr) {
          child = node->getRight();
        }
        child->setParent(nullptr);
        BinarySearchTree<Key, Value>::root_ = child;

        // has left child
      } else if (node->getLeft() != nullptr) {
        child = node->getLeft();
        if (parent->getLeft() == node) {
          parent->setLeft(child);
        } else if (parent->getRight() == node) {
          parent->setRight(child);
        }
        child->setParent(parent);

        // has right child
      } else if (node->getRight() != nullptr) {
        child = node->getRight();
        if (parent->getLeft() == node) {
          parent->setLeft(child);
        } else if (parent->getRight() == node) {
          parent->setRight(child);
        }
        child->setParent(parent);
      }
    }
    parent = node->getParent();
    delete temp;
    //temp->~Node();
    //BinarySearchTree<Key, Value>::remove(key);
    /*if (parent != nullptr) {
      parent->updateBalance(diff);
    }*/
    if (diff != 0) {
      removeFix(parent, diff);
    }



}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
