#include "performance.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>

#include "GC/GC.hpp"


using GC::Ref;

void time_native(const std::vector<int>& values)
{
    std::vector<std::unique_ptr<int>> result;
    result.reserve(values.size());
    
    for (auto& value : values)
    {
        result.emplace_back(std::make_unique<int>(value));
    }
}

void time_gc(const std::vector<int>& values)
{
    std::vector<Ref<int>> result;
    result.reserve(values.size());
    
    for (auto& value : values)
    {
        result.emplace_back(Ref<int>(value));
    }
}

void time_shared(const std::vector<int>& values)
{
    std::vector<std::shared_ptr<int>> result;
    result.reserve(values.size());
    
    for (auto& value : values)
    {
        result.emplace_back(std::make_shared<int>(value));
    }
}

void time()
{
    std::mt19937 engine;
    std::chrono::high_resolution_clock clock;
    std::vector<long double> native_results;
    std::vector<long double> gc_results;
    std::vector<long double> shared_results;
    
    for (int test_num { 1 }; test_num <= 100; ++test_num)
    {
        std::vector<int> values ( 100000 );
        engine.seed(test_num);
        std::generate(std::begin(values), std::end(values), [&engine](){return engine(); });
        
        std::cout << "Test #" << test_num << " ";
        
        auto native_start = clock.now();
        time_native(values);
        auto native_stop = clock.now();
        std::chrono::duration<double> native_diff = native_stop - native_start;
        native_results.emplace_back(native_diff.count());
        std::cout << "Native time: " << native_diff.count() << " ";
        
        auto gc_start = clock.now();
        time_gc(values);
        GC::collect();
        auto gc_stop = clock.now();
        std::chrono::duration<double> gc_diff = gc_stop - gc_start;
        gc_results.emplace_back(gc_diff.count());
        std::cout << "GC time: " << gc_diff.count() << " ";
        
        auto shared_start = clock.now();
        time_shared(values);
        auto shared_stop = clock.now();
        std::chrono::duration<double> shared_diff = shared_stop - shared_start;
        shared_results.emplace_back(shared_diff.count());
        std::cout << "Shared time: " << shared_diff.count() << std::endl;
    }
    
    double native_avg = std::accumulate(std::begin(native_results), std::end(native_results), 0.0L) / native_results.size();
    double gc_avg = std::accumulate(std::begin(gc_results), std::end(gc_results), 0.0L) / gc_results.size();
    double shared_avg = std::accumulate(std::begin(shared_results), std::end(shared_results), 0.0L) / shared_results.size();
    std::cout << "Average:" <<
        " Native: " << native_avg <<
        " GC: " << gc_avg <<
        " Shared: " << shared_avg <<
        std::endl;
}
