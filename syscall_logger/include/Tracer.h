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
        // File I/O
        {0, "read"},
        {1, "write"},
        {2, "open"},
        {3, "close"},
        {21, "access"},
        {257, "openat"},
        {262, "newfstatat"},
    
        // Memory
        {9, "mmap"},
        {12, "brk"},
    
        // Process
        {59, "execve"},
        {17, "getpid"},
    
        // Threading
        {334, "futex"},
        {318, "rseq"},
    
        // Timing
        {230, "clock_nanosleep"},
    
        // Networking
        {41, "socket"},
        {42, "connect"},
        {43, "accept"},
        {44, "sendto"},
        {45, "recvfrom"},
        {46, "sendmsg"},
        {47, "recvmsg"},
        {48, "shutdown"},
        {49, "bind"},
        {50, "listen"},
        {51, "getsockname"},
        {52, "getpeername"},
        {53, "socketpair"},
        {54, "setsockopt"},
        {55, "getsockopt"},
    
        // Add more as needed...
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

    // Networking-related errors
    {98, "EADDRINUSE"},      // Address already in use
    {99, "EADDRNOTAVAIL"},   // Cannot assign requested address
    {101, "ENETUNREACH"},    // Network is unreachable
    {102, "ENETRESET"},      // Network dropped connection on reset
    {103, "ECONNABORTED"},   // Software caused connection abort
    {104, "ECONNRESET"},     // Connection reset by peer
    {105, "ENOBUFS"},        // No buffer space available
    {106, "EISCONN"},        // Transport endpoint is already connected
    {107, "ENOTCONN"},       // Transport endpoint is not connected
    {111, "ECONNREFUSED"},   // Connection refused
    {113, "EHOSTUNREACH"},   // No route to host
    {115, "EINPROGRESS"},    // Operation now in progress
    {116, "EALREADY"},       // Operation already in progress
};


    // Fills the sysCallInfo struct with the current syscall information
    void fillSysCallInfo(SyscallInfo& syscallInfo, int number, std::string name, long returnValue) const;
};