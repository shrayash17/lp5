#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Merge two sorted subarrays: arr[l..m] and arr[m+1..r]
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = new int[n1];
    int *R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);
            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

// Wrapper to start OpenMP parallelism
void startParallelMergeSort(int arr[], int n) {
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(arr, 0, n - 1);
    }
}

// Sequential Merge Sort (without OpenMP)
void sequentialMergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Utility to print the array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; ++i) cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter array size: ";
    cin >> n;

    int *original = new int[n];
    for (int i = 0; i < n; i++) original[i] = rand() % 1000;

    // Sequential
    int *arr1 = new int[n];
    copy(original, original + n, arr1);
    auto start = high_resolution_clock::now();
    sequentialMergeSort(arr1, 0, n - 1);
    auto end = high_resolution_clock::now();
    cout << "\nSequential Merge Sort:\n";
    // printArray(arr1, n);
    cout << "Time: " << duration_cast<microseconds>(end - start).count() << " microseconds\n";

    // Parallel
    int *arr2 = new int[n];
    copy(original, original + n, arr2);
    start = high_resolution_clock::now();
    startParallelMergeSort(arr2, n);
    end = high_resolution_clock::now();
    cout << "\nParallel Merge Sort:\n";
    // printArray(arr2, n);
    cout << "Time: " << duration_cast<microseconds>(end - start).count() << " microseconds\n";

    delete[] original;
    delete[] arr1;
    delete[] arr2;

    return 0;
}
