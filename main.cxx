#include "DoubleGyrefield.cxx"
#include <stdlib.h>
#include <iostream>
#include <vector>

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

vector<float> x_current, y_current;
vector<float> x_gt, y_gt;
vector<float> error;


/* Define seed starting locations */

int num_seeds = dims[0]*dims[1];

float x_spacing, y_spacing;
x_spacing = (bounds[1]-bounds[0])/(dims[0]-1);
y_spacing = (bounds[1]-bounds[0])/(dims[0]-1);

for(int i = 0; i < dims[1]; i++)
{
	for(int j = 0; j < dims[0]; j++)
	{
		float x, y;
		x = bounds[0] + x_spacing*j;	
		y = bounds[2] + y_spacing*i;	
		
		x_current.push_back(x);
		y_current.push_back(y);
		x_gt.push_back(x);
		y_gt.push_back(y);
	}
}

float h = 0.01; // step size

for(int c = 0; c < num_cycles; c++)
{
	if(c == cycle_next)
	{
		cycle_prev = c;
		cycle_next = cycle_prev + interval;	
	}	

	#pragma omp parallel for
	for(int n = 0; n < num_seeds; n++)
	{
		float vel_gt[2], vel_prev[2], vel_next[2], vel_eul[2];
		float loc_gt[2], loc_eul[2];
		float t = c*h; // this is the current time.

		// Ground truth computation
		loc_gt[0] = x_gt[n];
		loc_gt[1] = y_gt[n];

		field.calculateVelocity(loc_gt, t, vel_gt);

		x_gt[n] = loc_gt[0] + h*vel_gt[0];
		y_gt[n] = loc_gt[1] + h*vel_gt[1];
	

		// Subsampled Eulerian computation
		
		float t_prev, t_next;
		t_prev = cycle_prev*h;
		t_next = cycle_next*h;
		
		loc_eul[0] = x_current[n];
		loc_eul[1] = y_current[n];

		field.calculateVelocity(loc_eul, t_prev, vel_prev);
		field.calculateVelocity(loc_eul, t_next, vel_next);

		float d = (t - t_prev)/(t_next - t_prev);

		vel_eul[0] = vel_prev[0] + d*(vel_next[0] - vel_prev[0]);
		vel_eul[1] = vel_prev[1] + d*(vel_next[1] - vel_prev[1]);

		x_current[n] = loc_eul[0] + h*vel_eul[0];
		y_current[n] = loc_eul[1] + h*vel_eul[1];
	}
}

float min = 999999999.0;
float max = 0.0;
float sum = 0.0;
float avg;


for(int n = 0; n < num_seeds; n++)
{
	float pt1[2], pt2[2];
	
	pt1[0] = x_gt[n];
	pt1[1] = x_gt[n];
	
	pt2[0] = x_current[n];
	pt2[1] = x_current[n];
	
	float dist = sqrt(pow(pt1[0] - pt2[0], 2.0) + pow(pt1[1]-pt2[1], 2.0));
	error.push_back(dist);

	if(dist < min)
		min = dist;

	if(dist > max)
		max = dist;

	sum += dist;
}

	avg = sum/num_seeds;

	cout << "Stats for interval = " << interval << ": " << endl;
	cout << "Average error: " << avg << endl;
	cout << "Min error: " << min << endl;
	cout << "Max error: " << max << endl;

}

