#include <iostream>
#include <string>
#include <omp.h>
#include <chrono>

using namespace std;

int* generateIntArray(int size, int minVal, int maxVal) {
    int* arr = new int[size];

    for (int i = 0; i < size; ++i) {
        arr[i] = minVal + rand() % (maxVal - minVal + 1);
    }

    return arr;
}

void printArray(int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int* oddEvenSort(int* arr, int N, int threads_num){
    omp_set_num_threads(threads_num);
    auto start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel shared(arr)
    {
        int thread_id = omp_get_thread_num();
        for (int i = 0; i < N; i++) {
            if (i % 2 == 0) {
                for (int j = thread_id*2; j < N; j += threads_num*2)
                {
                    if(arr[j] > arr[j+1]){
                        int temp = arr[j+1];
                        arr[j+1] = arr[j];
                        arr[j] = temp;
                    }
                }
            } else {
                for (int j = thread_id*2 + 1; j < N; j += threads_num*2)
                {
                    if(arr[j] > arr[j+1]){
                        int temp = arr[j+1];
                        arr[j+1] = arr[j];
                        arr[j] = temp;
                    }
                }
            }
            #pragma omp barrier
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    string str = "Time spent on parallel: " + to_string(duration.count()) + " milliseconds\n";
    cout << str;
    return arr;
}

int* oddEvenSortNonParallel(int* arr, int N){
    for (int i = 0; i < N; i++) {
        for (int pos = 0; pos < N; pos++) {
            if (i % 2 == 0) {
                if (pos % 2 == 0) {
                    if (arr[pos] > arr[pos + 1]) {
                        int temp = arr[pos + 1];
                        arr[pos + 1] = arr[pos];
                        arr[pos] = temp;
                    }
                }
            } else {
                if (pos % 2 == 1) {
                    if (arr[pos] > arr[pos + 1]) {
                        int temp = arr[pos + 1];
                        arr[pos + 1] = arr[pos];
                        arr[pos] = temp;
                    }
                }
            }
        }
    }
    return arr;
}

int main() {
    srand(time(nullptr));

    int N = 200000;
    int* myArray = generateIntArray(N, 1, 100000);
    int* sortedArray = oddEvenSort(myArray,N,5);
//    printArray(sortedArray, N);


    auto start = std::chrono::high_resolution_clock::now();
    sortedArray = oddEvenSortNonParallel(myArray,N);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Time spent on sequential: " << duration.count() << " milliseconds" << endl;
//    printArray(sortedArray, N);

    delete[] myArray;

    return 0;
}
















