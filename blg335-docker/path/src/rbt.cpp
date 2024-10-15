////////////////////////////
// BLG 335 - Project 3
// Emil Huseynov - 150210906
////////////////////////////
// EB
// Implementing Red-Black Tree in C++

#include <iostream>
#include <string>


namespace RBT {
    enum Color { RED, BLACK }; // Color definition for Red-Black Tree nodes

    struct Node {
        int data;
        std::string name;
        Node *parent, *left, *right;
        Color color;

        Node(int data, std::string name) : data(data), name(name), parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
    };
}

class RedBlackTree {
private:
    RBT::Node *root;

    void leftRotate(RBT::Node *x) {
        // x is the node around which the left rotation is performed

        RBT::Node *y = x->right; // Set y as the right child of x
        x->right = y->left;      // Turn y's left subtree into x's right subtree

        if (y->left != nullptr) {
            y->left->parent = x; // Update parent pointer of y's left
        }

        y->parent = x->parent; // Link x's parent to y

        if (x->parent == nullptr) {
            root = y; // y becomes the new root if x was the root
        } else if (x == x->parent->left) {
            x->parent->left = y; // If x was a left child, y becomes a left child
        } else {
            x->parent->right = y; // If x was a right child, y becomes a right child
        }

        y->left = x; // Put x on y's left
        x->parent = y; // Update x's parent
    }

    void rightRotate(RBT::Node *x) {
        // x is the node around which the right rotation is performed

        RBT::Node *y = x->left; // Set y as the left child of x
        x->left = y->right;     // Turn y's right subtree into x's left subtree

        if (y->right != nullptr) {
            y->right->parent = x; // Update parent pointer of y's right
        }

        y->parent = x->parent;  // Link x's parent to y

        if (x->parent == nullptr) {
            root = y; // y becomes the new root if x was the root
        } else if (x == x->parent->right) {
            x->parent->right = y; // If x was a right child, y becomes a right child
        } else {
            x->parent->left = y; // If x was a left child, y becomes a left child
        }

        y->right = x; // Put x on y's right
        x->parent = y; // Update x's parent
    }

    void fixViolation(RBT::Node *&root, RBT::Node *&pt) {
    RBT::Node *parent_pt = nullptr;
    RBT::Node *grand_parent_pt = nullptr;

    while ((pt != root) && (pt->color != RBT::BLACK) && (pt->parent->color == RBT::RED)) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        // Case A: Parent of pt is left child of Grand-parent of pt
        if (parent_pt == grand_parent_pt->left) {
            RBT::Node *uncle_pt = grand_parent_pt->right;

            // Case 1: The uncle of pt is also red, only Recoloring required
            if (uncle_pt != nullptr && uncle_pt->color == RBT::RED) {
                grand_parent_pt->color = RBT::RED;
                parent_pt->color = RBT::BLACK;
                uncle_pt->color = RBT::BLACK;
                pt = grand_parent_pt;
            } else {
                // Case 2: pt is right child of its parent, Left-rotation required
                if (pt == parent_pt->right) {
                    leftRotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                // Case 3: pt is left child of its parent, Right-rotation required
                rightRotate(grand_parent_pt);
                std::swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }

        // Case B: Parent of pt is right child of Grand-parent of pt
        else {
            RBT::Node *uncle_pt = grand_parent_pt->left;

            // Case 1: The uncle of pt is also red, only Recoloring required
            if ((uncle_pt != nullptr) && (uncle_pt->color == RBT::RED)) {
                grand_parent_pt->color = RBT::RED;
                parent_pt->color = RBT::BLACK;
                uncle_pt->color = RBT::BLACK;
                pt = grand_parent_pt;
            } else {
                // Case 2: pt is left child of its parent, Right-rotation required
                if (pt == parent_pt->left) {
                    rightRotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                // Case 3: pt is right child of its parent, Left-rotation required
                leftRotate(grand_parent_pt);
                std::swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    root->color = RBT::BLACK; // Keep root always black
}

    void inorderHelper(RBT::Node* node, std::pair<std::string, int>* array, int &index) {
        if (node != nullptr) {
            inorderHelper(node->left, array, index);
            array[index++] = std::make_pair(node->name, node->data);
            inorderHelper(node->right, array, index);
        }
    }


  void transplant(RBT::Node *u, RBT::Node *v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        if (v != nullptr) {
            v->parent = u->parent;
        }
    }

    void deleteNodeHelper(RBT::Node *&root, RBT::Node *node, int key) {
        RBT::Node *z = nullptr;
        RBT::Node *x, *y;

        // Find the node to delete
        while (node != nullptr) {
            if (node->data == key) {
                z = node;
            }

            if (node->data <= key) {
                node = node->right;
            } else {
                node = node->left;
            }
        }

        if (z == nullptr) {
            return; // Key not found, return
        }

        y = z;
        int y_original_color = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        if (y_original_color == RBT::BLACK) {
            fixDelete(root, x);
        }
    }

    void fixDelete(RBT::Node *&root, RBT::Node *x) {
        while (x != root && x->color == RBT::BLACK) {
            if (x == x->parent->left) {
                RBT::Node *w = x->parent->right;

                if (w->color == RBT::RED) {
                    w->color = RBT::BLACK;
                    x->parent->color = RBT::RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }

                if (w->left->color == RBT::BLACK && w->right->color == RBT::BLACK) {
                    w->color = RBT::RED;
                    x = x->parent;
                } else {
                    if (w->right->color == RBT::BLACK) {
                        w->left->color = RBT::BLACK;
                        w->color = RBT::RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = RBT::BLACK;
                    w->right->color = RBT::BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                // Mirror case: x is right child
                // Similar handling as the left child case but with mirrored operations
            }
        }
        x->color = RBT::BLACK;
    }

   RBT::Node* searchTreeHelper(RBT::Node* node, int key) {
        while (node != nullptr) {
            if (key == node->data) {
                return node; // Key found, return the node
            } else if (key < node->data) {
                node = node->left; // Move to left subtree if key is smaller
            } else {
                node = node->right; // Move to right subtree if key is larger
            }
        }
        return nullptr; // Key not found
    }

  void preorderHelper(RBT::Node* node) {
        if (node != nullptr) {
            std::cout << node->data << " ";  // Visit the root
            preorderHelper(node->left);      // Visit left subtree
            preorderHelper(node->right);     // Visit right subtree
        }
    }

  void postorderHelper(RBT::Node* node) {
        if (node != nullptr) {
            postorderHelper(node->left);     // Visit left subtree
            postorderHelper(node->right);    // Visit right subtree
            std::cout << node->data << " ";  // Visit the root
        }
    }

  int getHeightHelper(RBT::Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return 1 + std::max(getHeightHelper(node->left), getHeightHelper(node->right));
    }

  int getTotalNodesHelper(RBT::Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return 1 + getTotalNodesHelper(node->left) + getTotalNodesHelper(node->right);
    }

  void clearTreeHelper(RBT::Node* node) {
      if (node != nullptr) {
          clearTreeHelper(node->left);
          clearTreeHelper(node->right);
          delete node;
      }
  }

  RBT::Node* copyTree(RBT::Node* node, RBT::Node* parent) {
      if (node == nullptr) {
          return nullptr;
      }

      RBT::Node* newNode = new RBT::Node(node->data, node->name);
      newNode->color = node->color;
      newNode->parent = parent;
      newNode->left = copyTree(node->left, newNode);
      newNode->right = copyTree(node->right, newNode);
      return newNode;
  }

  bool isBalancedHelper(RBT::Node* node, int &blackHeight, int currentBlackHeight) {
        if (node == nullptr) {
            // When reaching a leaf (NIL node), check black height
            if (blackHeight == -1) {
                blackHeight = currentBlackHeight;
                return true;
            } else {
                return blackHeight == currentBlackHeight;
            }
        }

        // Red-Black Tree property: Red nodes cannot have red children
        if (node->color == RBT::RED) {
            if ((node->left && node->left->color == RBT::RED) || (node->right && node->right->color == RBT::RED)) {
                return false;
            }
        } else {
            // Count black nodes
            currentBlackHeight++;
        }

        // Check recursively for left and right subtrees
        return isBalancedHelper(node->left, blackHeight, currentBlackHeight) && 
               isBalancedHelper(node->right, blackHeight, currentBlackHeight);
    }

   void printTreeHelper(RBT::Node* node, int space) {
        const int LEVEL_GAP = 5;  // Gap between levels of tree

        if (node == nullptr)
            return;

        space += LEVEL_GAP;

        // Process right child first
        printTreeHelper(node->right, space);

        std::cout << std::endl;
        for (int i = LEVEL_GAP; i < space; i++)
            std::cout << " ";
        std::cout << node->data << (node->color == RBT::RED ? "R" : "B") << "\n";

        // Process left child
        printTreeHelper(node->left, space);
    }



public:
    RedBlackTree() : root(nullptr) {}

    void insert(const std::string &name, const int &data) {
        RBT::Node *newNode = new RBT::Node(data, name); // Create a new node

        // BST insertion starts here
        RBT::Node *y = nullptr;
        RBT::Node *x = this->root;

        while (x != nullptr) {
            y = x;
            if (newNode->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        // y is parent of x
        newNode->parent = y;
        if (y == nullptr) {
            root = newNode; // If new node is the first node, set as root
        } else if (newNode->data < y->data) {
            y->left = newNode;
        } else {
            y->right = newNode;
        }

        // If new node is root node, simply return
        if (newNode->parent == nullptr){
            newNode->color = RBT::BLACK;
            return;
        }

        // If the grandparent is null, simply return
        if (newNode->parent->parent == nullptr) {
            return;
        }

        // Fix the tree
        fixViolation(root, newNode);
    }

    void deleteNode(const int &data) {
      deleteNodeHelper(root, root, data);
    }

    RBT::Node* searchTree(int key) {
        return searchTreeHelper(this->root, key);
    }


    RBT::Node* minimum(RBT::Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    RBT::Node* maximum(RBT::Node* node) {
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }

    RBT::Node* successor(RBT::Node* node) {
        if (node == nullptr) {
            return nullptr;
        }

        // Case 1: If the right subtree is not null, find the minimum in the right subtree
        if (node->right != nullptr) {
            return minimum(node->right);
        }

        // Case 2: Otherwise, go up until the node is the left child of its parent
        RBT::Node *parentNode = node->parent;
        while (parentNode != nullptr && node == parentNode->right) {
            node = parentNode;
            parentNode = parentNode->parent;
        }
        return parentNode;
    }

   RBT::Node* predecessor(RBT::Node* node) {
        if (node == nullptr) {
            return nullptr;
        }

        // Case 1: If the left subtree is not null, find the maximum in the left subtree
        if (node->left != nullptr) {
            return maximum(node->left);
        }

        // Case 2: Otherwise, go up until the node is the right child of its parent
        RBT::Node *parentNode = node->parent;
        while (parentNode != nullptr && node == parentNode->left) {
            node = parentNode;
            parentNode = parentNode->parent;
        }
        return parentNode;
    }

    RBT::Node* getMinimum() {
        return minimum(root);
    }

    RBT::Node* getMaximum() {
        return maximum(root);
    }


    void inorder(std::pair<std::string, int>* array, int startIndex) {
        int index = startIndex;
        inorderHelper(root, array, index);
    }

   void preorder() {
        preorderHelper(root);
        std::cout << std::endl;
    }
  
    void postorder() {
        postorderHelper(root);
        std::cout << std::endl;
    }

   int getHeight() {
        return getHeightHelper(root) - 1; // Subtract 1 to get the height from the root
    }

    int getTotalNodes() {
          return getTotalNodesHelper(root);
      }

  void clearTree() {
    clearTreeHelper(root);
    root = nullptr;
  }

  // Copy constructor
  RedBlackTree(const RedBlackTree &other) {
      root = copyTree(other.root, nullptr);
  }

  // Assignment operator
  RedBlackTree& operator=(const RedBlackTree &other) {
      if (this != &other) {
          clearTree();
          root = copyTree(other.root, nullptr);
      }
      return *this;
  }

    bool isBalanced() {
        int blackHeight = -1;  // Initialized to -1 to indicate it's not set yet
        int currentBlackHeight = 0;  // Start with a black height of 0
        return isBalancedHelper(root, blackHeight, currentBlackHeight);
    }

  void printTree() {
      printTreeHelper(root, 0);
  }

  ~RedBlackTree() {
    clearTree();
}
};
