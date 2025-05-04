#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>

class Logger {
public:
    int filenum = 0;
    std::string TIME(const std::chrono::high_resolution_clock::time_point& tp) {
        auto time_t = std::chrono::high_resolution_clock::to_time_t(tp);
        std::tm* tm = std::localtime(&time_t);
        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S", tm);
        return std::string(buffer);
    }

    template<typename... Args>
    void DEBUGT(Args... args) {
        std::ofstream debugfile("outputs/debug.txt", std::ios::app);
        std::ostringstream oss;
        (oss << ... << args);
        debugfile << "[" << TIME(std::chrono::high_resolution_clock::now()) << "] " << oss.str() << "\n";
        debugfile.close();
    }

    template<typename... Args>
    void OUTPUTT(Args... args) {
        std::string filename = "outputs/log" + std::to_string(filenum) + ".txt";
        std::ofstream outputfile(filename, std::ios::app);
        std::ostringstream oss;
        (oss << ... << args);
        outputfile << "[" << TIME(std::chrono::high_resolution_clock::now()) << "] " << oss.str() << "\n";
        outputfile.close();
    }
};

#endif


// template<typename... Args>
// void DEBUG(Args... args)
// {
//     std::string filename = "debugs/debug.txt";
//     std::ofstream debugfile(filename, std::ios::app);
//     std::ostringstream oss;
//     (oss << ... << args);  // Use fold expression to stream all arguments
//     debugfile << oss.str() << "\n";
//     debugfile.close();
// }

// template<typename... Args>
// void OUTPUT(Args... args)
// {
//     std::string filename = "debugs/debug.txt";
//     std::ofstream outputfile(filename, std::ios::app);
//     std::ostringstream oss;
//     (oss << ... << args);  // Stream all arguments
//     outputfile << oss.str() << "\n";
//     outputfile.close();
// }