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
        std::cout << "choose 1 to enter expression, 2 to enter path and 0 to return the menu:";
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 1)
            solveFromConsol(mode, calc);
        else if (choice == 2)
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
            printResult(calc->solve(input));
            std::cout << "\n";
        }
        catch (const std::exception& e) { std::cerr << "Error: " << e.what() << "\n"; }
    }
}

template<typename T>
void Calculator::solveFromFile(const std::string& mode, std::unique_ptr<BasicCalculator<T>>& calc) {
    std::cout << "File path: ";
    std::string path;
    std::getline(std::cin, path);
    std::ifstream file(path);
    if (!file.is_open()) { std::cerr << "Cannot open: " << path << "\n"; return; }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            std::cout << line << " = ";
            printResult(calc->solve(line));
            std::cout << "\n";
        }
        catch (const std::exception& e) { std::cerr << line << " -> Error: " << e.what() << "\n"; }
    }
}

template<typename T>
void Calculator::printResult(const T& result) {
    std::cout << result;
}

void Calculator::printResult(const std::complex<float>& r) {
    std::cout << r.real() << (r.imag() >= 0 ? "+" : "") << r.imag() << "i";
}
