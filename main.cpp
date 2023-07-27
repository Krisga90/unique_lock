#include <chrono>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::literals;

std::mutex PRINT_MUTEX;

void printWithMutex(const std::string &str, bool throw_exc);
void printWithLockGuard(const std::string &str, bool throw_exc);
void printWithUniqueLock(const std::string &str, bool throw_exc);
void printWithUniqueLockV2(const std::string &str, bool throw_exc);

int main() {
  std::string text = "Fizzz Buzzz!!";
  bool throw_except_argument = false;
  std::thread thr1(printWithUniqueLockV2, std::cref(text),
                   throw_except_argument);
  std::thread thr2(printWithUniqueLockV2, std::cref(text),
                   throw_except_argument);
  std::thread thr3(printWithUniqueLockV2, std::cref(text),
                   throw_except_argument);

  thr1.join();
  thr2.join();
  thr3.join();

  return 0;
}

void printWithMutex(const std::string &str, bool throw_exc) {
  try {
    PRINT_MUTEX.lock();

    for (char c : str) {
      std::cout << c;
    }
    std::cout << '\n';

    if (throw_exc) {
      // throw std::exception();
      throw std::runtime_error("Forced error");
    }
    PRINT_MUTEX.unlock();
    std::this_thread::sleep_for(500ms);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}
void printWithLockGuard(const std::string &str, bool throw_exc) {
  try {
    std::lock_guard<std::mutex> lg(PRINT_MUTEX);

    for (char c : str) {
      std::cout << c;
    }
    std::cout << '\n';

    if (throw_exc) {
      // throw std::exception();
      throw std::runtime_error("Forced error");
    }
    std::this_thread::sleep_for(500ms);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}
void printWithUniqueLock(const std::string &str, bool throw_exc) {
  try {
    std::unique_lock<std::mutex> u_lock(PRINT_MUTEX);

    for (char c : str) {
      std::cout << c;
    }
    std::cout << '\n';

    if (throw_exc) {
      // throw std::exception();
      throw std::runtime_error("Forced error");
    }
    u_lock.unlock();
    std::this_thread::sleep_for(500ms);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}

void printWithUniqueLockV2(const std::string &str, bool throw_exc) {
  try {
    std::unique_lock<std::mutex> u_lock(PRINT_MUTEX, std::defer_lock);
    u_lock.lock();
    std::unique_lock<std::mutex> u_lock2(PRINT_MUTEX, std::adopt_lock);
    for (char c : str) {
      std::cout << c;
    }
    std::cout << '\n';

    if (throw_exc) {
      // throw std::exception();
      throw std::runtime_error("Forced error");
    }
    // u_lock.unlock();
    u_lock2.unlock();
    std::this_thread::sleep_for(500ms);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}