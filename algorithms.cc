#include <iostream>
#include <random>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <utility>
#include <vector>


using namespace std;


//Insertion Sort

void InsertionSort(vector<int>& A) {
    for (size_t i = 1; i < A.size(); i++) {
        int key = A[i];  // value to insert
        int j = (int) i - 1;
        
        //shift larger value to the right
        while (j >= 0  && A[j] > key) {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = key; //insert key back in
    }
}

//Stable Count Sort right to left
vector<int> CountingSort(const vector<int>& A, int k){
    vector<int> count(k + 1, 0); // count array, length k + 1
    vector<int> output(A.size()); // output array

    // count every value
    for (int x : A) count[x]++;
    //cumulative count
    for(int i = 1; i <= k; i++){
        count[i] += count[i-1];
    }
    // place items right to left
    for (int i = (int)A.size() -1; i >=0; i--){
        output[--count[A[i]]] = A[i];
    }
    return output; //sorted array
}

//Randomized QuickSort
//Random index betweeen low and high
static mt19937 rng(random_device{}());

int Random(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);

}
//Rearrange around pivot
//get pivot final index
int Pivot(vector<int>& A, int lo, int hi) {
    int pivotIndex = Random(lo, hi); //random pivot
    swap(A[pivotIndex], A[hi]); //move to end
    int pivot = A[hi]; //assign pivot

    //left of the pivot smaller of pivot
    int small = lo - 1; // last small section index
    for (int j = lo; j < hi; j++) {
        if (A[j] <= pivot) { //also handles repeating with =
            small ++;
            swap(A[small], A[j]);
        }
    }

    // Place pivot after smaller than section
    swap(A[small+1], A[hi]);
    return small + 1; //final index of pivot
}

// Sorting recursivly helper
void QuickSortHelper(vector<int>& A, int lo, int hi) {
    if (lo >= hi){
        return; // 0 or 1 items base case
    }
    
    int p = Pivot(A, lo, hi); //call to the pivot function
    QuickSortHelper(A, lo, p - 1); //Sort left of pivot
    QuickSortHelper(A, p + 1, hi); // Sort right of pivot
}

//Quicksort
void QuickSort(vector<int>& A) {
    if (A.size() > 1) QuickSortHelper(A, 0, (int)A.size() -1); // sort whole array with helper 0 to last index
}

void PrintVector(const vector<int>& A) {
    for (int x : A) cout << x << " ";
    cout << endl;
}

void Test_Me_InsertionSort(vector<int> A) { //Passing copy
    vector<int> expected = A;
    sort(expected.begin(), expected.end()); // llibrary sort, for check

    InsertionSort(A); //Call to function
    cout << "InsertionSort: ";
    PrintVector(A); //call to helper function
    assert(A == expected);
}

void Test_Me_QuickSort(vector<int> A) { //Passing copy
    vector<int> expected = A;
    sort(expected.begin(), expected.end());

    QuickSort(A); // Call to function
    cout << "Quicksort: ";
    PrintVector(A);
    assert(A == expected);
}

void Test_Me_CountingSort(vector<int> A) { //Passing Copy
    vector<int> expected = A;
    sort(expected.begin(), expected.end());

    int k = *max_element(A.begin(), A.end()); // make k the largest value
    vector<int> result = CountingSort(A,k);
    cout<<"CountingSort: "; //Outputting
    PrintVector(result); // Call to function passed result
    assert(result == expected);
}

//Test Stabiliy in counting sort
void Test_CountSort_Stability() {
    // key then tag
    vector<pair<int, char>> A = {{2,'a'}, {1,'b'}, {2,'c'}, {1,'d'}};
    int k = 2;

    vector<int> count(k + 1, 0);
    vector<pair<int, char>> output(A.size());

    for (auto& p : A) count[p.first]++;                  // count keys
    for (int i = 1; i <= k; i++) count[i] += count[i-1]; // cumulative
    for (int i = (int)A.size() - 1; i >= 0; i--) {       // right to left
        output[--count[A[i].first]] = A[i];
    }

    cout << "Stability test: ";
    for (auto& p : output) cout << "(" << p.first << "," << p.second << ") ";
    cout << endl;

    vector<pair<int, char>> expected = {{1,'b'}, {1,'d'}, {2,'a'}, {2,'c'}};
    assert(output == expected);
}

//Driver, make random ints [0,k]
vector<int> RandomArrays(int n, int k) {
    uniform_int_distribution<int> dist(0,k);
    vector<int> A(n);
    for (int i = 0; i < n; i++) A[i] = dist(rng);
    return A;
}

// Median of 5 (or any number of) times
double Median(vector<double> times) {
    sort(times.begin(), times.end());
    return times[times.size() / 2];
}

// Milliseconds between two timer readings
double Times(chrono::steady_clock::time_point start,
                 chrono::steady_clock::time_point end) {
    return chrono::duration<double, milli>(end - start).count();
}

//The experiment, 5 trails, medians go in file
void RunExperiment(int n, int k, ofstream& out) {
    vector<double> insertTime, quickSortTime, countSortTime; //Times for algorithms
     for (int trial = 0; trial < 5; trial++) {
        // 1. Fresh random input + library-sorted answer key
        vector<int> original = RandomArrays(n, k);
        vector<int> expected = original;
        sort(expected.begin(), expected.end());

        // 2. Three identical copies, made BEFORE timing
        vector<int> copy1 = original;
        vector<int> copy2 = original;
        vector<int> copy3 = original;

        // 3. Time each sort on its own copy
        auto start = chrono::steady_clock::now();
        InsertionSort(copy1);
        auto end = chrono::steady_clock::now();
        insertTime.push_back(Times(start, end));

        start = chrono::steady_clock::now();
        QuickSort(copy2);
        end = chrono::steady_clock::now();
        quickSortTime.push_back(Times(start, end));

        start = chrono::steady_clock::now();
        vector<int> countResult = CountingSort(copy3, k);
        end = chrono::steady_clock::now();
        countSortTime.push_back(Times(start, end));

        // 4. Check results AFTER the timers stopped
        assert(copy1 == expected);
        assert(copy2 == expected);
        assert(countResult == expected);
    }

    // 5. Median of the 5 trials
    double ins = Median(insertTime);
    double quick = Median(quickSortTime);
    double count = Median(countSortTime);

    out << n << "," << k << "," << ins << "," << quick << "," << count << endl;
    cout << "n=" << n << " k=" << k << " done" << endl;
}

int main() {
   //Testing Samples
    vector<int> sample = {5, 2,9, 1, 5, 6, 0, 2};
    Test_Me_InsertionSort(sample); //Call passing test
    Test_Me_QuickSort(sample); //Call
    Test_Me_CountingSort(sample); //Call
    Test_CountSort_Stability(); //Call
    cout<<"All test passed" << endl;

    //Running Experiment
    ofstream out("results.csv"); //open file later using for ploting python
    out << "n,k,insertion_ms,quicksort_ms,counting_ms" << endl;

    int sizes[] = {10, 100, 1000, 2000, 5000, 10000, 20000}; //inputsizes and k from assined
    for (int n : sizes) {
        RunExperiment(n, n, out); // if k = n
        RunExperiment(n, 100 * n, out); // if k = 100n
    }

    out.close(); //close file
    cout << "Finished experiment testing for ploting" << endl;

    return 0;
}


