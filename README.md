# Pirate-Game-Player
The Pirate Game project simulates a map of islands in an expanse of water to find treasure. A Captain and First Mate character have to work together to find the treasure hidden on one of the islands. This project simulates a bot player that uses various search algorithms to work through the map and try to find the treasure. Project includes backend data structures, character structs, txt reader, txt files simulating maps with hidden treasure, and search algorithms automating the roles of the two characters. No UI.

The program will receive a txt file that will represent a map. The 'Captain' struct will travel from island to island, moving only over water locations that are represented on txt files with a '.'. As soon as land is discovered while sailing, a search party is put ashore before any other discovery is attempted. Once put ashore to lead a search party, the 'First Mate' character struct, will search all available land with a separate log and plans before returning to the ship, moving only over land locations represented on txt with 'o'. Any water (inland or open ocean) encountered by the search party is completely ignored. Treasure represented using '$'. Both Captain and First Mate are unable to move over impassable terrain '#'.

While trying to find treasure and create a map, the important terms are investigate and discover. Investigation is the act of examining squares adjacent the “current” location. An adjacent square can only be discovered if it is the proper terrain type (water and land while sailing, land while searching) and has not already been discovered. A newly discovered square is marked as discovered to prevent infinite loops while investigating. This is added to the proper container so that investigation can continue. Hunt order can be picked by user through command line. By default the hunt order is North->East->South->West around current location.

Code is optimized for memory usage and speed efficiency.

Command Line Interface:

$ ./hunt [options] < inputfile > outputfile



Sample outputs generated using:

$ make hunt

$ ./hunt -vspL < map1_input.lst.txt > map1_output.txt

$ ./hunt -vspM < map2_input.map.txt > map2_output.txt

Output without options will look like:

'No treasure found after investigating 5 locations.' OR 'Treasure found at 0,0 with path length 8.'


Supported [options]

--help, -h: Print a useful help message and exit, ignores all other options

--captain <QUEUE|STACK>, -c <QUEUE|STACK>: The route-finding container used while sailing in the water (if unspecified, captain default is stack)

--first-mate <QUEUE|STACK>, -f <QUEUE|STACK>: The route-finding container used while searching on land (if unspecified, first-mate default is queue)

--hunt-order <ORDER>, -o <ORDER>: The order of discovery of adjacent tiles around the current location, a four character string using exactly one of each of the four characters ‘N’, ‘E’, ‘S’, and ‘W’ (if unspecified, default order is: North->East->South->West)

--verbose, -v: Print verbose output while searching

--stats, -s: Display statistics after the search is complete

--show-path <M|L>, -p <M|L>: Display a treasure map or the list of locations that describe the path

