#include <iostream>
#include <random>

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
int Random(int lo, int hi) {
    return lo + rand() % (hi - lo + 1);

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

int main() {
    printf("Hello World");
    srand(time(0));
}