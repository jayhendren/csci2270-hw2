#include "maze.h"

maze::maze() {
    // open the file
    mazefile = fopen("Maze.txt", "r");

    // get the dimensions of the maze
    ydim = getdimension();
    xdim = getdimension();

    // allocate memory for the maze array
    mazearray = new char[xdim * ydim];

    // start constructing the maze
    for (int y = 0; y < ydim; y++) {
        for (int x = 0; x < xdim; x++) {
            char c = getcharacter();
            int i = getindex(x, y);
            mazearray[i] = c;
            // if we found the start point, record it
            if (c == '*') {
                start = i;
                // std::cout << "found the start at location: " << start << std::endl;
            }
        }
    }

    // close the file
    fclose(mazefile);

    // solve the maze
    // solvemaze();
}

int maze::getindex (int x, int y) const {
    return x + (y * xdim);
}

int maze::getdimension() {
    int n;
    fscanf(mazefile, "%d", &n);    
    return n;
}

char maze::getcharacter() {
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
            std::cout << "Character not allowed: " << c << std::endl;
            exit(1);
            break;
    }
}

std::ostream& operator << (std::ostream& os, const maze& m) {
    for (int y = 0; y < m.ydim; y++) {
        for (int x = 0; x < m.xdim; x++) {
            os << m.mazearray[m.getindex(x, y)];
        }
    os << std::endl;
    }
    return os;
}

void maze::init() {
    // if (traversemaze(start)) {
    //     std::cout << "Solved the maze! Solution:" << std::endl;
    // } else {
    //     std::cout << "Could not solve the maze :(" << std::endl;
    // }
    traversemaze(start);
    return;
}

bool maze::traversemaze(int index) {
    // check to see if we are at a valid location
    // coords must be 0 <= position < max; char must be - or $ or *

    // std::cout << "traversing the maze. position: " << index % xdim << ", " << index / xdim << std::endl;
    if ( index < 0 || index >= xdim * ydim) {
        // std::cout << "not a valid position." << std::endl;
        return false;
    }
    char c = mazearray[index];
    if (c == '$') {
        // yay! we are at the end!
        // std::cout << "found the end!" << std::endl;
        return true;
    } else if (c != '-' && c != '*') {
        // std::cout << "can't be here!" << std::endl;
        return false;
    }

    // we are at a valid position. mark it and check our neighbors
    mazearray[index] = 'v';
    // int next;

    // go left
    if (index % xdim != 0 && traversemaze(index - 1)) { return true; }  
    // go up
    if (index / xdim != 0 && traversemaze(index - xdim)) { return true; }
    // go right
    if (index % xdim != xdim - 1 && traversemaze(index + 1)) { return true; }
    // go down
    if (index / xdim != ydim - 1 && traversemaze(index + xdim)) { return true; }

    return false;
}
