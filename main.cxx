#include "DoubleGyrefield.cxx"
#include <stdlib.h>
#include <iostream>

using namespace std;

#define FIELD DoubleGyrefield 

int main(int argc, char* argv[])
{

FIELD field;

int num_cycles = atoi(argv[1]);
int interval = atoi(argv[2]);
int cycle_prev = 0;
int cycle_next = cycle_prev + interval;

int dims[2];
dims[0] = atoi(argv[3]);
dims[1] = atoi(argv[4]);

float bounds[4];
bounds[0] = atof(argv[5]);
bounds[1] = atof(argv[6]);
bounds[2] = atof(argv[7]);
bounds[3] = atof(argv[8]);


for(int c = 0; c < num_cycles; c++)
{
	if(c == cycle_next)
	{
		cycle_prev = c;
		cycle_next = cycle_prev + interval;	
	}	

	cout << "At cycle: " << c << " Interpolating between " << cycle_prev <<  " and " << cycle_next << endl;
	// Get velocity at time_prev, time_next
	// Compute velocity at c using linear interpolation
	// Advect particle forward
}


}

