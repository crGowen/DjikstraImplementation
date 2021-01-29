// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "nodegraph.h"
#include <iostream>

//CONNECTION
Connection::Connection(const int i, const unsigned __int32 c) : index{ i }, cost{ c } {}

//NODE
Node::Node(const std::string& n) : name{ n }, isLocked{ false }, associatedCost{ -1 }, priorNode{ -1 } {}

bool Node::CreateConnection(const int &i, unsigned __int32 c) {
	this->connections.push_back(Connection(i, c));

	return true;
}

//GRAPH
Graph::Graph(const std::string& n) : name{ n }, optimalRoute{ "" }, optimalRouteCost{ 0 } {}

Graph::ErrState Graph::CreateNode(const std::string& newNode) {
	std::vector<Node>::iterator it = std::find_if(this->nodes.begin(), this->nodes.end(),
		[&] (Node const& n) { return n.name == newNode; }
	);

	if (it != this->nodes.end()) return ErrState::attemptToCreateExistingNode;

	this->nodes.push_back(Node(newNode));

	return ErrState::success;
}

Graph::ErrState Graph::CoutListNodes() {
	std::cout << "Graph: " << this->name << ". Contains " << this->nodes.size() << " nodes.\n\n";
	for (Node n : this->nodes) {
		std::cout << "Node " << n.name << ":\n";
		for (Connection c : n.connections) {
			std::cout << "\tConnected to " << this->nodes[c.index].name << " with cost " <<  c.cost << ".\n";
		}
		std::cout << "\n";
	}

	return ErrState::success;
}

Graph::ErrState Graph::ConnectNodes(const std::string& node1, const std::string& node2, const unsigned __int32 forwardCost, const unsigned __int32 backwardCost) {
	int index1, index2;
	
	// Get indexes
	std::vector<Node>::iterator it = std::find_if(this->nodes.begin(), this->nodes.end(),
		[&](Node const& n) { return n.name == node1; }
	);

	index1 = it - this->nodes.begin();
	if (it == this->nodes.end()) {
		Graph::CreateNode(node1);
	}

	it = std::find_if(this->nodes.begin(), this->nodes.end(),
		[&](Node const& n) { return n.name == node2; }
	);

	index2 = it - this->nodes.begin();
	if (it == this->nodes.end()) {
		Graph::CreateNode(node2);
	}

	// Pass to index function
	if (this->nodes[index1].CreateConnection(index2, forwardCost) && this->nodes[index2].CreateConnection( index1, backwardCost))
		return ErrState::success;
	else
		return ErrState::unkCNErr;
}

Graph::ErrState Graph::ConnectNodes(const std::string& node1, const std::string& node2, const unsigned __int32 edgeCost) {
	return Graph::ConnectNodes(node1, node2, edgeCost, edgeCost);
}

Graph::ErrState Graph::RunDjikstrasAlgorithm(const std::string& start, const std::string& end) {
	int indexS, indexE;

	// Get indexes
	std::vector<Node>::iterator it = std::find_if(this->nodes.begin(), this->nodes.end(),
		[&](Node const& n) { return n.name == start; }
	);

	indexS = it - this->nodes.begin();
	if (it == this->nodes.end()) {
		return ErrState::nodeNotFound;
	}

	it = std::find_if(this->nodes.begin(), this->nodes.end(),
		[&](Node const& n) { return n.name == end; }
	);

	indexE = it - this->nodes.begin();
	if (it == this->nodes.end()) {
		return ErrState::nodeNotFound;
	}

	for (Node n : this->nodes) {
		n.isLocked = false;
		n.associatedCost = -1;
	}

	int currentNode = indexS;
	this->nodes[currentNode].associatedCost = 0;
	this->nodes[currentNode].isLocked = true;

	int lowestCost;
	int lcIndex;

	while (currentNode != indexE) {
		// traverse all connections to current node and update where necessary nearby nodes
		for (Connection c : this->nodes[currentNode].connections) {
			if (this->nodes[c.index].associatedCost < 0
				||
				(this->nodes[currentNode].associatedCost + c.cost) < this->nodes[c.index].associatedCost) {

				this->nodes[c.index].associatedCost = this->nodes[currentNode].associatedCost + c.cost;
				this->nodes[c.index].priorNode = currentNode;
			}
		}

		// find the next node to be set to currentNode
		lowestCost = -1;
		lcIndex = -1;
		for (int i = 0; i < this->nodes.size(); i++) {
			if (!this->nodes[i].isLocked && this->nodes[i].associatedCost > 0) {
				if (lowestCost < 0 || this->nodes[i].associatedCost < lowestCost) {
					lowestCost = this->nodes[i].associatedCost;
					lcIndex = i;
				}
			}
		}
		currentNode = lcIndex;
		if (currentNode < 0) return ErrState::noPossiblePath;
		this->nodes[currentNode].isLocked = true;
	}

	this->optimalRouteCost = this->nodes[currentNode].associatedCost;

	this->optimalRoute = this->nodes[currentNode].name;
	currentNode = this->nodes[currentNode].priorNode;
	while (currentNode != indexS) {
		this->optimalRoute = this->nodes[currentNode].name + " -> " + this->optimalRoute;
		currentNode = this->nodes[currentNode].priorNode;
	}
	this->optimalRoute = this->nodes[currentNode].name + " -> " + this->optimalRoute;

	return ErrState::success;
}

