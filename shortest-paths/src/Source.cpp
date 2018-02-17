#include "Header.h"

int main()
{
	// Number of currency pairs
	static constexpr int LENGTH = 5;

	// Set the filename and input stream properties
	std::string filename("D:\\Materials\\Programming\\Projekty\\princeton-algorithms\\shortest-paths\\src\\forex.txt");
	std::ifstream input(filename, std::ios::in);

	if (!input.is_open())
	{
		std::cout << "Failed to open: " << filename << '\n';
	}
	else
	{
		// Create edge-weighted graph
		EdgeWeightedGraph ew_graph = EdgeWeightedGraph(input, LENGTH);

		// Define the starting currency and bellman-ford algorithm instance
		int start = 0;
		BellmanFord bellman_ford = BellmanFord(ew_graph, start);

		// Check for arbitrage
		bool arbitrage = bellman_ford.isArbitrage();

		// Print the arbitrage route and profit
		if(arbitrage)
		{
			std::vector<int> path = bellman_ford.cyclePath(start);
			std::cout << "PATH TO " << start << ": ";
			for (int i : path)
			{
				std::cout << i << ", ";
			}
			std::cout << std::endl;

			double profit = bellman_ford.profit(start);
			std::cout << "PROFIT: " << profit <<"\n";
		}
	}

	return 0;
}