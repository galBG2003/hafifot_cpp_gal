#pragma once
#include "..\include\BasicCalculator.h"

template<typename T>
class AdvancedCalculator : public BasicCalculator<T>
{
public:
    // Registers "^" and "sqrt" directly
    AdvancedCalculator();

protected:
    virtual T power(T a, T b);
    virtual T sqrtOp(T a);

    /**
     * @explanation Reads a float literal (digits + decimal point) from the expression.
     * @param expression The full expression string being parsed.
     * @param index Current position, pointing at the first digit.
     * @return The parsed number as type T.
     */
    T parseValue(const std::string& expression, int& index) override;
};

