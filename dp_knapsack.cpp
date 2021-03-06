#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
 
int dp_knapsack(int weight, int num, std::vector<std::vector<int> > input_list) {
    std::vector<std::vector<int> > table(num+1, std::vector<int>(weight+1,0));
    int i, j;
    for (i = 0; i <= num; i++) {
	for (j = 0; j <= weight; j++) {
		if (i == 0 || j == 0) { table.at(i).at(j) = 0; }
		else if (input_list.at(i - 1).at(2) <= j) {
		 table.at(i).at(j) = std::max(input_list.at(i - 1).at(1) + 
			table.at(i - 1).at(j - input_list.at(i - 1).at(2)),
			table.at(i - 1).at(j));
		}
		else table.at(i).at(j) = table.at(i - 1).at(j);
	}
    }
    return table.at(num).at(weight);
}

int main(int argc, char *argv[]) {
    clock_t start = clock();
    int num_items = 0;
    int cap = 0;
    
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
        std::cout << "optimal value: " << dp_knapsack(cap, num_items, input_list) << std::endl; 
	std::cout << "optimality: 1" << std::endl;
	std::cout << "time to solve: " << double(clock() - start) / (CLOCKS_PER_SEC) << " seconds" << std::endl;
    	file.close();
    }
}
