#include <chrono>
#include <cstdio>
#include <iostream>
#include <bitset>
#include <random>
#include <thread>
// #include <sstream>

// this will NOT return a perfect answer most of the time
// if you want an answer better than 98% use simulated annealing

// for proper display of greek characters
#if __linux__
    #define NANO "\xC2\xB5"
#elif _WIN32
    #define NANO '\xE6'
#else
    #define NANO "\xC2\xB5"
#endif

// this much memory will be allocated, the knapsack can be smaller
#define MAX_KNAPSACK_SIZE 32

constexpr int INSTANCE_NUMBER = 8;
constexpr int ITERATION_LIMIT = 8388608 /*2^23*/ / INSTANCE_NUMBER;

int best_value = -1;
int knapsack_bit_size, capacity;
int weights, values [MAX_KNAPSACK_SIZE];
std::bitset<MAX_KNAPSACK_SIZE> solution = {};

std::random_device rd;
std::mt19937 gen( rd());
std::bernoulli_distribution d(.025);  // <--- this changes the starting point of the search 
std::uniform_int_distribution<> u;

template<int size>
typename std::bitset<size> random_bitset() {
    typename std::bitset<size> bits;
    for (int n = knapsack_bit_size; --n;) {
        bits[size-n] =
        // 0; 
        d(gen);
    }
    return bits;
}

void local_search(int (&weights) [MAX_KNAPSACK_SIZE], int (&values) [MAX_KNAPSACK_SIZE], std::uniform_int_distribution<> u) {
    std::bitset<MAX_KNAPSACK_SIZE> local_solution = random_bitset<MAX_KNAPSACK_SIZE>();
    size_t i = 0;
    bool fin = true;
    while (fin) {
        fin = false;
        std::vector<std::bitset<MAX_KNAPSACK_SIZE>> neighbors;
        for (int i = 0; i < knapsack_bit_size; i++) {
            std::bitset<MAX_KNAPSACK_SIZE> neighbor = local_solution;
            neighbors.push_back(neighbor.flip(i));
        }
        for(auto n : neighbors) {
            int sum_of_values = 0;
            int sum_of_weights = 0;
            for (int i = 0; i < knapsack_bit_size; i++) {
                sum_of_values += n[MAX_KNAPSACK_SIZE - i - 1] * values[i];
                sum_of_weights += n[MAX_KNAPSACK_SIZE - i - 1] * weights[i];
            }
            if (sum_of_weights > capacity) continue;
            if (sum_of_values >= best_value) {
                fin = true;
                best_value = sum_of_values;
                local_solution = n;
                solution = local_solution;
            }  
        }
        if (++i == ITERATION_LIMIT) break;
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    // sample data
    int weights[MAX_KNAPSACK_SIZE] = {3, 1, 6, 10, 1, 4, 9, 1, 7, 2, 6, 1, 6, 2, 2, 4, 8, 1, 7, 3, 6, 2, 9, 5, 3, 3, 4, 7, 3, 5, 30, 50};
    int values[MAX_KNAPSACK_SIZE] = {7, 4, 9, 18, 9, 15, 4, 2, 6, 13, 18, 12, 12, 16, 19, 19, 10, 16, 14, 3, 14, 4, 15, 7, 5, 10, 10, 13, 19, 9, 8, 5};
    knapsack_bit_size = 32;
    capacity = 75;
    u = std::uniform_int_distribution<>(0, knapsack_bit_size);
    std::vector<std::thread > thread_pool;
    thread_pool.reserve(INSTANCE_NUMBER);
    
    /* uncomment the below code to show the input data at the start */ 

    // std::ostringstream os;
    // std::string s;
    // for(size_t i = knapsack_bit_size; i--;) os << weights[i] << " ";
    // s = os.str();
    // os.str(std::string());
    // for(size_t i = knapsack_bit_size; i--;) os << values[i] << " ";
    // printf("Input weights:  %s\nInput values:   %s\n\n", s.c_str(), os.str().c_str());

    auto prep = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < INSTANCE_NUMBER; ++i) {
        thread_pool.push_back(std::thread(local_search, std::ref(weights), std::ref(values), u));
    }
    auto thread_init = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < INSTANCE_NUMBER; ++i) { 
        thread_pool[i].join();
    }
    auto threadfin = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    printf(
    "Solution:\t%s\
    \nSum of values:\t%d\
    \n\nProgram took %lld microseconds (%f seconds) to finish\
    \nData init and stream prep: %lld %cs\
    \nThread creation: %lld %cs\
    \nCalculation: %lld %cs\
    \n\nPress Return to exit", 
        solution.to_string().c_str(), best_value, time, (float)time/1000000,
        std::chrono::duration_cast<std::chrono::microseconds>(prep - start).count(), NANO,
        std::chrono::duration_cast<std::chrono::microseconds>(thread_init - prep).count(), NANO,
        std::chrono::duration_cast<std::chrono::microseconds>(threadfin - thread_init).count(), NANO
    );
    std::cin.get();
}
