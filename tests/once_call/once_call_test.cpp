//
// Created by uyplayer on 2025/8/25.
//
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <cxxlazy/components/once_call.h>

using namespace components;

void test_once_call()
{
    OnceCall once;
    int counter = 0;

    auto init_fn = [&]
    {
        counter++;
        std::cout << "初始化函数被执行, counter=" << counter << "\n";
    };

    // 多次调用，只执行一次
    once.call(init_fn);
    once.call(init_fn);
    assert(counter == 1);

    std::cout << "[OK] OnceCall 只执行一次测试通过\n";
}

void test_once_cell_single_thread()
{
    OnceCell<int> cell;

    int& v1 = cell.get_or_init([] { return 42; });
    int& v2 = cell.get_or_init([] { return 123; });

    assert(v1 == 42 && v2 == 42);
    assert(cell.is_initialized());
    assert(*cell == 42);

    std::cout << "[OK] OnceCell 单线程测试通过，值=" << *cell << "\n";
}

void test_once_cell_multi_thread()
{
    OnceCell<int> cell;
    std::vector<std::thread> threads;

    for (int i = 0; i < 8; ++i)
    {
        threads.emplace_back([&]
        {
            int& v = cell.get_or_init([]
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                std::cout << "初始化函数被执行\n";
                return 99;
            });
            assert(v == 99);
        });
    }

    for (auto& t : threads)
    {
        t.join();
    }

    assert(*cell == 99);
    std::cout << "[OK] OnceCell 多线程测试通过，值=" << *cell << "\n";
}

void test_once_cell_reset()
{
    OnceCell<int> cell;

    int& v1 = cell.get_or_init([] { return 77; });
    assert(v1 == 77);

    cell.reset();
    assert(!cell.is_initialized());

    int& v2 = cell.get_or_init([] { return 88; });
    assert(v2 == 88);

    std::cout << "[OK] OnceCell 重置功能测试通过，值=" << *cell << "\n";
}

int main()
{
    test_once_call();
    test_once_cell_single_thread();
    test_once_cell_multi_thread();
    test_once_cell_reset();

    std::cout << "所有测试全部通过！\n";
    return 0;
}
