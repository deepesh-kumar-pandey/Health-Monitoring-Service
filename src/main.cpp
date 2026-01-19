#include "../include/Monitor.h"
#include "../include/Config.h"
#include<iostream>
#include<string>
#include<limits>

int main() {
    std::string secret_key = Config::get_encryption_key();
    float threshold;
    std::string log_file;
    int interval;

    std::cout <<"-------------------------------------------\n";
    std::cout << "      DEEP GUARD: SYSTEM SETUP         " << "\n";
    std::cout <<"-------------------------------------------\n";

    std::cout << "[1/3] Enter CPU Load Threshold (e.g. 0.75 for 75%): ";
    while (!(std::cin >> threshold) || threshold < 0 || threshold > 1.0) {
        std::cout << "Invalid input. Please enter a positive number between 0.0 and 1.0: ";
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}