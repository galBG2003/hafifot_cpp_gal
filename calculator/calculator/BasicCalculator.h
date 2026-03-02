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
     * @brief Evaluates an infix expression string using the shunting-yard algorithm.
     * @param expression The expression to evaluate (e.g. "3 + 4 * 2").
     * @return Result of the expression as type T.
     * @throws std::runtime_error if the expression is malformed or division by zero occurs.
     */
    virtual T solve(const std::string& expression);

protected:
    void addBinaryOperation(char op, int precedence, std::function<T(T, T)> func);
    void addUnaryOperation(char op, int precedence, std::function<T(T)>    func);

    virtual T add(T a, T b) { return a + b; }
    virtual T subtract(T a, T b) { return a - b; }
    virtual T multiply(T a, T b) { return a * b; }
    virtual T divide(T a, T b) {
        if (b == T(0)) throw std::runtime_error("Division by zero");
        return a / b;
    }

    /**
     * @brief Reads a number literal from the expression starting at index.
     * @param expression The full expression string being parsed.
     * @param index Current position in the expression, pointing at the first digit.
     *              On return, points at the last consumed character.
     * @return The parsed number as type T.
     * @throws std::runtime_error if no digits are found at the current position.
     */
    virtual T parseValue(const std::string& expression, int& index);

    /**
     * @brief Pops the top operator and applies it to the top value(s) on the value stack.
     * @param values Stack of operands.
     * @param operations Stack of operators.
     * @throws std::runtime_error if either stack has insufficient elements.
     */
    virtual void applyTopOp(std::stack<T>& values, std::stack<char>& operations);
    bool hasUnaryOp(char op) const { return unaryOps_.count(op); }
    bool hasBinaryOp(char op) const { return binOps_.count(op); }
    bool hasPrecedence(char op) const { return precedence_.count(op); }
    int  getPrecedence(char op) const { return precedence_.at(op); }

    T callUnaryOp(char op, T a) { return unaryOps_.at(op)(a); }
    T callBinaryOp(char op, T a, T b) { return binOps_.at(op)(a, b); }
private:
    std::map<char, std::function<T(T, T)>> binOps_;
    std::map<char, std::function<T(T)>>    unaryOps_;
    std::map<char, int>                    precedence_;
};