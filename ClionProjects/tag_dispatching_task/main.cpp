#include <iostream>
#include <limits>

struct signed_based {};
struct unsigned_based {};

template<typename T>
struct number_traits {
    typedef signed_based category;
};

template <>
struct number_traits<int> {
    typedef signed_based category;
};

template <>
struct number_traits<unsigned> {
    typedef unsigned_based category;
};

template <typename T>
struct checked
{
    T value;
    checked<T>(T value) {
        this->value = value;
    }
    checked<T>(const checked<T> &other) {
        this->value = other.value;
    }
    checked<T>() {
        this->value = 0;
    }
    checked<T> &operator=(checked<T> const &other) {
        this->value = other.value;
        return *this;
    }
    checked<T> operator-() const;

    std::ostream &operator<<(std::ostream &s, checked<T> const &a) {
        return s << a.value;
    }
};

template <typename T>
bool check_unary_impl(checked<T> a, signed_based) {
    return a.value == std::numeric_limits<T>::min();
}

template <typename T>
bool check_unary_impl(checked<T> a, unsigned_based) {
    return a.value != 0;
}

template <typename T>
checked<T> checked<T>::operator-() const {
    if (!check_unary_impl(*this, typename number_traits<T>::category())) {
        checked<T> res(-this->value);
        return res;
    } else {
        throw 0;
    }
}

template <typename T>
bool check_add_impl(checked<T> a, checked<T> b, signed_based) {
    T arg1 = a.value;
    T arg2 = b.value;
    return (arg1 > 0 ? arg2 > std::numeric_limits<T>::max() - arg1 : arg2 < std::numeric_limits<T>::max() - arg1);
}

template <typename T>
checked<T> operator +(checked<T> a, checked<T> const& b) {
    if (!check_add_impl(a, b, typename number_traits<T>::category())) {
        return checked<T>(a.value + b.value);
    } else {
        throw 0;
    }
}

template <typename T>
bool check_sub_impl(checked<T> a, checked<T> b, signed_based) {
    T arg1 = a.value;
    T arg2 = b.value;
    return arg1 >= 0 ?
   arg2 < arg1 - std::numeric_limits<T>::max() :
   arg2 > arg1 - std::numeric_limits<T>::min();
}

template <typename T>
checked<T> operator -(checked<T> a, checked<T> const& b) {
    if (!check_sub_impl(a, b, typename number_traits<T>::category())) {
        return checked<T>(a.value - b.value);
    } else {
        throw 0;
    }
}

template <typename T>
bool check_mul_impl(checked<T> a, checked<T> b, signed_based) {
    T arg1 = a.value;
    T arg2 = b.value;
    if (arg1 > 0) {
        return (arg2 > std::numeric_limits<T>::max() / arg1 || arg2 < std::numeric_limits<T>::min() / arg1);
    }
    if (arg1 < -1) {
        return (arg2 > std::numeric_limits<T>::min() / arg1 || arg2 < std::numeric_limits<T>::max() / arg1);
    }
    if (arg1 == -1) {
        return (arg2 == std::numeric_limits<T>::min());
    }
    return false;
}

template <typename T>
checked<T> operator *(checked<T> a, checked<T> const& b) {
    if (!check_mul_impl(a, b, typename number_traits<T>::category())){
        return checked<T>(a.value * b.value);
    } else {
        throw 0;
    }
}

template <typename T>
bool check_div_impl(checked<T> a, checked<T> b, signed_based) {
    T arg1 = a.value;
    T arg2 = b.value;
    if (arg2 == 0) {
        return true;
    }
    return arg1 == std::numeric_limits<T>::min() && arg2 == -1;
}

template <typename T>
checked<T> operator /(checked<T> a, checked<T> const& b) {
    if (!check_div_impl(a, b, typename number_traits<T>::category())) {
        return checked<T>(a.value / b.value);
    } else {
        throw 0;
    }
}

template <typename T>
bool check_add_impl(checked<T> a, checked<T> b, unsigned_based) {
    T arg1 = a.value;
    T arg2 = b.value;
    return (arg2 > std::numeric_limits<T>::max() - arg1);
}
template <typename T>
bool check_sub_impl(checked<T> a, checked<T> b, unsigned_based) {
    T arg1 = a.value;
    T arg2 = b.value;
    return (arg2 > arg1 - std::numeric_limits<T>::max());
}
template <typename T>
bool check_mul_impl(checked<T> a, checked<T> b, unsigned_based) {
    T arg1 = a.value;
    T arg2 = b.value;
    if (arg1 > 0) {
        return (arg2 > std::numeric_limits<T>::max() / arg1 || arg2 < std::numeric_limits<T>::min() / arg1);
    }
    return false;
}
template <typename T>
bool check_div_impl(checked<T> a, checked<T> b, unsigned_based) {
    T arg2 = b.value;
    return arg2 == 0;
}


int main() {
    checked<unsigned> a(std::numeric_limits<unsigned>::max() / 2);
    checked<unsigned> b(unsigned(2));
    checked<unsigned > c;
    try {
        a * b;
    } catch (int e) {
        std::cerr << "overflow" << std::endl;
    }

}