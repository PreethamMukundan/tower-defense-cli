//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : logFile("log.txt", std::ios_base::trunc) {
    logThread = std::thread(&Logger::ProcessLogs, this);
}

Logger::~Logger() {
    {
        exitFlag = true;
    }
    if (logThread.joinable()) {
        logThread.join();
    }

}

void Logger::Log(const std::string& message)
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (exitFlag) {
            return;  // shutting down: nothing will write it
        }
        // Timestamp taken under the lock so file order and time order agree.
        logQueue.push_back(GetCurrentTimestamp() + " " + message);
    }
    workAvailable.notify_one();
}

void Logger::Flush()
{
    std::unique_lock<std::mutex> lock(queueMutex);
    // Done when nothing is queued AND the worker isn't mid-batch.
    queueDrained.wait(lock, [this] { return logQueue.empty() && !writing; });
}

void Logger::ProcessLogs() {
    while (true) {
        while (!logQueue.empty()) {
            logFile << logQueue.back() << std::endl;
            logQueue.pop_back();
        }

        if (exitFlag) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

std::string Logger::GetCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}
