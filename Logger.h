#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <chrono>

class Logger
{
public:
    // Function to convert time_point to a formatted string
    std::string TIME(const std::chrono::high_resolution_clock::time_point& tp)
    {
        auto time_t = std::chrono::high_resolution_clock::to_time_t(tp);
        std::tm* tm = std::localtime(&time_t); // Convert to local time
        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S", tm);
        return std::string(buffer);
    }
    
    template<typename... Args>
    void DEBUGT(Args... args)
    {
        std::ofstream debugfile("outputs/debug.txt", std::ios::app);
        std::ostringstream oss;
        (oss << ... << args);  // Use fold expression to stream all arguments
        debugfile << "[" << TIME(std::chrono::high_resolution_clock::now()) << "] " << oss.str() << "\n";
        debugfile.close();
    }

    template<typename... Args>
    void OUTPUTT(Args... args)
    {
        
        std::ofstream outputfile("outputs/log.txt", std::ios::app);
        std::ostringstream oss;
        (oss << ... << args);  // Stream all arguments
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