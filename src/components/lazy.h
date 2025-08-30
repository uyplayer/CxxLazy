//
// Created by uyplayer on 2025/8/25.
//

#pragma once

#include "once_call.h"
#include <functional>
#include <utility>

namespace components {
    /**
     * @brief 一个延迟求值的封装类
     * @tparam T 要求值的类型
     */
    template<typename T>
    class Lazy {
    public:
        using InitFn = std::function<T()>;

        /**
         * @brief 构造一个 Lazy 对象
         * @param init_fn 用于初始化值的函数
         */
        explicit Lazy(InitFn init_fn);

        Lazy(const Lazy &) = delete;

        Lazy &operator=(const Lazy &) = delete;

        Lazy(Lazy &&) noexcept = default;

        Lazy &operator=(Lazy &&) noexcept = default;

        /**
         * @brief 获取值，如果尚未初始化，则会先进行初始化
         * @return 值的引用
         */
        T &get();

        /**
         * @brief 解引用操作符，获取值的引用
         * @return 值的引用
         */
        T &operator*();

        /**
         * @brief 成员访问操作符，获取值的指针
         * @return 值的指针
         */
        T *operator->();

        /**
         * @brief 检查值是否已经初始化
         * @return 如果已经初始化，返回 true，否则返回 false
         */
        [[nodiscard]] bool is_initialized() const;

        /**
         * @brief 尝试获取值
         * @return 如果已经初始化，返回值指针，否则返回 nullptr
         */
        [[nodiscard]] const T *try_get() const;

        /**
         * @brief 重置，清空已有的值
         */
        void reset();

        /**
         * @brief bool 类型转换操作符，检查是否已经初始化
         * @return 如果已经初始化，返回 true，否则返回 false
         */
        explicit operator bool() const { return is_initialized(); }

    private:
        OnceCell<T> cell_;
        InitFn init_fn_;
    };

    // ---------------- 实现 ----------------

    template<typename T>
    Lazy<T>::Lazy(InitFn init_fn)
        : init_fn_(std::move(init_fn)) {
    }

    template<typename T>
    T &Lazy<T>::get() {
        return cell_.get_or_init(init_fn_);
    }

    template<typename T>
    T &Lazy<T>::operator*() {
        return get();
    }

    template<typename T>
    T *Lazy<T>::operator->() {
        return &get();
    }

    template<typename T>
    bool Lazy<T>::is_initialized() const {
        return cell_.is_initialized();
    }

    template<typename T>
    const T *Lazy<T>::try_get() const {
        return cell_.get();
    }

    template<typename T>
    void Lazy<T>::reset() {
        cell_.reset();
    }

    /**
     * @brief Lazy<void> 的特化版本
     */
    template<>
    class Lazy<void> {
    public:
        using InitFn = std::function<void()>;

        /**
         * @brief 构造一个 Lazy<void> 对象
         * @param init_fn 用于初始化的函数
         */
        explicit Lazy(InitFn init_fn)
            : init_fn_(std::move(init_fn)) {
        }

        Lazy(const Lazy &) = delete;

        Lazy &operator=(const Lazy &) = delete;

        Lazy(Lazy &&) noexcept = delete;

        Lazy &operator=(Lazy &&) noexcept = delete;

        /**
         * @brief 执行初始化函数
         */
        void get() {
            once_.call(init_fn_);
        }

        /**
         * @brief 检查是否已经初始化
         * @return 如果已经初始化，返回 true，否则返回 false
         */
        [[nodiscard]] bool is_initialized() const { return once_.is_initialized(); }

        /**
         * @brief bool 类型转换操作符，检查是否已经初始化
         * @return 如果已经初始化，返回 true，否则返回 false
         */
        explicit operator bool() const { return is_initialized(); }

        /**
         * @brief 重置
         */
        void reset() {
            once_.reset();
        }

    private:
        OnceCall once_;
        InitFn init_fn_;
    };
}