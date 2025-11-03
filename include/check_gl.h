#pragma once

#include <glad/gl.h>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace check_gl {
const char *opengl_errno_name(GLenum err);
void opengl_check_error(const char *filename, int lineno, const char *expr);

} // namespace check_gl

#define CHECK_GL(x)                                                            \
  do {                                                                         \
    (x);                                                                       \
    ::check_gl::opengl_check_error(__FILE__, __LINE__, #x);                    \
  } while (0)

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

template <class Derived> struct GLHandleImpl : GLHandleBase {
  using GLHandleBase::GLHandleBase;
  GLHandleImpl(GLHandleImpl &&) = default;
  GLHandleImpl &operator=(GLHandleImpl &&) = default;

  struct [[nodiscard]] BindGuard : GLHandleBase {
  private:
    BindGuard(unsigned int handle) noexcept : GLHandleBase(handle) {}
    friend GLHandleImpl;

  public:
    BindGuard() = default;
    BindGuard(BindGuard &&) = default;

    ~BindGuard() noexcept(false) { Derived::on_bind(0); }
  };
  struct [[nodiscard]] BindTargetGuard : GLHandleBase {
  private:
    unsigned int target;
    BindTargetGuard(unsigned int target, unsigned int handle) noexcept
        : GLHandleBase(handle), target(target) {}
    friend GLHandleImpl;

  public:
    BindTargetGuard() = default;
    BindTargetGuard(BindTargetGuard &&) = default;
    BindTargetGuard &operator=(BindTargetGuard &&) = default;

    ~BindTargetGuard() noexcept(false) { Derived::on_bind(target, 0); }
  };

  template <class... Args> Derived &&remake(Args... args) {
    if (this->handle) {
      Derived::on_delete(1, &this->handle);
    }
    Derived::on_gen(1, &this->handle, args...);
    return std::move(static_cast<Derived &>(*this));
  }

  template <class... Args> Derived &&make(Args... args) {
    if (!this->handle) {
      Derived::on_gen(1, &this->handle, args...);
    }
    return std::move(static_cast<Derived &>(*this));
  }

  void reset(unsigned int handle) {
    if (this->handle) {
      Derived::on_delete(1, &this->handle);
    }
    this->handle = handle;
  }

  ~GLHandleImpl() {
    static_assert(std::is_base_of_v<GLHandleImpl, Derived>);
    if (this->handle) {
      Derived::on_delete(1, &this->handle);
    }
  }
};

struct GLProgram : GLHandleImpl<GLProgram> {
  static void on_gen(size_t count, unsigned int *handles) {
    for (size_t i = 0; i < count; ++i) {
      CHECK_GL(handles[i] = glCreateProgram());
    }
  }
  static void on_delete(size_t count, const unsigned int *handles) {
    for (size_t i = 0; i < count; ++i) {
      CHECK_GL(glDeleteProgram(handles[i]));
    }
  }
};

struct GLShader : GLHandleImpl<GLShader> {
  static void on_gen(size_t count, unsigned int *handles, unsigned int type) {
    for (size_t i = 0; i < count; ++i) {
      CHECK_GL(handles[i] = glCreateShader(type));
    }
  }

  static void on_delete(size_t count, const unsigned int *handles) {
    for (size_t i = 0; i < count; ++i) {
      CHECK_GL(glDeleteShader(handles[i]));
    }
  }
};

struct GLBuffer : GLHandleImpl<GLBuffer> {
  static void on_gen(size_t count, unsigned int *handles) {
    CHECK_GL(glGenBuffers(count, handles));
  }

  static void on_delete(size_t count, unsigned int *handles) {
    CHECK_GL(glDeleteBuffers(count, handles));
  }

  static void on_bind(size_t count, unsigned int handle) {
    CHECK_GL(glBindBuffer(count, handle));
  }
};

struct GLVertexArray : GLHandleImpl<GLVertexArray> {
  static void on_gen(size_t count, unsigned int *handles) {
    CHECK_GL(glGenVertexArrays(count, handles));
  }

  static void on_delete(size_t count, unsigned int *handles) {
    CHECK_GL(glDeleteVertexArrays(count, handles));
  }

  static void on_bind(unsigned int handle) {
    CHECK_GL(glBindVertexArray(handle));
  }
};

struct GLFramebuffer : GLHandleImpl<GLFramebuffer> {
  static void on_gen(size_t count, unsigned int *handles) {
    CHECK_GL(glGenFramebuffers(count, handles));
  }

  static void on_delete(size_t count, unsigned int *handles) {
    CHECK_GL(glDeleteFramebuffers(count, handles));
  }

  static void on_bind(unsigned int target, unsigned int handle) {
    CHECK_GL(glBindFramebuffer(target, handle));
  }
};

struct GLRenderbuffer : GLHandleImpl<GLRenderbuffer> {
  static void on_gen(size_t count, unsigned int *handles) {
    CHECK_GL(glGenRenderbuffers(count, handles));
  }

  static void on_delete(size_t count, unsigned int *handles) {
    CHECK_GL(glDeleteRenderbuffers(count, handles));
  }

  static void on_bind(unsigned int target, unsigned int handle) {
    CHECK_GL(glBindRenderbuffer(target, handle));
  }
};

struct GLTexture : GLHandleImpl<GLTexture> {
  static void on_gen(size_t count, unsigned int *handles) {
    CHECK_GL(glGenTextures(count, handles));
  }

  static void on_delete(size_t count, unsigned int *handles) {
    CHECK_GL(glDeleteTextures(count, handles));
  }

  static void on_bind(unsigned int target, unsigned int handle) {
    CHECK_GL(glBindTexture(target, handle));
  }
};
