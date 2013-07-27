#include<cassert>//provides assert
#include <iostream> //we are adding ostream and istream
#include <cstdlib>
#include <fstream>
#include "maze.h"
using namespace std;


int main()
{
	maze newMaze;
	cout<<"\nWELCOME TO THE MAZE SOLVER \n";
	cout<<"here is the unsolved maze: \n";
	cout<<newMaze<<endl;
	
	newMaze.init();
	cout<<"here is the solved maze: \n";
	cout<<newMaze<<endl;
	
	return 0;
}