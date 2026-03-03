#include "..\include\ComplexCalculator.h"

template<typename T>
T ComplexCalculator<T>::parseValue(const std::string& expression, int& index) {
    std::string buffer;
    while (index < expression.length() && (isdigit(expression[index]) || expression[index] == '.'))
        buffer += expression[index++];

    bool isImaginary = false;
    if (index < expression.length() && expression[index] == 'i') {
        isImaginary = true;
        index++;
    }

    index--;
    float val = buffer.empty() ? 1.0f : std::stof(buffer);

    return isImaginary ? T(0, val) : T(val, 0);
}

template<typename T>
std::string ComplexCalculator<T>::normaliseI(const std::string& expr) {
    std::string out;
    out.reserve(expr.size() + 4);
    for (int index = 0; index < (int)expr.size(); index++) {
        char c = expr[index];
        if (c == 'i' && (out.empty() || !isdigit(out.back())))
            out += "1i";
        else
            out += c;
    }
    return out;
}

template<typename T>
T ComplexCalculator<T>::solve(const std::string& expression) {
    return AdvancedCalculator<T>::solve(normaliseI(expression));
}

template class ComplexCalculator<std::complex<float>>;