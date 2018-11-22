#include <iostream>
#include <string.h>
#include <vector>
#include <unordered_set>

using namespace std;

class PermuteString {
public:
  void permute(const string& inputString, vector<string>* permutedStringArrayPtr) {
    const int stringLength = inputString.size();
    char rawString[stringLength + 1];
    std::copy(inputString.begin(), inputString.end(), rawString);
    rawString[stringLength] = '\0';
    permute(rawString, 0, stringLength - 1, permutedStringArrayPtr);
  }

  void permute(char* rawString, const int startIndex, const int endIndex, vector<string>* permutedStringArrayPtr) {
      const bool isInputValid = (rawString != NULL) && (permutedStringArrayPtr != NULL) &&
                                startIndex >=0 && startIndex <= endIndex && endIndex >= 0;

      if (!isInputValid) return;

      if (startIndex == endIndex) {
        permutedStringArrayPtr->push_back(string(rawString));
        return;
      }

      std::unordered_set<char> alreadySwappedSet;

      for(int index = startIndex; index <= endIndex; ++index) {
        char& currentChar = rawString[index];
        char& firstChar = rawString[startIndex];

        const bool isSameIndex = (index == startIndex);
        const bool alreadySwapedThisChar = alreadySwappedSet.find(currentChar) != alreadySwappedSet.end();
        const bool canPermute = isSameIndex || (!alreadySwapedThisChar);

        if (canPermute) {
          swap(firstChar, currentChar);
          permute(rawString, startIndex + 1, endIndex, permutedStringArrayPtr);
          swap(firstChar, currentChar);
          alreadySwappedSet.insert(currentChar);
        }
      }
  }
};

int main(const int argc, char** argv) {
  string input = "Farhan";
  vector<string> permuttedStringArray;
  PermuteString().permute(input, &permuttedStringArray);

  for(const string& eachPermutation : permuttedStringArray) {
    cout << eachPermutation << endl;
  }
  return 1;
}
// n = Length(inputString)
// Time Complexity: O(n * n!) 
// Space Complexity: O(n)
