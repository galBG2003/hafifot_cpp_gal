#pragma once
#include "ComplexCalculator.h"
#include <fstream >
class Calculator
{
public:
	void turnOn();
private:
	template<typename T>
	void runSession(const std::string& mode, std::unique_ptr<BasicCalculator<T>> calc);
	template<typename T>
	void solveFromConsol(const std::string& mode, std::unique_ptr<BasicCalculator<T>>& calc);
	template<typename T>
	void solveFromFile(const std::string& mode, std::unique_ptr<BasicCalculator<T>>& calc);
	template<typename T>
	void printResult(const T& result);
	void printResult(const std::complex<float>& r);
};
