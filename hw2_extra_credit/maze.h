// maze.h
# ifndef MAZE_H_
# define MAZE_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

class mazeNode {
    // represents one point on the maze. essentially a reverse linked list
    // constructed using the maze::traversegraph() function
    public:
        mazeNode(int x, int y);
        friend class maze;

    private:
        int coords[2];
        mazeNode* prev_node; // a link to the node from which this one can be reached
};

struct to_do_item {
    // a data structure for the maze::traversegraph() algorithm
    // essentially a linked list, but we only need to define append behavior
    mazeNode* node;
    to_do_item* next;
};

void append_item(to_do_item** head_ref, mazeNode* node);

class maze {
    public:
        maze();
        bool isvalid(int x, int y);
        friend std::ostream& operator << (std::ostream& os, maze& m);
        void init(); // makes an array of mazenodes and finds the solution

        mazeNode* traversegraph(); // constructs a solution, if there is one

    private:
        int xdim;
        int ydim;
        char** mazearray;
        int start[2];
        FILE * mazefile;
        int getdimension(); // reads a dimension from the file
        char getcharacter(); // reads a character from the file

        mazeNode* startnode;
        mazeNode*** nodearray;
};

# endif
