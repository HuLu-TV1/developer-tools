#include <string>
#include <sstream>
#include <iostream>
#include <thread>

inline std::string tid_to_string(const std::thread::id& tid)
{
    std::ostringstream oss;
    oss << tid << std::endl;
    return oss.str();
}

inline long long tid_to_ll(const std::thread::id& tid)
{
    return atoll(tid_to_string(tid).c_str());
}