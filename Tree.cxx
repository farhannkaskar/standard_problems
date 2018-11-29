#include <iostream>
#include <vector>
#include <climits>

using namespace std;
class Node {
public:
  Node* left;
  Node* right;
  int data;
  Node(const int data_)
  :data(data_) {}
};

class Tree {
public:
  int BinarySearch(const vector<int>& array, const int si, const int ei, const int val) {
    const bool isInputValid = !array.empty() && si >=0 && si < array.size() &&
                              ei >=0 && ei < array.size() && si <= ei;

    if (!isInputValid) return -1;

    const int mi = (si + ei)/2;
    if (array[mi] == val) {
      return mi;
    } else if (array[mi] < val) {
      return BinarySearch(array, si, mi - 1, val);
    } else {
      return BinarySearch(array, mi + 1, ei, val);
    }
  }
  //
  // Example:
  // In : 3 5 6 9 11 13 14
  // Pre: 9 5 3 6 13 11 14
  //
  // is(0), ie (6), pi(0)
  //               9  <- Root
  //
  // is(0), ie (2), pi(1)
  //         5
  Node* BuildTreeUsingInorderAndPreorder(const vector<int>& in, const vector<int>& pre, const int is, const int ie, int* pi) {
    const bool isInputValid = !in.empty() & !pre.empty() && in.size() == pre.size() &&
                              is >= 0 && is < in.size() &&  ie >= is &&  ie < in.size() && *pi >= 0 && *pi < pre.size();

    if (!isInputValid) {
      return NULL;
    }

    Node* root(new Node(pre[*pi]));
    const int rootIndex = BinarySearch(in, is, ie, root->data);

    if (rootIndex == -1) {
      return NULL;
    }

    (*pi)++;
    root->left = BuildTreeUsingInorderAndPreorder(in, pre, is, rootIndex - 1, pi);
    root->right = BuildTreeUsingInorderAndPreorder(in, pre, rootIndex + 1, ie, pi);

    return root;
  }

  void printPreorder(const Node* root) {
    if (root == NULL) return;

    cout << root->data << ", " ;
    printPreorder(root->left);
    printPreorder(root->right);
  }

  Node* BuildTreeUsingInorderAndPostorder(const vector<int>& in, const vector<int>& post, const int is, const int ie, int* pi) {
    const bool isInputValid = !in.empty() && !post.empty() && in.size() == post.size() &&
                              is >=0 && is < in.size() && ie >= 0 && ie < in.size() && is <= ie &&
                              pi != NULL && (*pi) >= 0 && (*pi) < post.size();

    if (!isInputValid) return NULL;

    Node* root(new Node(post[(*pi)]));
    const int rootIndex = BinarySearch(in, is, ie, root->data);

    if (rootIndex == -1) return NULL;
    (*pi)--;
    root->right = BuildTreeUsingInorderAndPostorder(in, post, rootIndex + 1, ie, pi);
    root->left = BuildTreeUsingInorderAndPostorder(in, post, is, rootIndex - 1, pi);
    return root;
  }

  Node* BuildTreeUsingPostorderUtil(const vector<int>& post, int& pi, const int min, const int max) {
    if (post.empty() || pi < 0 || pi >= post.size()) return NULL;
    const int val = post[pi];
    const bool isInRange = val > min && val <= max;

    if (!isInRange) return NULL;

    Node* root = new Node(val);
    pi--;
    root->right = BuildTreeUsingPostorderUtil(post, pi, val, max);
    root->left = BuildTreeUsingPostorderUtil(post, pi, min, val);
    return root;
  }

  Node* BuildTreeUsingPostorder(const vector<int>& post) {
    if (post.empty()) return NULL;

    int pi = post.size() - 1;
    Node* root = BuildTreeUsingPostorderUtil(post, pi, INT_MIN, INT_MAX);
    return root;
  }

  Node* BuildTreeUsingPreorderUtil(const vector<int>& pre, int& pi, const int min, const int max) {
    if (pre.empty() || pi < 0 || pi >= pre.size()) return NULL;

    const int val = pre[pi];
    Node* root = new Node(val);
    const bool isInRange = val > min && val <= max;

    if (!isInRange) return NULL;

    pi++;
    root->left = BuildTreeUsingPreorderUtil(pre, pi, min, val);
    root->right = BuildTreeUsingPreorderUtil(pre, pi, val, max);
  }

  Node* BuildTreeUsingPreorder(const vector<int>& pre) {
    if (pre.empty()) return NULL;

    int pi = 0;
    Node* root = BuildTreeUsingPreorderUtil(pre, pi, INT_MIN, INT_MAX);
    return root;
  }
};

int main(int argc, char** argv) {
  vector<int> in  =  {3, 5, 6, 9, 11, 13, 14};
  vector<int> pre =  {9, 5, 3, 6, 13, 11, 14};
  vector<int> post = {3, 6, 5, 11, 14, 13, 9};

  Tree bTree;
  int pi = 0;

  Node* root = bTree.BuildTreeUsingInorderAndPreorder(in, pre, 0, in.size()-1, &pi);
  bTree.printPreorder(root);

  cout << endl;

  pi = post.size() - 1;
  root = bTree.BuildTreeUsingInorderAndPostorder(in, post, 0, in.size()-1, &pi);
  bTree.printPreorder(root);

  cout << endl;
  root = bTree.BuildTreeUsingPostorder(post);
  bTree.printPreorder(root);

  cout << endl;
  root = bTree.BuildTreeUsingPreorder(pre);
  bTree.printPreorder(root);

}
