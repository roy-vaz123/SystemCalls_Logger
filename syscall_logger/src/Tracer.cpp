#include "Tracer.h"
#include <sys/ptrace.h>// ptrace system call
#include <sys/wait.h> // waitpid to wait for child
#include <sys/types.h> // pid_t
#include <sys/user.h> // user_regs_struct to access registers
#include <unistd.h> // fork, execvp
#include <iostream>
#include <signal.h> // kill, SIGSTOP
#include <cerrno> // To keep error number in case of error



// A helper function that help to extrat string from int, string const unordered map (no[])
std::string getStringFromMap(const std::unordered_map<int, std::string>& map, int num){
    if(map.find(num) != map.end())
        return map.at(num);
    return "";
}

bool Tracer::startTrace(char* const argv[]){
    pid_t child = fork(); // Create a child process

    if (child == 0) { // Child process (the one well trace)
        ptrace(PTRACE_TRACEME, 0, nullptr, nullptr); // Request tracing by parent
        kill(getpid(), SIGSTOP); // Stop ourselves until the parent is ready

        execvp(argv[0], argv); // Replace this process image with target program
        // If execvp fails:
        perror("execvp failed");
        return false;
    
    
    } else if (child > 0) {// Parent proccess (the tracer)
        int status = 0;
        waitpid(child, &status, 0); // Wait for child to send SIGSTOP (first wait, the child and parent wait for each other to prepare)
        
        // Tells kernel When a syscall stop happens, tag it with 0x80 (bit 7 set)
        if (ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD) == -1) {
            perror("PTRACE_SETOPTIONS failed");
        }

        // Will be used to extract the syscall number and return value from the cpu regiser
        struct user_regs_struct regs;
         
        // Keep syscall info in the first iteration of that syscall (entering kernel)
        std::string sysCallName;
        int sysCallNumber;        
        
        bool inSyscall = false;// To seperate syscalls enrty and return values

        // Start tracing syscalls — child will now stop at every syscall entry and exit
        ptrace(PTRACE_SYSCALL, child, nullptr, nullptr);

        // run until child exits
        while (true) {
            waitpid(child, &status, 0); // Wait until child stops at next syscall or signal
        
            if (WIFEXITED(status)) {
                // Child has exited, stop tracing
                break;
            }
        
            if (WIFSIGNALED(status)) { // If child was killed by a signal
                std::cerr << "Child killed by signal: " << WTERMSIG(status) << "\n";
                break;
            }
        
            if (WIFSTOPPED(status)) { // Stopped by a signal or syscall
                int sig = WSTOPSIG(status); // Extract the signal number
        
                std::cout << "Stopped: sig=" << sig << " raw status=" << std::hex << status << std::dec << "\n";
        
                if (sig == (SIGTRAP | 0x80)) { //  = 133, sigstop with the syscall mark value
                    ptrace(PTRACE_GETREGS, child, nullptr, &regs); // Get the CPU registers of the child
        
                    if (!inSyscall) { // Syscall entry
                        
                        std::cout << "Syscall entry: " << regs.orig_rax << std::endl; // debugging
                        sysCallNumber = regs.orig_rax; // Save syscall number
                        sysCallName = getStringFromMap(syscallNames, sysCallNumber); // Save syscall name
                        inSyscall = true;
                    
                    } else { // Syscall return
                        std::cout << "Return value: " << regs.rax << std::endl; // debugging
                        
                        SyscallInfo sysCallInfo;
                        fillSysCallInfo(sysCallInfo, sysCallNumber, sysCallName, static_cast<long>(regs.rax)); // Fill info
                        if (!sysCallInfo.name.empty()) {
                            log.push_back(sysCallInfo); // Save to log
                        }
                        inSyscall = false;
                    }
        
                    // Continue to next syscall event (entry or exit)
                    ptrace(PTRACE_SYSCALL, child, nullptr, nullptr);
                } else if ((sig & 0x7f) == SIGTRAP) { // If it's a plain SIGTRAP (not syscall)
                    
                    // Dont forward SIGTRAP to child
                    ptrace(PTRACE_SYSCALL, child, nullptr, nullptr);
                }else {
                    // Forward real signals
                    ptrace(PTRACE_SYSCALL, child, nullptr, sig);
                }
            }
        }
        

        return true;
    }

    // If fork failed
    return false;
}

// Return the log (const ref)
const std::vector<SyscallInfo>& Tracer::getLog() const{
    return log;
}

// Fills the sysCallInfo struct with the current syscall information
void Tracer::fillSysCallInfo(SyscallInfo& syscallInfo, int number, std::string name, long returnValue) const{
    syscallInfo.number = number;
    syscallInfo.name = name;
    syscallInfo.success = returnValue >= 0;
    
    std::cout << "return value in log"<< returnValue << std::endl; // debugging
    
    if(!syscallInfo.success){// If call wasnt successful keep the error number
        int errnum = -static_cast<int>(returnValue); // convert to positive errno, even though the return is long, itll be a small number
        syscallInfo.errorType = getStringFromMap(errnoNames, errnum);
        if(syscallInfo.errorType.empty())// Failed to extract error number
            syscallInfo.errorType = "unknown error";
        return;
    }
    syscallInfo.errorType = "";
}

