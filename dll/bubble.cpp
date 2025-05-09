#include <iostream>
#include <ctime>
#include <cstdlib>
#include <omp.h>
#include <chrono>
 
using namespace std;
using namespace std::chrono;
 
void swap(int &a, int &b)
{
    int test;
    test = a;
    a = b;
    b = test;
}


void bubbleSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(int *a, int n)
{
    for (int i = 0; i < n; i++)
    {   
        int first = i % 2;  
        #pragma omp parallel for shared(a, first)
        for (int j = first; j < n-1; j += 2)
        {   
            if (a[j] > a[j+1])
            {   
                swap(a[j], a[j+1]);
            }   
        }   
    }
}


void printArray(int arr[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}
 

int main()
{
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;
 
    int *arr = new int[n];
    int *arr_copy = new int[n];
    
    for (int i = 0; i < n; ++i)
    {
        // cout << "Enter element " << i+1 << ": ";
        // cin >> arr[i];
        arr[i] = rand() % 1000;
        arr_copy[i] = arr[i]; // Create a copy for each sorting method
    }
 
    cout << "Original array: ";
    // printArray(arr, n);
 
    // Time measurement variables
    auto start = high_resolution_clock::now();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);   
 
    // Sequential Bubble Sort
    int *arr1 = new int[n];
    for (int i = 0; i < n; i++) arr1[i] = arr_copy[i];
    
    start = high_resolution_clock::now();
    bubbleSort(arr1, n);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "\nSequential Bubble Sorted array: ";
    // printArray(arr1, n);
    cout << "Time taken: " << duration.count() << " microseconds\n";
 
    // Parallel Bubble Sort (Improved)
    int *arr2 = new int[n];
    for (int i = 0; i < n; i++) arr2[i] = arr_copy[i];
    
    start = high_resolution_clock::now();
    parallelBubbleSort(arr2, n);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);
    cout << "\nParallel Bubble Sorted array: ";
    // printArray(arr2, n);
    cout << "Time taken: " << duration.count() << " microseconds\n";
 
    
   
   
    // Clean up memory
    delete[] arr;
    delete[] arr_copy;
    delete[] arr1;
    delete[] arr2;
    
 
    return 0;
}