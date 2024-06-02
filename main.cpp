#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // For std::remove_if
#include <cctype>    // For std::isspace
#include <conio.h> // for _getch()

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

std::string toLowerString(const std::string& str) {
    std::string result;
    result.reserve(str.size());

    for (char c : str) {
        result += std::tolower(static_cast<unsigned char>(c)); // Convert each character to lowercase
    }

    return result;
}

int main() {
    std::string inputFileName, outputFileName;
    std::cout << "Provide the input file name: ";
    std::cin >> inputFileName;
    inputFileName += ".txt";
    std::cout << "Provide the output file name (will be created if not existing): ";
    std::cin >> outputFileName;
    outputFileName += ".txt";

    std::ifstream file(inputFileName, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file. This may be due to missing the file with provided name.\n Keep in mind that"
                     "file extension should not be passed and is allways set to .txt" << std::endl;
        return 1;
    }

    std::vector<std::pair<std::string, std::string>> data;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value1, value2;

        // Read the first value
        if (std::getline(ss, value1, ';')) {
            value1 = trim(value1);
        }

        // Read the second value
        if (std::getline(ss, value2, ';')) {
            value2 = trim(value2);
        }

        // Debug output to trace values being read
        std::cout << "Read values: [" << value1 << "] and [" << value2 << "]" << std::endl;

        // Store the values in a pair and add to the vector
        data.push_back(std::make_pair(value1, value2));
    }

    file.close();

    // Output the contents of the vector
    std::vector<std::string> output;
    for (const auto& t : data) {
        if (t.first[0] == '@') {
            std::string trimmed = t.first.substr(1, t.first.length()-1);
            std::string heading = "<h1 class=\"wp-block-heading\" id=\"h-" + toLowerString(trimmed) + "\">" + trimmed + "</h2> \n";
            output.push_back(heading);
        } else if (t.second.substr(0, 5) == "https") {
            std::string linkedText = "<a href=\"" + t.second + "\" target=\"_blank\">" + t.first + "</a> <br /><br />\n";
            output.push_back(linkedText);
        } else if (t.second[0] == '&') {
            std::string notAvailableText = t.first + " (Jeszcze niedostępne, sprawdź ponownie za jakiś czas!) <br /><br />\n";
            output.push_back(notAvailableText);
        }
    }

    for (const auto& t : output) {
        std::cout << t << std::endl;
    }

    std::ofstream outputFile(outputFileName);
    if (outputFile.is_open()) {
        for (const auto& line : output) {
            outputFile << line << std::endl;
        }
        outputFile.close();
        std::cout << "Output saved to " + outputFileName << std::endl;
    } else {
        std::cerr << "Failed to open output file for writing." << std::endl;
        return 1;
    }

    char close;
    std::cout << "Press any key to close...\n";
    _getch();

    return 0;
}
