#include "stdafx.h"
#include "DjikAlg.h"
#include <iostream>

namespace DjikAlg
{
	//Connection
	Network::Node::Connection::Connection() {
	}

	Network::Node::Connection::Connection(Node* n, unsigned __int32 c) {
		connectedNode = n;
		edgeCost = c;
	}

	Network::Node* Network::Node::Connection::GetDestPointer() {
		return connectedNode;
	}

	void Network::Node::Connection::ShiftPointerDown() {
		connectedNode -= 1;
	}

	unsigned __int32 Network::Node::Connection::GetEdgeCost() {
		return edgeCost;
	}

	//Node
	Network::Node::Node() {
		id = '\0';
		numConnections = 0;
		numConnectionSlots = 0;
		isLocked = false;
	}

	Network::Node::Node(char identifier) {
		id = identifier;
		numConnections = 0;
		numConnectionSlots = 0;
		isLocked = false;
	}

	char Network::Node::GetIdentifier() {
		return id;
	}

	void Network::Node::SetIdentifier(char identifier) {
		id = identifier;
	}

	unsigned __int32 Network::Node::GetNumConnections() {
		return numConnections;
	}

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

	char Network::Node::GetConnectedNode(int index) {
		if (index >= numConnections) {
			std::cout << std::endl << "NodeInfo-CONNECTIONS-oob Error." << std::endl;
			return '\0';
		}

		return connections[index].GetDestPointer()->GetIdentifier();
	}

	unsigned __int32 Network::Node::GetConnectionCost(int index) {
		if (index >= numConnections) {
			std::cout << std::endl << "NodeInfo-CONNECTIONS-oob Error." << std::endl;
			return '\0';
		}

		return connections[index].GetEdgeCost();
	}

	unsigned __int32 Network::Node::GetConnectionCost(char identifier) {
		int index = -1;
		for (int i = 0; i < numConnections; i++) {
			if (identifier == connections[i].GetDestPointer()->GetIdentifier()) index = i;
		}

		if (index == -1) {
			std::cout << std::endl << "NodeInfo-COST-noid Error." << std::endl;
			return 0;
		}

		return GetConnectionCost(index);
	}

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

	void Network::Node::RemoveConnection(char identifier) {
		int index = -1;
		for (int i = 0; i < numConnections; i++) {
			if (identifier == connections[i].GetDestPointer()->GetIdentifier()) index = i;
		}

		if (index == -1) {
			std::cout << std::endl << "RemoveConnection Error: The ID '" << identifier << "' could not be found." << std::endl;
			return;
		}

		RemoveConnection(index);
	}

	void Network::Node::ShiftConnectionPointers(Node* address) {
		for (int i = 0; i < numConnections; i++) {
			if (connections[i].GetDestPointer() > address) {
				connections[i].ShiftPointerDown();
			}
		}
	}

	void Network::Node::SetAssociatedCost(unsigned __int32 i) {
		associatedCost = i;
	}

	unsigned __int32 Network::Node::GetAssociatedCost() {
		return associatedCost;
	}

	bool Network::Node::GetIsLocked() {
		return isLocked;
	}

	void Network::Node::LockNode() {
		isLocked = true;
	}

	//Network
	void Network::AddNode(char identifier) {
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
		else if (numNodes==numNodeSlots) {
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
			if (i > index) nodes[i-1] = nodes[i];
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

	void Network::RemoveNode(char identifier) {
		int index = -1;
		for (int i = 0; i < numNodes; i++) {
			if (identifier == nodes[i].GetIdentifier()) index = i;
		}

		if (index == -1) {
			std::cout << std::endl << "RemoveNode Error: The ID '" << identifier << "' could not be found." << std::endl;
			return;
		}

		RemoveNode(index);
	}

	void Network::ConnectNodes(int index1, int index2, unsigned __int32 forwardCost, unsigned __int32 backwardCost) {
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
				std::cout << std::endl << "ConnectNodes Error: The specified nodes '" << nodes[index1].GetIdentifier() << "' and'" << nodes[index2].GetIdentifier() << "' are already connected."<< std::endl;
				return;
			}
		}

		nodes[index1].AddConnection(&nodes[index2], forwardCost);
		nodes[index2].AddConnection(&nodes[index1], backwardCost);
	}
	
	void Network::ConnectNodes(int index1, int index2, unsigned __int32 edgeCost) {
		ConnectNodes(index1, index2, edgeCost, edgeCost);
	}

	void Network::ConnectNodes(char id1, char id2, unsigned __int32 forwardCost, unsigned __int32 backwardCost) {
		int index1 = -1;
		int index2 = -1;
		for (int i = 0; i < numNodes; i++) {
			if (id1 == nodes[i].GetIdentifier()) index1 = i;
			if (id2 == nodes[i].GetIdentifier()) index2 = i;
		}

		if (index1 == -1) {
			std::cout << std::endl << "RemoveNode Error: The ID '" << id1 << "' could not be found." << std::endl;
			return;
		}
		if (index2 == -1) {
			std::cout << std::endl << "RemoveNode Error: The ID '" << id2 << "' could not be found." << std::endl;
			return;
		}

		ConnectNodes(index1, index2, forwardCost, backwardCost);
	}

	void Network::ConnectNodes(char id1, char id2, unsigned __int32 edgeCost) {
		int index1 = -1;
		int index2 = -1;
		for (int i = 0; i < numNodes; i++) {
			if (id1 == nodes[i].GetIdentifier()) index1 = i;
			if (id2 == nodes[i].GetIdentifier()) index2 = i;
		}

		if (index1 == -1) {
			std::cout << std::endl << "RemoveNode Error: The ID '" << id1 << "' could not be found." << std::endl;
			return;
		}
		if (index2 == -1) {
			std::cout << std::endl << "RemoveNode Error: The ID '" << id2 << "' could not be found." << std::endl;
			return;
		}

		ConnectNodes(index1, index2, edgeCost);
	}

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

	void Network::DisconnectNodes(char id1, char id2) {
		int index1 = -1;
		int index2 = -1;
		for (int i = 0; i < numNodes; i++) {
			if (id1 == nodes[i].GetIdentifier()) index1 = i;
			if (id2 == nodes[i].GetIdentifier()) index2 = i;
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

	void Network::DetailedInfoForNode(char identifier) {
		int index = -1;
		for (int i = 0; i < numNodes; i++) {
			if (identifier == nodes[i].GetIdentifier()) index = i;
		}

		if (index == -1) {
			std::cout << std::endl << "NodeInfo Error: The ID '" << identifier << "' could not be found." << std::endl;
			return;
		}

		DetailedInfoForNode(index);
	}

	void Network::RenameNode(int index, char newName) {
		for (int i = 0; i < numNodes; i++) {
			if (newName == nodes[i].GetIdentifier()) {
				std::cout << std::endl << "RenameNode Error: Node with ID '" << newName << "' already exists, and names cannot be duplicated." << std::endl;
				return;
			}
		}

		nodes[index].SetIdentifier(newName);
	}

	void Network::RenameNode(char identifier, char newName) {
		int index = -1;
		for (int i = 0; i < numNodes; i++) {
			if (identifier == nodes[i].GetIdentifier()) index = i;
		}

		if (index == -1) {
			std::cout << std::endl << "RenameNode Error: The ID '" << identifier << "' could not be found." << std::endl;
			return;
		}

		RenameNode(index, newName);
	}

	void Network::ListNodes() {
		std::cout << std::endl << "Node list:" << std::endl;
		for (int i = 0; i < numNodes; i++) {
			std::cout << "\tNode " << nodes[i].GetIdentifier() << " (index = " << i << "), " << nodes[i].GetNumConnections() << " connections" << std::endl;
		}
	}

	Network::Network() {
		numNodes = 0;
		numNodeSlots = 0;
	}
}