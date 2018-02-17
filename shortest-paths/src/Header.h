#pragma once
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class DirectedEdge
{
public:
	DirectedEdge(double weight, int v, int w) : 
		_weight(weight), _v(v), _w(w) {};

	int from() const { return _v; }

	int to() const { return _w; }

	double weight() const { return _weight; }

	bool right(int v, int w) const { return v == _v && w == _w; }

private:
	double _weight;
	int _v, _w;
};

class EdgeWeightedGraph
{
public:
	EdgeWeightedGraph(std::ifstream& s, int length);

	void print() const;

	const std::list<DirectedEdge>& adj(int v) const
	{
		return _vertices[v];
	}

	int getLength() const { return _V; }

	DirectedEdge edge(int v, int w) const
	{
		for (std::list<DirectedEdge>::const_iterator it = _vertices[v].begin(); 
			it != _vertices[v].end(); it++)
		{
			if (it->right(v, w))
			{
				return *it;
			}
		}

		throw;
	}

	~EdgeWeightedGraph()
	{
		delete[] _vertices;
	}

private:
	int _V;
	std::list<DirectedEdge> *_vertices;
};

EdgeWeightedGraph::EdgeWeightedGraph(std::ifstream& s, int length) : 
	_V(length)
{
	_vertices = new std::list<DirectedEdge>[length];

	double value;
	for (int i = 0; i != _V; ++i)
	{
		for (int j = 0; j != _V - 1; ++j)
		{
			std::string line;
			std::getline(s, line, ' ');

			value = -log(std::stof(line));
			_vertices[i].emplace_back(value, i, j);
		}

		std::string line;
		std::getline(s, line, '\n');

		value = -log(std::stof(line));
		_vertices[i].emplace_back(value, i, _V - 1);
	}
};

void EdgeWeightedGraph::print() const
{
	for (int i = 0; i != _V; ++i)
	{
		std::cout << i << ": ";

		for (std::list<DirectedEdge>::const_iterator it = _vertices[i].begin();
			it != _vertices[i].end(); ++it)
		{
			std::cout << it->weight() << ", ";
		}

		std::cout << '\n';
	}
}

class BellmanFord
{
public:
	BellmanFord(const EdgeWeightedGraph& e, int start);

	virtual double distTo(int v) const { return _distanceTo[v];  };

	virtual std::vector<int> cyclePath(int v) const;

	virtual bool isArbitrage() const;

	virtual void relax(DirectedEdge e);

	virtual double profit(int v);

	virtual ~BellmanFord() { delete[] _connectedTo; delete[] _distanceTo; };

private:
	int *_connectedTo;
	double *_distanceTo;
	int _start;
	const EdgeWeightedGraph *graph;
};

BellmanFord::BellmanFord(const EdgeWeightedGraph& e, int start) : 
	_start(start), graph(&e)
{
	int length = e.getLength();

	// Allocate objects
	_connectedTo = new int[length];
	_distanceTo = new double[length];

	// Initialize distances
	double d_inf = (double)INFINITY;
	for (int i = 0; i != length; ++i)
	{
		_distanceTo[i] = d_inf;
	}
	_distanceTo[start] = 0.0;
	_connectedTo[start] = start;

	// Run the algorithm
	for (int i = 0; i != length; ++i)
	{
		for (int j = 0; j != length; ++j)
		{
			const std::list<DirectedEdge>& list = e.adj(j);

			for (std::list<DirectedEdge>::const_iterator it = list.begin(); 
				it != list.end(); it++)
			{
				this->relax(*it);
			}
		}
	}
}

void BellmanFord::relax(DirectedEdge e)
{
	int to = e.to();
	int from = e.from();

	if (_distanceTo[to] > _distanceTo[from] + e.weight())
	{
		_distanceTo[to] = _distanceTo[from] + e.weight();
		_connectedTo[to] = e.from();
	}
}

bool BellmanFord::isArbitrage() const
{
	std::vector<int> arbitrage;
	std::vector<int> passed;

	// Check for arbitrage from the start point
	int index = _connectedTo[_start];
	passed.push_back(_start);
	while (std::find(passed.begin(), passed.end(), index) == passed.end())
	{
		passed.push_back(index);
		index = _connectedTo[index];
	}

	if (index == _start)
	{
		return true;
	}

	return false;
}

std::vector<int> BellmanFord::cyclePath(int v) const
{
	std::vector<int> path;
	std::vector<int> passed;
	path.push_back(v);
	passed.push_back(v);

	// Find the route of the cyclic path
	int index = _connectedTo[v];
	path.push_back(index);
	while (std::find(passed.begin(), passed.end(), index) == passed.end())
	{
		passed.push_back(index);
		index = _connectedTo[index];
		path.push_back(index);
	}

	// Revert the sequence
	int path_size = path.size();
	int middle_path = int(path_size * 0.5);
	int tmp;
	for (int i = 0; i != middle_path; i++)
	{
		tmp = path[i];
		path[i] = path[path_size - i - 1];
		path[path_size - i - 1] = tmp;
	}
	
	return path;
}

double BellmanFord::profit(int v)
{
	double profit = 1.0;

	int index1 = v;
	int index2 = _connectedTo[v];
	profit *= exp(-graph->edge(index2, index1).weight());

	while (index2 != v)
	{
		index1 = index2;
		index2 = _connectedTo[index2];
		profit *= exp(-graph->edge(index2, index1).weight());
	}
	
	return profit - 1.0;
}