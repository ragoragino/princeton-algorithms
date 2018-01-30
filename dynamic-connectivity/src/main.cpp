#include <cstdlib> 
#include <cstdio>
#include <time.h>
#include "Grid.h"

void shuffle(int *arr, int size)
{
	int i, r, tmp;
	for (i = size - 1; i != 1; i--)
	{
		r = rand() % (i + 1);

		tmp = arr[i];
		arr[i] = arr[r];
		arr[r] = tmp;
	}
}

int main()
{
	// Set parameters of the simulation
	unsigned int sim_length = 1000;
	unsigned int lattice_length = 500;

	// Initialize random engine
	srand(time(NULL));

	// Create grid object with N
	Grid grid = Grid(lattice_length);

	double avg_perc_percentage = 0;
	unsigned int pick;

	// Initialize buffer of random indices
	int buffer_index = lattice_length * lattice_length;
	int *buffer = new int[buffer_index];
	for (int i = 0; i != buffer_index; ++i)
	{
		buffer[i] = 1 + i;
	}

	// Shuffle the array few times
	for (int i = 0; i != 10; i++)
	{
		shuffle(buffer, buffer_index);
	}

	// Simulate for sim_length
	for (int i = 0; i != sim_length; i++)
	{
		// Until the upper and bottom corners of the grid are not connected
		while (!grid.connected())
		{
			pick = buffer[--buffer_index];
			
			// Open the site of the random pick
			grid.activate(pick);
		}

		// Shuffle the buffer de nuovo
		buffer_index = lattice_length * lattice_length;
		shuffle(buffer, buffer_index);

		printf("PERCOLATION PERCENTAGE: %f \n", grid.get_percolation_percentage());

		avg_perc_percentage += grid.get_percolation_percentage();

		// Clear the grid object for new simulation 
		grid.clear();
	}

	printf("\n OVERALL PERCOLATION PERCENTAGE: %f \n", avg_perc_percentage / sim_length);

return 0;
}
