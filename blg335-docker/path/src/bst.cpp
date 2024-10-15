////////////////////////////
// BLG 335 - Project 3
// Emil Huseynov - 150210906
////////////////////////////
// EB
// Implementing Binary Search Tree in C++

#include <iostream>
#include <algorithm>  // For std::max used in getHeight()
#include <string>


namespace BST { 
    struct Node {
        int data;
        std::string name;
        Node *parent, *left, *right;

        // Constructor for creating a new Node
        Node(int data, std::string name) : data(data), name(name), parent(nullptr), left(nullptr), right(nullptr) {}
    };
}

class BinarySearchTree {
private:
  BST::Node *root;

  // Helper function for inorder traversal
  void inorderHelper(BST::Node* node, std::pair<std::string, int>* array, int &index) {
      if (node != nullptr) {
          inorderHelper(node->left, array, index);
          array[index++] = std::make_pair(node->name, node->data);
          inorderHelper(node->right, array, index);
      }
  }

  // Helper function for searching a tree for a given key
  BST::Node* searchTreeHelper(BST::Node* node, int key) {
    if (node == nullptr || node->data == key) {
      return node;
    }

    if (node->data < key) {
      return searchTreeHelper(node->right, key);
    }

  return searchTreeHelper(node->left, key);
  }

  // Helper function for getting the height of the tree
  int getHeightHelper(BST::Node* node) {
    if (node == nullptr) {
    return 0;
    }
  return 1 + std::max(getHeightHelper(node->left), getHeightHelper(node->right));
  }


  // Helper function for counting total nodes in the tree
  int getTotalNodesHelper(BST::Node* node) {
    if (node == nullptr) {
      return 0;
    }
  return 1 + getTotalNodesHelper(node->left) + getTotalNodesHelper(node->right);
  }


  // Helper function for inserting a new node
  void insertHelper(BST::Node *&root, BST::Node *node, BST::Node *parent) {
    if (root == nullptr) {
      root = node;
      node->parent = parent;
    } else if (node->data < root->data) {
      insertHelper(root->left, node, root);
    } else {
      insertHelper(root->right, node, root);
    }
  }


      void deleteNodeHelper(BST::Node *&node, int key) {
        if (node == nullptr) {
            return; // Node not found
        }

        if (key < node->data) {
            deleteNodeHelper(node->left, key);
        } else if (key > node->data) {
            deleteNodeHelper(node->right, key);
        } else {
            if (node->left == nullptr || node->right == nullptr) {
                BST::Node *temp = node->left ? node->left : node->right;

                if (temp == nullptr) { // No child case
                    temp = node;
                    node = nullptr;
                } else { // One child case
                    BST::Node *tempParent = node->parent;
                    *node = *temp; // Copy the contents of the non-empty child
                    node->parent = tempParent;
                    delete temp;
                }
            } else {
                BST::Node *temp = minimum(node->right);
                node->data = temp->data;
                node->name = temp->name;
                deleteNodeHelper(node->right, temp->data);
            }
        }
        }

  // Helper function to find the minimum node in a subtree
  BST::Node* minimum(BST::Node* node) {
      BST::Node* current = node;
      while (current->left != nullptr) {
          current = current->left;
      }
      return current;
  }

  // Helper function to find the maximum node in a subtree
  BST::Node* maximum(BST::Node* node) {
      BST::Node* current = node;
      while (current->right != nullptr) {
          current = current->right;
      }
      return current;
  }


// Helper function for preorder traversal
  void preorderHelper(BST::Node* node) {
      if (node != nullptr) {
          std::cout << node->name << " (" << node->data << ")" << std::endl;
          preorderHelper(node->left);
          preorderHelper(node->right);
      }
  }

  // Helper function for postorder traversal
  void postorderHelper(BST::Node* node) {
      if (node != nullptr) {
          postorderHelper(node->left);
          postorderHelper(node->right);
          std::cout << node->name << " (" << node->data << ")" << std::endl;
      }
  }

  // Helper function to clear the tree
  void clearTree(BST::Node* node) {
  if (node != nullptr) {
      clearTree(node->left);   // Delete left subtree
      clearTree(node->right);  // Delete right subtree
      delete node;             // Delete the current node
  }
}

  // // Get the maximum value in the tree
  // BST::Node* getMaximum(BST::Node* node = nullptr) {
  //     if (!node) node = root;
  //     while (node->right != nullptr) {
  //         node = node->right;
  //     }
  //     return node;
  // }

  // // Get the minimum value in the tree
  // BST::Node* getMinimum(BST::Node* node = nullptr) {
  //   if (!node) node = root;
  //   while (node->left != nullptr) {
  //       node = node->left;
  //   }
  //   return node;
  // }

public:
  // Constructor to initialize root to nullptr
  BinarySearchTree() : root(nullptr) {}

  // Insert a node with given data
  void insert(std::string name, int data) {
      BST::Node *newNode = new BST::Node(data, name);
      insertHelper(root, newNode, nullptr);
  }

  // Delete a node with given data
  void deleteNode(int data) {
      deleteNodeHelper(root, data);
  }

  // Search a node with given data
  BST::Node* searchTree(int key) {
      return searchTreeHelper(root, key);
  }

  // Inorder traversal of the tree
  // void inorder() {
  //     inorderHelper(root);
  // }

  void inorder(std::pair<std::string, int>* array, int startIndex) {
    int index = startIndex;
    inorderHelper(root, array, index);
}

  // Get the height of the tree
  int getHeight() {
      return getHeightHelper(root);
  }

  // Get the total number of nodes in the tree
  int getTotalNodes() {
      return getTotalNodesHelper(root);
  }

  // Destructor to free allocated memory
  ~BinarySearchTree() {
  clearTree(root);
  }

  // Perform a preorder traversal of the tree
  void preorder() {
      preorderHelper(root);
  }

  // Perform a postorder traversal of the tree
  void postorder() {
      postorderHelper(root);
  }


  // Get the successor of a given node
  BST::Node* successor(BST::Node* node) {
      if (node == nullptr) return nullptr;

      // If right subtree is not null, successor is the minimum in right subtree
      if (node->right != nullptr) {
          return minimum(node->right);
      }

      // Find the nearest ancestor for which given node would be in left subtree
      BST::Node* parentNode = node->parent;
      while (parentNode != nullptr && node == parentNode->right) {
          node = parentNode;
          parentNode = parentNode->parent;
      }
      return parentNode;
  }

  // Get the predecessor of a given node
  BST::Node* predecessor(BST::Node* node) {
      if (node == nullptr) return nullptr;

      // If left subtree is not null, predecessor is the maximum in left subtree
      if (node->left != nullptr) {
          return maximum(node->left);
      }

      // Find the nearest ancestor for which given node would be in right subtree
      BST::Node* parentNode = node->parent;
      while (parentNode != nullptr && node == parentNode->left) {
          node = parentNode;
          parentNode = parentNode->parent;
      }
      return parentNode;
  }

  BST::Node* getMinimum() {
    return minimum(root);
  }


  BST::Node* getMaximum() {
    return maximum(root);
  }
};
