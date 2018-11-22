#include <iostream>
#include <vector>

using namespace std;

class AllSubSetsOfSet {
public:
  void Merge(const int element, const vector<vector<int> >& subsets, vector<vector<int> >* mergedSubsetPtr) {
    if (subsets.empty()) {
      mergedSubsetPtr->push_back(vector<int>()); // Empty set
      mergedSubsetPtr->push_back(vector<int>(1, element));
      return;
    }

    const int setSize = subsets.size();

    for(int index = 0; index < setSize; ++index) {
      const vector<int>& currentSubset = subsets[index];

      // Set at current index is accumulation of previous subset and merge of current element with previous subset
      mergedSubsetPtr->push_back(currentSubset);

      if (currentSubset.empty()) {
        mergedSubsetPtr->push_back(vector<int>(1, element));
      } else {
        mergedSubsetPtr->push_back(currentSubset);
        mergedSubsetPtr->back().push_back(element);
      }
    }
  }

  void findUsingDynamicProgramming(const vector<int>& inputSet, vector<vector<int> >* allSubsetsPtr) {
    const bool isInputValid = allSubsetsPtr != NULL;

    if (!isInputValid) return;

    if (inputSet.empty()) {
      allSubsetsPtr->push_back(vector<int>());  //empty set
      return;
    }

    const int setSize = inputSet.size();
    vector<vector<vector<int> > > subsetForGivenSize;
    subsetForGivenSize.resize(setSize);

    // Initialize all subset for size 1
    {
      vector<int> emptySet;
      vector<int> firstElementSet(1, inputSet[0]);
      vector<vector<int> > allSubSetForSizeOne;
      allSubSetForSizeOne.push_back(emptySet);
      allSubSetForSizeOne.push_back(firstElementSet);
      subsetForGivenSize[0] = allSubSetForSizeOne;
    }

    // Now initialize all subset of size > 1 using the subset found using previous subset
    for (int index = 1; index < setSize; ++index) {
      const int currentElement = inputSet[index];
      vector<vector<int> >& previousSubset = subsetForGivenSize[index - 1];
      Merge(currentElement, previousSubset, &subsetForGivenSize[index]);
    }

    *allSubsetsPtr = subsetForGivenSize[setSize - 1];
  }
};
//
// Elements  1      2       3
// Subsets  []     []       []
//          [1]    [1]      [1]
//                 [2]      [2]
//                 [1, 2]   [1, 2]
//                          [3]
//                          [1, 3]
//                          [2, 3]
//                          [1, 2, 3]
//
// n: Number of elements in set
// Time Complexity: O(n^2 * 2^n). Basically we have n^2 solution as 2^n is all possible subsets that we cannot avoid. We need n^2 time to come up with 2^n subsets
// Space complexity: O(2^n). Dynamic programming brought time 2^n time complexity to n^2 by addition of 2^n space.
//
// If we use bit mask to get all combination from 1 to 2^n-1 then space complexity will be O(c) but the max size of set that we can support will be just 64 (by using 64 bit long variable)
//
int main(int argc, char** argv) {
  vector<int> set =  {1, 2};//, 3};
  vector<vector<int> > allSubset;
  AllSubSetsOfSet().findUsingDynamicProgramming(set, &allSubset);

  for(const vector<int>& subset: allSubset) {
    for(int element : subset) {
      cout << element << ", ";
    }

    cout << endl;
  }
}
