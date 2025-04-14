#pragma once

#include <string>
#include <unordered_map>
#include <vector>



// Orgenized way to keep system call's Info
struct SyscallInfo {
    std::string name;
    int number; // even though the register is long (unsigned) the syscall number will be small
    long returnValue;
    bool success;
    std::string errorType; // Only valid if returnValue < 0
};

class Tracer{
public:
    // Start to trace the child process's system calls
    bool startTrace(char* const args[]);

    // Return the log (const ref)
    const std::vector<SyscallInfo>& getLog() const;

private:
    // A db contain all the system calls the traced program made
    std::vector<SyscallInfo> log;
    
    // To quickly map system calls entry number to their name
    const std::unordered_map<int, std::string> syscallNames = {
        {0, "read"},
        {1, "write"},
        {2, "open"},
        {3, "close"},
        {9, "mmap"},
        {12, "brk"},
        {17, "getpid"},
        {21, "access"},
        {59, "execve"},
        {230, "clock_nanosleep"},
        {257, "openat"},
        {262, "newfstatat"},
        {318, "rseq"},
        {334, "futex"},
        // Add more as needed, or automate it later
    };

    // To quickly map system calls error number to their name
    const std::unordered_map<int, std::string> errnoNames = {
        {1, "EPERM"},
        {2, "ENOENT"},
        {5, "EIO"},
        {9, "EBADF"},
        {11, "EAGAIN"},
        {13, "EACCES"},
        {17, "EEXIST"},
        {22, "EINVAL"},
        {28, "ENOSPC"},
        {32, "EPIPE"},
        {38, "ENOSYS"},
        {110, "ETIMEDOUT"},
        // Add more as needed
    };

    // Fills the sysCallInfo struct with the current syscall information
    void fillSysCallInfo(SyscallInfo& syscallInfo, int number, std::string name, long returnValue) const;
};