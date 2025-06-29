# Pirate-Game
Pirate Game is a project to travel through islands in an expanse of water to find treasure. Project includes backend data structures, character structs, txt files simulating maps with hidden treasure, and search algorithms automating the roles of the two characters. No UI.

The Captain will travel from island to island with their own log and future plans, moving only over water locations that are represented on txt files with a '.'. As soon as land is discovered while sailing, a search party is put ashore before any other discovery is attempted. Once put ashore to lead a search party, the First Mate, which is a second character struct, will search all available land with a separate log and plans before returning to the ship, moving only over land locations represented on txt with 'o'. Any water (inland or open ocean) encountered by the search party is completely ignored. Treasure represented using '$'. Both Captain and First Mate are unable to move over impassable terrain '#'.

While trying to find treasure and create a map, the important terms are investigate and discover. Investigation is the act of examining squares adjacent the “current” location. An adjacent square can only be discovered if it is the proper terrain type (water and land while sailing, land while searching) and has not already been discovered. A newly discovered square is marked as discovered to prevent infinite loops while investigating. This is added to the proper container so that investigation can continue.
Hunt order can be picked by user through command line. By default the hunt order is North->East->South->West aronnd current location. 

Code is optimized for memory usage and speed efficiency.
