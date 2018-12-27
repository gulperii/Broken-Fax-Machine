#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;
const long long int prime = 17;
long long int modulo = 1000000007;
vector<long long int> hashedDictionary;
vector<long long int> hashedText;
vector<string> dictionaryWords;
vector<long long int> memoization;
vector<long long int> powersOfPrime;
string givenText;
long long int numWords;

void computePower(int textLength) {
    powersOfPrime[0] = 1;
    for (int i = 1; i < textLength; i++) {
        powersOfPrime[i] = (powersOfPrime[i - 1] * prime) % modulo;
    }
}


long long int computeHash(string const &s) {
    long long int hashValue = 0;
    hashValue = s[0] - 96;
    for (int i = 1; i < s.length(); i++) {
        hashValue = (hashValue * prime + (s[i] - 96)) % modulo;
    }
    return hashValue;

}

long long int rehash(int index1, int index2) {
    int powerFactor = index1 - index2;
    long long int rehash;
    long long int a2 = (hashedText[index2]) * powersOfPrime[powerFactor];
    a2 = a2 % modulo;
    if (a2 > hashedText[index1]) {
        rehash = (hashedText[index1] - a2 + modulo) % modulo;

    } else {
        rehash = (hashedText[index1] - a2) % modulo;
    }
    return rehash;
}


long long int util(long long int index, vector<long long int> &memoization) {
    long long int result = 0;
    if (index == givenText.length()) {
        result = 1;
        return result;
    }

    if (memoization[index] == -1) {
        for (int i = 0; i < numWords; i++) {

            long long int dictWordLength = dictionaryWords[i].length();
            long long int wordHash = hashedDictionary[i];

            if (index + dictWordLength > givenText.length())
                continue;

            if (index == 0 && wordHash % modulo == hashedText[dictWordLength - 1] % modulo) {
                result += util(index + dictWordLength, memoization);
                result = result % modulo;
            }

            if (index != 0) {
                long long int textRehash = rehash(dictWordLength + index - 1, index - 1);
                textRehash = textRehash % modulo;
                if (textRehash == wordHash) {
                    result += util(index + dictWordLength, memoization);
                    result %= modulo;
                }
            }
        }
        result = result % modulo;
        memoization[index] = result;
    }
    return memoization[index];
}

int main(int argc, char *argv[]) {

    ofstream write(argv[2]);
    ifstream read;
    read.open(argv[1]);
    read >> givenText;
    read >> numWords;
    powersOfPrime.resize(givenText.length());
    computePower(givenText.length());
    for (int i = 0; i < numWords; i++) {
        string temp;
        read >> temp;
        dictionaryWords.push_back(temp);
    }

    for (int i = 0; i < givenText.length(); i++) {
        hashedText.push_back(computeHash(givenText.substr(0, i + 1)));
    }

    for (int i = 0; i < numWords; i++) {
        hashedDictionary.push_back(computeHash(dictionaryWords[i]));
    }
    memoization.resize(givenText.length(), -1);
    write << util(0, memoization);


}