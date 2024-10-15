////////////////////////////////
//BLG335E-Analysis of algorithms homework 2
//Student: Emil Huseynov
//ID: 150210906
//Date: 14.12.2023
/////////////////////////////

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

// Define a structure to hold a pair of string and int
struct MyPair {
    string first;
    int second;

    MyPair(string f, int s) : first(f), second(s) {}// Constructor to initialize MyPair
};

// Swap function for MyPair
void my_swap(MyPair& a, MyPair& b) {
    MyPair temp = a;
    a = b;
    b = temp;
}

// Increases the value of an element in the heap and adjusts the heap to maintain the heap property.
void heap_increase_key(vector<MyPair>& arr, int i, int new_val) {
    if (new_val < arr[i].second) {
        cout << "New value is smaller than current value\n";
        return;
    }
    arr[i].second = new_val;
    // Adjusts the heap upwards as needed
    while (i > 0 && arr[(i - 1) / 2].second < arr[i].second) {
        my_swap(arr[i], arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Ensures the max-heap property of a subtree with node i at the root.
void max_heapify(vector<MyPair>& arr, int n, int i) {
    int largest = i; // Initialize largest as root
    int left = 2 * i + 1; // left child index
    int right = 2 * i + 2; // right child index

    // Compare root with its left and right child and find largest value
    if (left < n && arr[left].second > arr[largest].second)
        largest = left;
    if (right < n && arr[right].second > arr[largest].second)
        largest = right;

    // Swap and continue heapifying if root is not largest
    if (largest != i) {
        my_swap(arr[i], arr[largest]); // Swap the root with the largest

        // Recursively heapify the affected sub-tree
        max_heapify(arr, n, largest);
    }
}

// Builds a max-heap from an unordered vector.
void build_max_heap(vector<MyPair>& arr) {
    int n = arr.size();

    // Start from the last non-leaf node and move upwards in the tree
    for (int i = n / 2 - 1; i >= 0; i--) {
        max_heapify(arr, n, i);
    }
}

// Implements the heap sort algorithm to sort a vector in ascending order.
void heapsort(vector<MyPair>& arr) {
    build_max_heap(arr);
    int n = arr.size();

    // Extract elements from the heap one by one
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end (swap the first and last element)
        my_swap(arr[0], arr[i]);

        // Call max_heapify on the reduced heap
        max_heapify(arr, i, 0);
    }
}

// Inserts a new key into the max-heap.
void max_heap_insert(vector<MyPair>& arr, int key) {
    arr.emplace_back("Placeholder", key); // Add a new key at the end
    heap_increase_key(arr, arr.size() - 1, key); // Adjust position of new key
}

// Extracts the maximum element from the max-heap and maintains the max-heap property.
int heap_extract_max(vector<MyPair>& arr) {
    if (arr.size() < 1) {
        cout << "Heap underflow\n";
        return -2147483648; // Minimum integer value, indicating an error
    }
    int max = arr[0].second;
    arr[0] = arr.back();
    arr.pop_back();
    max_heapify(arr, arr.size(), 0);
    return max;
}

// Returns the maximum element from the max-heap.
int heap_maximum(const vector<MyPair>& arr) {
    if (arr.empty()) {
        cout << "Heap is empty\n";
        return -2147483648; // Minimum integer value, indicating an error
    }
    return arr[0].second;
}


// Function to calculate the height of a d-ary heap
int log_d(int n, int d) { // Calculates the logarithmic value of n with base d.
    int logValue = 0;
    while (n /= d) logValue++;
    return logValue;
}


// Ensures the max-heap property of a d-ary heap for a subtree with node idx at the root.
void max_heapify_down(vector<MyPair>& heap, int idx, int n, int d) {
    int largest = idx;
    
    for (int i = 0; i < d; ++i) {
        int child_idx = d * idx + i + 1;
        if (child_idx < n && heap[child_idx].second > heap[largest].second) {
            largest = child_idx;
        }
    }

    if (largest != idx) {
        my_swap(heap[idx], heap[largest]);
        max_heapify_down(heap, largest, n, d);
    }
}


// Adjusts the heap upwards as needed for a d-ary heap.
void heapify_up(vector<MyPair>& heap, int idx, int d) {
    while (idx > 0 && heap[idx].second > heap[(idx - 1) / d].second) {
        my_swap(heap[idx], heap[(idx - 1) / d]);
        idx = (idx - 1) / d; // Move to the parent node
    }
}


// Calculates the height of a d-ary heap for a given number of elements (n) and degree (d).
int dary_calculate_height(int n, int d) {
    return log_d(n * (d - 1) + 1, d) - 1;
}

// Extracts the maximum element from a d-ary max-heap and maintains the heap property.
int dary_extract_max(vector<MyPair>& heap, int d) {
    if (heap.empty()) {
        return -1; // Heap underflow
    }
    int max = heap[0].second; // The root of the heap is the maximum element
    heap[0] = heap.back(); // Move the last element to the root
    heap.pop_back(); // Remove the last element
    max_heapify_down(heap, 0, heap.size(), d); // Restore the heap property
    return max;
}

// Inserts a new element into a d-ary max-heap.
void dary_insert_element(vector<MyPair>& heap, int key, int d) {
    heap.emplace_back("Placeholder", key); // Add the new key at the end
    heapify_up(heap, heap.size() - 1, d); // Adjust the heap upwards as necessary
}

// Increases the value of an element in a d-ary max-heap and adjusts the heap to maintain the heap property.
void dary_increase_key(vector<MyPair>& heap, int i, int k, int d) {
    if (k < heap[i].second) {
        return; // New key is smaller than current key
    }
    heap[i].second = k; // Update the key value
    heapify_up(heap, i, d); // Adjust the heap upwards as necessary
}


// Function to split a string using a delimiter
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    //start of input data
    if (argc < 4) {
        cout << "Usage: ./Heapsort <input_file> <operation> <output_file> [optional_arguments]\n";
        return 1;
    }

    string inputFile = argv[1];
    string operation = argv[2];
    string outputFile = argv[3];
    int d = 2; // Default degree for d-ary heap
    int i = -1; // Default index for heap_increase_key
    int k = -1; // Default key value for heap_increase_key

    for (int j = 4; j < argc; ++j) {
        string arg = argv[j];
        if (arg[0] == 'd') {
            d = std::stoi(arg.substr(1));
        } else if (arg[0] == 'i') {
            i = std::stoi(arg.substr(1)) - 1; // Convert to 0-based index
        } else if (arg[0] == 'k') {
            k = std::stoi(arg.substr(1));
        }
    }
    //end of input data


    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        std::cout << "Error opening file: " << inputFile << "\n";
        return 1;
    }

    vector<MyPair> data;
    string line;

    while (getline(inFile, line)) {
        vector<string> parts = split(line, ';');
        if (parts.size() == 2) {
            try {
                int num = std::stoi(parts[1]);
                data.emplace_back(parts[0], num);
            } catch (const std::invalid_argument& e) {
                cerr << "Invalid number in line: " << line << "\n";
                return 1;
            } catch (const std::out_of_range& e) {
                cerr << "Number out of range in line: " << line << "\n";
                return 1;
            }
        }
    }
    inFile.close();


    if (operation == "max_heapify"){
        if(i == -1){
            cout << "Invalid key for max_heapify\n";
            return 1;
        }
        max_heapify(data, data.size(), i);

    }else if (operation == "build_max_heap"){
        build_max_heap(data);

    }else if (operation == "heapsort") {
        heapsort(data);

    }else if (operation == "max_heap_insert"){
        if (k < 0) {
            cout << "Invalid key for max_heap_insert\n";
            return 1;
        }
        max_heap_insert(data, k);

    }else if (operation == "heap_extract_max"){
        build_max_heap(data);
        cout << "heap_extract_max = " << heap_extract_max(data) << endl;

    }else if (operation == "heap_increase_key") {
        if (i < 0 || i >= data.size() || k < 0) {
            std::cout << "Invalid index or key for heap_increase_key\n";
            return 1;
        }
        heap_increase_key(data, i, k);

    }else if (operation == "heap_maximum"){
        build_max_heap(data);
        cout << "heap_maximum = " << heap_maximum(data) << endl;

    }else if (operation == "dary_calculate_height") {
        int height = dary_calculate_height(data.size(), d);
        cout << "Heap height: " << height << endl;
        return 0;

    }else if (operation == "dary_extract_max"){
        cout << "dary_extract_max = " << dary_extract_max(data, d) << endl;

    }else if (operation == "dary_insert_element"){
        if (k < 0) {
            cout << "Invalid key for dary_insert_element\n";
            return 1;
        }
        dary_insert_element(data, k, d);

    }else if (operation == "dary_increase_key"){
        if (i < 0 || i >= data.size() || k < 0) {
            cout << "Invalid index or key for dary_increase_key\n";
            return 1;
        }
        dary_increase_key(data, i, k, d);

    }else {
        cout << "Unknown operation\n";
        return 1;
    }

    // Write the output to the specified file
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cout << "Error opening file: " << outputFile << "\n";
        return 1;
    }

    for (const auto& pair : data) {
        outFile << pair.first << ";" << pair.second << "\n";
    }
    outFile.close();

    return 0;
}