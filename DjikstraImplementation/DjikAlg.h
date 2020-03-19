namespace DjikAlg {
	class Network {
	public:
		class Node {
		public:
			class Connection {
			private:
				Node* connectedNode;
				unsigned __int32 edgeCost;
			public:
				Connection();
				Connection(Node* n, unsigned __int32 c);
				Node* GetDestPointer();
				void ShiftPointerDown();
				unsigned __int32 GetEdgeCost();

			};

			Node();
			Node(char identifier);
			void AddConnection(Node* destination, unsigned __int32 cost);
			char GetConnectedNode(int index);
			unsigned __int32 GetConnectionCost(int index);
			unsigned __int32 GetConnectionCost(char identifier);

			unsigned __int32 GetConnectionAssociatedCost(int index);

			void RemoveConnection(int index);
			void RemoveConnection(char identifier);
			char GetIdentifier();
			void SetIdentifier(char identifier);
			unsigned __int32 GetAssociatedCost();
			void SetAssociatedCost(unsigned __int32 i);
			bool GetIsLocked();
			bool GetConnectionIsLocked(int index);
			void LockNode();
			void ShiftConnectionPointers(Node* address);
			unsigned __int32 GetNumConnections();
			char GetPriorNode();
			void SetPriorNode(char c);

			void ClearConnections();

			void SetAsConnectionPriorNode(int index, unsigned __int32 newAssociatedCost);

		private:

			Connection* connections;
			unsigned __int32 numConnections;
			unsigned __int32 numConnectionSlots;
			char id;
			unsigned __int32 associatedCost;
			bool isLocked;
			char priorNode;
		};

		void AddNode(char identifier);

		void RemoveNode(int index);
		void RemoveNode(char identifier);

		void ConnectNodes(int index1, int index2, unsigned __int32 edgeCost);
		void ConnectNodes(int index1, int index2, unsigned __int32 forwardCost, unsigned __int32 backwardCost);
		void ConnectNodes(char id1, char id2, unsigned __int32 edgeCost);
		void ConnectNodes(char id1, char id2, unsigned __int32 forwardCost, unsigned __int32 backwardCost);

		void DisconnectNodes(int index1, int index2);
		void DisconnectNodes(char id1, char id2);

		void RenameNode(int index, char newName);
		void RenameNode(char identifier, char newName);

		void ListNodes();
		void DetailedInfoAllNodes();

		void DetailedInfoForNode(int index);
		void DetailedInfoForNode(char identifier);

		void RunDjikstrasAlgorithm(int startNode, int destNode);
		void RunDjikstrasAlgorithm(char startNode, char destNode);		

		char* GetOptimalPath(); // remember to add null character
		__int32 GetOptimalCost();

		void ClearAllocatedMemory();

		Network();

	private:
		Node* nodes;
		unsigned __int32 numNodes;
		unsigned __int32 numNodeSlots;


		char* optimalPath;
		__int32 optimalCost;
	};
}