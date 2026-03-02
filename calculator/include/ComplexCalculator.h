#pragma once
#include "..\include\AdvancedCalculator.h"

template<typename T>
class ComplexCalculator : public AdvancedCalculator<T>
{
public:
    /**
     * @explanation Normalises bare 'i' tokens to "1i", then calls to AdvancedCalculator::solve().
     * @param expression The expression to evaluate (e.g. "1 + 2i * 3i").
     * @return Result of the expression as type T.
     */
    T solve(const std::string& expression) override;

protected:
    /**
     * @explanation Reads a number literal, searching 'i' to produce an imaginary value.
     * @param expression The full expression string being parsed.
     * @param index Current position, pointing at the first digit.
     * @return T(val, 0) for real, T(0, val) for imaginary literals like "2i".
     */
    T parseValue(const std::string& expression, int& index) override;
private:
    static std::string normaliseI(const std::string& expr);
};