#pragma once

#include <array>
#include <functional>

class Log {
public:
    static void log(const char* fmtstr, ...);
    static void forEach(std::function<void(const char*)> fn);
    static void forEach(std::function<void(const char*, int i)> fn);

private:
    Log() {}
    static inline const size_t MAX_SIZE = 50;
    static std::array<std::string, MAX_SIZE> s_messages;
    static unsigned int s_next_idx;
};
