#pragma once
#include <ostream>
#include <vector>

namespace ostream_helpers {

};

namespace std {
    template<class T>
    std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
        out << "[";
        size_t last = v.size() - 1;
        for(size_t i = 0; i < v.size(); ++i) {
            out << v[i];
            if (i != last) {
                out << ", ";
            }
        }
        out << "]";
        return out;
    }

    template<class T1, class T2>
    std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& v) {
        out << "(" << v.first << ", " << v.second << ")";
        return out;
    }
}