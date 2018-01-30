#pragma once

class Grid
{
public:
	Grid(int N) : length(N), grid(new int[N * N + 2]),
		size(new int[N * N + 2]), open(new bool[N * N + 2]), 
		neighbour_length(8), final_neighbours(new int[8]) // 8 neighbours
	{
		this->initialize();
	}

	void initialize()
	{
		// Initialize array elements
		for (int i = 0; i != length * length + 2; i++)
		{
			grid[i] = i;
			size[i] = 1;
			open[i] = false;
		}

		// Set virtual parents for the bottom and upper row
		for (int i = 0; i != length; i++)
		{
			grid[i + 1] = 0;
			grid[length * length - i] = length * length + 1;
		}
	}

	// Function that traverses the array and finds the root
	int find_root(int i)
	{
		while (i != grid[i])
		{
			grid[i] = grid[grid[i]];
			i = grid[i];
		}

		return i;
	}

	// Function that unites a root of p with a root of q
	void unite_root(int p, int q)
	{
		int i = find_root(p);
		int j = find_root(q);

		if (i == j)
		{
			return;
		}

		if (size[i] < size[j])
		{
			grid[i] = j;
			size[j] += size[i];
		}
		else
		{
			grid[j] = i;
			size[i] += size[j];
		}
	}

	void activate(int p)
	{
		// Return if already activated -> should not happen!
		if (open[p])
		{
			return;
		}

		// Unite root with all open neighbours
		int index = 0;
		neighbours(p, &index);
		for (int i = 0; i != index; i++)
		{
			if (open[final_neighbours[i]])
			{
				unite_root(final_neighbours[i], p);
			}
		}

		open[p] = true;
	}

	// Function that specifies all available neighbours
	// Useful for the edge sites
	void neighbours(int p, int *neighbour_index)
	{
		static const int neighbour_indices[] = 
		{
			- length - 1, - length, - length + 1,
			- 1, 1,
			length - 1, length, length + 1
		};

		static bool erase_indices[] =
		{
			true, true, true, true, 
			true, true, true, true
		};

		if (p % length)
		{
			erase_indices[2] = false;
			erase_indices[4] = false;
			erase_indices[7] = false;
		}
		else if ((p % length) == 1)
		{
			erase_indices[0] = false;
			erase_indices[3] = false;
			erase_indices[5] = false;
		}
		
		if (p <= length)
		{
			erase_indices[0] = false;
			erase_indices[1] = false;
			erase_indices[2] = false;
		}
		else if(p >= length * (length - 1) + 1)
		{
			erase_indices[5] = false;
			erase_indices[6] = false;
			erase_indices[7] = false;
		}

		for (int i = 0; i != neighbour_length; i++)
		{
			if (erase_indices[i])
			{
				final_neighbours[(*neighbour_index)++] = p + neighbour_indices[i];
			}

			erase_indices[i] = true;
		}
	}

	// Test whether the top and bottom grid values are connected
	bool connected()
	{
		unsigned int i = find_root(grid[0]);
		unsigned int j = find_root(grid[length * length + 1]);

		return i == j;
	}

	// Get percentage of open sites to all sites
	double get_percolation_percentage()
	{
		double percolation_amount = 0;
		double overall_size = length * length;

		for (int i = 1; i != overall_size + 1; i++)
		{
			percolation_amount += open[i];
		}

		return percolation_amount / overall_size;
	}

	void clear()
	{
		this->initialize();
	}
	
	~Grid() { delete[] grid; delete[] size; delete[] open; delete[] final_neighbours; }

private:
	int *final_neighbours; // array containing size of the neighbours of a site
	int *grid, *size; // arrays containing the grid itself and the size parameters
	bool *open; // array containing the indicator of activation
	
	// length of the grid/size/open arrays and length of neighbour array
	int length, neighbour_length; 
};