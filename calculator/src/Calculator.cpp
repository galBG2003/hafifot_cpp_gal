#include "..\include\Calculator.h"

void Calculator::turnOn() {
	bool running = true;
	while (running) {
		std::cout << "\n--- Calculator Menu ---\n";
		std::cout << "1. Basic (int)\n2. Advanced (float)\n3. Complex (std::complex<float>)\n4. Exit\n";
		std::cout << "Choose (1-4): ";
		int choice;
		std::cin >> choice;
		std::cin.ignore();
		if (choice == 4) {
			running = false;
			break;
		}
		switch (choice) {
		case 1: runSession<int>("Basic", std::make_unique<BasicCalculator<int>>()); break;
		case 2: runSession<float>("Advanced", std::make_unique<AdvancedCalculator<float>>()); break;
		case 3: runSession<std::complex<float>>("Complex", std::make_unique<ComplexCalculator<std::complex<float>>>()); break;
		default: std::cout << "Invalid choice.\n"; break;
		}
	}
}

template<typename T>
void Calculator::runSession(const std::string& mode, std::unique_ptr<BasicCalculator<T>> calc) {
	int choice;
	while (true) {
		std::cout << "choose 0 to enter expression, 1 to enter path and 2 to return the menu:";
		std::cin >> choice;
		std::cin.ignore();
		if (choice == 0)
			solveFromConsol(mode, calc);
		else if (choice == 1)
			solveFromFile(mode, calc);
		else
			break;
	}
}

template<typename T>
void Calculator::solveFromConsol(const std::string& mode, std::unique_ptr<BasicCalculator<T>>& calc) {
	std::string input;
	while (true) {
		std::cout << "\n[" << mode << "] Expression (0 to go back): ";
		if (!std::getline(std::cin, input) || input == "0") break;
		try {
			printResult(calc->solve(input), std::cout);
			std::cout << "\n";
		}
		catch (const std::exception& e) { std::cerr << "Error: " << e.what() << "\n"; }
	}
}

template<typename T>
void Calculator::solveFromFile(const std::string& mode, std::unique_ptr<BasicCalculator<T>>& calc) {
	std::cout << "Enter input file path: ";
	std::string path;
	std::getline(std::cin, path);
	std::ifstream file(path);
	if (!file.is_open()) { std::cerr << "Cannot open: " << path << "\n"; return; }
	std::cout << "Enter 1 to print the result to console and 2 to a file and 3 for both: ";
	int choice;
	std::cin >> choice;
	std::cin.ignore();
	if (choice == 1)
		printToConsol(file, calc);
	else if (choice == 2)
		printToFile(file, calc);
	else if (choice == 3) {
		printToConsol(file, calc);
		printToFile(file, calc);
	}
	else
		return;
}

std::string Calculator::stripCarriageReturn(const std::string& line) {
	if (!line.empty() && line.back() == '\r')
		return line.substr(0, line.size() - 1);
	return line;
}

template<typename T>
void Calculator::processLine(const std::string& rawLine, std::unique_ptr<BasicCalculator<T>>& calc, std::ostream& out) {
	std::string line = stripCarriageReturn(rawLine);
	if (line.empty()) return;
	try {
		out << line << " = ";
		printResult(calc->solve(line), out);
		out << "\n";
	}
	catch (const std::exception& e) {
		std::cerr << line << " -> Error: " << e.what() << "\n";
	}
}

template<typename T>
void Calculator::processFile(std::ifstream& file, std::unique_ptr<BasicCalculator<T>>& calc, std::ostream& out) {
	std::string line;
	while (std::getline(file, line))
		processLine(line, calc, out);
}

template<typename T>
void Calculator::printToConsol(std::ifstream& file, std::unique_ptr<BasicCalculator<T>>& calc) {
	processFile(file, calc, std::cout);
}

template<typename T>
void Calculator::printToFile(std::ifstream& file, std::unique_ptr<BasicCalculator<T>>& calc) {
	std::cout << "Enter output file path: ";
	std::string path;
	std::cin >> path;
	std::cin.ignore();
	std::ofstream outFile(path, std::ios::app);
	if (!outFile) throw std::runtime_error("Failed to open output file: " + path);
	processFile(file, calc, outFile);
}

template<typename T>
void Calculator::printResult(const T& result, std::ostream& out) {
	out << result;
}

void Calculator::printResult(const std::complex<float>& r, std::ostream& out) {
	out << r.real() << (r.imag() >= 0 ? "+" : "") << r.imag() << "i";
}