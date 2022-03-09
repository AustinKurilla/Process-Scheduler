#include "estring.h"

// Some quality of life string stuff that brings functionality closer to higher level languages.
// - Jack Happe

// Returns a vector of <string> based on delimiter <char> provided.
std::vector<std::string> split(std::string old, char delim) {
    std::vector<std::string> str_vector;
    size_t curr_start = 0;
    size_t curr_end = 0;
    // Get the first character match index
    //std::cout << old << "\n";
    while (curr_start != -1) {
        curr_end = old.find(delim, curr_start);
        if (curr_end != old.npos) {
            //std::cout << "Splitting at " << curr_start << " and " << curr_end << "\n";
            str_vector.push_back(old.substr(curr_start, curr_end - curr_start));
            curr_start = curr_end + 1;
        } else {
            str_vector.push_back(old.substr(curr_start, old.npos));
            curr_start = -1;
        }
    }
    return str_vector;
}

void print_str_vec(std::vector<std::string> new_vec) {
    for (int i = 0; i < new_vec.size(); i++) {
        std::cout << "["<< new_vec.at(i) << "] ";
    }
    std::cout << "\n";
}