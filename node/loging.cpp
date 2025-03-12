
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
class Logger
{
public:
    std::ofstream logFile;
    bool newline = true;
    Logger(const std::string &filename)
    {
        logFile.open(filename, std::ios::app); // Open in append mode
    }

    ~Logger()
    {
        if (logFile.is_open())
        {
            logFile.close();
        }
    }

    template <typename T>
    Logger &operator<<(const T &message)
    {
        if (newline)
        {
            // Prefix with timestamp
            newline=false;
            auto now = std::chrono::system_clock::now();
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);
            std::tm local_time = *std::localtime(&now_time);
            std::cout << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "] ";
            if (logFile.is_open())
            {
                logFile << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "] ";
            }
        }
        std::cout<<message;
        if(logFile.is_open()){
            logFile<<message;
        }

        return *this;
    }

    // Handle std::endl
    Logger &operator<<(std::ostream &(*func)(std::ostream &))
    {
        func(std::cout);
        newline = true;
        if (logFile.is_open())
        {
            func(logFile);
        }
        return *this;
    }
};

int main()
{
    Logger log = Logger("log.txt");
    log << "hi " << 5 << std::endl;
}