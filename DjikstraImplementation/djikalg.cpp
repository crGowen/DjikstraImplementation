#include "stdafx.h"
#include "djikalg.h"

// Potential TODO: replace console output with ENUMS so that this library doesn't require a console application.


	// Connection

	// Default constructor
Network::Node::Connection::Connection() {
}

// Parametized constructor
Network::Node::Connection::Connection(Node* n, unsigned __int32 c) {
	connectedNode = n;
	edgeCost = c;
}

// Return point to the node at the other end of the connection
Network::Node* Network::Node::Connection::GetDestPointer() {
	return connectedNode;
}

// Shift pointer addresses down (used when an entry in the node list is removed)
void Network::Node::Connection::ShiftPointerDown() {
	connectedNode -= 1;
}

// Return the edge cost of the connection
unsigned __int32 Network::Node::Connection::GetEdgeCost() {
	return edgeCost;
}

// Node

// Default constructor
Network::Node::Node() {
	id = '\0';
	numConnections = 0;
	numConnectionSlots = 0;
	isLocked = false;
	associatedCost = 2000000000;
	priorNode = '\0';
}

// Parametized constructor
Network::Node::Node(char identifier) {
	id = identifier;
	numConnections = 0;
	numConnectionSlots = 0;
	isLocked = false;
	associatedCost = 2000000000;
	priorNode = '\0';
}

// Get the prior node property (used for finding the optimal route during the run of Djikstra's Algorithm)
char Network::Node::GetPriorNode() {
	return priorNode;
}

// Set the prior node property (used for finding the optimal route during the run of Djikstra's Algorithm)
void Network::Node::SetPriorNode(char c) {
	priorNode = c;
}

// For the node in the *this* node's list of connections at the specified index, set *this* node as its prior node
void Network::Node::SetAsConnectionPriorNode(int index, unsigned __int32 newAssociatedCost) {
	connections[index].GetDestPointer()->SetPriorNode(this->GetIdentifier());
	connections[index].GetDestPointer()->SetAssociatedCost(newAssociatedCost);
}

// Return the character id for this node
char Network::Node::GetIdentifier() {
	return id;
}

// Set the character id for this node
void Network::Node::SetIdentifier(char identifier) {
	id = identifier;
}

// Get the number of connections that are currently registered to this node
unsigned __int32 Network::Node::GetNumConnections() {
	return numConnections;
}

// Add a new connection to the list of connections registered to this node
void Network::Node::AddConnection(Node* destination, unsigned __int32 cost) {
	if (numConnectionSlots == 0) {
		connections = new Connection[8];
		numConnectionSlots = 8;
	}
	else if (numConnections == numConnectionSlots) {
		Connection* tempConnections = new Connection[numConnections];

		for (int i = 0; i < numConnections; i++) {
			tempConnections[i] = connections[i];
		}

		delete[] connections;
		numConnectionSlots += 8;
		connections = new Connection[numConnectionSlots];

		for (int i = 0; i < numConnections; i++) {
			connections[i] = tempConnections[i];
		}

		delete[] tempConnections;
	}

	connections[numConnections] = Connection(destination, cost);
	numConnections++;
}

// Get the ID of the node at the end of the connection specified by the index argument
char Network::Node::GetConnectedNode(int index) {
	if (index >= numConnections) {
		std::cout << std::endl << "NodeInfo-CONNECTIONS-oob Error." << std::endl;
		return '\0';
	}

	return connections[index].GetDestPointer()->GetIdentifier();
}

// Get the edge cost of the connection specified by the index argument
unsigned __int32 Network::Node::GetConnectionCost(int index) {
	if (index >= numConnections) {
		std::cout << std::endl << "NodeInfo-CONNECTIONS-oob Error." << std::endl;
		return '\0';
	}

	return connections[index].GetEdgeCost();
}

// Get the edge cost of the connection that matches the specified node ID argument
unsigned __int32 Network::Node::GetConnectionCost(char identifier) {
	int index = -1;
	for (int i = 0; i < numConnections; i++) {
		if (identifier == connections[i].GetDestPointer()->GetIdentifier()) {
			index = i;
			i = numConnections;
		}
	}

	if (index == -1) {
		std::cout << std::endl << "NodeInfo-COST-noid Error." << std::endl;
		return 0;
	}

	return GetConnectionCost(index);
}

// Get the associated cost that is registered to a connection's node, specified by index argument (used in Djikstra's Algorithm)
unsigned __int32 Network::Node::GetConnectionAssociatedCost(int index) {
	return connections[index].GetDestPointer()->GetAssociatedCost();
}

// Remove a connection from the list of connections specified by the index argument
void Network::Node::RemoveConnection(int index) {
	if (index >= numConnections) {
		std::cout << std::endl << "RemoveConnection-CONNECTIONS-oob Error." << std::endl;
		return;
	}

	for (int i = 0; i < numConnections; i++) {
		if (i > index) connections[i - 1] = connections[i];
	}

	numConnections--;
	if (numConnectionSlots - numConnections >= 12) {
		Connection* tempConnections = new Connection[numConnections];

		for (int i = 0; i < numConnections; i++) {
			tempConnections[i] = connections[i];
		}

		delete[] connections;
		numConnectionSlots -= 8;
		connections = new Connection[numConnectionSlots];

		for (int i = 0; i < numConnections; i++) {
			connections[i] = tempConnections[i];
		}

		delete[] tempConnections;
	}
}

// Remove a connection from the list of connections specified by the node ID argument
void Network::Node::RemoveConnection(char identifier) {
	int index = -1;
	for (int i = 0; i < numConnections; i++) {
		if (identifier == connections[i].GetDestPointer()->GetIdentifier()) {
			index = i;
			i = numConnections;
		}
	}

	if (index == -1) {
		std::cout << std::endl << "RemoveConnection Error: The ID '" << identifier << "' could not be found." << std::endl;
		return;
	}

	RemoveConnection(index);
}

// For a given memory address (provided in the argument) shift all relevant pointers (called when a code is removed from the list of nodes; works as a quasi-interface for ShiftPointerDown() )
void Network::Node::ShiftConnectionPointers(Node* address) {
	for (int i = 0; i < numConnections; i++) {
		if (connections[i].GetDestPointer() > address) {
			connections[i].ShiftPointerDown();
		}
	}
}

// Set the associated cost that is registered to this node (used in Djikstra's Algorithm)
void Network::Node::SetAssociatedCost(unsigned __int32 i) {
	associatedCost = i;
}

// Get the associated cost that is registered to this node (used in Djikstra's Algorithm)
unsigned __int32 Network::Node::GetAssociatedCost() {
	return associatedCost;
}

// Get the state of the isLocked property (used in Djikstra's Algorithm)
bool Network::Node::GetIsLocked() {
	return isLocked;
}

// Get the isLocked property state for a connection specified by index argument
bool Network::Node::GetConnectionIsLocked(int index) {
	return connections[index].GetDestPointer()->GetIsLocked();
}

// Set this node's isLocked property to true
void Network::Node::LockNode() {
	isLocked = true;
}

// Set this node's isLocked property to false
void Network::Node::UnlockNode() {
	isLocked = false;
}

// Clear the dynamic memory for connections array
void Network::Node::ClearConnections() {
	delete[] connections;
}

//Network

// Add a node to the network, specifying what its ID should be
void Network::AddNode(char identifier) {
	if (identifier == '\0') {
		std::cout << std::endl << "AddNode Error: ID is invalid!" << std::endl;
		return;
	}

	for (int i = 0; i < numNodes; i++) {
		if (identifier == nodes[i].GetIdentifier()) {
			std::cout << std::endl << "AddNode Error: Node with ID '" << identifier << "' already exists, and will not be added." << std::endl;
			return;
		}
	}

	if (numNodeSlots == 0) {
		nodes = new Node[8];
		numNodeSlots = 8;
	}
	else if (numNodes == numNodeSlots) {
		Node* tempNodes = new Node[numNodes];

		for (int i = 0; i < numNodes; i++) {
			tempNodes[i] = nodes[i];
		}

		delete[] nodes;
		numNodeSlots += 8;
		nodes = new Node[numNodeSlots];

		for (int i = 0; i < numNodes; i++) {
			nodes[i] = tempNodes[i];
		}

		delete[] tempNodes;
	}

	nodes[numNodes] = Node(identifier);
	numNodes++;
}

// Remove a node from the network, specified by index
void Network::RemoveNode(int index) {
	if (index >= numNodes) {
		std::cout << std::endl << "RemoveNode Error: The specified index '" << index << "' is too high for the number of nodes in the network," << std::endl << "you must specify an index less than " << numNodes << "." << std::endl;
		return;
	}

	while (nodes[index].GetNumConnections() > 0) {
		Network::DisconnectNodes(nodes[index].GetIdentifier(), nodes[index].GetConnectedNode(0));
	}

	for (int i = 0; i < numNodes; i++) {
		nodes[i].ShiftConnectionPointers(&nodes[index]);
		if (i > index) nodes[i - 1] = nodes[i];
	}
	numNodes--;
	if (numNodeSlots - numNodes >= 12) {
		Node* tempNodes = new Node[numNodes];

		for (int i = 0; i < numNodes; i++) {
			tempNodes[i] = nodes[i];
		}

		delete[] nodes;
		numNodeSlots -= 8;
		nodes = new Node[numNodeSlots];

		for (int i = 0; i < numNodes; i++) {
			nodes[i] = tempNodes[i];
		}

		delete[] tempNodes;
	}
}

// Remove a node from the network, specified by node ID
void Network::RemoveNode(char identifier) {
	int index = -1;
	for (int i = 0; i < numNodes; i++) {
		if (identifier == nodes[i].GetIdentifier()) {
			index = i;
			i = numNodes;
		}
	}

	if (index == -1) {
		std::cout << std::endl << "RemoveNode Error: The ID '" << identifier << "' could not be found." << std::endl;
		return;
	}

	RemoveNode(index);
}

// Connect two nodes together, specified by index, with an asymmetrical edgecost (use case example: construct an energy-cost network, going up-hill costs more than going down, so the edgecost needs to be asymmetrical to produce accurate result)
void Network::ConnectNodes(int index1, int index2, unsigned __int32 forwardCost, unsigned __int32 backwardCost) {
	if (index1 == index2) {
		std::cout << std::endl << "ConnectNodes Error: You cannot connect a node to itself." << std::endl;
		return;
	}
	if (index1 >= numNodes) {
		std::cout << std::endl << "ConnectNodes Error: The specified index '" << index1 << "' is too high for the number of nodes in the network," << std::endl << "you must specify an index less than " << numNodes << "." << std::endl;
		return;
	}
	if (index2 >= numNodes) {
		std::cout << std::endl << "ConnectNodes Error: The specified index '" << index2 << "' is too high for the number of nodes in the network," << std::endl << "you must specify an index less than " << numNodes << "." << std::endl;
		return;
	}

	for (int i = 0; i < nodes[index1].GetNumConnections(); i++)
	{
		if (nodes[index1].GetConnectedNode(i) == nodes[index2].GetIdentifier()) {
			std::cout << std::endl << "ConnectNodes Error: The specified nodes '" << nodes[index1].GetIdentifier() << "' and'" << nodes[index2].GetIdentifier() << "' are already connected." << std::endl;
			return;
		}
	}

	if (forwardCost >= 1000 || backwardCost >= 1000) {
		std::cout << std::endl << "ConnectNodes Error: Edge costs must be less than 1000!" << std::endl;
		return;
	}

	nodes[index1].AddConnection(&nodes[index2], forwardCost);
	nodes[index2].AddConnection(&nodes[index1], backwardCost);
}

// Connect two nodes together, specified by index, with specified edgecost
void Network::ConnectNodes(int index1, int index2, unsigned __int32 edgeCost) {
	ConnectNodes(index1, index2, edgeCost, edgeCost);
}

// Connect two nodes together, specified by node IDs, with an asymmetrical edgecost (use case example: construct an energy-cost network, going up-hill costs more than going down, so the edgecost needs to be asymmetrical to produce accurate result)
void Network::ConnectNodes(char id1, char id2, unsigned __int32 forwardCost, unsigned __int32 backwardCost) {
	int index1 = -1;
	int index2 = -1;
	for (int i = 0; i < numNodes; i++) {
		if (id1 == nodes[i].GetIdentifier()) index1 = i;
		if (id2 == nodes[i].GetIdentifier()) index2 = i;
		if (index1 >= 0 && index2 >= 0) {
			i = numNodes;
		}
	}

	if (index1 == -1) {
		std::cout << std::endl << "ConnectNodes Error: The ID '" << id1 << "' could not be found." << std::endl;
		return;
	}
	if (index2 == -1) {
		std::cout << std::endl << "ConnectNodes Error: The ID '" << id2 << "' could not be found." << std::endl;
		return;
	}

	ConnectNodes(index1, index2, forwardCost, backwardCost);
}

// Connect two nodes together, specified by node IDs, with specified edgecost
void Network::ConnectNodes(char id1, char id2, unsigned __int32 edgeCost) {
	int index1 = -1;
	int index2 = -1;
	for (int i = 0; i < numNodes; i++) {
		if (id1 == nodes[i].GetIdentifier()) index1 = i;
		if (id2 == nodes[i].GetIdentifier()) index2 = i;
		if (index1 >= 0 && index2 >= 0) {
			i = numNodes;
		}
	}

	if (index1 == -1) {
		std::cout << std::endl << "ConnectNodes Error: The ID '" << id1 << "' could not be found." << std::endl;
		return;
	}
	if (index2 == -1) {
		std::cout << std::endl << "ConnectNodes Error: The ID '" << id2 << "' could not be found." << std::endl;
		return;
	}

	ConnectNodes(index1, index2, edgeCost);
}

// Erase the connection between two nodes, specified by index
void Network::DisconnectNodes(int index1, int index2) {
	if (index1 >= numNodes) {
		std::cout << std::endl << "DisconnectNodes Error: The specified index '" << index1 << "' is too high for the number of nodes in the network," << std::endl << "you must specify an index less than " << numNodes << "." << std::endl;
		return;
	}
	if (index2 >= numNodes) {
		std::cout << std::endl << "DisconnectNodes Error: The specified index '" << index2 << "' is too high for the number of nodes in the network," << std::endl << "you must specify an index less than " << numNodes << "." << std::endl;
		return;
	}

	bool nodeFound = false;
	for (int i = 0; i < nodes[index1].GetNumConnections(); i++)
	{
		if (nodes[index1].GetConnectedNode(i) == nodes[index2].GetIdentifier()) {
			nodes[index1].RemoveConnection(i);
			nodeFound = true;
		}
	}
	if (!nodeFound) {
		std::cout << std::endl << "DisconnectNodes Error: The specified nodes '" << nodes[index1].GetIdentifier() << "' (index = " << index1 << ") and '" << nodes[index2].GetIdentifier() << "' (index = " << index2 << ") are already disconnected." << std::endl;
		return;
	}

	for (int i = 0; i < nodes[index2].GetNumConnections(); i++)
	{
		if (nodes[index2].GetConnectedNode(i) == nodes[index1].GetIdentifier()) {
			nodes[index2].RemoveConnection(i);
		}
	}
}

// Erase the connection between two nodes, specified by node IDs
void Network::DisconnectNodes(char id1, char id2) {
	int index1 = -1;
	int index2 = -1;
	for (int i = 0; i < numNodes; i++) {
		if (id1 == nodes[i].GetIdentifier()) index1 = i;
		if (id2 == nodes[i].GetIdentifier()) index2 = i;
		if (index1 >= 0 && index2 >= 0) {
			i = numNodes;
		}
	}

	if (index1 == -1) {
		std::cout << std::endl << "DisconnectNodes Error: The ID '" << id1 << "' could not be found." << std::endl;
		return;
	}
	if (index2 == -1) {
		std::cout << std::endl << "DisconnectNodes Error: The ID '" << id2 << "' could not be found." << std::endl;
		return;
	}

	DisconnectNodes(index1, index2);
}

// Get detailed info on the node specified by index
void Network::DetailedInfoForNode(int index) {
	if (index >= numNodes) {
		std::cout << std::endl << "NodeInfo Error: The specified index '" << index << "' is too high for the number of nodes in the network," << std::endl << "you must specify an index less than " << numNodes << "." << std::endl;
		return;
	}

	if (nodes[index].GetNumConnections() == 0) {
		std::cout << std::endl << "Node " << nodes[index].GetIdentifier() << " (index = " << index << ") has no connections. " << std::endl;
		return;
	}

	std::cout << std::endl << "Connections for node " << nodes[index].GetIdentifier() << " (index = " << index << "): " << std::endl;
	for (int i = 0; i < nodes[index].GetNumConnections(); i++)
	{
		std::cout << "\tConnection to node '" << nodes[index].GetConnectedNode(i) << "' with cost " << nodes[index].GetConnectionCost(i) << std::endl;
	}
}

// Get detailed info on the node specified by node ID
void Network::DetailedInfoForNode(char identifier) {
	int index = -1;
	for (int i = 0; i < numNodes; i++) {
		if (identifier == nodes[i].GetIdentifier()) {
			index = i;
			i = numNodes;
		}
	}

	if (index == -1) {
		std::cout << std::endl << "NodeInfo Error: The ID '" << identifier << "' could not be found." << std::endl;
		return;
	}

	DetailedInfoForNode(index);
}

// Rename the specified (by index) node,  giving it a new ID
void Network::RenameNode(int index, char newName) {
	for (int i = 0; i < numNodes; i++) {
		if (newName == nodes[i].GetIdentifier()) {
			std::cout << std::endl << "RenameNode Error: Node with ID '" << newName << "' already exists, and names cannot be duplicated." << std::endl;
			return;
		}
	}

	nodes[index].SetIdentifier(newName);
}

// Rename the specified (by node ID) node, giving it a new ID
void Network::RenameNode(char identifier, char newName) {
	int index = -1;
	for (int i = 0; i < numNodes; i++) {
		if (identifier == nodes[i].GetIdentifier()) {
			index = i;
			i = numNodes;
		}
	}

	if (index == -1) {
		std::cout << std::endl << "RenameNode Error: The ID '" << identifier << "' could not be found." << std::endl;
		return;
	}

	RenameNode(index, newName);
}

// Provide a non-detailed list of nodes in the network
void Network::ListNodes() {
	std::cout << std::endl << "Node list:" << std::endl;
	for (int i = 0; i < numNodes; i++) {
		std::cout << "\tNode " << nodes[i].GetIdentifier() << " (index = " << i << "), " << nodes[i].GetNumConnections() << " connections" << std::endl;
	}
}

// Provide a detailed list of nodes in the network
void Network::DetailedInfoAllNodes() {
	std::cout << std::endl << "Node list:" << std::endl;
	for (int i = 0; i < numNodes; i++) {
		std::cout << "\tNode " << nodes[i].GetIdentifier() << " (index = " << i << "), " << nodes[i].GetNumConnections() << " connections" << std::endl;
		for (int j = 0; j < nodes[i].GetNumConnections(); j++) {
			std::cout << "\t\tConnection to node '" << nodes[i].GetConnectedNode(j) << "' with cost " << nodes[i].GetConnectionCost(j) << std::endl;
		}
		std::cout << std::endl;
	}
}

// Run Djikstra's Algorithm on the network, specified by index
void Network::RunDjikstrasAlgorithm(int startNode, int destNode) {
	for (int i = 0; i < numNodes; i++) {
		nodes[i].SetAssociatedCost(2000000000);
		nodes[i].UnlockNode();
	}

	nodes[startNode].SetAssociatedCost(0);

	//algorithm loop
	while (true) {
		unsigned __int32 lowestAssociatedCost = 2100000000;
		int selectedNode = 0;

		// get node with lowest associated cost
		for (int i = 0; i < numNodes; i++) {
			if (nodes[i].GetAssociatedCost() < lowestAssociatedCost && !nodes[i].GetIsLocked()) {
				selectedNode = i;
				lowestAssociatedCost = nodes[i].GetAssociatedCost();
			}
		}

		//lock it
		nodes[selectedNode].LockNode();

		// exit algorithm is we lock the destination/target node
		if (selectedNode == destNode) {
			if (nodes[selectedNode].GetAssociatedCost() == 2000000000) {
				std::cout << std::endl << "RunDjikstrasAlgorithm Error: No possible route found between '" << nodes[startNode].GetIdentifier() << "' (index = " << startNode << ") and '" << nodes[destNode].GetIdentifier() << "' (index = " << destNode << ")," << std::endl << "optimal cost and optimal path are either undefined or equal to previous algorithm run's result." << std::endl;
				return;
			}

			delete[] optimalPath;
			char* tempString = new char[numNodes + 1];

			optimalCost = nodes[selectedNode].GetAssociatedCost();

			tempString[numNodes] = '\0';

			int pathLength = 1;
			tempString[numNodes - 1] = nodes[selectedNode].GetIdentifier(); // start 1 before the last char (last char is the null char)	

			// write optimalpath string
			while (true) {
				// we only enter this if statement's body once we've built the full string representing the route (which will have null chars at the front), until then we go to the for loop below
				if (tempString[numNodes - pathLength] == nodes[startNode].GetIdentifier()) {
					// reduce string to the actual length of the path (+ null char)
					optimalPath = new char[pathLength + 1];
					optimalPath[pathLength] = '\0';
					for (int i = 0; i < pathLength; i++) {
						//check if place matches an node ID (i.e. that is ISN'T A NULL CHAR)
						optimalPath[i] = tempString[numNodes - pathLength + i];
					}

					delete[] tempString;
					optimalsFound = true;
					return;
				}

				// if we have not yet built the string representing the route, do this (find the index of the current end of the route-string)...
				int index = -1;

				for (int i = 0; i < numNodes; i++) {
					if (tempString[numNodes - pathLength] == nodes[i].GetIdentifier()) {
						index = i;
						i = numNodes;
					}
				}
				pathLength++;
				//then use this index to get the prior node in the optimal route.
				tempString[numNodes - pathLength] = nodes[index].GetPriorNode();
			}
		}

		// else run through each connection and update associated costs if needed
		for (int i = 0; i < nodes[selectedNode].GetNumConnections(); i++) {
			if (!nodes[selectedNode].GetConnectionIsLocked(i)) {
				unsigned int moveCost = nodes[selectedNode].GetAssociatedCost() + nodes[selectedNode].GetConnectionCost(i);
				if (moveCost < nodes[selectedNode].GetConnectionAssociatedCost(i)) {
					nodes[selectedNode].SetAsConnectionPriorNode(i, moveCost);
				}
			}
		}

	}
}

// Run Djikstra's Algorithm on the network, specified by node IDs
void Network::RunDjikstrasAlgorithm(char startNode, char destNode) {
	int index1 = -1;
	int index2 = -1;
	for (int i = 0; i < numNodes; i++) {
		if (startNode == nodes[i].GetIdentifier()) index1 = i;
		if (destNode == nodes[i].GetIdentifier()) index2 = i;
		if (index1 >= 0 && index2 >= 0) {
			i = numNodes;
		}
	}

	if (index1 == -1) {
		std::cout << std::endl << "RunDjikstrasAlgorithm Error: The ID '" << startNode << "' could not be found." << std::endl;
		return;
	}
	if (index2 == -1) {
		std::cout << std::endl << "RunDjikstrasAlgorithm Error: The ID '" << destNode << "' could not be found." << std::endl;
		return;
	}

	RunDjikstrasAlgorithm(index1, index2);
}

// After Djikstra's Algorithm has run on the network, get the optimal path as a nullchar terminated char array
char* Network::GetOptimalPath() {
	if (!optimalsFound) std::cout << std::endl << "GetOptimalPath Error: Algorithm has not yet run, and the optimal path is not yet defined (UNDEFINED)." << std::endl;
	return optimalPath;

}

// After Djikstra's Algorithm has run on the network, get the optimal cost
__int32 Network::GetOptimalCost() {
	if (!optimalsFound)
		std::cout << std::endl << "GetOptimalCost Error: Algorithm has not yet run, and the optimal cost is not yet defined (0)." << std::endl;
	return optimalCost;
}

// Default constructor
Network::Network() {
	numNodes = 0;
	numNodeSlots = 0;
	optimalsFound = false;

	// optimalPath and cost = undefined
	optimalPath = new char[10];
	optimalPath[0] = 'U';
	optimalPath[1] = 'N';
	optimalPath[2] = 'D';
	optimalPath[3] = 'E';
	optimalPath[4] = 'F';
	optimalPath[5] = 'I';
	optimalPath[6] = 'N';
	optimalPath[7] = 'E';
	optimalPath[8] = 'D';
	optimalPath[9] = '\0';

	optimalCost = 0;
}

// Clear all dynamically allocated memory for this network object (attempting to use this network object's methods after calling ClearAllocatedMemory() may cause undefined behavior, only use this function once you are finished)
void Network::ClearAllocatedMemory() {
	delete[] optimalPath;

	for (int i = 0; i < numNodes; i++) {
		nodes[i].ClearConnections();
	}

	delete[] nodes;
}
