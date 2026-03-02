#include "BasicCalculator.h"

template<typename T>
BasicCalculator<T>::BasicCalculator() {
    addBinaryOperation('+', 1, [this](T a, T b) { return add(a, b); });
    addBinaryOperation('-', 1, [this](T a, T b) { return subtract(a, b); });
    addBinaryOperation('*', 2, [this](T a, T b) { return multiply(a, b); });
    addBinaryOperation('/', 2, [this](T a, T b) { return divide(a, b); });
}

template<typename T>
void BasicCalculator<T>::addBinaryOperation(char op, int prec, std::function<T(T, T)> fn) {
    binOps_[op] = fn;
    precedence_[op] = prec;
}

template<typename T>
void BasicCalculator<T>::addUnaryOperation(char op, int prec, std::function<T(T)> fn) {
    unaryOps_[op] = fn;
    precedence_[op] = prec;
}

template<typename T>
T BasicCalculator<T>::parseValue(const std::string& expression, int& index) {
    std::string buffer;
    while (index < (int)expression.length() && isdigit(expression[index]))
        buffer += expression[index++];
    index--; // retrace index to the last digit
    if (buffer.empty()) throw std::runtime_error("Expected a number");
    std::stringstream ss(buffer);
    T value{};
    ss >> value;
    return value;
}

template<typename T>
void BasicCalculator<T>::applyTopOp(std::stack<T>& values, std::stack<char>& operations) {
    if (operations.empty()) throw std::runtime_error("Operator stack underflow");
    char operation = operations.top(); operations.pop();

    if (unaryOps_.count(operation)) {
        if (values.empty()) throw std::runtime_error("Not enough operands for unary operator");
        T a = values.top(); values.pop();
        values.push(unaryOps_.at(operation)(a));
    }
    else if (binOps_.count(operation)) {
        if (values.size() < 2) throw std::runtime_error("Not enough operands for binary operator");
        T b = values.top(); values.pop();
        T a = values.top(); values.pop();
        values.push(binOps_.at(operation)(a, b));
    }
}

template<typename T>
T BasicCalculator<T>::solve(const std::string& expression) {
    std::stack<T>    values;
    std::stack<char> operations;
    bool expectValue = true;

    for (int index = 0; index < (int)expression.length(); index++) {
        char c = expression[index];
        if (isspace(c)) continue;

        // Number literal
        if (isdigit(c)) {
            values.push(parseValue(expression, index));
            expectValue = false;
        }

        // Single-char unary ops registered by subclasses (e.g. 's' for sqrt)
        else if (unaryOps_.count(c) && expectValue) {
            operations.push(c);
        }

        // Open paren
        else if (c == '(') {
            operations.push('(');
            expectValue = true;
        }

        // Close paren
        else if (c == ')') {
            while (!operations.empty() && operations.top() != '(')
                applyTopOp(values, operations);
            if (operations.empty()) throw std::runtime_error("Mismatched parentheses");
            operations.pop(); // pop '('

            // apply any unary op waiting just outside the parens (e.g. 's' for sqrt)
            if (!operations.empty() && unaryOps_.count(operations.top()))
                applyTopOp(values, operations);

            expectValue = false;
        }

        // Binary operators (or unary minus)
        else if (binOps_.count(c)) {
            if (expectValue) {
                if (c == '-') {
                    values.push(T{});  // 0 - x trick for unary minus
                    operations.push('-');
                }

            }
            else {
                while (!operations.empty() && operations.top() != '(' &&
                    precedence_.count(operations.top()) &&
                    precedence_.at(operations.top()) >= precedence_.at(c))
                {
                    applyTopOp(values, operations);
                }
                operations.push(c);
                expectValue = true;
            }
        }

        else {
            throw std::runtime_error(std::string("Unknown character: ") + c);
        }
    }

    while (!operations.empty())
        applyTopOp(values, operations);

    if (values.empty()) throw std::runtime_error("Expression produced no result");
    return values.top();
}

template class BasicCalculator<int>;
template class BasicCalculator<float>;
template class BasicCalculator<std::complex<float>>;