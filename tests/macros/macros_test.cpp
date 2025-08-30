//
// Created by uyplayer on 2025/8/27.
//
#include <cxxlazy/components/macros.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <functional>

// 使用 LAZY_STATIC 定义一个静态延迟初始化变量
LAZY_STATIC(int, static_lazy_value, 100);

/**
 * @brief 测试 LAZY_STATIC 宏。
 *
 * 验证：
 * 1. 首次访问时，值被正确初始化。
 * 2. 可以对值进行修改并读取。
 */
void test_lazy_static() {
    assert(*static_lazy_value == 100);
    *static_lazy_value = 200;
    assert(*static_lazy_value == 200);
    std::cout << "[OK] test_lazy_static" << std::endl;
}


// 使用 THREAD_LOCAL_LAZY 定义一个线程局部延迟初始化变量
THREAD_LOCAL_LAZY(size_t, thread_local_lazy_value, 0);

/**
 * @brief 在线程中为线程局部变量赋值。
 */
void thread_local_func() {
    *thread_local_lazy_value = std::hash<std::thread::id>{}(std::this_thread::get_id());
    std::cout << "Thread " << std::this_thread::get_id() << " value: " << *thread_local_lazy_value << std::endl;
    assert(*thread_local_lazy_value != 0);
}

/**
 * @brief 测试 THREAD_LOCAL_LAZY 宏。
 *
 * 验证：
 * 1. 在多个线程中，每个线程都有自己独立的变量实例。
 */
void test_thread_local_lazy() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(thread_local_func);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "[OK] test_thread_local_lazy" << std::endl;
}


// 使用 LAZY_STATIC_VOID 定义一个延迟执行的操作
int side_effect_counter = 0;
LAZY_STATIC_VOID(lazy_void, side_effect_counter++);

/**
 * @brief 测试 LAZY_STATIC_VOID 宏。
 *
 * 验证：
 * 1. 在首次 get() 调用前，操作不执行。
 * 2. 首次 get() 调用时，操作执行一次。
 * 3. 后续 get() 调用，操作不再执行。
 */
void test_lazy_static_void() {
    assert(side_effect_counter == 0);
    lazy_void.get();
    assert(side_effect_counter == 1);
    lazy_void.get();
    assert(side_effect_counter == 1);
    std::cout << "[OK] test_lazy_static_void" << std::endl;
}


int main() {
    test_lazy_static();
    test_thread_local_lazy();
    test_lazy_static_void();
    return 0;
}
