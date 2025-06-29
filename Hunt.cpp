// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include "ReadGrid.cpp"
#include <deque>

class Hunt {

    private:
        ReadGrid &map;
        std::deque<Coor> first_mate;
        std::deque<Coor> captain;
        std::vector<bool> discovered_flags;
        std::string &hunt_order;
        Coor current_check; //empty if not yet found          
        uint32_t grid_size;  
        uint32_t water_disc_count;
        uint32_t land_disc_count;
        uint32_t ashore_count;
        char cp;
        char fm;
    
    public:

        Hunt(ReadGrid &m, std::string &h, char c, char f): map(m),hunt_order(h),
            current_check({0,0}), water_disc_count(0), 
            land_disc_count(0), ashore_count(0),cp(c),fm(f){
            //@ means path length starts at 1

            grid_size = map.get_size();
            discovered_flags.resize(grid_size*grid_size, false);
            
            Coor start = map.get_start();
            captain.emplace_back(start);
            discovered_flags[start.x * grid_size + start.y] = true;
        }

        bool invstg_loop(Coor c, bool is_captain, std::ostream &os){
            
            if (is_captain) {
                cp == 'S' ? captain.pop_back() : captain.pop_front();
                water_disc_count++;
            } else {
                fm == 'S' ? first_mate.pop_back() : first_mate.pop_front();
                land_disc_count++;
            }

            for (char direction: hunt_order) {
                current_check = c;

                switch(direction){
                    case 'N':
                        if (c.x >= 1){
                            c.x--;
                            //check if already discovered
                            if (discovered_flags[c.x * grid_size + c.y] == false) {
                                if(discvr_sq(c.x, c.y, is_captain, os)){
                                    return true;
                                }
                            }
                            c.x++;
                        }
                        break;
                    case 'E':
                        if ((++c.y) < map.get_size()){
                            if (discovered_flags[c.x * grid_size + c.y] == false) {
                                if(discvr_sq(c.x, c.y, is_captain, os)){
                                    return true;
                                }
                            }
                        }
                        c.y--;
                        break;
                    case 'S':
                        if ((++c.x) < map.get_size()){
                            if (discovered_flags[c.x * grid_size + c.y] == false) {
                                if(discvr_sq(c.x, c.y, is_captain, os)){
                                    return true;
                                }
                            }
                        }
                        c.x--;
                        break;
                    case 'W':
                        if (c.y >= 1){
                            c.y--;
                            if (discovered_flags[c.x * grid_size + c.y] == false) {
                                if(discvr_sq(c.x, c.y, is_captain, os)){
                                    return true;
                                }
                            }
                            c.y++;
                        }
                        break;
                } //switch

                //start an internal loop for search party
                while (!first_mate.empty() && is_captain){
                    if (invstg_loop(fm == 'S' ? first_mate.back() : first_mate.front(), 
                                                            false, os)){
                        return true;
                    }
                }

            } //for every directon char in hunt_order string

            if (first_mate.empty() && !is_captain){ //island had no treasure 
                os << "party returned with no treasure.\n";
            }
            return false;
        }

        bool discvr_sq(uint32_t row, uint32_t col, bool is_captain, std::ostream &os) {

            uint32_t index = row * grid_size + col;
            char terrain = map.get_grid()[index];

            switch (terrain) {
                case '$': // Treasure
                    map.get_prev_sqs()[index] = {current_check.x, current_check.y};
                    current_check = {row, col};
                    if (!is_captain) {
                        os << "party found treasure at " << row << "," << col << ".\n";
                        land_disc_count++;
                        return true;
                    } else {
                        ashore_count++;
                        os << "Went ashore at: " << row << "," << col << '\n'
                        << "Searching island... ";
                        land_disc_count++;
                        os << "party found treasure at " << row << "," << col << ".\n";
                        return true;
                    }
                case '#': // Wall
                    return false;
                case 'o': // Land
                    discovered_flags[index] = true;
                    if (is_captain) {
                        ashore_count++;
                        os << "Went ashore at: " << row << "," << col
                        << "\nSearching island... ";
                    }
                    map.get_prev_sqs()[index] = {current_check.x, current_check.y};
                    first_mate.emplace_back(Coor{row, col});
                    return false;
                case '.': // Water
                    if (is_captain) {
                        discovered_flags[index] = true;
                        map.get_prev_sqs()[index] = {current_check.x, current_check.y};
                        captain.emplace_back(Coor{row, col});
                        return false;
                    }
                    return false;
                default:
                    return false;
            }

        } //check_sq




        //overloaded get_path_length functions:

        uint32_t get_path_length() const { 

            uint32_t path_length = 0;
            Coor current = current_check;

            while (map.get_grid()[current.x * grid_size + current.y] != '@') {
                Coor prev = map.get_prev_sqs()[current.x * grid_size + current.y];
                if (prev.x >= grid_size || prev.y >= grid_size) {
                    break; // Reached an invalid or uninitialized square
                }
                current = prev;
                path_length++;
            } //while
            return path_length;
        } //path length function 1


        uint32_t get_path_length(char mode, std::ostream &os) const {
            std::vector<Coor> path;
            path.reserve(land_disc_count+water_disc_count);
            //path cant be longer than total squares discovered

            uint32_t path_length = 0;
            Coor current = current_check;
            uint32_t index = current.x * grid_size + current.y;

            while (map.get_grid()[index] != '@') {
                path.emplace_back(current);
                current = map.get_prev_sqs()[index];
                index = current.x * grid_size + current.y;
                //if (prev.x >= grid_size || prev.y >= grid_size) {
                    //break; // Reached an invalid or uninitialized square
                //}
                path_length++;
            } //while

            if (mode == 'L') {
                os << "Sail:" << std::endl;
                bool reached_island_sqs = false;
                os << map.get_start().x << "," << map.get_start().y << '\n';
                for (uint32_t index = path_length-1; index > 0; index--){
                    Coor buffer = path[index];
                    char current_terrain = map.get_grid()[buffer.x * 
                                            grid_size + buffer.y];
                    if ((current_terrain == 'o'||current_terrain == '$')
                                                && !reached_island_sqs){
                        os << "Search:\n";
                        reached_island_sqs = true;
                    } 
                    os << buffer.x << "," << buffer.y <<std::endl;
                } //for path_length
                os << path[0].x << "," << path[0].y <<std::endl;
            } else {
                path.pop_back();
                map.print_grid(path,path_length,os);
            }
            return path_length;

        } //path length w/ const


        Coor & get_start() { return map.get_start(); }
        Coor & get_current_loc() { return current_check; }
        uint32_t get_wtr_disc_cnt() const { return water_disc_count; }
        uint32_t get_lnd_disc_cnt() const { return land_disc_count; }
        uint32_t get_ashore_cnt() const { return ashore_count; }
        std::deque<Coor> & get_captain() 
        { return captain; }
        std::deque<Coor> & get_first_mate() 
        { return first_mate; }
        
};