#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Function to read a file into a vector of lines
std::vector<std::string> readFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

// Function to detect trivial conflicts
bool isTrivialConflict(const std::vector<std::string>& baseLines, const std::vector<std::string>& localLines, const std::vector<std::string>& remoteLines) {
    // Implement your conflict detection algorithm here
    // For example, you can use a simple diffing algorithm
    // to detect changes in the local and remote files
    for (size_t i = 0; i < baseLines.size(); ++i) {
        if (localLines[i] != baseLines[i] && remoteLines[i] != baseLines[i]) {
            // If both local and remote files have changed the same line,
            // it's likely a trivial conflict
            return true;
        }
    }
    return false;
}

// Function to resolve trivial conflicts
std::vector<std::string> resolveTrivialConflict(const std::vector<std::string>& baseLines, const std::vector<std::string>& localLines, const std::vector<std::string>& remoteLines) {
    // Implement your conflict resolution algorithm here
    // For example, you can take the changes from both local and remote files
    std::vector<std::string> resolvedLines;
    for (size_t i = 0; i < baseLines.size(); ++i) {
        if (localLines[i] != baseLines[i]) {
            resolvedLines.push_back(localLines[i]);
        } else if (remoteLines[i] != baseLines[i]) {
            resolvedLines.push_back(remoteLines[i]);
        } else {
            resolvedLines.push_back(baseLines[i]);
        }
    }
    return resolvedLines;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " base local remote merged" << std::endl;
        return 1;
    }

    std::string baseFile = argv[1];
    std::string localFile = argv[2];
    std::string remoteFile = argv[3];
    std::string mergedFile = argv[4];

    // Read the files
    std::vector<std::string> baseLines = readFile(baseFile);
    std::vector<std::string> localLines = readFile(localFile);
    std::vector<std::string> remoteLines = readFile(remoteFile);

    // Detect trivial conflicts
    if (isTrivialConflict(baseLines, localLines, remoteLines)) {
        // Resolve the conflict
        std::vector<std::string> resolvedLines = resolveTrivialConflict(baseLines, localLines, remoteLines);

        // Write the resolved conflict to the merged file
        std::ofstream mergedFileStream(mergedFile);
        for (const auto& line : resolvedLines) {
            mergedFileStream << line << std::endl;
        }
    } else {
        // Leave the conflict unresolved
        std::ofstream mergedFileStream(mergedFile);
        mergedFileStream << "<<<<<<< " << localFile << std::endl;
        for (const auto& line : localLines) {
            mergedFileStream << line << std::endl;
        }
        mergedFileStream << "=======" << std::endl;
        for (const auto& line : remoteLines) {
            mergedFileStream << line << std::endl;
        }
        mergedFileStream << ">>>>>>> " << remoteFile << std::endl;
    }

    return 0;
}