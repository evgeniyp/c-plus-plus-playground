#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <utility>

struct SimpleString {
  SimpleString(size_t max_size) : max_size{max_size}, length{} {
    printf("Constructor\n");
    if (max_size == 0) {
      throw std::runtime_error{"Max size must be at least 1."};
    }
    buffer = new char[max_size];
    buffer[0] = 0;
  }

  ~SimpleString() {
    printf("Destructor\n");
    delete[] buffer;
  }

  SimpleString(const SimpleString &other)
      : max_size{other.max_size}, buffer{new char[other.max_size]},
        length{other.length} {
    printf("Copy Constructor\n");
    std::strncpy(buffer, other.buffer, max_size);
  }

  SimpleString(SimpleString &&other) noexcept
      : max_size(other.max_size), buffer(other.buffer), length(other.length) {
    printf("Move Constructor\n");
    other.length = 0;
    other.buffer = nullptr;
    other.max_size = 0;
  }

  SimpleString &operator=(const SimpleString &other) {
    printf("Copy Assignment\n");
    if (this == &other)
      return *this;
    const auto new_buffer = new char[other.max_size];
    delete[] buffer;
    buffer = new_buffer;
    length = other.length;
    max_size = other.max_size;
    std::strncpy(buffer, other.buffer, max_size);
    return *this;
  }

  SimpleString &operator=(SimpleString &&other) noexcept {
    printf("Move Assignment\n");
    if (this == &other)
      return *this;
    delete[] buffer;
    buffer = other.buffer;
    length = other.length;
    max_size = other.max_size;
    other.buffer = nullptr;
    other.length = 0;
    other.max_size = 0;
    return *this;
  }

  void print(const char *tag) const { printf("%s: %s", tag, buffer); }

  bool append_line(const char *x) {
    const auto x_len = strlen(x);
    if (x_len + length + 2 > max_size)
      return false;
    std::strncpy(buffer + length, x, max_size - length);
    length += x_len;
    buffer[length++] = '\n';
    buffer[length] = 0;
    return true;
  }

private:
  size_t max_size;
  char *buffer;
  size_t length;
};

struct SimpleStringOwner {
  SimpleStringOwner(const char *x) : string{10} {
    if (!string.append_line(x)) {
      throw std::runtime_error{"Not enough memory!"};
    }
  }

  SimpleStringOwner(const SimpleString &x) : string{x} {}

  SimpleStringOwner(SimpleString &&x) : string{std::move(x)} {}

  ~SimpleStringOwner() = default;

private:
  SimpleString string;
};

int main() {
  {
    printf("Example 1\n");
    SimpleString ss{10};
    SimpleStringOwner o1{ss};

    // Output:
    //
    // Example 1
    // Constructor
    // Copy Constructor
    // Destructor
    // Destructor
  }
  {
    printf("Example 2\n");
    SimpleStringOwner o2{SimpleString{50}};

    // Output:
    //
    // Example 2
    // Constructor
    // Move Constructor
    // Destructor
    // Destructor
  }
}
