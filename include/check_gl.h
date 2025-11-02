#pragma once

#include <cstddef>
#include <utility>
struct GLHandleBase {
  unsigned int handle;
  GLHandleBase() noexcept : handle(0) {}
  explicit GLHandleBase(std::nullptr_t) noexcept : handle(0) {}
  explicit GLHandleBase(unsigned int handle) noexcept : handle(handle) {}

  // 定义类型转换函数
  // 它的主要作用是让包含它的类对象能够自动或显式地转换为 unsigned int类型
  operator unsigned int() const noexcept { return handle; }

  unsigned int get() const noexcept { return handle; }

  unsigned int release() noexcept {
    unsigned int ret = handle;
    handle = 0;
    return ret;
  }

  // 移动操作被显式定义
  // 拷贝构造函数和拷贝赋值运算符被隐式删除 因为定义了移动操作
  GLHandleBase(GLHandleBase &&that) noexcept : handle(that.handle) {
    that.handle = 0;
  }

  GLHandleBase &operator=(GLHandleBase &&that) noexcept {
    std::swap(handle, that.handle);
    return *this;
  }
};
