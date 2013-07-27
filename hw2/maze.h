// maze.h

# ifndef MAZE_H_
# define MAZE_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

class maze {
    public:
        maze();
        friend std::ostream& operator << (std::ostream& os, const maze& m);
        void init();

    private:
        int xdim;
        int ydim;

        char* mazearray;
        int start;
        FILE * mazefile;

        bool traversemaze(int index);

        int getindex (int x, int y) const;

        int getdimension();
        char getcharacter();
};

# endif
