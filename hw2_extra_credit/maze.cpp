#include "maze.h"

maze::maze() {
    // open the file
    mazefile = fopen("Maze.txt", "r");

    // get the dimensions of the maze
    ydim = getdimension();
    xdim = getdimension();

    // allocate memory for the maze array
    mazearray = new char*[xdim];
    for (int x = 0; x < xdim; x++) {
        mazearray[x] = new char[ydim];
    }

    // start constructing the maze
    for (int y = 0; y < ydim; y++) {
        for (int x = 0; x < xdim; x++) {
            char c = getcharacter();
            mazearray[x][y] = c;
            // if we found the start point, record it
            if (c == '*') {
                start[0] = x;
                start[1] = y;
                // std::cout << "found the start at location: " << start[0] << ", " << start[1] << std::endl;
            }
        }
    }

    // close the file
    fclose(mazefile);
}

int maze::getdimension() {
    // reads an integer from the file into a variable
    int n;
    fscanf(mazefile, "%d", &n);    
    return n;
}

char maze::getcharacter() {
    // reads the next character of the maze into a variable
    // skips over whitespace and newlines
    // complains if one of the characters is not *, -, $, or x
    char c;
    c = fgetc(mazefile);
    switch (c) {
        case 'x' :
        case '-' :
        case '*' :
        case '$' :
            return c;
            break;
        case '\n' :
        case ' ' :
            return getcharacter();
            break;
        default:
            std::cout << "an error occured with character: " << c << std::endl;
            exit(1);
            break;
    }
}

std::ostream& operator << (std::ostream& os, maze& m) {
    // overload the << operator so that we can call std::cout << to print the maze
    for (int y = 0; y < m.ydim; y++) {
        for (int x = 0; x < m.xdim; x++) {
            os << m.mazearray[x][y];
        }
    os << std::endl;
    }
    return os;
}

bool maze::isvalid(int x, int y) {
    // returns true if the given integers represent a valid path on the maze

    // are we off the edge of the maze?
    if (x < 0 || x >= xdim || y < 0 || y >= ydim) { return false; }

    // are we on anything but a '*', '$', or '-'?
    char c = mazearray[x][y];
    if (c != '*' && c != '$' && c != '-') { return false; }

    return true;
}

// the following two functions are helper functions for solving the maze

mazeNode::mazeNode(int x, int y) {
    // a constructor for the mazeNode class; represents one node on the maze
    coords[0] = x;
    coords[1] = y;
    prev_node = NULL;
}
 
void append_item(to_do_item** head_ref, mazeNode* node) {
    // appends a mazeNode to the "to_do_list"

    // "to_do_item" is a struct type, prototyped in the header.
    // it is a rudimentary implementation of a linked list.
    // this is the only defined function that acts directly 
    // on the to_do_item type.

    // create a new item
    to_do_item* new_item = new to_do_item;
    new_item->node = node; new_item->next = NULL;

    //create a new list if the current one is empty
    if(*head_ref == NULL){
        *head_ref = new_item;
        return;
    }

    //iterate through the list until there is an item without a next
    to_do_item* cur_item = *head_ref;

    while (true) {
        if(cur_item->next == NULL){
            cur_item->next = new_item;
            return;
        }
        cur_item = cur_item->next;
    }
}

// the following functions solve the maze and mark the path

void maze::init() {
    // kicks off the maze solving process

    // make an array as large as the maze
    // when we add mazeNode items to this array, we are effectively
    // marking that spot as visited
    nodearray = new mazeNode** [xdim];
    for (int x = 0; x < xdim; x++) {
        nodearray[x] = new mazeNode* [ydim] ();
    }
    
    // find the fastest path to the end
    mazeNode* final_node;
    final_node = traversegraph();

    // std::cout << "Map of nodes: " << std::endl;
    // for (int y = 0; y < ydim; y++) {
    //     for (int x = 0; x < xdim; x++) {
    //         if (nodearray[x][y] == NULL) { std::cout << ' '; } else { std::cout << '+'; }
    //     }
    //     std::cout << std::endl;
    // }

    // check to see that we found the solution
    if (final_node == NULL) { std::cout << "No path to finish! :(" << std::endl; return; }
    // std::cout << "Found a path to the finish:" << std::endl;

    // trace back to the start from the finish and update the maze
    mazeNode* cur_node = final_node->prev_node;
    while (cur_node != NULL) {
        int x = cur_node->coords[0];
        int y = cur_node->coords[1];
        mazearray[x][y] = 'v';
        cur_node = cur_node->prev_node;
    }
}

mazeNode* maze::traversegraph() {
    // traverses the maze using a breadth-first search algorithm

    // start at the start node
    int x = start[0]; int y = start[1];
    mazeNode* start_node = new mazeNode(x, y);
    nodearray[x][y] = start_node;

    // std::cout << "traversing, starting at: " << x << ", " << y << std::endl;

    // make a to-do list
    to_do_item* to_do_list = NULL;
    //to_do_list->node = NULL; to_do_list->next = NULL;

    // put the start node in the to-do list
    append_item(&to_do_list, start_node); 
    // std::cout << "starting the huge loop" << std::endl;
 
    // the meat of this algorithm:
    // pull an item off the front of the to do list
    // if its neighbors have not yet been visited,
    // add them onto the end of our to do list
    // repeat until we find the solution finish or run out of items

    // while the to-do list is not empty:
    while (to_do_list != NULL) {
    
        // pull the first item off the to-do list, call it cur_node
        mazeNode* cur_node = to_do_list->node;
        // std::cout << "pulled an item off the list" << std::endl;
        to_do_list = to_do_list->next;

        // find all its neighbors 
        // get ready to search in all 4 directions
        int x = cur_node->coords[0];
        int y = cur_node->coords[1];

        // std::cout << "currently visiting: " << x << ", " << y << std::endl;

        int up[2] = {x, y - 1};
        int down[2] = {x, y + 1};
        int left[2] = {x - 1, y};
        int right[2] = {x + 1, y};
        int* dirarray[4] = {left, up, right, down};

        // check each direction
        for (int i = 0; i < 4; i++) {
            int nextx = dirarray[i][0];
            int nexty = dirarray[i][1];

            // make sure we are checking a valid location
            if (!isvalid(nextx, nexty)) {
                continue;
            }

            // make sure it has not yet been visited
            if (nodearray[nextx][nexty] != NULL) { continue; }

            // mark them as visited and make their prev_nodes point to cur_node
            // std::cout << "constructing a new node at: " << nextx << ", " << nexty << std::endl;
            mazeNode* new_node = new mazeNode(nextx, nexty);
            new_node->prev_node = cur_node;
            nodearray[nextx][nexty] = new_node;

            // if we found the finish, we're done!
            if (mazearray[nextx][nexty] == '$') { return new_node; }

            // otherwise, append it to the to-do list
            append_item(&to_do_list, new_node);
        }
    }

    return NULL;
}

