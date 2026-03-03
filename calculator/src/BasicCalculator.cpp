#include "..\include\BasicCalculator.h"

template<typename T>
BasicCalculator<T>::BasicCalculator() {
    addBinaryOperation("+", 1, [this](T a, T b) { return add(a, b); });
    addBinaryOperation("-", 1, [this](T a, T b) { return subtract(a, b); });
    addBinaryOperation("*", 2, [this](T a, T b) { return multiply(a, b); });
    addBinaryOperation("/", 2, [this](T a, T b) { return divide(a, b); });
}

template<typename T>
void BasicCalculator<T>::addBinaryOperation(const std::string& op, int prec, std::function<T(T, T)> fn) {
    binOps_[op] = fn;
    precedence_[op] = prec;
}

template<typename T>
void BasicCalculator<T>::addUnaryOperation(const std::string& op, int prec, std::function<T(T)> fn) {
    unaryOps_[op] = fn;
    precedence_[op] = prec;
}

template<typename T>
T BasicCalculator<T>::parseValue(const std::string& expression, int& index) {
    std::string buffer;
    while (index < expression.length() && isdigit(expression[index]))
        buffer += expression[index++];
    index--;
    if (buffer.empty()) throw std::runtime_error("Expected a number");
    std::stringstream ss(buffer);
    T value{}; ss >> value;
    return value;
}

template<typename T>
std::string BasicCalculator<T>::matchOperator(const std::string& expr, int index) const {
    std::string match;
    for (auto& [key, _] : unaryOps_) {
        if (key.size() > match.size() &&
            expr.substr(index, key.size()) == key)
            match = key;
    }
    for (auto& [key, _] : binOps_) {
        if (key.size() > match.size() &&
            expr.substr(index, key.size()) == key)
            match = key;
    }
    return match;
}

template<typename T>
void BasicCalculator<T>::applyTopOp(std::stack<T>& values, std::stack<std::string>& operations) {
    if (operations.empty()) throw std::runtime_error("Operator stack underflow");
    std::string op = operations.top(); operations.pop();

    if (unaryOps_.count(op)) {
        if (values.empty()) throw std::runtime_error("Not enough operands for unary operator");
        T a = values.top(); values.pop();
        values.push(unaryOps_.at(op)(a));
    }
    else if (binOps_.count(op)) {
        if (values.size() < 2) throw std::runtime_error("Not enough operands for binary operator");
        T b = values.top(); values.pop();
        T a = values.top(); values.pop();
        values.push(binOps_.at(op)(a, b));
    }
}

template<typename T>
T BasicCalculator<T>::solve(const std::string& expression) {
    std::stack<T>           values;
    std::stack<std::string> operations;
    bool expectValue = true;

    for (int index = 0; index < expression.length(); index++) {
        char c = expression[index];
        if (isspace(c)) continue;

        // Number literal
        if (isdigit(c)) {
            values.push(parseValue(expression, index));
            expectValue = false;
            continue;
        }

        // Open paren
        if (c == '(') {
            operations.push("(");
            expectValue = true;
            continue;
        }

        // Close paren
        if (c == ')') {
            while (!operations.empty() && operations.top() != "(")
                applyTopOp(values, operations);
            if (operations.empty()) throw std::runtime_error("Mismatched parentheses");
            operations.pop(); // pop "("

            // apply any unary op waiting just outside the parens (e.g. "sqrt")
            if (!operations.empty() && unaryOps_.count(operations.top()))
                applyTopOp(values, operations);

            expectValue = false;
            continue;
        }
               
        std::string operatorSymbol = matchOperator(expression, index);
        if (!operatorSymbol.empty()) {
            if (unaryOps_.count(operatorSymbol) && expectValue) {
                // Unary op in value position (e.g. "sqrt")
                operations.push(operatorSymbol);
                // expectValue stays true — we expect '(' next
            }
            else if (binOps_.count(operatorSymbol)) {
                if (expectValue && operatorSymbol == "-") {
                    // Unary minus: push 0 so binary '-' computes 0 - x
                    values.push(T{});
                    operations.push("-");
                }
                else if (!expectValue) {
                    // Binary operator: pop higher/equal precedence ops first
                    while (!operations.empty() && operations.top() != "(" &&
                        precedence_.count(operations.top()) &&
                        precedence_.at(operations.top()) >= precedence_.at(operatorSymbol))
                    {
                        applyTopOp(values, operations);
                    }
                    operations.push(operatorSymbol);
                    expectValue = true;
                }
            }
            
            index += (int)operatorSymbol.size() - 1;
            continue;
        }

        throw std::runtime_error(std::string("Unknown character: ") + c);
    }

    while (!operations.empty())
        applyTopOp(values, operations);

    if (values.empty()) throw std::runtime_error("Expression produced no result");
    return values.top();
}

template class BasicCalculator<int>;
template class BasicCalculator<float>;
template class BasicCalculator<std::complex<float>>;
