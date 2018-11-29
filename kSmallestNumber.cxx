#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class KsmallestNumber {
public:
  /*
   * APPROACH 1: Using first element as a pivot
   */
  int findKthSmallestNumber(const vector<int>& inputArray, const int k) {
    if (inputArray.empty() || k < 0 || k > inputArray.size()) return -1;

    vector<int> auxillaryArray(inputArray.size() - 1);
    int startIndex = 0, endIndex = auxillaryArray.size() - 1;
    const int pivot = inputArray[0];

    for (int index = 1; index < inputArray.size(); ++index) {
      if (inputArray[index] <= pivot) {
        auxillaryArray[startIndex++] = inputArray[index];
      } else {
        auxillaryArray[endIndex--] = inputArray[index];
      }
    }

    const int currentSmallest = startIndex + 1;
    if ( currentSmallest == k) {
      return pivot;
    } else if ( k < currentSmallest) {
      vector<int> leftHalf(auxillaryArray.begin(), auxillaryArray.begin() + startIndex);
      return  findKthSmallestNumber(leftHalf, k);
    } else {
      vector<int> rightHalf(auxillaryArray.begin() + startIndex, auxillaryArray.end());
      return  findKthSmallestNumber(rightHalf, k - currentSmallest);
    }
  }

  void GetLeftMiddleAndRightPartition(const vector<int>& inputArray, const int startIndex, const int endIndex, const int pivot,
                                      vector<int>& leftPartition, vector<int>& middlePartition, vector<int>& rightPartition) {
    const bool isInputValid = !inputArray.empty() && startIndex >= 0 && startIndex < inputArray.size() &&
                              endIndex >= 0 && endIndex < inputArray.size() && startIndex <= endIndex;

    if (!isInputValid) return;

    for(int index = startIndex; index <= endIndex; ++index) {
      if (inputArray[index] > pivot) {
        rightPartition.push_back(inputArray[index]);
      } else if (inputArray[index] < pivot) {
        leftPartition.push_back(inputArray[index]);
      } else {
        middlePartition.push_back(inputArray[index]);
      }
    }
  }

  /*
   * APPROACH 2: Using median of median as a pivot so that we can split the array into two almost equal parts
   */
  int findKthSmallestOptimalSolution(const vector<int>& inputArray, const int startIndex, const int endIndex, const int k) {
    const int inputSize = endIndex - startIndex + 1;
    const bool isInputValid = !inputArray.empty() && startIndex >= 0 && startIndex < inputArray.size() &&
                              endIndex >= 0 && endIndex < inputArray.size() && startIndex <= endIndex &&
                              k >= 1 && k <= inputSize;
    if (!isInputValid) return -1;

    if (inputSize <= 10) {
      vector<int> temp(inputArray.begin() + startIndex, inputArray.begin() + endIndex + 1);
      std::sort(temp.begin(), temp.end());
      return temp[k-1];
    }

    const int numberOfSplits = (inputSize % 5) == 0 ? (inputSize / 5) : (inputSize / 5) + 1;
    vector<int> mediansOfEachSplit(numberOfSplits, -1);
    int currentSplit = 0;

    while(currentSplit < numberOfSplits) {
      const int sIndex = currentSplit * 5;
      const int eIndex = ((sIndex + 5 - 1) <= endIndex) ? sIndex + 5 - 1 : endIndex;
      const int newSize = (eIndex - sIndex + 1);
      const int medianPosition = (newSize / 2) + 1;
      mediansOfEachSplit[currentSplit] = findKthSmallestOptimalSolution(inputArray, sIndex, eIndex, medianPosition);
      ++currentSplit;
    }

    int medianOfMedian = -1;
    {
      const bool isEvenNumberOfSplits = (numberOfSplits % 2) == 0;

      if (isEvenNumberOfSplits) {
        const int firstMedian = findKthSmallestOptimalSolution(mediansOfEachSplit, 0, mediansOfEachSplit.size() - 1, (numberOfSplits/2) + 1 );
        const int secondMedian = findKthSmallestOptimalSolution(mediansOfEachSplit, 0, mediansOfEachSplit.size() - 1, (numberOfSplits/2) + 1 + 1);
        medianOfMedian = (firstMedian + secondMedian) / 2;
      }else {
        medianOfMedian = findKthSmallestOptimalSolution(mediansOfEachSplit, 0, mediansOfEachSplit.size() - 1, (numberOfSplits/2) + 1 );
      }
    }

    vector<int> leftPartition, rightPartition, middleParition;
    GetLeftMiddleAndRightPartition(inputArray, startIndex, endIndex, medianOfMedian, leftPartition, middleParition, rightPartition);

    if (k <= leftPartition.size()) {
      return findKthSmallestOptimalSolution(leftPartition, 0, leftPartition.size() - 1, k);
    } else if (k > (leftPartition.size() + middleParition.size())) {
      return findKthSmallestOptimalSolution(rightPartition, 0, rightPartition.size() - 1, k - (leftPartition.size() + middleParition.size()));
    } else {
      return medianOfMedian;
    }
  }

};

int main(const int argc, char** argv) {
  vector<int> array = {3, 6, 2, 1, 7, 5, 0, -1, -2, 10, 4};
  cout << KsmallestNumber().findKthSmallestNumber(array, 0) << endl;
  cout << KsmallestNumber().findKthSmallestOptimalSolution(array, 0, array.size() - 1, 5) << endl;

  return 0;
}
