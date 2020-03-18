namespace DjikAlg {
	class Network {
	public:
		class Node {
		public:
			class Connection {
			private:
				Node* connectedNode;
				__int32 edgeCost;
			public:
				Connection();
				Connection(Node* n, __int32 c);
				Node* GetDestPointer();
				__int32 GetEdgeCost();

			};

			Node();
			Node(char identifier);
			void AddConnection(Node* destination, __int32 cost);
			char GetConnectedNode(int index);
			__int32 GetConnectionCost(int index);
			__int32 GetConnectionCost(char identifier);

			void RemoveConnection(int index);
			void RemoveConnection(char identifier);
			char GetIdentifier();
			void SetIdentifier(char c);
			__int32 GetAssociatedCost();
			void SetAssociatedCost(__int32 i);
			bool GetIsLocked();
			void LockNode();
			unsigned __int32 GetNumConnections();
			void CleanUp();

		private:

			Connection* connections;
			unsigned __int32 numConnections;
			unsigned __int32 numConnectionSlots;
			char id;
			__int32 associatedCost;
			bool isLocked;
		};

		void AddNode(char identifier);

		void RemoveNode(int index);
		void RemoveNode(char identifier);

		void ConnectNodes(int index1, int index2, __int32 edgeCost);
		void ConnectNodes(int index1, int index2, __int32 forwardCost, __int32 backwardCost);
		void ConnectNodes(char id1, char id2, __int32 edgeCost);
		void ConnectNodes(char id1, char id2, __int32 forwardCost, __int32 backwardCost);

		void DisconnectNodes(int index1, int index2);

		void ListNodes();

		void DetailedInfoForNode(int index);
		void DetailedInfoForNode(char identifier);

		void RunDjikstrasAlgorithm(char startNode, char destNode);

		char* GetOptimalPath(); // remember to add null character
		__int32 GetOptimalCost();

		void ClearAllocatedMemory();

		Network();

	private:
		unsigned __int32 popSize;


		Node* nodes;
		unsigned __int32 numNodes;
		unsigned __int32 numNodeSlots;


		char* optimalPath;
		__int32 optimalCost;
	};
}