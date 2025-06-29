// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include "Hunt.cpp"
#include <cstring>
#include <getopt.h>

//cc_water: current coordinates of ship

bool verbose = false;
bool stats = false;
char show_path = 'U';
char captain_tp = 'S';
char first_mate_tp = 'Q';
std::string hunt_order = "NESW";


void printHelp() {
    std::cout << "--help, -h: Print a useful help message and exit, ignores all other options\n";
    std::cout << "--captain <QUEUE|STACK>, -c <QUEUE|STACK>: The route-finding container used" 
        << " while sailing in the water\n" ;
    std::cout << "--first-mate <QUEUE|STACK>, -f <QUEUE|STACK>: The route-finding container used while"
    " searching on land\n" ;
    std::cout << "--hunt-order <ORDER>, -o <ORDER>: The order of discovery of adjacent tiles " 
    "around the current location, a four character string using exactly one of each of the four "
    "characters 'N', 'E', 'S', and 'W'\n" ;
    std::cout << "--verbose, -v: Print verbose output while searching\n" ;
    std::cout << "--stats, -s: Display statistics after the search is complete\n";
    std::cout << "--show-path <M|L>, -p <M|L>: Display a treasure map or the list of locations "
    << "that describe the path" << std::endl;

}  //printHelp()

void getOptions(int argc, char **argv) {
    opterr = 0;  // Suppress getopt errors
    int choice;
    int index = 0;

    struct option longOptions[] = {
        {"help", no_argument, nullptr, 'h'},
        {"captain", required_argument, nullptr, 'c'},
        {"first-mate", required_argument, nullptr, 'f'},
        {"hunt-order", required_argument, nullptr, 'o'},
        {"verbose", no_argument, nullptr, 'v'},
        {"stats", no_argument, nullptr, 's'},
        {"show-path", required_argument, nullptr, 'p'},
        {nullptr, 0, nullptr, '\0'}
    };

    while ((choice = getopt_long(argc, argv, "hc:f:o:vsp:", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
            case 'c': {
                if (strcmp(optarg, "QUEUE") != 0 && strcmp(optarg, "STACK") != 0) {
                    std::cerr << "Invalid argument to --captain";
                    exit(1);
                }
                captain_tp = optarg[0];
                break;
            } //case c

            case 'f': {
                std::string arg { optarg };
                if (arg[0] != 'Q' && arg[0] != 'S') {
                    std::cerr << "Invalid argument to --first-mate'";
                    exit(1);
                }
                first_mate_tp = arg[0];
                break;
            } //case f

            case 'o': {
                std::string arg { optarg };
                if (arg.length() != 4) {
                    std::cerr << "Invalid --hunt-order\n";
                    exit(1);
                }
                for (char c : arg) {
                    if (c != 'N' && c != 'E' && c != 'W' && c != 'S') {
                        std::cerr << "Invalid --hunt-order\n";
                        exit(1);
                    }
                }
                hunt_order = arg;
                break;
            } //case o

            case 'v':
                verbose = true;
                break;

            case 's':
                stats = true;
                break;

            case 'p': {
                std::string arg { optarg };
                if (arg.length() != 1) {
                    std::cerr << "Specify --show-path only once\n" ;
                    exit(1);
                }
                if (arg[0] != 'M' && arg[0] != 'L') {
                    std::cerr << "Invalid argument to --show-path\n";
                    exit(1);
                }
                show_path = arg[0];
                break;
            } //case p

            case 'h':
                printHelp();
                exit(0);

            default:
                std::cerr << "Unknown option";
                exit(1);
        } //switch
    } //while get_opt_long

    // Handle non-option arguments
    if (optind < argc) {
        std::cerr << "Unexpected arguments: ";
        while (optind < argc) {
            std::cerr << argv[optind++] << " ";
        }
        std::cerr << "\n" ;
        exit(1);
    }
} //getOptions



int main(int argc, char *argv[]) {

    getOptions(argc, argv);
    ReadGrid grid(std::cin);
    Hunt hunt(grid,hunt_order,captain_tp,first_mate_tp);

    Coor cc_water = hunt.get_start();
    bool found = false;
    uint32_t path_length = 0;

    std::ostringstream verbose_out;
    std::ostringstream path_out;

    verbose_out << "Treasure hunt started at: " << cc_water.x 
                        << "," << cc_water.y << '\n'; //verbose


    //investigate loop -----------------------------
    while (!hunt.get_captain().empty()) {
        found = hunt.invstg_loop(cc_water, true, verbose_out);
        if (!hunt.get_captain().empty()){
            cc_water = captain_tp == 'S' ? hunt.get_captain().back() : 
                        hunt.get_captain().front();
        }

        if (found){
            break;
        }
    }
    //end of investigate loop ----------------------

    // --show-path
    if (show_path != 'U'){
        path_length = hunt.get_path_length(show_path,path_out);
    } else {
        path_length = hunt.get_path_length();
    }
    
    // --verbose
    if (verbose) {
        std::cout << verbose_out.str();
    }

    // --stats
    if (stats){
        cc_water = hunt.get_start();
        std::cout << "--- STATS ---\n";
        std::cout << "Starting location: "<<cc_water.x <<","<<cc_water.y<<'\n';
        std::cout << "Water locations investigated: "<<hunt.get_wtr_disc_cnt()<<'\n';
        std::cout << "Land locations investigated: "<<hunt.get_lnd_disc_cnt()<<'\n';
        std::cout << "Went ashore: " << hunt.get_ashore_cnt()<<'\n';
        if (found) {
            std::cout << "Path length: " << path_length << '\n';
            std::cout << "Treasure location: "<<hunt.get_current_loc().x<<","<<
                                        hunt.get_current_loc().y<<'\n';
        }
        std::cout << "--- STATS ---" << std::endl;
    }

    // --show-path
    if (show_path != 'U'){
        std::cout << path_out.str();
    }


    if (found){
        std::cout << "Treasure found at " << hunt.get_current_loc().x 
                                    << "," << hunt.get_current_loc().y <<
               " with path length " << path_length <<'.'<< std::endl;
    } else {
        std::cout << "Treasure hunt failed\n"; 
        std::cout << "No treasure found after investigating " << 
                (hunt.get_wtr_disc_cnt() + hunt.get_lnd_disc_cnt())
                << " locations." << std::endl;
    }

    return 0;
}