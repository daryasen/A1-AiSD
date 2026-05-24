#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <locale>

using namespace std;

long long charComparisons = 0;

const int MAX_SIZE = 3000;
const int STEP = 100;
const int MIN_STRING_LENGTH = 10;
const int MAX_STRING_LENGTH = 200;
const int R = 256;

int charAt(const string& s, int d) {
    if (d >= (int)s.size()) return -1;
    return (unsigned char)s[d];
}

bool compareStrings(const string& a, const string& b) {
    int n = min(a.size(), b.size());

    for (int i = 0; i < n; i++) {
        charComparisons++;

        if (a[i] < b[i]) return true;
        if (a[i] > b[i]) return false;
    }

    charComparisons++;
    return a.size() < b.size();
}

int compareStringsFromPosition(const string& a, const string& b, int startPosition) {
    int i = startPosition;
    int n = min(a.size(), b.size());

    while (i < n) {
        charComparisons++;

        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;

        i++;
    }

    charComparisons++;

    if (a.size() < b.size()) return -1;
    if (a.size() > b.size()) return 1;
    return 0;
}

int longestCommonPrefixLength(const string& a, const string& b, int startPosition) {
    int i = startPosition;
    int n = min(a.size(), b.size());

    while (i < n) {
        charComparisons++;

        if (a[i] != b[i]) break;
        i++;
    }

    return i;
}

class StringGenerator {
private:
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                      "abcdefghijklmnopqrstuvwxyz"
                      "0123456789"
                      "!@#%:;^&*()-";

    mt19937 rng;

public:
    StringGenerator() {
        rng.seed(random_device{}());
    }

    string generateString() {
        uniform_int_distribution<int> lengthDist(MIN_STRING_LENGTH, MAX_STRING_LENGTH);
        uniform_int_distribution<int> charDist(0, (int)alphabet.size() - 1);

        int length = lengthDist(rng);
        string s;
        s.reserve(length);

        for (int i = 0; i < length; i++) {
            s += alphabet[charDist(rng)];
        }

        return s;
    }

    vector<string> generateRandomArray(int size) {
        vector<string> arr;
        arr.reserve(size);

        for (int i = 0; i < size; i++) {
            arr.push_back(generateString());
        }

        return arr;
    }

    vector<string> generateReverseSortedArray(int size) {
        vector<string> arr = generateRandomArray(size);
        sort(arr.begin(), arr.end());
        reverse(arr.begin(), arr.end());
        return arr;
    }

    vector<string> generateNearlySortedArray(int size) {
        vector<string> arr = generateRandomArray(size);
        sort(arr.begin(), arr.end());

        int swapsCount = max(1, size / 20);
        uniform_int_distribution<int> indexDist(0, size - 1);

        for (int i = 0; i < swapsCount; i++) {
            int firstIndex = indexDist(rng);
            int secondIndex = indexDist(rng);
            swap(arr[firstIndex], arr[secondIndex]);
        }

        return arr;
    }

    vector<string> generatePrefixArray(int size, const string& prefix) {
        vector<string> arr;
        arr.reserve(size);

        for (int i = 0; i < size; i++) {
            arr.push_back(prefix + generateString());
        }

        return arr;
    }
};

vector<string> getSubArray(const vector<string>& source, int size) {
    return vector<string>(source.begin(), source.begin() + size);
}

int partition(vector<string>& arr, int low, int high) {
    string pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compareStrings(arr[j], pivot)) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<string>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

void merge(vector<string>& arr, int left, int mid, int right) {
    vector<string> leftPart(arr.begin() + left, arr.begin() + mid + 1);
    vector<string> rightPart(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0;
    int j = 0;
    int k = left;

    while (i < (int)leftPart.size() && j < (int)rightPart.size()) {
        if (compareStrings(leftPart[i], rightPart[j])) {
            arr[k++] = leftPart[i++];
        } else {
            arr[k++] = rightPart[j++];
        }
    }

    while (i < (int)leftPart.size()) arr[k++] = leftPart[i++];
    while (j < (int)rightPart.size()) arr[k++] = rightPart[j++];
}

void mergeSort(vector<string>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void stringMergeWithLCP(vector<string>& arr, int left, int mid, int right, int knownPrefixLength) {
    vector<string> temp;
    temp.reserve(right - left + 1);

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        int lcp = longestCommonPrefixLength(arr[i], arr[j], knownPrefixLength);
        int cmp = compareStringsFromPosition(arr[i], arr[j], lcp);

        if (cmp <= 0) {
            temp.push_back(arr[i]);
            i++;
        } else {
            temp.push_back(arr[j]);
            j++;
        }
    }

    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);

    for (int k = 0; k < (int)temp.size(); k++) {
        arr[left + k] = temp[k];
    }
}

void stringMergeSortWithLCP(vector<string>& arr, int left, int right, int knownPrefixLength) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    stringMergeSortWithLCP(arr, left, mid, knownPrefixLength);
    stringMergeSortWithLCP(arr, mid + 1, right, knownPrefixLength);
    stringMergeWithLCP(arr, left, mid, right, knownPrefixLength);
}

void ternaryQuickSort(vector<string>& arr, int low, int high, int d) {
    if (low >= high) return;

    int lt = low;
    int gt = high;
    int pivot = charAt(arr[low], d);
    int i = low + 1;

    while (i <= gt) {
        int current = charAt(arr[i], d);
        charComparisons++;

        if (current < pivot) {
            swap(arr[lt], arr[i]);
            lt++;
            i++;
        } else if (current > pivot) {
            swap(arr[i], arr[gt]);
            gt--;
        } else {
            i++;
        }
    }

    ternaryQuickSort(arr, low, lt - 1, d);

    if (pivot >= 0) {
        ternaryQuickSort(arr, lt, gt, d + 1);
    }

    ternaryQuickSort(arr, gt + 1, high, d);
}

void msdRadixSort(vector<string>& arr, int low, int high, int d) {
    if (low >= high) return;

    vector<int> count(R + 2, 0);

    for (int i = low; i <= high; i++) {
        int c = charAt(arr[i], d);
        charComparisons++;
        count[c + 2]++;
    }

    for (int r = 0; r < R + 1; r++) {
        count[r + 1] += count[r];
    }

    vector<int> start = count;
    vector<string> aux(high - low + 1);

    for (int i = low; i <= high; i++) {
        int c = charAt(arr[i], d);
        charComparisons++;
        aux[count[c + 1]++] = arr[i];
    }

    for (int i = low; i <= high; i++) {
        arr[i] = aux[i - low];
    }

    for (int r = 0; r < R; r++) {
        msdRadixSort(arr, low + start[r], low + start[r + 1] - 1, d + 1);
    }
}

void msdRadixSortWithSwitch(vector<string>& arr, int low, int high, int d) {
    if (low >= high) return;

    if (high - low + 1 < R) {
        ternaryQuickSort(arr, low, high, d);
        return;
    }

    vector<int> count(R + 2, 0);

    for (int i = low; i <= high; i++) {
        int c = charAt(arr[i], d);
        charComparisons++;
        count[c + 2]++;
    }

    for (int r = 0; r < R + 1; r++) {
        count[r + 1] += count[r];
    }

    vector<int> start = count;
    vector<string> aux(high - low + 1);

    for (int i = low; i <= high; i++) {
        int c = charAt(arr[i], d);
        charComparisons++;
        aux[count[c + 1]++] = arr[i];
    }

    for (int i = low; i <= high; i++) {
        arr[i] = aux[i - low];
    }

    for (int r = 0; r < R; r++) {
        msdRadixSortWithSwitch(arr, low + start[r], low + start[r + 1] - 1, d + 1);
    }
}

struct TestResult {
    double timeMs;
    long long comparisons;
};

class StringSortTester {
private:
    const int RUNS = 5;

public:
    TestResult testAlgorithm(const vector<string>& original, void (*sortFunc)(vector<string>&)) {
        double totalTime = 0.0;
        long long totalComparisons = 0;

        for (int i = 0; i < RUNS; i++) {
            vector<string> arr = original;
            charComparisons = 0;

            auto start = chrono::high_resolution_clock::now();
            sortFunc(arr);
            auto end = chrono::high_resolution_clock::now();

            double timeMs = chrono::duration<double, milli>(end - start).count();

            totalTime += timeMs;
            totalComparisons += charComparisons;

            if (!is_sorted(arr.begin(), arr.end())) {
                cerr << "Sorting error detected" << endl;
            }
        }

        return {totalTime / RUNS, totalComparisons / RUNS};
    }
};

void runQuickSort(vector<string>& arr) {
    if (!arr.empty()) {
        quickSort(arr, 0, (int)arr.size() - 1);
    }
}

void runMergeSort(vector<string>& arr) {
    if (!arr.empty()) {
        mergeSort(arr, 0, (int)arr.size() - 1);
    }
}

void runStringMergeSortWithLCP(vector<string>& arr) {
    if (!arr.empty()) {
        stringMergeSortWithLCP(arr, 0, (int)arr.size() - 1, 0);
    }
}

void runTernaryQuickSort(vector<string>& arr) {
    if (!arr.empty()) {
        ternaryQuickSort(arr, 0, (int)arr.size() - 1, 0);
    }
}

void runMSDRadixSort(vector<string>& arr) {
    if (!arr.empty()) {
        msdRadixSort(arr, 0, (int)arr.size() - 1, 0);
    }
}

void runMSDRadixSortWithSwitch(vector<string>& arr) {
    if (!arr.empty()) {
        msdRadixSortWithSwitch(arr, 0, (int)arr.size() - 1, 0);
    }
}

int main() {
    locale::global(locale("C"));

    ofstream file("all_results.csv");
    file.imbue(locale("C"));
    file << fixed << setprecision(6);

    StringGenerator generator;
    StringSortTester tester;

    vector<string> randomMax = generator.generateRandomArray(MAX_SIZE);

    vector<string> reverseMax = randomMax;
    sort(reverseMax.begin(), reverseMax.end());
    reverse(reverseMax.begin(), reverseMax.end());

    vector<string> nearlyMax = randomMax;
    sort(nearlyMax.begin(), nearlyMax.end());
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> indexDist(0, MAX_SIZE - 1);
    int swapsCount = MAX_SIZE / 20;

    for (int i = 0; i < swapsCount; i++) {
        int firstIndex = indexDist(rng);
        int secondIndex = indexDist(rng);
        swap(nearlyMax[firstIndex], nearlyMax[secondIndex]);
    }

    file << "ArrayType;"
         << "Size;"
         << "QuickTime;"
         << "QuickComparisons;"
         << "MergeTime;"
         << "MergeComparisons;"
         << "StringMergeLCPTime;"
         << "StringMergeLCPComparisons;"
         << "TernaryQuickTime;"
         << "TernaryQuickComparisons;"
         << "MSDTime;"
         << "MSDComparisons;"
         << "MSDSwitchTime;"
         << "MSDSwitchComparisons\n";

    for (int size = STEP; size <= MAX_SIZE; size += STEP) {
        vector<pair<string, vector<string>>> testArrays = {
            {"Random", getSubArray(randomMax, size)},
            {"Reverse", getSubArray(reverseMax, size)},
            {"Nearly", getSubArray(nearlyMax, size)}
        };

        for (const auto& testCase : testArrays) {
            string arrayType = testCase.first;
            vector<string> arr = testCase.second;

            TestResult quickResult = tester.testAlgorithm(arr, runQuickSort);
            TestResult mergeResult = tester.testAlgorithm(arr, runMergeSort);
            TestResult stringMergeLCPResult = tester.testAlgorithm(arr, runStringMergeSortWithLCP);
            TestResult ternaryResult = tester.testAlgorithm(arr, runTernaryQuickSort);
            TestResult msdResult = tester.testAlgorithm(arr, runMSDRadixSort);
            TestResult msdSwitchResult = tester.testAlgorithm(arr, runMSDRadixSortWithSwitch);

            file << arrayType << ";"
                 << size << ";"
                 << quickResult.timeMs << ";"
                 << quickResult.comparisons << ";"
                 << mergeResult.timeMs << ";"
                 << mergeResult.comparisons << ";"
                 << stringMergeLCPResult.timeMs << ";"
                 << stringMergeLCPResult.comparisons << ";"
                 << ternaryResult.timeMs << ";"
                 << ternaryResult.comparisons << ";"
                 << msdResult.timeMs << ";"
                 << msdResult.comparisons << ";"
                 << msdSwitchResult.timeMs << ";"
                 << msdSwitchResult.comparisons << "\n";

            cout << "Completed: " << arrayType << ", size = " << size << endl;
        }
    }

    file.close();

    cout << "Testing completed. Results saved to all_results.csv" << endl;

    return 0;
}
