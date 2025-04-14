#include "Tracer.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp> // For safer cleaner conversion to jsson format (works best with the python presentation)

#define OUTPUT_JSON_PATH "/tmp/syscalls_log.json" // Write to temp for now, always safe


// Save the logged syscalls to a json file at path
void dumpToJson(const std::vector<SyscallInfo>& log, const std::string& path) {
    nlohmann::json j;
    for (const auto& sysCallInfo : log) {
        j.push_back({
            {"name", sysCallInfo.name},
            {"number", sysCallInfo.number},
            {"returnValue", sysCallInfo.returnValue},
            {"success", sysCallInfo.success},
            {"errorType", sysCallInfo.errorType}
        });
    }
    std::ofstream out(path);
    out << j.dump(5);
}

int main(int argc, char* argv[]) {
    if (argc < 2) { // Not enough arguments calling the tracer (no program to trace)
        std::cerr << "Usage: " << argv[0] << " <program-to-trace> [args...]" << std::endl;
        return 1;
    }

    Tracer tracer;
    if (!tracer.startTrace(argv + 1)) {// Calls tracer with the args of the program we wan to trace
        std::cerr << "Tracing failed" << std::endl;
        return 1;
    }
    
    // Keep the output in json file to allow statistics presentation with python
    dumpToJson(tracer.getLog(), OUTPUT_JSON_PATH);
    std::cout << "Syscall log saved to: " << OUTPUT_JSON_PATH << std::endl;
    return 0;
}

