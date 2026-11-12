//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#ifndef LOGGER_H
#define LOGGER_H

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// Implement the following methods:
// Log
// Flush
// Message format is "2024-08-30 14:48:16 Starting game..."

class Logger {
   public:
    static Logger& GetInstance();


    //No copy cause its singleton
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    ~Logger();

    void Log(const std::string& message);
    void Flush();

   private:
    Logger();

    std::ofstream logFile;                  // touched only by the worker thread
    std::mutex queueMutex;                  // guards everything below it
    std::condition_variable workAvailable;  // worker waits on this
    std::condition_variable queueDrained;   // Flush() waits on this
    std::vector<std::string> logQueue;      // lines waiting to be written
    bool exitFlag = false;                  // set by the destructor
    bool writing = false;                   // worker is writing a batch
    std::thread logThread;

    void ProcessLogs();
    std::string GetCurrentTimestamp();
};

#endif  // LOGGER_H
