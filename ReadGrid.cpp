// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint> // For uint32_t

struct Coor {
    uint32_t x;
    uint32_t y;
};

class ReadGrid {
    private:

        //grid[row * grid_size + col] for index
        std::vector<Coor> previous_sqs;
        std::vector<char> grid;
        Coor start;
        uint32_t grid_size;
    
    public:
    
        explicit ReadGrid(std::istream &input) : grid_size(0) {

            std::string str;
            uint32_t row = 0;
            char map_mode = 'U';
            bool has_start = false;
            bool has_treasure = false;

            while (getline(input,str)){

                if (str.length() == 0){ //ignore blank lines
                    continue;
                }

                else if (map_mode != 'U' && grid_size != 0){ 
                //if map mode and grid_size are defined, read map

                    if (map_mode == 'M'){ //figure out mode

                        if (row == grid_size) { //grid too tall
                            exit(1);
                        }
                        if (str.length() != grid_size) { //grid too wide or too short
                            exit(1);
                        }

                        for (uint32_t col=0; col<grid_size; col++){

                            char t = str[col];
                            if (t != '@' && t != '.' && t != 'o'
                                && t != '$' && t != '#'){
                                std::cerr << "Invalid terrain type\n" << std::flush;
                                exit(1);
                            }

                            grid[row*grid_size + col] = t;

                            if (t == '@'){
                                if (!has_start){
                                    start.x = row;
                                    start.y = col;
                                    has_start = true;
                                } else {
                                    exit(1);
                                }
                            } else if (t == '$'){
                                if (!has_treasure){
                                    has_treasure = true;
                                } else {
                                    exit(1);
                                }
                            }
                        } //for every character on each line
                        row++;

                    } //if map in map mode
                    
                    
                    else if (map_mode == 'L'){

                        std::istringstream input_stream(str); 
                        std::string s;
                        input_stream >> s;
                        row = uint32_t(stoi(s));
                        input_stream >> s;
                        uint32_t col = uint32_t(stoi(s));
                        input_stream >> s;

                        if (row >= grid_size || col >= grid_size){
                            std::cerr << "Invalid coordinates in list mode"<< 
                                                    " input\n";
                            exit(1);
                        }

                        char t = s[0];
                            if (t != '@' && t != '.' && t != 'o'
                                && t != '$' && t != '#'){
                                std::cerr << "Invalid terrain type\n";
                                exit(1);
                            }

                        grid[row * grid_size + col] = t;
                        
                        if (t == '@'){
                            start.x = row;
                            start.y = col;
                            has_start = true;
                        } else if (t == '$'){
                            has_treasure = true;
                        }
                    } //if map in list mode

                } //end of reading map

                else if (str.length() != 1 && map_mode == 'U') {
                    if (str[0] == '#') 
                            { continue; }
                    else {exit(1);}
                } else if (map_mode == 'U'){
                    if (str[0] == 'M' || str[0] == 'L'){ //defining map type
                        map_mode = str[0];
                    } else {
                        exit(1);
                    }
                } else { //defining grid size
                    grid_size = uint32_t(stoi(str));
                    //reserve exact space for grid and previous_sqs
                    grid.resize(grid_size * grid_size, '.');
                    previous_sqs.resize(grid_size * grid_size, {0,0});
                }
            } //while line in txt file

            if (has_treasure == false){
                std::cerr << "Map does not have treasure\n" << std::flush;
            }

            if (has_start == false){
                std::cerr << "Map does not have a start location\n" << std::flush;
            }

        } //Grid Constructor

        void print_grid(std::vector<Coor> &path, u_int32_t path_length, std::ostream &os) {
            Coor next;
            Coor current;
            Coor previous;
            char new_char;

            for (; path_length > 0; path_length--){
                next = path[path_length-1];
                current = previous_sqs[next.x * grid_size + next.y];
                previous = previous_sqs[current.x * grid_size + current.y];

                if (current.x == previous.x && current.x == next.x){ //same row
                    new_char = '-';
                } else if (current.y == previous.y && current.y == next.y) {
                    new_char = '|';
                } else {
                    new_char = '+';
                }

                if (grid[current.x * grid_size + current.y] != '@'){
                    grid[current.x * grid_size + current.y] = new_char;
                }

                if ((path_length-1) == 0) {
                    if (current.x == next.x){ //same row
                        new_char = '-';
                    } else if (current.y == next.y) {
                        new_char = '|';
                    }
                    if (grid[next.x * grid_size + next.y] == '$'){
                        grid[next.x * grid_size + next.y] = 'X';
                    } else {
                        grid[next.x * grid_size + next.y] = new_char;
                    }
                    break;
                }

            } //for path_length

            for (uint32_t r = 0; r < grid_size; r++){
                for (uint32_t c = 0; c < grid_size; c++){
                    os << grid[r * grid_size + c];
                } //for c
                os << '\n';
            } //for r

        } //print_grid

        std::vector<Coor> & get_prev_sqs() 
        { return previous_sqs; }
        std::vector<char> & get_grid() { return grid; }
        uint32_t get_size() const { return grid_size; }
        Coor & get_start() { return start; }
};