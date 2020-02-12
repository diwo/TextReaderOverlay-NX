#include <Log.hpp>

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdarg>

std::array<std::string, Log::MAX_SIZE> Log::s_messages {};
unsigned int Log::s_next_idx = 0;

void Log::log(const char* fmtstr, ...) {
    va_list args;
    va_start(args, fmtstr);
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmtstr, args);
    s_messages[(s_next_idx++) % MAX_SIZE] = buf;
    va_end(args);
}

void Log::forEach(std::function<void(const char*)> fn) {
    forEach([fn](const char* msg, int i) {
        fn(msg);
    });
}

void Log::forEach(std::function<void(const char*, int i)> fn) {
    for (size_t i = 0; i < MAX_SIZE; i++) {
        unsigned int line = i;
        if (s_next_idx >= MAX_SIZE)
            line = s_next_idx - MAX_SIZE + i;
        else if (i >= s_next_idx)
            break;

        std::stringstream stream;
        stream << std::setw(std::to_string(s_next_idx).size()) << line << ": "
               << s_messages[line % MAX_SIZE];
        fn(stream.str().c_str(), i);
    }
}
