//
// Created by uyplayer on 2025/8/25.
//

#pragma once

#include "lazy.h"

/**
 * @brief 定义一个静态的延迟初始化对象
 * @details
 * 这个宏创建了一个静态的 `components::Lazy` 实例，它将在首次访问时通过执行 `expr` 来初始化
 * @param type 对象的类型
 * @param name 对象的名称
 * @param expr 用于初始化对象的表达式
 */
#define LAZY_STATIC(type, name, expr) \
static components::Lazy<type> name([] { return expr; })

/**
 * @brief 定义一个线程局部的延迟初始化对象
 * @details
 * 这个宏创建了一个线程局部的 `components::Lazy` 实例，每个线程都会有自己的独立实例，
 * 并在首次访问时通过执行 `expr` 来初始化
 * @param type 对象的类型
 * @param name 对象的名称
 * @param expr 用于初始化对象的表达式
 */
#define THREAD_LOCAL_LAZY(type, name, expr) \
static thread_local components::Lazy<type> name([] { return expr; })

/**
 * @brief 定义一个静态的延迟执行的 void 操作
 * @details
 * 这个宏创建了一个 `components::Lazy<void>` 实例，它将在首次访问时执行 `expr`
 * @param name 操作的名称
 * @param expr 要执行的表达式
 */
#define LAZY_STATIC_VOID(name, expr) \
static components::Lazy<void> name([] { expr; })
