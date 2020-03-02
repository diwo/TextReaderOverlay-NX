#pragma once

#include <string>
#include <array>
#include <functional>

class Log {
public:
    template<typename ...Args>
    static void log(std::string fmtstr, Args&&... args) {
        char buf[1024];
        snprintf(buf, sizeof(buf), fmtstr.c_str(), std::forward<Args>(args)...);
        s_messages[(s_next_idx++) % MAX_SIZE] = buf;
    }

    static void forEach(std::function<void(std::string const &msg)>&& fn);
    static void forEach(std::function<void(std::string const &msg, int i)>&& fn);

private:
    Log() {}
    static inline const size_t MAX_SIZE = 45;
    static std::array<std::string, MAX_SIZE> s_messages;
    static size_t s_next_idx;
};
