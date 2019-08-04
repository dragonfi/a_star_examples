#pragma once
#include <ostream>
#include <vector>

namespace ostream_helpers {
    template<class T>
    struct StreamableVector {
        std::vector<T> const& vector;
    };

    template<class T>
    std::ostream& operator<<(std::ostream& out, const StreamableVector<T>& wrapper) {
        std::vector<T> const& v = wrapper.vector;
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
    struct StreamablePair {
        std::pair<T1, T2> const& pair;
    };

    template<class T1, class T2>
    std::ostream& operator<<(std::ostream& out, const StreamablePair<T1, T2>& wrapper) {
        std::pair<T1, T2> p = wrapper.pair;
        out << "(" << p.first << ", " << p.second << ")";
        return out;
    }
}