//
// Created by uyplayer on 2025/8/27.
//
#include <cxxlazy/components/lazy.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>

using namespace components;

/**
 * @brief 测试 Lazy 的基本初始化功能。
 *
 * 验证：
 * 1. 构造时，初始化函数不被执行。
 * 2. 首次访问时，初始化函数被执行一次。
 * 3. 多次访问，初始化函数仅执行一次。
 * 4. 获取到的值符合预期。
 */
void test_lazy_initialization() {
    int initialization_count = 0;
    auto initializer = [&]() {
        initialization_count++;
        return 42;
    };

    Lazy<int> lazy_value(initializer);

    assert(initialization_count == 0);
    std::cout << "Value: " << *lazy_value << std::endl;
    assert(initialization_count == 1);
    assert(*lazy_value == 42);

    // 再次访问，不应再次初始化
    std::cout << "Value again: " << *lazy_value << std::endl;
    assert(initialization_count == 1);

    std::cout << "[OK] test_lazy_initialization" << std::endl;
}

/**
 * @brief 测试 Lazy 在多线程环境下的行为。
 *
 * 验证：
 * 1. 即使在多线程并发访问下，初始化函数也只执行一次。
 * 2. 所有线程获取到的值都是一致且正确的。
 */
void test_lazy_multithreaded() {
    int initialization_count = 0;
    auto initializer = [&]() {
        initialization_count++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return 99;
    };

    Lazy<int> lazy_value(initializer);
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&]() {
            assert(*lazy_value == 99);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    assert(initialization_count == 1);
    std::cout << "[OK] test_lazy_multithreaded" << std::endl;
}


int main() {
    test_lazy_initialization();
    test_lazy_multithreaded();
    return 0;
}
