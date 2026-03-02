#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <functional>
#include <cmath>
#include <complex>
#include <sstream>
#include <cctype>
#include <stdexcept>

template<typename T>
class BasicCalculator
{
public:
    BasicCalculator();
    virtual ~BasicCalculator() = default;

    /**
     * @explanation Evaluates an infix expression string using the shunting-yard algorithm.
     * @param expression The expression to evaluate (e.g. "3 + 4 * 2").
     * @return Result of the expression as type T.
     * @throws std::runtime_error if the expression is malformed or division by zero occurs.
     */
    virtual T solve(const std::string& expression);

protected:
    void addBinaryOperation(const std::string& op, int precedence, std::function<T(T, T)> func);
    void addUnaryOperation(const std::string& op, int precedence, std::function<T(T)>    func);

    T add(T a, T b) { return a + b; }
    T subtract(T a, T b) { return a - b; }
    T multiply(T a, T b) { return a * b; }
    T divide(T a, T b) {
        if (b == T(0)) throw std::runtime_error("Division by zero");
        return a / b;
    }

    /**
     * @explanation Reads a number literal from the expression starting at index.
     * @param expression The full expression string being parsed.
     * @param index Current position in the expression, pointing at the first digit.
     * @return The parsed number as type T.
     * @throws std::runtime_error if no digits are found at the current position.
     */
    virtual T parseValue(const std::string& expression, int& index);

    /**
     * @explanation Pops the top operator and applies it to the top value/values on the value stack.
     * @param values Stack of operands.
     * @param operations Stack of operator tokens.
     * @throws std::runtime_error if either stack has insufficient elements.
     */
    virtual void applyTopOp(std::stack<T>& values, std::stack<std::string>& operations);
    /**
     * @explanation Tries to match the longest registered operator token at position index.
     * @param expr The expression being parsed.
     * @param index Current position to match from.
     * @return The matched token string, or "" if no token matches.
     */
    std::string matchOperator(const std::string& expr, int index) const;

private:
    std::map<std::string, std::function<T(T, T)>> binOps_;
    std::map<std::string, std::function<T(T)>>    unaryOps_;
    std::map<std::string, int>                    precedence_;
};
