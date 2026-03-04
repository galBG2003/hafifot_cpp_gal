#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "ComplexCalculator.h"

class Calculator
{
public:
	/**
	 * @explanation Starts the calculator - displays the main menu, creates
	 *        the user choice of calculator type (Basic, Advanced, Complex) and start the session..
	 */
	void turnOn();
private:
	/**
	 * @explanation Manages a session for a specific calculator type.
	 *        Lets the user choose between console input, file input, or returning to the menu.
	 * @param mode Display name of the calculator type (e.g. "Basic", "Advanced").
	 * @param calc points to The calculator object to use for this session.
	 */
	template<typename T>
	void runSession(const std::string& mode, std::unique_ptr<BasicCalculator<T>> calc);
	/**
	 * @explanation Reads expressions from the console, solves them and prints results.
	 * @param mode Display name of the calculator type.
	 * @param calc points to The calculator object to use.
	 */
	template<typename T>
	void solveFromConsol(const std::string& mode, std::unique_ptr<BasicCalculator<T>>& calc);
	/**
	 * @explanation Opens an input file and asks the user whether to print
	 *        results to the console or to an output file.
	 * @param mode Display name of the calculator type.
	 * @param calc points to The calculator object to use.
	 */
	template<typename T>
	void solveFromFile(const std::string& mode, std::unique_ptr<BasicCalculator<T>>& calc);
	/**
	 * @explanation writes a result to the given output stream.
	 * @param result The computed result.
	 * @param out The output stream to write to (e.g. std::cout or a file stream).
	 */
	template<typename T>
	void printResult(const T& result, std::ostream& out);
	// Specific for std::complex<float> to print in a+bi notation.
	void printResult(const std::complex<float>& r, std::ostream& out);
	/**
	 * @explanation Reads expressions from a file and prints results to the console.
	 * @param file The open input file.
	 * @param calc points to The calculator object to use.
	 */
	template<typename T>
	void printToConsol(std::ifstream& file, std::unique_ptr<BasicCalculator<T>>& calc);
	/**
	 * @explanation Asks for an output file path and writes results to it.
	 * @param file The open input file.
	 * @param calc points to The calculator object to use.
	 */
	template<typename T>
	void printToFile(std::ifstream& file, std::unique_ptr<BasicCalculator<T>>& calc);
	/**
	 * @explanation Removes the carriage '\r' from a string.
	 *        Needed because Windows text files use \r\n line endings
	 *        and std::getline only removes the \n.
	 * @param line The line to clean.
	 * @return The line without the trailing '\r', or unchanged if not present.
	 */
	static std::string stripCarriageReturn(const std::string& line);
	template<typename T>
	/**
	 * @explanation removes '\r', solves a single expression line and writes
	 *        "expression = result" to the given output stream.
	 * @param rawLine The raw line as read from the file.
	 * @param calc points to The calculator object to use.
	 * @param out The output stream to write to(file or consul(std::cout)).
	 */
	void processLine(const std::string& raw, std::unique_ptr<BasicCalculator<T>>& calc, std::ostream& out);
	template<typename T>
	/**
	 * @explanation Reads an input file line by line and calls processLine for each.
	 * @param file The open input file.
	 * @param points to The calculator object to use.
	 * @param out The output stream to write results to((file or consul(std::cout)).
	 */
	void processFile(std::ifstream& file, std::unique_ptr<BasicCalculator<T>>& calc, std::ostream& out);
};

