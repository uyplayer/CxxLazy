# CxxLazy

Thread-safe lazy initialization for modern C++ | 现代 C++ 的线程安全惰性初始化

## 📌 CxxLazy 特征 / Features

### 🔹 中文

* **线程安全**：基于 `std::once_flag` + `std::call_once`，保证多线程下初始化只执行一次。
* **异常可重试**：初始化函数如果抛出异常，会重置标志，下一次访问时可再次尝试。
* **值容器封装**：提供 `OnceCell<T>`、`Lazy<T>` 类型，封装值存储与生命周期，不需要手动管理指针。
* **全局变量友好**：通过 `LAZY_STATIC` 宏，避免 C++ 全局对象析构顺序问题。
* **简洁 API**：`get_or_init`、`get`、`is_initialized`，语义清晰；支持 `operator*`、`operator->`。
* **可扩展**：可进一步扩展 `ThreadLocalLazy`、`ResettableLazy`、`constexpr Lazy` 等功能。

---

### 🔹 English

* **Thread-safe**: Uses `std::once_flag` + `std::call_once` to ensure one-time initialization across threads.
* **Exception-safe retry**: If initialization throws, the flag resets and future calls can retry.
* **Value container abstraction**: Provides `OnceCell<T>` and `Lazy<T>` as safe value holders, no manual pointer
  handling.
* **Global-friendly**: `LAZY_STATIC` macro avoids C++ static destruction order issues.
* **Simple API**: Clear semantics with `get_or_init`, `get`, `is_initialized`; supports `operator*` and `operator->`.
* **Extensible**: Future support for `ThreadLocalLazy`, `ResettableLazy`, `constexpr Lazy`, etc.

## 📥 安装 / Installation

- 安装 conan 包管理工具

```bash

pip install conan
```

- 克隆仓库

```bash

git clone https://github.com/uyplayer/CxxLazy.git
cd CxxLazy
````

- 执行脚本（使用 Conan + CMake 自动构建 & 安装）

```bash

./scripts/conan.sh

# 头文件：`/usr/local/include/cxxlazy/`
# 静态库：`/usr/local/lib/libcxxlazy.a`
```

 




- 导入头文件
```bash


#include <cxxlazy/components/once_call.h>
```
 
- 链接库

```bash

target_link_libraries(myapp  pthread cxxlazy)
```