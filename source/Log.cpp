#include <Log.hpp>

#include <string>
#include <sstream>
#include <iomanip>

std::array<std::string, Log::MAX_SIZE> Log::s_messages {};
size_t Log::s_next_idx = 0;

void Log::forEach(std::function<void(std::string const&)>&& fn) {
    forEach([&fn](std::string const &msg, int i) {
        fn(msg);
    });
}

void Log::forEach(std::function<void(std::string const&, int)>&& fn) {
    for (size_t i = 0; i < MAX_SIZE; ++i) {
        size_t line = i;
        if (s_next_idx >= MAX_SIZE)
            line = s_next_idx - MAX_SIZE + i;
        else if (i >= s_next_idx)
            break;

        std::stringstream stream;
        stream << std::setw(std::to_string(s_next_idx).size()) << (line + 1) << " "
               << s_messages[line % MAX_SIZE];
        fn(stream.str(), i);
    }
}
