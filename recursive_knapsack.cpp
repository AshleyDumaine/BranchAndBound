#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>

int recursive_knapsack(int idx, int cap, std::vector<std::vector<int> > contents, std::vector<std::vector<int> > input_list) {
    if (cap == 0 || idx == input_list.size() - 1) {
        return 0;
    }
    else if (cap - input_list.at(idx).at(2) < 0) {
        return recursive_knapsack(idx+1, cap, contents, input_list);
    }
    else {
	contents.push_back(input_list.at(idx));
        return std::max(recursive_knapsack(idx+1, cap, contents, input_list), recursive_knapsack(idx+1, cap-input_list.at(idx).at(2), contents, input_list) + input_list.at(idx).at(1));
    }
}

int main(int argc, char *argv[]) {
    clock_t start = clock();
    int num_items = 0;
    int cap = 0;
    std::vector<std::vector<int> > contents;
    
    // read input
    std::ifstream file;
    file.open(argv[1]);
    if(file.is_open()) {
	std::string str_line;
        getline(file, str_line);
	num_items = atoi(str_line.c_str());
        std::vector<std::vector<int> > input_list(num_items); // 2d array for items to pick from
        for (int i = 0; i < num_items; i++) {
	    getline(file, str_line);
            std::stringstream ss(str_line);
            int num;
            while (ss >> num) {
                input_list[i].push_back(num);
            }
        }
        getline(file, str_line);
	cap = atoi(str_line.c_str());
	std::cout.precision(6);
	std::cout << std::fixed;
        std::cout << "optimal value: " << recursive_knapsack(0, cap, contents, input_list) << std::endl; 
	std::cout << "optimality: 1" << std::endl;
	std::cout << "time to solve: " << double(clock() - start) / (CLOCKS_PER_SEC) << " seconds" << std::endl;
    	file.close();
    }
}
