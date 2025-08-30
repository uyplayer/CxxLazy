//
// Created by uyplayer on 2025/8/25.
//

#pragma once
#include <mutex>
#include <atomic>
#include <optional>
#include <utility>

namespace components
{
    /**
     * @class OnceCall
     * @brief 一个线程安全的工具类，用于确保某个操作（函数）在多线程环境中只被成功执行一次
     * @details
     * 类似于 std::call_once，但提供了更简洁的 API
     * 它利用原子变量和互斥锁来实现双重检查锁定，以保证线程安全和高效性
     * 如果初始化函数抛出异常，它会自动重置状态，允许其他线程再次尝试
     */
    class OnceCall
    {
    public:
        /**
         * @brief 构造一个新的 OnceCall 实例，初始状态为未初始化
         */
        OnceCall();
        /**
         * @brief 默认析构函数
         */
        ~OnceCall();

        /**
         * @brief 尝试执行一个函数，只有第一次成功调用会执行该函数
         * @details
         * 此函数是线程安全的它会检查是否已经有操作被成功执行过
         * 如果没有，它将尝试执行传入的 `fn` 函数
         * 如果 `fn` 抛出异常，状态将回滚到未初始化，允许后续的 `call` 再次尝试
         * @tparam Fn 可调用对象的类型，例如函数指针、lambda 表达式等
         * @param fn 将要被执行的函数，签名为 `void()`
         */
        template <typename Fn>
        void call(Fn&& fn);

        /**
         * @brief 重置状态，允许再次执行一次操作
         * @details 此操作是线程安全的
         */
        void reset() {
            std::lock_guard<std::mutex> lock(mtx_);
            state_.store(State::Uninitialized, std::memory_order_release);
        }
        /**
         * @brief 检查初始化是否已经成功完成
         * @return 如果操作已经成功执行过，返回 `true`；否则返回 `false`
         */
        bool is_initialized() const;

    private:
        /// @brief 表示初始化状态的枚举
        enum class State { Uninitialized, Initializing, Initialized };

        /// @brief 原子地存储当前的状态，用于无锁快速检查
        std::atomic<State> state_{};
        /// @brief 在初始化期间保护数据访问的互斥锁
        mutable std::mutex mtx_;
    };


    /**
     * @class OnceCell
     * @brief 一个线程安全的、只能被赋值一次的容器
     * @details
     * 用于延迟初始化一个对象在多线程环境中，它确保初始化过程只发生一次
     * 第一次调用 `get_or_init` 时会执行初始化函数并存储结果
     * 后续的访问将直接返回已存储的值，无需再次初始化
     * @tparam T 容器中存储的数据类型
     */
    template <typename T>
    class OnceCell
    {
    public:
        /**
         * @brief 构造一个新的、空的 OnceCell 实例
         */
        OnceCell();
        /**
         * @brief 默认析构函数
         */
        ~OnceCell();

        /**
         * @brief 获取或初始化单元中的值
         * @details
         * 如果值已存在，则立即返回其引用
         * 如果值不存在，则调用 `fn` 函数来生成值，并安全地存储它，然后返回其引用
         * 如果 `fn` 在执行期间抛出异常，状态将回滚，允许后续调用再次尝试初始化
         * @tparam Fn 用于生成值的可调用对象的类型，其返回值必须可以转换为 `T`
         * @param fn 初始化函数
         * @return 对单元中值的引用
         */
        template <typename Fn>
        T& get_or_init(Fn&& fn);

        /**
         * @brief 检查单元是否已经被成功初始化
         * @return 如果值已存在，返回 `true`；否则返回 `false`
         */
        bool is_initialized() const;

        /**
         * @brief 获取指向单元中值的可变指针
         * @return 如果值已存在，返回指向该值的指针；否则返回 `nullptr`
         */
        T* get();

        /**
         * @brief 尝试获取指向单元中值的只读指针，此操作不触发初始化
         * @return 如果值已存在，返回指向该值的只读指针；否则返回 `nullptr`
         */
        const T* try_get() const;

        /**
         * @brief 重置单元的状态，清除已存储的值
         * @details
         * 这使得单元可以被重新初始化此操作是线程安全的
         * 主要用于测试或需要动态更新配置的场景
         */
        void reset();

        /**
         * @brief 解引用操作符，提供对内部值的直接访问
         * @warning 在调用此操作符前，请确保值已经被初始化，否则行为未定义
         * @return 对内部值的引用
         */
        T& operator*() { return *value_; }
        /**
         * @brief 只读解引用操作符
         * @warning 在调用此操作符前，请确保值已经被初始化，否则行为未定义
         * @return 对内部值的只读引用
         */
        const T& operator*() const { return *value_; }

        /**
         * @brief 指针访问操作符，允许像指针一样访问内部值的成员
         * @warning 在调用此操作符前，请确保值已经被初始化，否则行为未定义
         * @return 指向内部值的指针
         */
        T* operator->() { return &(*value_); }
        /**
         * @brief 只读指针访问操作符
         * @warning 在调用此操作符前，请确保值已经被初始化，否则行为未定义
         * @return 指向内部值的只读指针
         */
        const T* operator->() const { return &(*value_); }

    private:
        /// @brief 表示初始化状态的枚举
        enum class State { Uninitialized, Initializing, Initialized };

        /// @brief 使用 std::optional 存储值，以处理未初始化的情况
        std::optional<T> value_;
        /// @brief 原子地存储当前的状态
        std::atomic<State> state_;
        /// @brief 用于保护初始化过程的互斥锁
        mutable std::mutex mtx_;
    };


    // ------------------ OnceCall 实现 ------------------
    /**
     * @brief 尝试调用一个函数，如果之前没有函数被调用过，则该函数会被执行
     * @tparam Fn 函数的类型
     * @param fn 要被调用的函数（使用完美转发）
     */
    template <typename Fn>
    void OnceCall::call(Fn&& fn)
    {
        if (state_.load(std::memory_order_acquire) == State::Initialized)
            return;

        std::unique_lock<std::mutex> lock(mtx_);
        if (state_.load(std::memory_order_relaxed) == State::Initialized)
            return;

        state_.store(State::Initializing, std::memory_order_relaxed);
        try
        {
            std::forward<Fn>(fn)(); // 完美转发
            state_.store(State::Initialized, std::memory_order_release);
        }
        catch (...)
        {
            state_.store(State::Uninitialized, std::memory_order_release);
            throw;
        }
    }


    // ------------------ OnceCell 实现 ------------------
    /**
     * @brief 构造一个新的 OnceCell 对象，初始状态为未初始化
     * @tparam T 单元中存储的数据类型
     */
    template <typename T>
    OnceCell<T>::OnceCell() : state_(State::Uninitialized)
    {
    }

    /**
     * @brief 销毁 OnceCell 对象
     * @tparam T 单元中存储的数据类型
     */
    template <typename T>
    OnceCell<T>::~OnceCell() = default;

    /**
     * @brief 获取单元中的值，如果单元未被初始化，则使用给定的函数进行初始化
     * @tparam T 单元中存储的数据类型
     * @tparam Fn 用于初始化的函数的类型
     * @param fn 用于初始化的函数（使用完美转发）
     * @return 单元中值的引用
     */
    template <typename T>
    template <typename Fn>
    T& OnceCell<T>::get_or_init(Fn&& fn)
    {
        if (state_.load(std::memory_order_acquire) == State::Initialized)
            return *value_;

        std::unique_lock<std::mutex> lock(mtx_);
        if (state_.load(std::memory_order_relaxed) == State::Initialized)
            return *value_;

        state_.store(State::Initializing, std::memory_order_relaxed);
        try
        {
            value_.emplace(std::forward<Fn>(fn)()); // 完美转发
            state_.store(State::Initialized, std::memory_order_release);
            return *value_;
        }
        catch (...)
        {
            state_.store(State::Uninitialized, std::memory_order_release);
            throw;
        }
    }

    /**
     * @brief 检查单元是否已经被初始化
     * @tparam T 单元中存储的数据类型
     * @return 如果单元已经被初始化，则返回 true，否则返回 false
     */
    template <typename T>
    bool OnceCell<T>::is_initialized() const
    {
        return state_.load(std::memory_order_acquire) == State::Initialized;
    }

    /**
     * @brief 获取指向单元中值的可变指针
     * @tparam T 单元中存储的数据类型
     * @return 如果单元已经被初始化，则返回指向值的指针，否则返回 nullptr
     */
    template <typename T>
    T* OnceCell<T>::get()
    {
        if (state_.load(std::memory_order_acquire) == State::Initialized)
            return &(*value_);
        return nullptr;
    }

    /**
     * @brief 尝试获取指向单元中值的只读指针，此操作不触发初始化
     * @tparam T 单元中存储的数据类型
     * @return 如果单元已经被初始化，则返回指向值的只读指针，否则返回 nullptr
     */
    template <typename T>
    const T* OnceCell<T>::try_get() const
    {
        if (state_.load(std::memory_order_acquire) == State::Initialized)
            return &(*value_);
        return nullptr;
    }

    /**
     * @brief 重置单元的状态，清除已存储的值，使其可以被重新初始化
     * @tparam T 单元中存储的数据类型
     */
    template <typename T>
    void OnceCell<T>::reset()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        value_.reset();
        state_.store(State::Uninitialized, std::memory_order_release);
    }
}