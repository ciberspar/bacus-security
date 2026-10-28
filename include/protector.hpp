
#ifndef BACUS_PROTECTOR_HPP
#define BACUS_PROTECTOR_HPP

#include <string>
#include <vector>

namespace bacus {

struct CheckResult {
    std::string name;
    std::string status;
    std::string details;
};

std::string version();

std::vector<CheckResult> audit_system();
std::vector<CheckResult> audit_ports();
std::vector<CheckResult> audit_tools();
std::vector<CheckResult> scan_file(const std::string& path);

} // namespace bacus

#endif
