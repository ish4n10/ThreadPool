#include "ThreadPool.h"
#include <thread>
#include <iostream>


int main() {
    ThreadPool pool(4);
    std::vector<std::future<int>> results;

    for (int i = 0; i < 8; ++i)
    {
        auto future = pool.enqueue([i] {
            return i + i;
            });
        results.emplace_back(std::move(future));
    }

    for (auto& result : results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
}