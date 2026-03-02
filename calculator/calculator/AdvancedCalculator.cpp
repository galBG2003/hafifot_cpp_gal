#include "AdvancedCalculator.h"

template<typename T>
AdvancedCalculator<T>::AdvancedCalculator() {
    this->addBinaryOperation("^", 3, [this](T a, T b) { return power(a, b); });
    this->addUnaryOperation("sqrt", 4, [this](T a) { return sqrtOp(a);  });
}

template<typename T>
T AdvancedCalculator<T>::power(T a, T b) {
    return std::pow(a, b);
}

template<typename T>
T AdvancedCalculator<T>::sqrtOp(T a) {
    return std::sqrt(a);
}

template<typename T>
T AdvancedCalculator<T>::parseValue(const std::string& expression, int& index) {
    std::string buf;
    while (index < (int)expression.length() && (isdigit(expression[index]) || expression[index] == '.'))
        buf += expression[index++];
    index--;
    if (buf.empty()) throw std::runtime_error("Expected a number");
    std::stringstream ss(buf);
    T val{}; ss >> val;
    return val;
}

template class AdvancedCalculator<float>;
template class AdvancedCalculator<std::complex<float>>;