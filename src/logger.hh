#ifndef _LOGGER_HH_
#define _LOGGER_HH_

#include "common.hh"

template <typename T>
concept printable = requires(T t, std::ostream& os) {
    { os << t } -> std::same_as<std::ostream&>;
};

class Logger {
private:
    template<printable T>
    void print(T t) const { std::cerr << t; }

    template<printable T, typename... Args>
    void print(T t, Args... args) const { std::cerr << t; print(args...); }

    template<typename... args>
    void print(void) const {}

public:
    template<typename... Args>
    [[noreturn]] void panic(Args... args) const
    {
        // TODO: refactor.
        std::cerr << "\x1b[31merror: ";
        print(args...);
        std::cerr << "\n\x1b[0m";
        exit(1);
    }

    template<typename... Args>
    void log(Args... args) const
    {
        std::cerr << "\x1b[34mlog: ";
        print(args...);
        std::cerr << "\n\x1b[0m";
    }

    template<typename... Args>
    void warn(Args... args) const
    {
        std::cerr << "\x1b[33mwarning: ";
        print(args...);
        std::cerr << "\n\x1b[0m";
    }
};

#endif /* _LOGGER_HH_ */
