#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <map>

struct variable_info
{
    int line_number;
    std::string name;
    bool used;
};

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        std::ifstream file_handle(argv[1]);
        std::string line;
        std::map<std::string, variable_info> variable_map;
        int lineNumber = 0;

        while (getline(file_handle, line))
        {
            lineNumber++;
            std::smatch match;
            std::regex reg("(let|var|const)\\s([a-z:A-Z:1-9]+)(;|\\s=)");
            if (std::regex_search(line, match, reg))
            {
                variable_info info = {lineNumber, match[2], false};
                variable_map.insert(std::pair<std::string, variable_info>(match[2], info));
            }
        }

        file_handle.clear();
        file_handle.seekg(0, std::ios::beg);

        while (getline(file_handle, line))
        {
            std::smatch match;
            std::regex reg("[\\s]?([a-z:A-Z:1-9]+)\\s=");
            if (std::regex_search(line, match, reg))
            {
                variable_map[match[1]].used = true;
            }
        }

        for (auto const &kv : variable_map) // or with iterator (auto it = variable_map.begin(); it != variable_map.end(); it++) then use '->' instead of '.'
        {
            //std::cout << kv.first << " => " << kv.second.name << " : line " << kv.second.line_number << " used = " << kv.second.used << '\n';
            auto item = kv.second;
            if (!item.used)
            {
                std::cout << "Variable " << item.name << " not used. Declared at line number " << item.line_number << '\n';
            }
        }
        
        file_handle.close();
    }

    return 0;
}