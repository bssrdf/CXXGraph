/***********************************************************/
/***      ______  ____  ______                 _         ***/
/***     / ___\ \/ /\ \/ / ___|_ __ __ _ _ __ | |__	     ***/
/***    | |    \  /  \  / |  _| '__/ _` | '_ \| '_ \	 ***/
/***    | |___ /  \  /  \ |_| | | | (_| | |_) | | | |    ***/
/***     \____/_/\_\/_/\_\____|_|  \__,_| .__/|_| |_|    ***/
/***                                    |_|			     ***/
/***********************************************************/
/***     Header-Only C++ Library for Graph			     ***/
/***	 Representation and Algorithms				     ***/
/***********************************************************/
/***     Author: ZigRazor			     			     ***/
/***	 E-Mail: zigrazor@gmail.com 				     ***/
/***********************************************************/
/***	 Collaboration: ----------- 				     ***/
/***********************************************************/
/***	 License: AGPL v3.0							     ***/
/***********************************************************/

#ifndef __CXXGRAPH_H__
#define __CXXGRAPH_H__

#pragma once

#include <utility>
#include <set>
#include <map>
#include <optional>
#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <string>
#include <cstring>
#include <functional>
#include <fstream>
#include <limits.h>
#include <mutex>
#include <set>
#include <atomic>
#include <thread>
#include <cmath>
#include "zlib.h"

namespace CXXGRAPH
{
	//STRING ERROR CONST EXPRESSION
	constexpr char ERR_NO_DIR_OR_UNDIR_EDGE[] = "Edge are neither Directed neither Undirected";
	constexpr char ERR_NO_WEIGHTED_EDGE[] = "Edge are not Weighted";
	constexpr char ERR_TARGET_NODE_NOT_REACHABLE[] = "Target Node not Reachable";
	constexpr char ERR_TARGET_NODE_NOT_IN_GRAPH[] = "Target Node not inside Graph";
	constexpr char ERR_SOURCE_NODE_NOT_IN_GRAPH[] = "Source Node not inside Graph";
	///////////////////////////////
	constexpr double INF_DOUBLE = std::numeric_limits<double>::max();
	template <typename T>
	class Node;
	template <typename T>
	class Edge;
	template <typename T>
	class DirectedEdge;
	template <typename T>
	class UndirectedEdge;
	template <typename T>
	class DirectedWeightedEdge;
	template <typename T>
	class UndirectedWeightedEdge;
	template <typename T>
	class Graph;
	template <typename T>
	class Partition;

	class Weighted;

	class ThreadSafe;

	template <typename T>
	class Writer;
	template <typename T>
	class Reader;

	/// Struct that contains the information about Dijsktra's Algorithm results
	struct DijkstraResult_struct
	{
		bool success;			  // TRUE if the function does not return error, FALSE otherwise
		std::string errorMessage; //message of error
		double result;			  //result (valid only if success is TRUE)
	};
	typedef DijkstraResult_struct DijkstraResult;

	/// Struct that contains the information about Dijsktra's Algorithm results
	struct DialResult_struct
	{
		bool success;								  // TRUE if the function does not return error, FALSE otherwise
		std::string errorMessage;					  //message of error
		std::map<unsigned long, long> minDistanceMap; //result a map that contains the node id and the minumum distance from source (valid only if success is TRUE)
	};
	typedef DialResult_struct DialResult;

	/// Struct that contains the information about the partitioning statistics
	struct PartitioningStats_struct
	{
		unsigned int numberOfPartitions;   // The number of Partitions
		unsigned int numberOfNodes;		   // The number of Nodes
		unsigned int replicatedNodesCount; // The number of Nodes that are replicated
		unsigned int numberOfEdges;		   // The number of edges
		unsigned int replicatedEdgesCount; // The number of edges that are replicated
		unsigned int maxEdgesLoad;		   // Maximum edges load of the partitions
		unsigned int minEdgesLoad;		   // Minimun edges load of the partitions
		unsigned int maxNodesLoad;		   // Maximum nodes load of the partitions
		unsigned int minNodesLoad;		   // Minimun nodes load of the partitions
		double balanceEdgesFactor;		   // The balance edges factor of the partitions (maxEdgesLoad - minEdgesLoad) / (maxEdgesLoad), 0 is the optimal partitioning
		double balanceNodesFactor;		   // The balance edges factor of the partitions (maxNodesLoad - minNodesLoad) / (maxNodesLoad), 0 is the optimal partitioning
		double nodesReplicationFactor;	   // The replication factor of the Nodes (replicatedNodesCount / numberOfNodes), 1 is the optimal partitioning
		double edgesReplicationFactor;	   // The replication factor of the edges (replicatedEdgesCount / numberOfEdges), 1 is the optimal partitioning

		friend std::ostream &operator<<(std::ostream &os, const PartitioningStats_struct &partitionStats)
		{
			os << "Partitioning Stats:\n";
			os << "\tNumber of Partitions: " << partitionStats.numberOfPartitions << "\n";
			os << "\tNumber of Nodes: " << partitionStats.numberOfNodes << "\n";
			os << "\tNumber of Edges: " << partitionStats.numberOfEdges << "\n";
			os << "\tNumber of Nodes Replica: " << partitionStats.replicatedNodesCount << "\n";
			os << "\tNumber of Edges Replica: " << partitionStats.replicatedEdgesCount << "\n";
			os << "\tNodes Replication Factor: " << partitionStats.nodesReplicationFactor << "\n";
			os << "\tEdges Replication Factor: " << partitionStats.edgesReplicationFactor << "\n";
			os << "\tMax Edges Load: " << partitionStats.maxEdgesLoad << "\n";
			os << "\tMin Edges Load: " << partitionStats.minEdgesLoad << "\n";
			os << "\tBalance Edges Factor: " << partitionStats.balanceEdgesFactor << "\n";
			os << "\tMax Nodes Load: " << partitionStats.maxNodesLoad << "\n";
			os << "\tMin Nodes Load: " << partitionStats.minNodesLoad << "\n";
			os << "\tBalance Nodes Factor: " << partitionStats.balanceNodesFactor << "\n";
			return os;
		}
	};
	typedef PartitioningStats_struct PartitioningStats;

	template <typename T>
	using AdjacencyMatrix = std::map<const Node<T> *, std::vector<std::pair<const Node<T> *, const Edge<T> *>>>;

	template <typename T>
	std::ostream &operator<<(std::ostream &o, const Node<T> &node);
	template <typename T>
	std::ostream &operator<<(std::ostream &o, const Edge<T> &edge);
	template <typename T>
	std::ostream &operator<<(std::ostream &o, const DirectedEdge<T> &edge);
	template <typename T>
	std::ostream &operator<<(std::ostream &o, const UndirectedEdge<T> &edge);
	template <typename T>
	std::ostream &operator<<(std::ostream &o, const DirectedWeightedEdge<T> &edge);
	template <typename T>
	std::ostream &operator<<(std::ostream &o, const UndirectedWeightedEdge<T> &edge);
	template <typename T>
	std::ostream &operator<<(std::ostream &o, const Graph<T> &graph);
	template <typename T>
	std::ostream &operator<<(std::ostream &o, const AdjacencyMatrix<T> &adj);
	template <typename T>
	using PartitionMap = std::map<unsigned int, Partition<T> *>;

	template <typename T>
	class Node
	{
	private:
		unsigned long id;
		T data;

	public:
		Node(const unsigned long id, const T &data);
		~Node() = default;
		const unsigned long &getId() const;
		const T &getData() const;
		//operator
		bool operator==(const Node<T> &b) const;
		bool operator<(const Node<T> &b) const;
		friend std::ostream &operator<<<>(std::ostream &os, const Node<T> &node);
	};

	template <typename T>
	Node<T>::Node(const unsigned long id, const T &data)
	{
		this->id = id;
		this->data = data;
	}

	template <typename T>
	const unsigned long &Node<T>::getId() const
	{
		return id;
	}

	template <typename T>
	const T &Node<T>::getData() const
	{
		return data;
	}

	template <typename T>
	bool Node<T>::operator==(const Node<T> &b) const
	{
		return (this->id == b.id && this->data == b.data);
	}

	template <typename T>
	bool Node<T>::operator<(const Node<T> &b) const
	{
		return (this->id < b.id);
	}

	class Weighted
	{
	private:
		double weight;

	public:
		Weighted();
		Weighted(const double weight);
		virtual ~Weighted() = default;
		double getWeight() const;
		void setWeight(const double weight);
	};

	//inline because the implementation of non-template function in header file
	inline Weighted::Weighted()
	{
		weight = 0.0;
	}

	//inline because the implementation of non-template function in header file
	inline Weighted::Weighted(const double weight)
	{
		this->weight = weight;
	}

	//inline because the implementation of non-template function in header file
	inline double Weighted::getWeight() const
	{
		return weight;
	}

	//inline because the implementation of non-template function in header file
	inline void Weighted::setWeight(const double weight)
	{
		this->weight = weight;
	}

	class ThreadSafe
	{
	public:
		void getLock() const;
		void releaseLock() const;

	protected:
		mutable std::mutex mutex;
	};
	//inline because the implementation of non-template function in header file
	inline void ThreadSafe::getLock() const
	{
		mutex.lock();
	}
	//inline because the implementation of non-template function in header file
	inline void ThreadSafe::releaseLock() const
	{
		mutex.unlock();
	}

	template <typename T>
	class Edge
	{
	private:
		unsigned long id;
		std::pair<const Node<T> *, const Node<T> *> nodePair;

	public:
		Edge(const unsigned long id, const Node<T> &node1, const Node<T> &node2);
		Edge(const unsigned long id, const std::pair<const Node<T> *, const Node<T> *> &nodepair);
		virtual ~Edge() = default;
		const unsigned long &getId() const;
		const std::pair<const Node<T> *, const Node<T> *> &getNodePair() const;
		virtual const std::optional<bool> isDirected() const;
		virtual const std::optional<bool> isWeighted() const;
		//operator
		virtual bool operator==(const Edge<T> &b) const;
		bool operator<(const Edge<T> &b) const;
		//operator DirectedEdge<T>() const { return DirectedEdge<T>(id, nodePair); }
		//operator UndirectedEdge<T>() const { return UndirectedEdge<T>(id, nodePair); }

		friend std::ostream &operator<<<>(std::ostream &os, const Edge<T> &edge);
	};

	template <typename T>
	Edge<T>::Edge(const unsigned long id, const Node<T> &node1, const Node<T> &node2) : nodePair(&node1, &node2)
	{
		this->id = id;
	}

	template <typename T>
	Edge<T>::Edge(const unsigned long id, const std::pair<const Node<T> *, const Node<T> *> &nodepair) : nodePair(nodepair)
	{
		this->id = id;
	}

	template <typename T>
	const unsigned long &Edge<T>::getId() const
	{
		return id;
	}

	template <typename T>
	const std::pair<const Node<T> *, const Node<T> *> &Edge<T>::getNodePair() const
	{
		return nodePair;
	}

	template <typename T>
	const std::optional<bool> Edge<T>::isDirected() const
	{
		return std::nullopt;
	}

	template <typename T>
	const std::optional<bool> Edge<T>::isWeighted() const
	{
		return std::nullopt;
	}

	template <typename T>
	bool Edge<T>::operator==(const Edge<T> &b) const
	{
		return (this->id == b.id && this->nodePair == b.nodePair);
	}

	template <typename T>
	bool Edge<T>::operator<(const Edge<T> &b) const
	{
		return (this->id < b.id);
	}

	template <typename T>
	class DirectedEdge : public Edge<T>
	{
	public:
		DirectedEdge(const unsigned long id, const Node<T> &node1, const Node<T> &node2);
		DirectedEdge(const unsigned long id, const std::pair<const Node<T> *, const Node<T> *> &nodepair);
		DirectedEdge(const Edge<T> &edge);
		virtual ~DirectedEdge() = default;
		const Node<T> &getFrom() const;
		const Node<T> &getTo() const;
		const std::optional<bool> isDirected() const override;
		const std::optional<bool> isWeighted() const override;
		//operator
		explicit operator UndirectedEdge<T>() const { return UndirectedEdge<T>(Edge<T>::getId(), Edge<T>::getNodePair()); }

		friend std::ostream &operator<<<>(std::ostream &os, const DirectedEdge<T> &edge);
	};

	template <typename T>
	DirectedEdge<T>::DirectedEdge(const unsigned long id, const Node<T> &node1, const Node<T> &node2) : Edge<T>(id, node1, node2)
	{
	}

	template <typename T>
	DirectedEdge<T>::DirectedEdge(const unsigned long id, const std::pair<const Node<T> *, const Node<T> *> &nodepair) : Edge<T>(id, nodepair)
	{
	}

	template <typename T>
	DirectedEdge<T>::DirectedEdge(const Edge<T> &edge) : DirectedEdge(edge.getId(), *(edge.getNodePair().first), *(edge.getNodePair().second))
	{
	}

	template <typename T>
	const Node<T> &DirectedEdge<T>::getFrom() const
	{
		return *(Edge<T>::getNodePair().first);
	}

	template <typename T>
	const Node<T> &DirectedEdge<T>::getTo() const
	{
		return *(Edge<T>::getNodePair().second);
	}

	template <typename T>
	const std::optional<bool> DirectedEdge<T>::isDirected() const
	{
		return true;
	}

	template <typename T>
	const std::optional<bool> DirectedEdge<T>::isWeighted() const
	{
		return false;
	}

	template <typename T>
	class UndirectedEdge : public Edge<T>
	{
	public:
		UndirectedEdge(const unsigned long id, const Node<T> &node1, const Node<T> &node2);
		UndirectedEdge(const unsigned long id, const std::pair<const Node<T> *, const Node<T> *> &nodepair);
		UndirectedEdge(const Edge<T> &edge);
		virtual ~UndirectedEdge() = default;
		const Node<T> &getNode1() const;
		const Node<T> &getNode2() const;
		const std::optional<bool> isDirected() const override;
		const std::optional<bool> isWeighted() const override;
		//operator
		explicit operator DirectedEdge<T>() const { return DirectedEdge<T>(Edge<T>::getId(), Edge<T>::getNodePair()); }

		friend std::ostream &operator<<<>(std::ostream &os, const UndirectedEdge<T> &edge);
	};

	template <typename T>
	UndirectedEdge<T>::UndirectedEdge(const unsigned long id, const Node<T> &node1, const Node<T> &node2) : Edge<T>(id, node1, node2)
	{
	}

	template <typename T>
	UndirectedEdge<T>::UndirectedEdge(const unsigned long id, const std::pair<const Node<T> *, const Node<T> *> &nodepair) : Edge<T>(id, nodepair)
	{
	}

	template <typename T>
	UndirectedEdge<T>::UndirectedEdge(const Edge<T> &edge) : UndirectedEdge(edge.getId(), *(edge.getNodePair().first), *(edge.getNodePair().second))
	{
	}

	template <typename T>
	const Node<T> &UndirectedEdge<T>::getNode1() const
	{
		return *(Edge<T>::getNodePair().first);
	}

	template <typename T>
	const Node<T> &UndirectedEdge<T>::getNode2() const
	{
		return *(Edge<T>::getNodePair().second);
	}

	template <typename T>
	const std::optional<bool> UndirectedEdge<T>::isDirected() const
	{
		return false;
	}

	template <typename T>
	const std::optional<bool> UndirectedEdge<T>::isWeighted() const
	{
		return false;
	}

	template <typename T>
	class DirectedWeightedEdge : public DirectedEdge<T>, public Weighted
	{
	public:
		DirectedWeightedEdge(const unsigned long id, const Node<T> &node1, const Node<T> &node2, const double weight);
		DirectedWeightedEdge(const unsigned long id, const std::pair<const Node<T> *, const Node<T> *> &nodepair, const double weight);
		DirectedWeightedEdge(const DirectedEdge<T> &edge, const double weight);
		DirectedWeightedEdge(const Edge<T> &edge, const double weight);
		DirectedWeightedEdge(const DirectedEdge<T> &edge);
		DirectedWeightedEdge(const Edge<T> &edge);
		DirectedWeightedEdge(const UndirectedWeightedEdge<T> &edge);
		virtual ~DirectedWeightedEdge() = default;
		const std::optional<bool> isWeighted() const override;
		//operator
		explicit operator UndirectedWeightedEdge<T>() const { return UndirectedWeightedEdge<T>(Edge<T>::getId(), Edge<T>::getNodePair(), Weighted::getWeight()); }

		friend std::ostream &operator<<<>(std::ostream &os, const DirectedWeightedEdge<T> &edge);
	};

	template <typename T>
	DirectedWeightedEdge<T>::DirectedWeightedEdge(const unsigned long id, const Node<T> &node1, const Node<T> &node2, const double weight) : DirectedEdge<T>(id, node1, node2), Weighted(weight)
	{
	}

	template <typename T>
	DirectedWeightedEdge<T>::DirectedWeightedEdge(const unsigned long id, const std::pair<const Node<T> *, const Node<T> *> &nodepair, const double weight) : DirectedEdge<T>(id, nodepair), Weighted(weight)
	{
	}

	template <typename T>
	DirectedWeightedEdge<T>::DirectedWeightedEdge(const DirectedEdge<T> &edge, const double weight) : DirectedEdge<T>(edge), Weighted(weight)
	{
	}

	template <typename T>
	DirectedWeightedEdge<T>::DirectedWeightedEdge(const Edge<T> &edge, const double weight) : DirectedEdge<T>(edge), Weighted(weight)
	{
	}

	template <typename T>
	DirectedWeightedEdge<T>::DirectedWeightedEdge(const DirectedEdge<T> &edge) : DirectedEdge<T>(edge), Weighted()
	{
	}

	template <typename T>
	DirectedWeightedEdge<T>::DirectedWeightedEdge(const Edge<T> &edge) : DirectedEdge<T>(edge), Weighted()
	{
	}

	template <typename T>
	DirectedWeightedEdge<T>::DirectedWeightedEdge(const UndirectedWeightedEdge<T> &edge) : DirectedEdge<T>(edge), Weighted(edge.getWeight())
	{
	}

	template <typename T>
	const std::optional<bool> DirectedWeightedEdge<T>::isWeighted() const
	{
		return true;
	}

	template <typename T>
	class UndirectedWeightedEdge : public UndirectedEdge<T>, public Weighted
	{
	public:
		UndirectedWeightedEdge(const unsigned long id, const Node<T> &node1, const Node<T> &node2, const double weight);
		UndirectedWeightedEdge(const unsigned long id, const std::pair<const Node<T> *, const Node<T> *> &nodepair, const double weight);
		UndirectedWeightedEdge(const UndirectedEdge<T> &edge, const double weight);
		UndirectedWeightedEdge(const Edge<T> &edge, const double weight);
		UndirectedWeightedEdge(const UndirectedEdge<T> &edge);
		UndirectedWeightedEdge(const Edge<T> &edge);
		UndirectedWeightedEdge(const DirectedWeightedEdge<T> &edge);
		virtual ~UndirectedWeightedEdge() = default;
		const std::optional<bool> isWeighted() const override;
		//operator
		explicit operator DirectedWeightedEdge<T>() const { return DirectedWeightedEdge<T>(Edge<T>::getId(), Edge<T>::getNodePair(), Weighted::getWeight()); }

		friend std::ostream &operator<<<>(std::ostream &os, const UndirectedWeightedEdge<T> &edge);
	};

	template <typename T>
	UndirectedWeightedEdge<T>::UndirectedWeightedEdge(const unsigned long id, const Node<T> &node1, const Node<T> &node2, const double weight) : UndirectedEdge<T>(id, node1, node2), Weighted(weight)
	{
	}

	template <typename T>
	UndirectedWeightedEdge<T>::UndirectedWeightedEdge(const unsigned long id, const std::pair<const Node<T> *, const Node<T> *> &nodepair, const double weight) : UndirectedEdge<T>(id, nodepair), Weighted(weight)
	{
	}

	template <typename T>
	UndirectedWeightedEdge<T>::UndirectedWeightedEdge(const UndirectedEdge<T> &edge, const double weight) : UndirectedEdge<T>(edge), Weighted(weight)
	{
	}

	template <typename T>
	UndirectedWeightedEdge<T>::UndirectedWeightedEdge(const Edge<T> &edge, const double weight) : UndirectedEdge<T>(edge), Weighted(weight)
	{
	}

	template <typename T>
	UndirectedWeightedEdge<T>::UndirectedWeightedEdge(const UndirectedEdge<T> &edge) : UndirectedEdge<T>(edge), Weighted()
	{
	}

	template <typename T>
	UndirectedWeightedEdge<T>::UndirectedWeightedEdge(const Edge<T> &edge) : UndirectedEdge<T>(edge), Weighted()
	{
	}

	template <typename T>
	UndirectedWeightedEdge<T>::UndirectedWeightedEdge(const DirectedWeightedEdge<T> &edge) : UndirectedEdge<T>(edge), Weighted(edge.getWeight())
	{
	}

	template <typename T>
	const std::optional<bool> UndirectedWeightedEdge<T>::isWeighted() const
	{
		return true;
	}

	template <typename T>
	class Partition;

	/// Specify the Input/Output format of the Graph for Import/Export functions
	enum E_InputOutputFormat
	{
		STANDARD_CSV, ///< A standard csv format
		STANDARD_TSV, ///< A standard tsv format
		OUT_1,
		OUT_2
	};

	typedef E_InputOutputFormat InputOutputFormat;

	/// Specify the Partition Algorithm
	enum E_PartitionAlgorithm
	{
		GREEDY_VC, ///< A Greedy Vertex-Cut Algorithm
		HDRF,	   ///< High-Degree (are) Replicated First (HDRF) Algorithm (Stream-Based Vertex-Cut Partitioning)
		ALG_2
	};
	typedef E_PartitionAlgorithm PartitionAlgorithm;

	/// Class that implement the Graph. ( This class is not Thread Safe )
	template <typename T>
	class Graph
	{
	private:
		std::list<const Edge<T> *> edgeSet;
		void addElementToAdjMatrix(AdjacencyMatrix<T> &adjMatrix, const Node<T> *nodeFrom, const Node<T> *nodeTo, const Edge<T> *edge) const;
		int writeToStandardFile_csv(const std::string &workingDir, const std::string &OFileName, bool compress, bool writeNodeFeat, bool writeEdgeWeight) const;
		int readFromStandardFile_csv(const std::string &workingDir, const std::string &OFileName, bool compress, bool readNodeFeat, bool readEdgeWeight);
		int writeToStandardFile_tsv(const std::string &workingDir, const std::string &OFileName, bool compress, bool writeNodeFeat, bool writeEdgeWeight) const;
		int readFromStandardFile_tsv(const std::string &workingDir, const std::string &OFileName, bool compress, bool readNodeFeat, bool readEdgeWeight);
		void recreateGraphFromReadFiles(std::map<unsigned long, std::pair<unsigned long, unsigned long>> &edgeMap, std::map<unsigned long, bool> &edgeDirectedMap, std::map<unsigned long, T> &nodeFeatMap, std::map<unsigned long, double> &edgeWeightMap);
		int compressFile(const std::string &inputFile, const std::string &outputFile) const;
		int decompressFile(const std::string &inputFile, const std::string &outputFile) const;
		void greedyPartition(PartitionMap<T> &partitionMap) const;
		void HDRFPartition(PartitionMap<T> &partitionMap) const;

	public:
		Graph() = default;
		Graph(const std::list<const Edge<T> *> &edgeSet);
		~Graph() = default;
		/**
 		* \brief
 		* Function that return the Edge set of the Graph
		* Note: No Thread Safe
		*
 		* @returns a list of Edges of the graph
 		*
 		*/
		virtual const std::list<const Edge<T> *> &getEdgeSet() const;
		/**
 		* \brief
 		* Function set the Edge Set of the Graph
		* Note: No Thread Safe
 		*
 		* @param edgeSet The Edge Set
 		*
 		*/
		virtual void setEdgeSet(std::list<const Edge<T> *> &edgeSet);
		/**
 		* \brief
 		* Function add an Edge to the Graph Edge Set
		* Note: No Thread Safe
 		*
 		* @param edge The Edge to insert
 		*
 		*/
		virtual void addEdge(const Edge<T> *edge);
		/**
 		* \brief
 		* Function remove an Edge from the Graph Edge Set
		* Note: No Thread Safe
 		*
 		* @param edgeId The Edge Id to remove
 		*
 		*/
		virtual void removeEdge(unsigned long edgeId);
		/**
 		* \brief
 		* Function that return the Node Set of the Graph
		* Note: No Thread Safe
 		*
 		* @returns a list of Nodes of the graph
 		*
 		*/
		virtual const std::list<const Node<T> *> getNodeSet() const;
		/**
 		* \brief
 		* Function that return an Edge with specific ID if Exist in the Graph
		* Note: No Thread Safe
 		*
		* @param edgeId The Edge Id to return
 		* @returns the Edge if exist
 		*
 		*/
		virtual const std::optional<const Edge<T> *> getEdge(unsigned long edgeId) const;
		/**
		* @brief This function generate a list of adjacency matrix with every element of the matrix
		* contain the node where is directed the link and the Edge corrispondent to the link
		* Note: No Thread Safe
		*/
		virtual const AdjacencyMatrix<T> getAdjMatrix() const;
		/**
 		* @brief Function runs the dijkstra algorithm for some source node and
 		* target node in the graph and returns the shortest distance of target
 		* from the source.
		* Note: No Thread Safe
 		*
		* @param source source vertex
 		* @param target target vertex
 		*
 		* @return shortest distance if target is reachable from source else ERROR in
 		* case if target is not reachable from source or there is error in the computation.
 		*/
		virtual const DijkstraResult dijkstra(const Node<T> &source, const Node<T> &target) const;
		/**
 		* \brief
 		* Function performs the breadth first search algorithm over the graph
		* Note: No Thread Safe
 		*
 		* @param start Node from where traversing starts
 		* @returns a vector of Node indicating which Node were visited during the
 		* search.
 		*
 		*/
		virtual const std::vector<Node<T>> breadth_first_search(const Node<T> &start) const;
		/**
 		* \brief
 		* Function performs the depth first search algorithm over the graph
		* Note: No Thread Safe
 		*
 		* @param start Node from where traversing starts
 		* @returns a vector of Node indicating which Node were visited during the
 		* search.
 		*
 		*/
		virtual const std::vector<Node<T>> depth_first_search(const Node<T> &start) const;

		/**
		* \brief
     	* This function uses DFS to check for cycle in the graph.
     	* Pay Attention, this function work only with directed Graph
		* Note: No Thread Safe
     	*
     	* @return true if a cycle is detected, else false. ( false is returned also if the graph in indirected)
     	*/
		virtual bool isCyclicDirectedGraphDFS() const;

		/**
		* \brief
     	* This function uses BFS to check for cycle in the graph.
     	* Pay Attention, this function work only with directed Graph
		* Note: No Thread Safe
     	*
     	* @return true if a cycle is detected, else false. ( false is returned also if the graph in indirected)
     	*/
		virtual bool isCyclicDirectedGraphBFS() const;

		/**
     	* \brief
     	* This function checks if a graph is directed
		* Note: No Thread Safe
     	*
     	* @return true if the graph is directed, else false.
     	*/
		virtual bool isDirectedGraph() const;

		/**
     	* \brief
     	* This function write the graph in an output file
		* Note: No Thread Safe
     	*
		* @param format The Output format of the file
		* @param workingDir The path to the directory in which will be placed the output file
		* @param OFileName The Output File Name ( )
		* @param compress Indicates if the output will be compressed
		* @param writeNodeFeat Indicates if export also Node Features
		* @param writeEdgeWeight Indicates if export also Edge Weights
     	* @return 0 if all OK, else return a negative value
     	*/

		/**
 		* @brief Function runs the Dial algorithm  (Optimized Dijkstra for small range weights) for some source node and
 		* target node in the graph and returns the shortest distance of target
 		* from the source.
		* Note: No Thread Safe
 		*
		* @param source source vertex
		* @param maxWeight maximum weight of the edge
 		*
 		* @return shortest distance for all nodes reachable from source else ERROR in
 		* case there is error in the computation.
 		*/
		virtual const DialResult dial(const Node<T> &source, int maxWeight) const;

		/**
     	* \brief
     	* This function write the graph in an output file
		* Note: No Thread Safe
     	*
		* @param format The Output format of the file
		* @param workingDir The path to the directory in which is placed the Output file
		* @param OFileName The Output File Name ( )
		* @param compress Indicates if the Output will be compressed ( Pay Attention if compress flag is true, not compressed files will be deleted [ #48 ] )
		* @param writeNodeFeat Indicates if export also Node Features
		* @param writeEdgeWeight Indicates if export also Edge Weights
     	* @return 0 if all OK, else return a negative value
     	*/
		virtual int writeToFile(InputOutputFormat format = InputOutputFormat::STANDARD_CSV, const std::string &workingDir = ".", const std::string &OFileName = "graph", bool compress = false, bool writeNodeFeat = false, bool writeEdgeWeight = false) const;

		/**
     	* \brief
     	* This function read the graph from an input file
		* Note: No Thread Safe
     	*
		* @param format The Input format of the file
		* @param workingDir The path to the directory in which is placed the Input file
		* @param OFileName The Input File Name ( )
		* @param compress Indicates if the Input is compressed
		* @param writeNodeFeat Indicates if import also Node Features
		* @param writeEdgeWeight Indicates if import also Edge Weights
     	* @return 0 if all OK, else return a negative value
     	*/
		virtual int readFromFile(InputOutputFormat format = InputOutputFormat::STANDARD_CSV, const std::string &workingDir = ".", const std::string &OFileName = "graph", bool compress = false, bool readNodeFeat = false, bool readEdgeWeight = false);

		/**
     	* \brief
     	* This function partition a graph in a set of partitions
		* Note: No Thread Safe
     	*
		* @param algorithm The partition algorithm
		* @param numberOfPartition The number of partitions
		* @return The partiton Map of the partitioned graph
     	*/
		virtual PartitionMap<T> partitionGraph(PartitionAlgorithm algorithm, unsigned int numberOfPartitions) const;

		friend std::ostream &operator<<<>(std::ostream &os, const Graph<T> &graph);
		friend std::ostream &operator<<<>(std::ostream &os, const AdjacencyMatrix<T> &adj);
	};

	template <typename T>
	Graph<T>::Graph(const std::list<const Edge<T> *> &edgeSet)
	{
		for (auto edgeSetIt = edgeSet.begin(); edgeSetIt != edgeSet.end(); ++edgeSetIt)
		{
			if (std::find_if(this->edgeSet.begin(), this->edgeSet.end(), [edgeSetIt](const Edge<T> *edge)
							 { return (*edge == **edgeSetIt); }) == this->edgeSet.end())
			{
				this->edgeSet.push_back(*edgeSetIt);
			}
		}
	}

	template <typename T>
	const std::list<const Edge<T> *> &Graph<T>::getEdgeSet() const
	{
		return edgeSet;
	}

	template <typename T>
	void Graph<T>::setEdgeSet(std::list<const Edge<T> *> &edgeSet)
	{
		this->edgeSet.clear();
		for (auto edgeSetIt = edgeSet.begin(); edgeSetIt != edgeSet.end(); ++edgeSetIt)
		{
			if (std::find_if(this->edgeSet.begin(), this->edgeSet.end(), [edgeSetIt](const Edge<T> *edge)
							 { return (*edge == **edgeSetIt); }) == this->edgeSet.end())
			{
				this->edgeSet.push_back(*edgeSetIt);
			}
		}
	}

	template <typename T>
	void Graph<T>::addEdge(const Edge<T> *edge)
	{
		if (std::find_if(edgeSet.begin(), edgeSet.end(), [edge](const Edge<T> *edge_a)
						 { return (*edge == *edge_a); }) == edgeSet.end())
		{
			edgeSet.push_back(edge);
		}
	}

	template <typename T>
	void Graph<T>::removeEdge(unsigned long edgeId)
	{
		auto edgeOpt = Graph<T>::getEdge(edgeId);
		if (edgeOpt.has_value())
		{
			edgeSet.erase(std::find_if(this->edgeSet.begin(), this->edgeSet.end(), [edgeOpt](const Edge<T> *edge)
									   { return (*(edgeOpt.value()) == *edge); }));
		}
	}

	template <typename T>
	const std::list<const Node<T> *> Graph<T>::getNodeSet() const
	{
		std::list<const Node<T> *> nodeSet;
		for (auto edge : edgeSet)
		{
			if (std::find_if(nodeSet.begin(), nodeSet.end(), [edge](const Node<T> *node)
							 { return (*edge->getNodePair().first == *node); }) == nodeSet.end())
			{
				nodeSet.push_back(edge->getNodePair().first);
			}
			if (std::find_if(nodeSet.begin(), nodeSet.end(), [edge](const Node<T> *node)
							 { return (*edge->getNodePair().second == *node); }) == nodeSet.end())
			{
				nodeSet.push_back(edge->getNodePair().second);
			}
		}
		return nodeSet;
	}

	template <typename T>
	const std::optional<const Edge<T> *> Graph<T>::getEdge(unsigned long edgeId) const
	{

		auto it = edgeSet.begin();
		for (it; it != edgeSet.end(); ++it)
		{
			if ((*it)->getId() == edgeId)
			{
				return *it;
			}
		}

		return std::nullopt;
	}

	template <typename T>
	void Graph<T>::addElementToAdjMatrix(AdjacencyMatrix<T> &adjMatrix, const Node<T> *nodeFrom, const Node<T> *nodeTo, const Edge<T> *edge) const
	{
		std::pair<const Node<T> *, const Edge<T> *> elem = {nodeTo, edge};
		adjMatrix[nodeFrom].push_back(elem);

		//adjMatrix[nodeFrom.getId()].push_back(std::make_pair<const Node<T>,const Edge<T>>(nodeTo, edge));
	}

	template <typename T>
	int Graph<T>::writeToStandardFile_csv(const std::string &workingDir, const std::string &OFileName, bool compress, bool writeNodeFeat, bool writeEdgeWeight) const
	{
		std::ofstream ofileGraph;
		std::string completePathToFileGraph = workingDir + "/" + OFileName + ".csv";
		ofileGraph.open(completePathToFileGraph);
		if (!ofileGraph.is_open())
		{
			// ERROR File Not Open
			return -1;
		}
		auto printOutGraph = [&ofileGraph](const Edge<T> *e)
		{ ofileGraph << e->getId() << "," << e->getNodePair().first->getId() << "," << e->getNodePair().second->getId() << "," << ((e->isDirected().has_value() && e->isDirected().value()) ? 1 : 0) << std::endl; };
		std::for_each(edgeSet.cbegin(), edgeSet.cend(), printOutGraph);
		ofileGraph.close();

		if (writeNodeFeat)
		{
			std::ofstream ofileNodeFeat;
			std::string completePathToFileNodeFeat = workingDir + "/" + OFileName + "_NodeFeat"
																					".csv";
			ofileNodeFeat.open(completePathToFileNodeFeat);
			if (!ofileNodeFeat.is_open())
			{
				// ERROR File Not Open
				return -1;
			}
			auto printOutNodeFeat = [&ofileNodeFeat](const Node<T> *node)
			{ ofileNodeFeat << node->getId() << "," << node->getData() << std::endl; };
			auto nodeSet = getNodeSet();
			std::for_each(nodeSet.cbegin(), nodeSet.cend(), printOutNodeFeat);
			ofileNodeFeat.close();
		}

		if (writeEdgeWeight)
		{
			std::ofstream ofileEdgeWeight;
			std::string completePathToFileEdgeWeight = workingDir + "/" + OFileName + "_EdgeWeight"
																					  ".csv";
			ofileEdgeWeight.open(completePathToFileEdgeWeight);
			if (!ofileEdgeWeight.is_open())
			{
				// ERROR File Not Open
				return -1;
			}
			auto printOutEdgeWeight = [&ofileEdgeWeight](const Edge<T> *e)
			{ ofileEdgeWeight << e->getId() << "," << (e->isWeighted().has_value() && e->isWeighted().value() ? (dynamic_cast<const Weighted *>(e))->getWeight() : 0.0) << "," << (e->isWeighted().has_value() && e->isWeighted().value() ? 1 : 0) << std::endl; };

			std::for_each(edgeSet.cbegin(), edgeSet.cend(), printOutEdgeWeight);
			ofileEdgeWeight.close();
		}
		return 0;
	}

	template <typename T>
	int Graph<T>::readFromStandardFile_csv(const std::string &workingDir, const std::string &OFileName, bool compress, bool readNodeFeat, bool readEdgeWeight)
	{
		std::ifstream ifileGraph;
		std::ifstream ifileNodeFeat;
		std::ifstream ifileEdgeWeight;
		std::map<unsigned long, std::pair<unsigned long, unsigned long>> edgeMap;
		std::map<unsigned long, bool> edgeDirectedMap;
		std::map<unsigned long, T> nodeFeatMap;
		std::map<unsigned long, double> edgeWeightMap;
		std::string completePathToFileGraph = workingDir + "/" + OFileName + ".csv";
		ifileGraph.open(completePathToFileGraph);
		if (!ifileGraph.is_open())
		{
			// ERROR File Not Open
			return -1;
		}
		char comma;
		for (;;)
		{ /* loop continually */
			unsigned long edgeId;
			unsigned long nodeId1;
			unsigned long nodeId2;
			bool directed;
			ifileGraph >> edgeId >> comma >> nodeId1 >> comma >> nodeId2 >> comma >> directed;
			edgeMap[edgeId] = std::pair<unsigned long, unsigned long>(nodeId1, nodeId2);
			edgeDirectedMap[edgeId] = directed;
			if (ifileGraph.fail() || ifileGraph.eof())
				break;
			ifileGraph.ignore(128, '\n');
		}
		ifileGraph.close();

		if (readNodeFeat)
		{
			std::string completePathToFileNodeFeat = workingDir + "/" + OFileName + "_NodeFeat"
																					".csv";
			ifileNodeFeat.open(completePathToFileNodeFeat);
			if (!ifileNodeFeat.is_open())
			{
				// ERROR File Not Open
				return -1;
			}
			for (;;)
			{ /* loop continually */
				unsigned long nodeId;
				T nodeFeat;
				ifileNodeFeat >> nodeId >> comma >> nodeFeat;
				nodeFeatMap[nodeId] = nodeFeat;
				if (ifileNodeFeat.fail() || ifileNodeFeat.eof())
					break;
				ifileNodeFeat.ignore(128, '\n');
			}
			ifileNodeFeat.close();
		}

		if (readEdgeWeight)
		{
			std::string completePathToFileEdgeWeight = workingDir + "/" + OFileName + "_EdgeWeight"
																					  ".csv";
			ifileEdgeWeight.open(completePathToFileEdgeWeight);
			if (!ifileEdgeWeight.is_open())
			{
				// ERROR File Not Open
				return -1;
			}
			for (;;)
			{ /* loop continually */
				unsigned long edgeId;
				double weight;
				bool weighted;
				ifileEdgeWeight >> edgeId >> comma >> weight >> comma >> weighted;
				if (weighted)
				{
					edgeWeightMap[edgeId] = weight;
				}
				if (ifileEdgeWeight.fail() || ifileEdgeWeight.eof())
					break;
				ifileEdgeWeight.ignore(128, '\n');
			}
			ifileEdgeWeight.close();
		}
		recreateGraphFromReadFiles(edgeMap, edgeDirectedMap, nodeFeatMap, edgeWeightMap);
		return 0;
	}

	template <typename T>
	int Graph<T>::writeToStandardFile_tsv(const std::string &workingDir, const std::string &OFileName, bool compress, bool writeNodeFeat, bool writeEdgeWeight) const
	{
		std::ofstream ofileGraph;
		std::string completePathToFileGraph = workingDir + "/" + OFileName + ".tsv";
		ofileGraph.open(completePathToFileGraph);
		if (!ofileGraph.is_open())
		{
			// ERROR File Not Open
			return -1;
		}
		auto printOutGraph = [&ofileGraph](const Edge<T> *e)
		{ ofileGraph << e->getId() << "\t" << e->getNodePair().first->getId() << "\t" << e->getNodePair().second->getId() << "\t" << ((e->isDirected().has_value() && e->isDirected().value()) ? 1 : 0) << std::endl; };
		std::for_each(edgeSet.cbegin(), edgeSet.cend(), printOutGraph);
		ofileGraph.close();

		if (writeNodeFeat)
		{
			std::ofstream ofileNodeFeat;
			std::string completePathToFileNodeFeat = workingDir + "/" + OFileName + "_NodeFeat"
																					".tsv";
			ofileNodeFeat.open(completePathToFileNodeFeat);
			if (!ofileNodeFeat.is_open())
			{
				// ERROR File Not Open
				return -1;
			}
			auto printOutNodeFeat = [&ofileNodeFeat](const Node<T> *node)
			{ ofileNodeFeat << node->getId() << "\t" << node->getData() << std::endl; };
			auto nodeSet = getNodeSet();
			std::for_each(nodeSet.cbegin(), nodeSet.cend(), printOutNodeFeat);
			ofileNodeFeat.close();
		}

		if (writeEdgeWeight)
		{
			std::ofstream ofileEdgeWeight;
			std::string completePathToFileEdgeWeight = workingDir + "/" + OFileName + "_EdgeWeight"
																					  ".tsv";
			ofileEdgeWeight.open(completePathToFileEdgeWeight);
			if (!ofileEdgeWeight.is_open())
			{
				// ERROR File Not Open
				return -1;
			}
			auto printOutEdgeWeight = [&ofileEdgeWeight](const Edge<T> *e)
			{ ofileEdgeWeight << e->getId() << "\t" << (e->isWeighted().has_value() && e->isWeighted().value() ? (dynamic_cast<const Weighted *>(e))->getWeight() : 0.0) << "\t" << (e->isWeighted().has_value() && e->isWeighted().value() ? 1 : 0) << std::endl; };

			std::for_each(edgeSet.cbegin(), edgeSet.cend(), printOutEdgeWeight);
			ofileEdgeWeight.close();
		}
		return 0;
	}

	template <typename T>
	int Graph<T>::readFromStandardFile_tsv(const std::string &workingDir, const std::string &OFileName, bool compress, bool readNodeFeat, bool readEdgeWeight)
	{
		std::ifstream ifileGraph;
		std::ifstream ifileNodeFeat;
		std::ifstream ifileEdgeWeight;
		std::map<unsigned long, std::pair<unsigned long, unsigned long>> edgeMap;
		std::map<unsigned long, bool> edgeDirectedMap;
		std::map<unsigned long, T> nodeFeatMap;
		std::map<unsigned long, double> edgeWeightMap;
		std::string completePathToFileGraph = workingDir + "/" + OFileName + ".tsv";
		ifileGraph.open(completePathToFileGraph);
		if (!ifileGraph.is_open())
		{
			// ERROR File Not Open
			return -1;
		}
		for (;;)
		{ /* loop continually */
			unsigned long edgeId;
			unsigned long nodeId1;
			unsigned long nodeId2;
			bool directed;
			ifileGraph >> edgeId >> std::ws >> nodeId1 >> std::ws >> nodeId2 >> std::ws >> directed;
			edgeMap[edgeId] = std::pair<unsigned long, unsigned long>(nodeId1, nodeId2);
			edgeDirectedMap[edgeId] = directed;
			if (ifileGraph.fail() || ifileGraph.eof())
				break;
			ifileGraph.ignore(128, '\n');
		}
		ifileGraph.close();

		if (readNodeFeat)
		{
			std::string completePathToFileNodeFeat = workingDir + "/" + OFileName + "_NodeFeat"
																					".tsv";
			ifileNodeFeat.open(completePathToFileNodeFeat);
			if (!ifileNodeFeat.is_open())
			{
				// ERROR File Not Open
				return -1;
			}
			for (;;)
			{ /* loop continually */
				unsigned long nodeId;
				T nodeFeat;
				ifileNodeFeat >> nodeId >> std::ws >> nodeFeat;
				nodeFeatMap[nodeId] = nodeFeat;
				if (ifileNodeFeat.fail() || ifileNodeFeat.eof())
					break;
				ifileNodeFeat.ignore(128, '\n');
			}
			ifileNodeFeat.close();
		}

		if (readEdgeWeight)
		{
			std::string completePathToFileEdgeWeight = workingDir + "/" + OFileName + "_EdgeWeight"
																					  ".tsv";
			ifileEdgeWeight.open(completePathToFileEdgeWeight);
			if (!ifileEdgeWeight.is_open())
			{
				// ERROR File Not Open
				return -1;
			}
			for (;;)
			{ /* loop continually */
				unsigned long edgeId;
				double weight;
				bool weighted;
				ifileEdgeWeight >> edgeId >> std::ws >> weight >> std::ws >> weighted;
				if (weighted)
				{
					edgeWeightMap[edgeId] = weight;
				}
				if (ifileEdgeWeight.fail() || ifileEdgeWeight.eof())
					break;
				ifileEdgeWeight.ignore(128, '\n');
			}
			ifileEdgeWeight.close();
		}
		recreateGraphFromReadFiles(edgeMap, edgeDirectedMap, nodeFeatMap, edgeWeightMap);
		return 0;
	}

	template <typename T>
	void Graph<T>::recreateGraphFromReadFiles(std::map<unsigned long, std::pair<unsigned long, unsigned long>> &edgeMap, std::map<unsigned long, bool> &edgeDirectedMap, std::map<unsigned long, T> &nodeFeatMap, std::map<unsigned long, double> &edgeWeightMap)
	{
		std::map<unsigned long, Node<T> *> nodeMap;
		for (auto edgeIt = edgeMap.begin(); edgeIt != edgeMap.end(); ++edgeIt)
		{
			Node<T> *node1 = nullptr;
			Node<T> *node2 = nullptr;
			if (nodeMap.find(edgeIt->second.first) == nodeMap.end())
			{
				//Create new Node
				T feat;
				if (nodeFeatMap.find(edgeIt->second.first) != nodeFeatMap.end())
				{
					feat = nodeFeatMap.at(edgeIt->second.first);
				}
				node1 = new Node<T>(edgeIt->second.first, feat);
				nodeMap[edgeIt->second.first] = node1;
			}
			else
			{
				node1 = nodeMap.at(edgeIt->second.first);
			}
			if (nodeMap.find(edgeIt->second.second) == nodeMap.end())
			{
				//Create new Node
				T feat;
				if (nodeFeatMap.find(edgeIt->second.second) != nodeFeatMap.end())
				{
					feat = nodeFeatMap.at(edgeIt->second.second);
				}
				node2 = new Node<T>(edgeIt->second.second, feat);
				nodeMap[edgeIt->second.second] = node2;
			}
			else
			{
				node2 = nodeMap.at(edgeIt->second.second);
			}

			if (edgeWeightMap.find(edgeIt->first) != edgeWeightMap.end())
			{
				if (edgeDirectedMap.find(edgeIt->first) != edgeDirectedMap.end() && edgeDirectedMap.at(edgeIt->first))
				{
					auto edge = new DirectedWeightedEdge<T>(edgeIt->first, *node1, *node2, edgeWeightMap.at(edgeIt->first));
					addEdge(edge);
				}
				else
				{
					auto edge = new UndirectedWeightedEdge<T>(edgeIt->first, *node1, *node2, edgeWeightMap.at(edgeIt->first));
					addEdge(edge);
				}
			}
			else
			{
				if (edgeDirectedMap.find(edgeIt->first) != edgeDirectedMap.end() && edgeDirectedMap.at(edgeIt->first))
				{
					auto edge = new DirectedEdge<T>(edgeIt->first, *node1, *node2);
					addEdge(edge);
				}
				else
				{
					auto edge = new UndirectedEdge<T>(edgeIt->first, *node1, *node2);
					addEdge(edge);
				}
			}
		}
	}

	template <typename T>
	int Graph<T>::compressFile(const std::string &inputFile, const std::string &outputFile) const
	{

		std::ifstream ifs;
		ifs.open(inputFile);
		if (!ifs.is_open())
		{
			// ERROR File Not Open
			return -1;
		}
		std::string content((std::istreambuf_iterator<char>(ifs)),
							(std::istreambuf_iterator<char>()));

		const char *content_ptr = content.c_str();
		gzFile outFileZ = gzopen(outputFile.c_str(), "wb");
		if (outFileZ == NULL)
		{
			//printf("Error: Failed to gzopen %s\n", outputFile.c_str());
			return -1;
		}

		unsigned int zippedBytes;
		zippedBytes = gzwrite(outFileZ, content_ptr, strlen(content_ptr));

		ifs.close();
		gzclose(outFileZ);
		return 0;
	}

	template <typename T>
	int Graph<T>::decompressFile(const std::string &inputFile, const std::string &outputFile) const
	{

		gzFile inFileZ = gzopen(inputFile.c_str(), "rb");
		if (inFileZ == NULL)
		{
			//printf("Error: Failed to gzopen %s\n", inputFile.c_str());
			return -1;
		}
		unsigned char unzipBuffer[8192];
		unsigned int unzippedBytes;
		std::vector<unsigned char> unzippedData;
		std::ofstream ofs;
		ofs.open(outputFile);
		if (!ofs.is_open())
		{
			// ERROR File Not Open
			return -1;
		}
		while (true)
		{
			unzippedBytes = gzread(inFileZ, unzipBuffer, 8192);
			if (unzippedBytes > 0)
			{
				unzippedData.insert(unzippedData.end(), unzipBuffer, unzipBuffer + unzippedBytes);
			}
			else
			{
				break;
			}
		}
		for (auto c : unzippedData)
		{
			ofs << c;
		}
		ofs << std::endl;
		ofs.close();
		gzclose(inFileZ);
		return 0;
	}

	template <typename T>
	void Graph<T>::greedyPartition(PartitionMap<T> &partitionMap) const
	{
		unsigned int index = 0;
		unsigned int numberOfPartitions = partitionMap.size();
		if (index == numberOfPartitions)
		{
			//ERROR partition map of zero element
			return;
		}
		auto edgeSet = getEdgeSet();
		for (auto edge : edgeSet)
		{
			partitionMap.at(index)->addEdge(edge);
			index++;
			index = index % numberOfPartitions;
		}
	}

	template <typename T>
	void Graph<T>::HDRFPartition(PartitionMap<T> &partitionMap) const
	{
	}

	template <typename T>
	const AdjacencyMatrix<T> Graph<T>::getAdjMatrix() const
	{
		AdjacencyMatrix<T> adj;
		auto edgeSetIt = edgeSet.begin();
		for (edgeSetIt; edgeSetIt != edgeSet.end(); ++edgeSetIt)
		{
			if ((*edgeSetIt)->isDirected().has_value() && (*edgeSetIt)->isDirected().value())
			{
				const DirectedEdge<T> *d_edge = dynamic_cast<const DirectedEdge<T> *>(*edgeSetIt);
				addElementToAdjMatrix(adj, &(d_edge->getFrom()), &(d_edge->getTo()), d_edge);
			}
			else if ((*edgeSetIt)->isDirected().has_value() && !(*edgeSetIt)->isDirected().value())
			{
				const UndirectedEdge<T> *ud_edge = dynamic_cast<const UndirectedEdge<T> *>(*edgeSetIt);
				;
				addElementToAdjMatrix(adj, &(ud_edge->getNode1()), &(ud_edge->getNode2()), ud_edge);
				addElementToAdjMatrix(adj, &(ud_edge->getNode2()), &(ud_edge->getNode1()), ud_edge);
			}
			else
			{ //is a simple edge we cannot create adj matrix
				return adj;
			}
		}
		return adj;
	}

	template <typename T>
	const DijkstraResult Graph<T>::dijkstra(const Node<T> &source, const Node<T> &target) const
	{
		DijkstraResult result;
		result.success = false;
		result.errorMessage = "";
		result.result = INF_DOUBLE;
		auto nodeSet = getNodeSet();
		if (std::find(nodeSet.begin(), nodeSet.end(), &source) == nodeSet.end())
		{
			// check if source node exist in the graph
			result.errorMessage = ERR_SOURCE_NODE_NOT_IN_GRAPH;
			return result;
		}
		if (std::find(nodeSet.begin(), nodeSet.end(), &target) == nodeSet.end())
		{
			// check if target node exist in the graph
			result.errorMessage = ERR_TARGET_NODE_NOT_IN_GRAPH;
			return result;
		}
		const AdjacencyMatrix<T> adj = getAdjMatrix();
		// n denotes the number of vertices in graph
		int n = adj.size();

		// setting all the distances initially to INF_DOUBLE
		std::map<const Node<T> *, double> dist;

		for (auto elem : adj)
		{
			dist[elem.first] = INF_DOUBLE;
		}

		// creating a min heap using priority queue
		// first element of pair contains the distance
		// second element of pair contains the vertex
		std::priority_queue<std::pair<double, const Node<T> *>, std::vector<std::pair<double, const Node<T> *>>,
							std::greater<std::pair<double, const Node<T> *>>>
			pq;

		// pushing the source vertex 's' with 0 distance in min heap
		pq.push(std::make_pair(0.0, &source));

		// marking the distance of source as 0
		dist[&source] = 0;

		while (!pq.empty())
		{
			// second element of pair denotes the node / vertex
			const Node<T> *currentNode = pq.top().second;

			// first element of pair denotes the distance
			double currentDist = pq.top().first;

			pq.pop();

			// for all the reachable vertex from the currently exploring vertex
			// we will try to minimize the distance
			if (adj.find(currentNode) != adj.end())
			{
				for (std::pair<const Node<T> *, const Edge<T> *> elem : adj.at(currentNode))
				{
					// minimizing distances
					if (elem.second->isWeighted().has_value() && elem.second->isWeighted().value())
					{
						if (elem.second->isDirected().has_value() && elem.second->isDirected().value())
						{
							const DirectedWeightedEdge<T> *dw_edge = dynamic_cast<const DirectedWeightedEdge<T> *>(elem.second);
							if (currentDist + dw_edge->getWeight() < dist[elem.first])
							{
								dist[elem.first] = currentDist + dw_edge->getWeight();
								pq.push(std::make_pair(dist[elem.first], elem.first));
							}
						}
						else if (elem.second->isDirected().has_value() && !elem.second->isDirected().value())
						{
							const UndirectedWeightedEdge<T> *udw_edge = dynamic_cast<const UndirectedWeightedEdge<T> *>(elem.second);
							if (currentDist + udw_edge->getWeight() < dist[elem.first])
							{
								dist[elem.first] = currentDist + udw_edge->getWeight();
								pq.push(std::make_pair(dist[elem.first], elem.first));
							}
						}
						else
						{
							//ERROR it shouldn't never returned ( does not exist a Node Weighted and not Directed/Undirected)
							result.errorMessage = ERR_NO_DIR_OR_UNDIR_EDGE;
							return result;
						}
					}
					else
					{
						// No Weighted Edge
						result.errorMessage = ERR_NO_WEIGHTED_EDGE;
						return result;
					}
				}
			}
		}
		if (dist[&target] != INF_DOUBLE)
		{
			result.success = true;
			result.errorMessage = "";
			result.result = dist[&target];
			return result;
		}
		result.errorMessage = ERR_TARGET_NODE_NOT_REACHABLE;
		result.result = -1;
		return result;
	}

	template <typename T>
	const std::vector<Node<T>> Graph<T>::breadth_first_search(const Node<T> &start) const
	{
		// vector to keep track of visited nodes
		std::vector<Node<T>> visited;
		auto nodeSet = getNodeSet();
		//check is exist node in the graph
		if (std::find(nodeSet.begin(), nodeSet.end(), &start) == nodeSet.end())
		{
			return visited;
		}
		const AdjacencyMatrix<T> adj = getAdjMatrix();
		// queue that stores vertices that need to be further explored
		std::queue<const Node<T> *> tracker;

		// mark the starting node as visited
		visited.push_back(start);
		tracker.push(&start);
		while (!tracker.empty())
		{
			const Node<T> *node = tracker.front();
			tracker.pop();
			if (adj.find(node) != adj.end())
			{
				for (auto elem : adj.at(node))
				{
					// if the node is not visited then mark it as visited
					// and push it to the queue
					if (std::find(visited.begin(), visited.end(), *(elem.first)) == visited.end())
					{
						visited.push_back(*(elem.first));
						tracker.push(elem.first);
					}
				}
			}
		}

		return visited;
	}

	template <typename T>
	const std::vector<Node<T>> Graph<T>::depth_first_search(const Node<T> &start) const
	{
		// vector to keep track of visited nodes
		std::vector<Node<T>> visited;
		auto nodeSet = getNodeSet();
		//check is exist node in the graph
		if (std::find(nodeSet.begin(), nodeSet.end(), &start) == nodeSet.end())
		{
			return visited;
		}
		const AdjacencyMatrix<T> adj = getAdjMatrix();
		std::function<void(const AdjacencyMatrix<T> &, const Node<T> &, std::vector<Node<T>> &)> explore;
		explore = [&explore](const AdjacencyMatrix<T> &adj, const Node<T> &node, std::vector<Node<T>> &visited) -> void
		{
			visited.push_back(node);
			if (adj.find(&node) != adj.end())
			{
				for (auto x : adj.at(&node))
				{
					if (std::find(visited.begin(), visited.end(), *(x.first)) == visited.end())
					{
						explore(adj, *(x.first), visited);
					}
				}
			}
		};
		explore(adj, start, visited);

		return visited;
	}

	template <typename T>
	bool Graph<T>::isCyclicDirectedGraphDFS() const
	{
		if (!isDirectedGraph())
		{
			return false;
		}
		enum nodeStates : uint8_t
		{
			not_visited,
			in_stack,
			visited
		};
		auto nodeSet = this->getNodeSet();
		auto adjMatrix = this->getAdjMatrix();

		/* State of the node.
         *
         * It is a vector of "nodeStates" which represents the state node is in.
         * It can take only 3 values: "not_visited", "in_stack", and "visited".
         *
         * Initially, all nodes are in "not_visited" state.
         */
		std::map<unsigned long, nodeStates> state;
		for (auto node : nodeSet)
		{
			state[node->getId()] = not_visited;
		}
		int stateCounter = 0;

		// Start visiting each node.
		for (auto node : nodeSet)
		{
			// If a node is not visited, only then check for presence of cycle.
			// There is no need to check for presence of cycle for a visited
			// node as it has already been checked for presence of cycle.
			if (state[node->getId()] == not_visited)
			{
				// Check for cycle.
				std::function<bool(AdjacencyMatrix<T> &, std::map<unsigned long, nodeStates> &, const Node<T> *)> isCyclicDFSHelper;
				isCyclicDFSHelper = [this, &isCyclicDFSHelper](AdjacencyMatrix<T> &adjMatrix, std::map<unsigned long, nodeStates> &states, const Node<T> *node)
				{
					// Add node "in_stack" state.
					states[node->getId()] = in_stack;

					// If the node has children, then recursively visit all children of the
					// node.
					auto const it = adjMatrix.find(node);
					if (it != adjMatrix.end())
					{
						for (auto child : it->second)
						{
							// If state of child node is "not_visited", evaluate that child
							// for presence of cycle.
							auto state_of_child = states.at((std::get<0>(child))->getId());
							if (state_of_child == not_visited)
							{
								if (isCyclicDFSHelper(adjMatrix, states, std::get<0>(child)))
								{
									return true;
								}
							}
							else if (state_of_child == in_stack)
							{
								// If child node was "in_stack", then that means that there
								// is a cycle in the graph. Return true for presence of the
								// cycle.
								return true;
							}
						}
					}

					// Current node has been evaluated for the presence of cycle and had no
					// cycle. Mark current node as "visited".
					states[node->getId()] = visited;
					// Return that current node didn't result in any cycles.
					return false;
				};
				if (isCyclicDFSHelper(adjMatrix, state, node))
				{
					return true;
				}
			}
		}

		// All nodes have been safely traversed, that means there is no cycle in
		// the graph. Return false.
		return false;
	}

	template <typename T>
	bool Graph<T>::isCyclicDirectedGraphBFS() const
	{
		if (!isDirectedGraph())
		{
			return false;
		}
		auto adjMatrix = this->getAdjMatrix();
		auto nodeSet = this->getNodeSet();

		std::map<unsigned long, unsigned int> indegree;
		for (auto node : nodeSet)
		{
			indegree[node->getId()] = 0;
		}
		// Calculate the indegree i.e. the number of incident edges to the node.
		for (auto const &list : adjMatrix)
		{
			auto children = list.second;
			for (auto const &child : children)
			{
				indegree[std::get<0>(child)->getId()]++;
			}
		}

		std::queue<const Node<T> *> can_be_solved;
		for (auto node : nodeSet)
		{
			// If a node doesn't have any input edges, then that node will
			// definately not result in a cycle and can be visited safely.
			if (!indegree[node->getId()])
			{
				can_be_solved.emplace(&(*node));
			}
		}

		// Vertices that need to be traversed.
		auto remain = this->getNodeSet().size();
		// While there are safe nodes that we can visit.
		while (!can_be_solved.empty())
		{
			auto solved = can_be_solved.front();
			// Visit the node.
			can_be_solved.pop();
			// Decrease number of nodes that need to be traversed.
			remain--;

			// Visit all the children of the visited node.
			auto it = adjMatrix.find(solved);
			if (it != adjMatrix.end())
			{
				for (auto child : it->second)
				{
					// Check if we can visited the node safely.
					if (--indegree[std::get<0>(child)->getId()] == 0)
					{
						// if node can be visited safely, then add that node to
						// the visit queue.
						can_be_solved.emplace(std::get<0>(child));
					}
				}
			}
		}

		// If there are still nodes that we can't visit, then it means that
		// there is a cycle and return true, else return false.
		return !(remain == 0);
	}

	template <typename T>
	bool Graph<T>::isDirectedGraph() const
	{
		auto edgeSet = this->getEdgeSet();
		for (auto edge : edgeSet)
		{
			if (!(edge->isDirected().has_value() && edge->isDirected().value()))
			{
				//Found Undirected Edge
				return false;
			}
		}
		//No Undirected Edge
		return true;
	}

	template <typename T>
	const DialResult Graph<T>::dial(const Node<T> &source, int maxWeight) const
	{
		DialResult result;
		result.success = false;

		auto adj = getAdjMatrix();
		auto nodeSet = getNodeSet();

		if (std::find(nodeSet.begin(), nodeSet.end(), &source) == nodeSet.end())
		{
			// check if source node exist in the graph
			result.errorMessage = ERR_SOURCE_NODE_NOT_IN_GRAPH;
			return result;
		}
		/* With each distance, iterator to that vertex in
       		its bucket is stored so that vertex can be deleted
       		in O(1) at time of updation. So
    		dist[i].first = distance of ith vertex from src vertex
    		dits[i].second = vertex i in bucket number */
		unsigned int V = nodeSet.size();
		std::map<const Node<T> *, std::pair<long, const Node<T> *>> dist;

		// Initialize all distances as infinite (INF)
		for (auto node : nodeSet)
		{
			dist[&(*node)].first = std::numeric_limits<long>::max();
		}

		// Create buckets B[].
		// B[i] keep vertex of distance label i
		std::list<const Node<T> *> B[maxWeight * V + 1];

		B[0].push_back(&source);
		dist[&source].first = 0;

		int idx = 0;
		while (1)
		{
			// Go sequentially through buckets till one non-empty
			// bucket is found
			while (B[idx].size() == 0 && idx < maxWeight * V)
			{
				idx++;
			}

			// If all buckets are empty, we are done.
			if (idx == maxWeight * V)
			{
				break;
			}

			// Take top vertex from bucket and pop it
			auto u = B[idx].front();
			B[idx].pop_front();

			// Process all adjacents of extracted vertex 'u' and
			// update their distanced if required.
			for (auto i = adj[u].begin(); i != adj[u].end(); ++i)
			{
				auto v = (*i).first;
				int weight = 0;
				if ((*i).second->isWeighted().has_value() && (*i).second->isWeighted().value())
				{
					if ((*i).second->isDirected().has_value() && (*i).second->isDirected().value())
					{
						const DirectedWeightedEdge<T> *dw_edge = dynamic_cast<const DirectedWeightedEdge<T> *>((*i).second);
						weight = dw_edge->getWeight();
					}
					else if ((*i).second->isDirected().has_value() && !(*i).second->isDirected().value())
					{
						const UndirectedWeightedEdge<T> *udw_edge = dynamic_cast<const UndirectedWeightedEdge<T> *>((*i).second);
						weight = udw_edge->getWeight();
					}
					else
					{
						//ERROR it shouldn't never returned ( does not exist a Node Weighted and not Directed/Undirected)
						result.errorMessage = ERR_NO_DIR_OR_UNDIR_EDGE;
						return result;
					}
				}
				else
				{
					// No Weighted Edge
					result.errorMessage = ERR_NO_WEIGHTED_EDGE;
					return result;
				}
				auto u_i = std::find_if(dist.begin(), dist.end(), [u](std::pair<const Node<T> *, std::pair<long, const Node<T> *>> const &it)
										{ return (*u == *(it.first)); });

				auto v_i = std::find_if(dist.begin(), dist.end(), [v](std::pair<const Node<T> *, std::pair<long, const Node<T> *>> const &it)
										{ return (*v == *(it.first)); });
				long du = u_i->second.first;
				long dv = v_i->second.first;

				// If there is shorted path to v through u.
				if (dv > du + weight)
				{
					// If dv is not INF then it must be in B[dv]
					// bucket, so erase its entry using iterator
					// in O(1)
					if (dv != std::numeric_limits<long>::max())
					{
						auto findIter = std::find(B[dv].begin(), B[dv].end(), dist[v].second);
						B[dv].erase(findIter);
					}

					//  updating the distance
					dist[v].first = du + weight;
					dv = dist[v].first;

					// pushing vertex v into updated distance's bucket
					B[dv].push_front(v);

					// storing updated iterator in dist[v].second
					dist[v].second = *(B[dv].begin());
				}
			}
		}
		for (auto dist_i : dist)
		{
			result.minDistanceMap[dist_i.first->getId()] = dist_i.second.first;
		}
		result.success = true;

		return result;
	}

	template <typename T>
	int Graph<T>::writeToFile(InputOutputFormat format, const std::string &workingDir, const std::string &OFileName, bool compress, bool writeNodeFeat, bool writeEdgeWeight) const
	{
		int result = 0;
		if (format == InputOutputFormat::STANDARD_CSV)
		{
			result = writeToStandardFile_csv(workingDir, OFileName, compress, writeNodeFeat, writeEdgeWeight);
		}
		else if (format == InputOutputFormat::STANDARD_TSV)
		{
			result = writeToStandardFile_tsv(workingDir, OFileName, compress, writeNodeFeat, writeEdgeWeight);
		}
		else
		{
			//OUTPUT FORMAT NOT RECOGNIZED
			return -1;
		}
		if (result == 0 && compress)
		{
			auto compress = [this, &workingDir, &OFileName, &writeNodeFeat, &writeEdgeWeight](const std::string &extension)
			{
				std::string completePathToFileGraph = workingDir + "/" + OFileName + extension;
				std::string completePathToFileGraphCompressed = workingDir + "/" + OFileName + extension + ".gz";
				int _result = compressFile(completePathToFileGraph, completePathToFileGraphCompressed);
				if (_result == 0)
				{
					_result = remove(completePathToFileGraph.c_str());
				}
				if (_result == 0)
				{
					if (writeNodeFeat)
					{
						std::string completePathToFileNodeFeat = workingDir + "/" + OFileName + "_NodeFeat" + extension;
						std::string completePathToFileNodeFeatCompressed = workingDir + "/" + OFileName + "_NodeFeat" + extension + ".gz";
						_result = compressFile(completePathToFileNodeFeat, completePathToFileNodeFeatCompressed);
						if (_result == 0)
						{
							_result = remove(completePathToFileNodeFeat.c_str());
						}
					}
				}
				if (_result == 0)
				{
					if (writeEdgeWeight)
					{
						std::string completePathToFileEdgeWeight = workingDir + "/" + OFileName + "_EdgeWeight" + extension;
						std::string completePathToFileEdgeWeightCompressed = workingDir + "/" + OFileName + "_EdgeWeight" + extension + ".gz";
						_result = compressFile(completePathToFileEdgeWeight, completePathToFileEdgeWeightCompressed);
						if (_result == 0)
						{
							_result = remove(completePathToFileEdgeWeight.c_str());
						}
					}
				}
				return _result;
			};
			if (format == InputOutputFormat::STANDARD_CSV)
			{
				auto result = compress(".csv");
			}
			else if (format == InputOutputFormat::STANDARD_TSV)
			{
				auto result = compress(".tsv");
			}
			else
			{
				//OUTPUT FORMAT NOT RECOGNIZED
				result = -1;
			}
		}
		return result;
	}

	template <typename T>
	int Graph<T>::readFromFile(InputOutputFormat format, const std::string &workingDir, const std::string &OFileName, bool compress, bool readNodeFeat, bool readEdgeWeight)
	{
		int result = 0;
		if (compress)
		{
			auto decompress = [this, &workingDir, &OFileName, &readNodeFeat, &readEdgeWeight](const std::string &extension)
			{
				std::string completePathToFileGraph = workingDir + "/" + OFileName + extension;
				std::string completePathToFileGraphCompressed = workingDir + "/" + OFileName + extension + ".gz";
				int _result = decompressFile(completePathToFileGraphCompressed, completePathToFileGraph);
				if (_result == 0)
				{
					if (readNodeFeat)
					{
						std::string completePathToFileNodeFeat = workingDir + "/" + OFileName + "_NodeFeat" + extension;
						std::string completePathToFileNodeFeatCompressed = workingDir + "/" + OFileName + "_NodeFeat" + extension + ".gz";
						_result = decompressFile(completePathToFileNodeFeatCompressed, completePathToFileNodeFeat);
					}
				}
				if (_result == 0)
				{
					if (readEdgeWeight)
					{
						std::string completePathToFileEdgeWeight = workingDir + "/" + OFileName + "_EdgeWeight" + extension;
						std::string completePathToFileEdgeWeightCompressed = workingDir + "/" + OFileName + "_EdgeWeight" + extension + ".gz";
						_result = decompressFile(completePathToFileEdgeWeightCompressed, completePathToFileEdgeWeight);
					}
				}
				return _result;
			};
			if (format == InputOutputFormat::STANDARD_CSV)
			{
				result = decompress(".csv");
			}
			else if (format == InputOutputFormat::STANDARD_TSV)
			{
				result = decompress(".tsv");
			}
			else
			{
				//OUTPUT FORMAT NOT RECOGNIZED
				result = -1;
			}
		}
		if (result == 0)
		{
			if (format == InputOutputFormat::STANDARD_CSV)
			{
				result = readFromStandardFile_csv(workingDir, OFileName, compress, readNodeFeat, readEdgeWeight);
			}
			else if (format == InputOutputFormat::STANDARD_TSV)
			{
				result = readFromStandardFile_tsv(workingDir, OFileName, compress, readNodeFeat, readEdgeWeight);
			}
			else
			{
				//OUTPUT FORMAT NOT RECOGNIZED
				result = -1;
			}
		}
		return result;
	}

	template <typename T>
	PartitionMap<T> Graph<T>::partitionGraph(PartitionAlgorithm algorithm, unsigned int numberOfPartitions) const
	{
		PartitionMap<T> partitionMap;
		for (unsigned int i = 0; i < numberOfPartitions; ++i)
		{
			partitionMap[i] = new Partition<T>(i);
		}
		if (algorithm == PartitionAlgorithm::GREEDY_VC)
		{
			greedyPartition(partitionMap);
		}
		else if (algorithm == PartitionAlgorithm::HDRF)
		{
			HDRFPartition(partitionMap);
		}
		else
		{
			//Error not recognized algorithm
			partitionMap.clear();
		}
		return partitionMap;
	}

	/// Class that implement the Thread Safe Graph.
	template <typename T>
	class Graph_TS : public Graph<T>, public ThreadSafe
	{
	public:
		Graph_TS() = default;
		Graph_TS(const std::list<const Edge<T> *> &edgeSet);
		Graph_TS(const Graph<T> &graph);
		~Graph_TS() = default;
		/**
 		* \brief
 		* Function that return the Edge set of the Graph
		* Note: Thread Safe
		*
 		* @returns a list of Edges of the graph
 		*
 		*/
		const std::list<const Edge<T> *> &getEdgeSet() const override;
		/**
 		* \brief
 		* Function set the Edge Set of the Graph
		* Note: Thread Safe
 		*
 		* @param edgeSet The Edge Set
 		*
 		*/
		void setEdgeSet(std::list<const Edge<T> *> &edgeSet) override;
		/**
 		* \brief
 		* Function add an Edge to the Graph Edge Set
		* Note: Thread Safe
 		*
 		* @param edge The Edge to insert
 		*
 		*/
		void addEdge(const Edge<T> *edge) override;
		/**
 		* \brief
 		* Function remove an Edge from the Graph Edge Set
		* Note: Thread Safe
 		*
 		* @param edgeId The Edge Id to remove
 		*
 		*/
		void removeEdge(unsigned long edgeId) override;
		/**
 		* \brief
 		* Function that return the Node Set of the Graph
		* Note: Thread Safe
 		*
 		* @returns a list of Nodes of the graph
 		*
 		*/
		const std::list<const Node<T> *> getNodeSet() const override;
		/**
 		* \brief
 		* Function that return an Edge with specific ID if Exist in the Graph
		* Note: Thread Safe
 		*
		* @param edgeId The Edge Id to return
 		* @returns the Edge if exist
 		*
 		*/
		const std::optional<const Edge<T> *> getEdge(unsigned long edgeId) const override;
		/**
		* @brief This function generate a list of adjacency matrix with every element of the matrix
		* contain the node where is directed the link and the Edge corrispondent to the link
		* Note: Thread Safe
		*/
		const AdjacencyMatrix<T> getAdjMatrix() const override;
		/**
 		* @brief Function runs the dijkstra algorithm for some source node and
 		* target node in the graph and returns the shortest distance of target
 		* from the source.
		* Note: Thread Safe
 		*
		* @param source source vertex
 		* @param target target vertex
 		*
 		* @return shortest distance if target is reachable from source else ERROR in
 		* case if target is not reachable from source or there is error in the computation.
 		*/
		const DijkstraResult dijkstra(const Node<T> &source, const Node<T> &target) const override;
		/**
 		* \brief
 		* Function performs the breadth first search algorithm over the graph
		* Note: Thread Safe
 		*
 		* @param start Node from where traversing starts
 		* @returns a vector of Node indicating which Node were visited during the
 		* search.
 		*
 		*/
		const std::vector<Node<T>> breadth_first_search(const Node<T> &start) const override;
		/**
 		* \brief
 		* Function performs the depth first search algorithm over the graph
		* Note: Thread Safe
 		*
 		* @param start Node from where traversing starts
 		* @returns a vector of Node indicating which Node were visited during the
 		* search.
 		*
 		*/
		const std::vector<Node<T>> depth_first_search(const Node<T> &start) const override;

		/**
		* \brief
     	* This function uses DFS to check for cycle in the graph.
     	* Pay Attention, this function work only with directed Graph
		* Note: Thread Safe
     	*
     	* @return true if a cycle is detected, else false. ( false is returned also if the graph in indirected)
     	*/
		bool isCyclicDirectedGraphDFS() const override;

		/**
		* \brief
     	* This function uses BFS to check for cycle in the graph.
     	* Pay Attention, this function work only with directed Graph
		* Note: Thread Safe
     	*
     	* @return true if a cycle is detected, else false. ( false is returned also if the graph in indirected)
     	*/
		bool isCyclicDirectedGraphBFS() const override;

		/**
     	* \brief
     	* This function checks if a graph is directed
		* Note: Thread Safe
     	*
     	* @return true if the graph is directed, else false.
     	*/
		bool isDirectedGraph() const override;

		/**
     	* \brief
     	* This function write the graph in an output file
		* Note: Thread Safe
     	*
		* @param format The Output format of the file
		* @param workingDir The path to the directory in which will be placed the output file
		* @param OFileName The Output File Name ( )
		* @param compress Indicates if the output will be compressed
		* @param writeNodeFeat Indicates if export also Node Features
		* @param writeEdgeWeight Indicates if export also Edge Weights
     	* @return 0 if all OK, else return a negative value
     	*/

		/**
 		* @brief Function runs the Dial algorithm  (Optimized Dijkstra for small range weights) for some source node and
 		* target node in the graph and returns the shortest distance of target
 		* from the source.
		* Note: Thread Safe
 		*
		* @param source source vertex
		* @param maxWeight maximum weight of the edge
 		*
 		* @return shortest distance for all nodes reachable from source else ERROR in
 		* case there is error in the computation.
 		*/
		const DialResult dial(const Node<T> &source, int maxWeight) const override;

		/**
     	* \brief
     	* This function write the graph in an output file
		* Note: Thread Safe
     	*
		* @param format The Output format of the file
		* @param workingDir The path to the directory in which is placed the Output file
		* @param OFileName The Output File Name ( )
		* @param compress Indicates if the Output will be compressed ( Pay Attention if compress flag is true, not compressed files will be deleted [ #48 ] )
		* @param writeNodeFeat Indicates if export also Node Features
		* @param writeEdgeWeight Indicates if export also Edge Weights
     	* @return 0 if all OK, else return a negative value
     	*/
		int writeToFile(InputOutputFormat format = InputOutputFormat::STANDARD_CSV, const std::string &workingDir = ".", const std::string &OFileName = "graph", bool compress = false, bool writeNodeFeat = false, bool writeEdgeWeight = false) const override;

		/**
     	* \brief
     	* This function read the graph from an input file
		* Note: Thread Safe
     	*
		* @param format The Input format of the file
		* @param workingDir The path to the directory in which is placed the Input file
		* @param OFileName The Input File Name ( )
		* @param compress Indicates if the Input is compressed
		* @param readNodeFeat Indicates if import also Node Features
		* @param readEdgeWeight Indicates if import also Edge Weights
     	* @return 0 if all OK, else return a negative value
     	*/
		int readFromFile( InputOutputFormat format = InputOutputFormat::STANDARD_CSV, const std::string &workingDir = ".", const std::string &OFileName = "graph", bool compress = false, bool readNodeFeat = false, bool readEdgeWeight = false) override;

		/**
     	* \brief
     	* This function partition a graph in a set of partitions
		* Note: Thread Safe
     	*
		* @param algorithm The partition algorithm
		* @param numberOfPartition The number of partitions
		* @return The partiton Map of the partitioned graph
     	*/
		PartitionMap<T> partitionGraph( PartitionAlgorithm algorithm, unsigned int numberOfPartitions) const override;
	};

	template <typename T>
	Graph_TS<T>::Graph_TS(const std::list<const Edge<T> *> &edgeSet) : Graph<T>(edgeSet), ThreadSafe() {}

	template <typename T>
	Graph_TS<T>::Graph_TS(const Graph<T> &graph) : Graph<T>(graph), ThreadSafe() {}

	template <typename T>
	const std::list<const Edge<T> *> &Graph_TS<T>::getEdgeSet() const
	{
		std::lock_guard<std::mutex> lock(mutex);
		return Graph<T>::getEdgeSet();
	}

	template <typename T>
	void Graph_TS<T>::setEdgeSet(std::list<const Edge<T> *> &edgeSet)
	{
		getLock();
		Graph<T>::setEdgeSet(edgeSet);
		releaseLock();
	}

	template <typename T>
	void Graph_TS<T>::addEdge(const Edge<T> *edge)
	{
		getLock();
		Graph<T>::addEdge(edge);
		releaseLock();
	}

	template <typename T>
	void Graph_TS<T>::removeEdge(unsigned long edgeId)
	{
		getLock();
		Graph<T>::removeEdge(edgeId);
		releaseLock();
	}

	template <typename T>
	const std::list<const Node<T> *> Graph_TS<T>::getNodeSet() const
	{
		getLock();
		auto ns = Graph<T>::getNodeSet();
		releaseLock();
		return ns;
	}

	template <typename T>
	const std::optional<const Edge<T> *> Graph_TS<T>::getEdge(unsigned long edgeId) const
	{
		getLock();
		auto e = Graph<T>::getEdge(edgeId);
		releaseLock();
		return e;
	}

	template <typename T>
	const AdjacencyMatrix<T> Graph_TS<T>::getAdjMatrix() const
	{
		getLock();
		auto adjm = Graph<T>::getAdjMatrix();
		releaseLock();
		return adjm;
	}

	template <typename T>
	const DijkstraResult Graph_TS<T>::dijkstra(const Node<T> &source, const Node<T> &target) const
	{
		getLock();
		auto dij = Graph<T>::dijkstra(source, target);
		releaseLock();
		return dij;
	}

	template <typename T>
	const std::vector<Node<T>> Graph_TS<T>::breadth_first_search(const Node<T> &start) const
	{
		getLock();
		auto bfs = Graph<T>::breadth_first_search(start);
		releaseLock();
		return bfs;
	}

	template <typename T>
	const std::vector<Node<T>> Graph_TS<T>::depth_first_search(const Node<T> &start) const
	{
		getLock();
		auto dfs = Graph<T>::depth_first_search(start);
		releaseLock();
		return dfs;
	}

	template <typename T>
	bool Graph_TS<T>::isCyclicDirectedGraphDFS() const
	{
		getLock();
		auto result = Graph<T>::isCyclicDirectedGraphDFS();
		releaseLock();
		return result;
	}

	template <typename T>
	bool Graph_TS<T>::isCyclicDirectedGraphBFS() const
	{
		getLock();
		auto result = Graph<T>::isCyclicDirectedGraphBFS();
		releaseLock();
		return result;
	}

	template <typename T>
	bool Graph_TS<T>::isDirectedGraph() const
	{
		getLock();
		auto result = Graph<T>::isDirectedGraph();
		releaseLock();
		return result;
	}

	template <typename T>
	const DialResult Graph_TS<T>::dial(const Node<T> &source, int maxWeight) const
	{
		getLock();
		auto dial = Graph<T>::dial(source, maxWeight);
		releaseLock();
		return dial;
	}

	template <typename T>
	int Graph_TS<T>::writeToFile( InputOutputFormat format, const std::string &workingDir, const std::string &OFileName, bool compress, bool writeNodeFeat, bool writeEdgeWeight) const
	{
		getLock();
		auto result = Graph<T>::writeToFile(format, workingDir, OFileName, compress, writeNodeFeat, writeEdgeWeight);
		releaseLock();
		return result;
	}

	template <typename T>
	int Graph_TS<T>::readFromFile( InputOutputFormat format, const std::string &workingDir, const std::string &OFileName, bool compress, bool readNodeFeat, bool readEdgeWeight)
	{
		getLock();
		auto result = Graph<T>::readFromFile(format, workingDir, OFileName, compress, readNodeFeat, readEdgeWeight);
		releaseLock();
		return result;
	}

	template <typename T>
	PartitionMap<T> Graph_TS<T>::partitionGraph( PartitionAlgorithm algorithm, unsigned int numberOfPartitions) const
	{
		getLock();
		auto partitions = Graph<T>::partitionGraph(algorithm, numberOfPartitions);
		releaseLock();
		return partitions;
	}

	template <typename T>
	class Partition : public Graph<T>
	{
	public:
		Partition();
		Partition(unsigned int partitionId);
		Partition(const std::list<const Edge<T> *> &edgeSet);
		Partition(unsigned int partitionId, const std::list<const Edge<T> *> &edgeSet);
		~Partition() = default;
		/**
		* @brief Get the Partition ID
		*
		* @return The ID of the partition
		*/
		unsigned int getPartitionId() const;
		/**
 		* @brief Set the Partition ID
 		*
		* @param partitionId the ID to set
 		*/
		void setPartitionId(unsigned int partitionId);

	private:
		unsigned int partitionId;
	};

	/**
 	* @brief Calculate and return the statistic of the Partitioned Graph
 	*
	* @param partitionMap the Partition Map
	*
	* @return The Statistic of the Partioned Graph
 	*/
	template <typename T>
	static PartitioningStats getPartitionStats(const PartitionMap<T> &partitionMap);

	/**
 	* @brief Calculate the Maximum Load in a single partition (in terms of edges) for the Partioned Graph
 	*
	* @param partitionMap the Partition Map
	*
	* @return The value of the Maximum Load
 	*/
	template <typename T>
	static unsigned int getMaxEdgesLoad(const PartitionMap<T> &partitionMap);

	/**
 	* @brief Calculate the Minimum Load in a single partition (in terms of edges) for the Partioned Graph
 	*
	* @param partitionMap the Partition Map
	*
	* @return The value of the Minimum Load
 	*/
	template <typename T>
	static unsigned int getMinEdgesLoad(const PartitionMap<T> &partitionMap);

	/**
 	* @brief Calculate the Maximum Load in a single partition (in terms of nodes) for the Partioned Graph
 	*
	* @param partitionMap the Partition Map
	*
	* @return The value of the Maximum Load
 	*/
	template <typename T>
	static unsigned int getMaxNodesLoad(const PartitionMap<T> &partitionMap);

	/**
 	* @brief Calculate the Minimum Load in a single partition (in terms of nodes) for the Partioned Graph
 	*
	* @param partitionMap the Partition Map
	*
	* @return The value of the Minimum Load
 	*/
	template <typename T>
	static unsigned int getMinNodesLoad(const PartitionMap<T> &partitionMap);

	/**
 	* @brief Calculate the Number of Unique Edges in the Partitioned Graph ( this value is equal to the number of edges in the Original Graph)
 	*
	* @param partitionMap the Partition Map
	*
	* @return The number of Edges
 	*/
	template <typename T>
	static unsigned int getNumberOfEdges(const PartitionMap<T> &partitionMap);

	/**
 	* @brief Calculate the Number of Unique Nodes in the Partitioned Graph ( this value is equal to the number of nodes in the Original Graph)
 	*
	* @param partitionMap the Partition Map
	*
	* @return The number of Nodes
 	*/
	template <typename T>
	static unsigned int getNumberOfNodes(const PartitionMap<T> &partitionMap);

	/**
 	* @brief Calculate the Total Number of Edges in the Partitioned Graph
 	*
	* @param partitionMap the Partition Map
	*
	* @return The number of Edges
 	*/
	template <typename T>
	static unsigned int getNumberOfReplicatedEdges(const PartitionMap<T> &partitionMap);

	/**
 	* @brief Calculate the Total Number of Nodes in the Partitioned Graph
 	*
	* @param partitionMap the Partition Map
	*
	* @return The number of Nodes
 	*/
	template <typename T>
	static unsigned int getNumberOfReplicatedNodes(const PartitionMap<T> &partitionMap);

	template <typename T>
	Partition<T>::Partition() : Graph<T>()
	{
		partitionId = 0;
	}

	template <typename T>
	Partition<T>::Partition(unsigned int partitionId) : Graph<T>()
	{
		this->partitionId = partitionId;
	}

	template <typename T>
	Partition<T>::Partition(const std::list<const Edge<T> *> &edgeSet) : Graph<T>(edgeSet)
	{
		partitionId = 0;
	}

	template <typename T>
	Partition<T>::Partition(unsigned int partitionId, const std::list<const Edge<T> *> &edgeSet) : Graph<T>(edgeSet)
	{
		this->partitionId = partitionId;
	}

	template <typename T>
	unsigned int Partition<T>::getPartitionId() const
	{
		return partitionId;
	}

	template <typename T>
	void Partition<T>::setPartitionId(unsigned int partitionId)
	{
		this->partitionId = partitionId;
	}

	template <typename T>
	PartitioningStats getPartitionStats(const PartitionMap<T> &partitionMap)
	{
		PartitioningStats result;
		result.numberOfPartitions = partitionMap.size();
		result.numberOfNodes = getNumberOfNodes(partitionMap);
		result.numberOfEdges = getNumberOfEdges(partitionMap);
		result.replicatedNodesCount = getNumberOfReplicatedNodes(partitionMap);
		result.replicatedEdgesCount = getNumberOfReplicatedEdges(partitionMap);
		result.maxEdgesLoad = getMaxEdgesLoad(partitionMap);
		result.minEdgesLoad = getMinEdgesLoad(partitionMap);
		result.maxNodesLoad = getMaxNodesLoad(partitionMap);
		result.minNodesLoad = getMinNodesLoad(partitionMap);
		result.edgesReplicationFactor = (double)result.replicatedEdgesCount / result.numberOfEdges;
		result.nodesReplicationFactor = (double)result.replicatedNodesCount / result.numberOfNodes;
		result.balanceEdgesFactor = (double)(result.maxEdgesLoad - result.minEdgesLoad) / (result.maxEdgesLoad);
		result.balanceNodesFactor = (double)(result.maxNodesLoad - result.minNodesLoad) / (result.maxNodesLoad);
		return result;
	}

	template <typename T>
	unsigned int getMaxEdgesLoad(const PartitionMap<T> &partitionMap)
	{
		unsigned int maxLoad = 0;
		for (auto it = partitionMap.begin(); it != partitionMap.end(); ++it)
		{
			if (it->second->getEdgeSet().size() > maxLoad)
			{
				maxLoad = it->second->getEdgeSet().size();
			}
		}
		return maxLoad;
	}

	template <typename T>
	unsigned int getMinEdgesLoad(const PartitionMap<T> &partitionMap)
	{
		unsigned int minLoad = std::numeric_limits<unsigned int>::max();
		for (auto it = partitionMap.begin(); it != partitionMap.end(); ++it)
		{
			if (it->second->getEdgeSet().size() < minLoad)
			{
				minLoad = it->second->getEdgeSet().size();
			}
		}
		return minLoad;
	}

	template <typename T>
	unsigned int getMaxNodesLoad(const PartitionMap<T> &partitionMap)
	{
		unsigned int maxLoad = 0;
		for (auto it = partitionMap.begin(); it != partitionMap.end(); ++it)
		{
			if (it->second->getNodeSet().size() > maxLoad)
			{
				maxLoad = it->second->getNodeSet().size();
			}
		}
		return maxLoad;
	}

	template <typename T>
	unsigned int getMinNodesLoad(const PartitionMap<T> &partitionMap)
	{
		unsigned int minLoad = std::numeric_limits<unsigned int>::max();
		for (auto it = partitionMap.begin(); it != partitionMap.end(); ++it)
		{
			if (it->second->getNodeSet().size() < minLoad)
			{
				minLoad = it->second->getNodeSet().size();
			}
		}
		return minLoad;
	}

	template <typename T>
	unsigned int getNumberOfEdges(const PartitionMap<T> &partitionMap)
	{
		unsigned int numberOfEdges = 0;
		std::list<const Edge<T> *> edgeSet;

		for (auto it = partitionMap.begin(); it != partitionMap.end(); ++it)
		{
			const std::list<const Edge<T> *> partitionEdgeSet = it->second->getEdgeSet();
			for (auto it2 = partitionEdgeSet.begin(); it2 != partitionEdgeSet.end(); ++it2)
			{
				if (std::find_if(edgeSet.begin(), edgeSet.end(), [it2](const Edge<T> *edge)
								 { return (*(*it2) == *edge); }) == edgeSet.end())
				{
					edgeSet.push_back(*it2);
				}
			}
		}

		return edgeSet.size();
	}

	template <typename T>
	unsigned int getNumberOfNodes(const PartitionMap<T> &partitionMap)
	{
		unsigned int numberOfNodes = 0;
		std::list<const Node<T> *> nodeSet;

		for (auto it = partitionMap.begin(); it != partitionMap.end(); ++it)
		{
			const std::list<const Node<T> *> partitionNodeSet = it->second->getNodeSet();
			for (auto it2 = partitionNodeSet.begin(); it2 != partitionNodeSet.end(); ++it2)
			{
				if (std::find_if(nodeSet.begin(), nodeSet.end(), [it2](const Node<T> *node)
								 { return (*(*it2) == *node); }) == nodeSet.end())
				{
					nodeSet.push_back(*it2);
				}
			}
		}

		return nodeSet.size();
	}

	template <typename T>
	unsigned int getNumberOfReplicatedEdges(const PartitionMap<T> &partitionMap)
	{

		unsigned int numberOfEdges = 0;
		for (auto it = partitionMap.begin(); it != partitionMap.end(); ++it)
		{
			numberOfEdges += it->second->getEdgeSet().size();
		}
		return numberOfEdges;
	}

	template <typename T>
	unsigned int getNumberOfReplicatedNodes(const PartitionMap<T> &partitionMap)
	{
		unsigned int numberOfNodes = 0;
		for (auto it = partitionMap.begin(); it != partitionMap.end(); ++it)
		{
			numberOfNodes += it->second->getNodeSet().size();
		}
		return numberOfNodes;
	}

	/*!
  	Interface to implement for a custom writer.
	*/

	template <typename T>
	class Writer
	{
	public:
		/**
 		* \brief
 		* Function performs the writing of the Graph to the file.
 		*
 		* @param graph The graph to be written.
		* @param file The output file to be written.
 		* @returns a negative value if is impossible to write the graph from the file, else 0 if the graph is write successfully.
 		*
 		*/
		virtual int writeGraph(const Graph<T> &graph, std::ofstream &file) = 0;
	};

	/*!
  	Interface to implement for a custom reader.
	*/
	template <typename T>
	class Reader
	{
		/**
 		* \brief
 		* Function performs the writing of the Graph to the file.
 		*
 		* @param graph The graph to be filled.
		* @param file The input file to be read.
 		* @returns a negative value if is impossible to read the graph from the file, else 0 if the graph is read successfully.
 		*
 		*/
		virtual int ReadGraph(Graph<T> &graph, std::ifstream &file) = 0;
	};

	//ostream overload
	template <typename T>
	std::ostream &
	operator<<(std::ostream &os, const Node<T> &node)
	{
		os << "Node: {\n"
		   << "  Id:\t" << node.id << "\n  Data:\t" << node.data << "\n}";
		return os;
	}

	template <typename T>
	std::ostream &operator<<(std::ostream &os, const Edge<T> &edge)
	{
		os << "((Node: " << edge.nodePair.first->getId() << ")) ?----- |Edge: " << edge.id << "|-----? ((Node: " << edge.nodePair.second->getId() << "))";
		return os;
	}

	template <typename T>
	std::ostream &operator<<(std::ostream &os, const DirectedEdge<T> &edge)
	{
		os << "((Node: " << edge.getFrom().getId() << ")) +----- |Edge: #" << edge.getId() << "|-----> ((Node: " << edge.getTo().getId() << "))";
		return os;
	}

	template <typename T>
	std::ostream &operator<<(std::ostream &os, const UndirectedEdge<T> &edge)
	{
		os << "((Node: " << edge.getNode1().getId() << ")) <----- |Edge: #" << edge.getId() << "|-----> ((Node: " << edge.getNode2().getId() << "))";
		return os;
	}

	template <typename T>
	std::ostream &operator<<(std::ostream &os, const DirectedWeightedEdge<T> &edge)
	{
		os << "((Node: " << edge.getFrom().getId() << ")) +----- |Edge: #" << edge.getId() << " W:" << edge.getWeight() << "|-----> ((Node: " << edge.getTo().getId() << "))";
		return os;
	}

	template <typename T>
	std::ostream &operator<<(std::ostream &os, const UndirectedWeightedEdge<T> &edge)
	{
		os << "((Node: " << edge.getNode1().getId() << ")) <----- |Edge: #" << edge.getId() << " W:" << edge.getWeight() << "|-----> ((Node: " << edge.getNode2().getId() << "))";
		return os;
	}

	template <typename T>
	std::ostream &operator<<(std::ostream &os, const AdjacencyMatrix<T> &adj)
	{
		os << "Adjacency Matrix:\n";
		auto it = adj.begin();
		unsigned long max_column = 0;
		for (it; it != adj.end(); ++it)
		{
			if (it->second.size() > max_column)
			{
				max_column = it->second.size();
			}
		}
		if (max_column == 0)
		{
			os << "ERROR in Print\n";
			return os;
		}
		else
		{
			it = adj.begin();
			os << "|--|";
			for (int i = 0; i < max_column; i++)
			{
				os << "-----|";
			}
			os << "\n";
			for (it; it != adj.end(); ++it)
			{
				os << "|N" << it->first->getId() << "|";
				auto it2 = it->second.begin();
				for (it2; it2 != it->second.end(); ++it2)
				{
					os << "N" << it2->first->getId() << ",E" << it2->second->getId() << "|";
				}
				os << "\n|--|";
				for (int i = 0; i < max_column; i++)
				{
					os << "-----|";
				}
				os << "\n";
			}
		}
		return os;
	}

	// Partitioning Utils //////////////

	namespace PARTITIONING
	{
		class Globals
		{
		private:
		public:
			Globals(int numberOfPartiton, double lambda = 1, PartitionAlgorithm algorithm = PartitionAlgorithm::HDRF, unsigned int threads = std::thread::hardware_concurrency());
			~Globals();

			void print();

			//CONSTANT
			const int SLEEP_LIMIT = 1024;
			const int PLACES = 4;

			int numberOfPartition; //number of partitions
			//OPTIONAL
			PartitionAlgorithm partitionStategy;
			double lambda;
			unsigned int threads;
		};

		inline Globals::Globals(int numberOfPartiton, double lambda, PartitionAlgorithm algorithm, unsigned int threads)
		{
			this->numberOfPartition = numberOfPartiton;
			this->partitionStategy = algorithm;
			this->lambda = lambda;
			this->threads = threads;
		}

		inline Globals::~Globals()
		{
		}

		template<typename T>
		class Record
		{
		public:
			virtual PartitionMap<T>& getPartitions() = 0;
			virtual void addPartition(int m) = 0;
			virtual bool hasReplicaInPartition(int m) = 0;
			virtual bool getLock() = 0;
			virtual bool releaseLock() = 0;
			virtual int getReplicas() = 0;
			virtual int getDegree() = 0;
			virtual void incrementDegree() = 0;
		};
		template<typename T>
		class PartitionState
		{
		public:
			virtual Record<T> &getRecord(int x) = 0;
			virtual int getMachineLoad(int m) = 0;
			virtual void incrementMachineLoad(int m, Edge<T> e) = 0;
			virtual int getMinLoad() = 0;
			virtual int getMaxLoad() = 0;
			virtual std::vector<int> getMachines_load() = 0;
			virtual int getTotalReplicas() = 0;
			virtual int getNumVertices() = 0;
			virtual std::set<int> getVertexIds() = 0;
		};
		template<typename T>
		class PartitionStrategy
		{
		public:
			virtual void performStep(Edge<T> &t, PartitionState<T> &Sstate) = 0;
		};
		class Runnable
		{
		public:
			virtual void run() = 0;
		};

		template<typename T>
		class CoordinatedRecord : public Record<T>
		{
		private:
			PartitionMap<T> partitions;
			std::mutex lock;
			int degree;

		public:
			CoordinatedRecord();
			~CoordinatedRecord();

			PartitionMap<T>& getPartitions();
			void addPartition(int m);
			bool hasReplicaInPartition(int m);
			bool getLock();
			bool releaseLock();
			int getReplicas();
			int getDegree();
			void incrementDegree();

			void addAll(std::set<int> &set);
			static std::set<int> intersection(CoordinatedRecord &x, CoordinatedRecord &y);
		};
		template<typename T>
		CoordinatedRecord<T>::CoordinatedRecord() : partitions(), lock()
		{
			degree = 0;
		}
		template<typename T>
		CoordinatedRecord<T>::~CoordinatedRecord()
		{
		}
		template<typename T>
		PartitionMap<T>& CoordinatedRecord<T>::getPartitions()
		{
			return partitions;
		}
		template<typename T>
		void CoordinatedRecord<T>::addPartition(int m)
		{
			if (m == -1)
			{
		std::cout << "ERROR! record.addPartition(-1)" << std::endl;
        exit(-1);
			}
			partitions.insert(m);
		}
		template<typename T>
		bool CoordinatedRecord<T>::hasReplicaInPartition(int m)
		{
			return partitions.find(m) != partitions.end();
		}
		template<typename T>
		bool CoordinatedRecord<T>::getLock()
		{
			return lock.try_lock();
		}
		template<typename T>
		bool CoordinatedRecord<T>::releaseLock()
		{
			lock.unlock();
			return true;
		}
		template<typename T>
		int CoordinatedRecord<T>::getReplicas()
		{
			return partitions.size();
		}
		template<typename T>
		int CoordinatedRecord<T>::getDegree()
		{
			return degree;
		}
		template<typename T>
		void CoordinatedRecord<T>::incrementDegree()
		{
			degree++;
		}
		template<typename T>
		void CoordinatedRecord<T>::addAll(std::set<int> &set)
		{
			partitions.insert(set.begin(), set.end());
		}
		template<typename T>
		std::set<int> CoordinatedRecord<T>::intersection(CoordinatedRecord &x, CoordinatedRecord &y)
		{
			std::set<int> result;
			set_intersection(x.partitions.begin(), x.partitions.end(), y.partitions.begin(), y.partitions.end(),
							 std::inserter(result, result.begin()));
			return result;
		}
		
		template<typename T>
		class CoordinatedPartitionState : public PartitionState<T>
		{
		private:
			std::map<int, CoordinatedRecord<T>> record_map;
			std::vector<std::atomic<int>> machines_load_edges;
			std::vector<std::atomic<int>> machines_load_vertices;
			Globals GLOBALS;
			int MAX_LOAD;
			//DatWriter out; //to print the final partition of each edge
		public:
			CoordinatedPartitionState(Globals &G);
			~CoordinatedPartitionState();

			Record<T> &getRecord(int x);
			int getMachineLoad(int m);
			void incrementMachineLoad(int m, Edge<T> &e);
			int getMinLoad();
			int getMaxLoad();
			std::vector<int> getMachines_load();
			int getTotalReplicas();
			int getNumVertices();
			std::set<int> getVertexIds();

			void incrementMachineLoadVertices(int m);
			std::vector<int> getMachines_loadVertices();
		};
		template<typename T>
		CoordinatedPartitionState<T>::CoordinatedPartitionState(Globals &G) : record_map()
		{
			this->GLOBALS = G;
			for (int i = 0; i < GLOBALS.numberOfPartition; i++)
			{
				machines_load_edges.push_back(std::atomic<int>(0));
				machines_load_vertices.push_back(std::atomic<int>(0));
			}
			MAX_LOAD = 0;
		}
		template<typename T>
		CoordinatedPartitionState<T>::~CoordinatedPartitionState()
		{
		}
		template<typename T>
		Record<T> &CoordinatedPartitionState<T>::getRecord(int x)
		{
			if (record_map.find(x) == record_map.end())
			{
				record_map[x] = CoordinatedRecord<T>();
			}
			return record_map.at(x);
		}
		template<typename T>
		int CoordinatedPartitionState<T>::getMachineLoad(int m)
		{
			return machines_load_edges.at(m);
		}
		template<typename T>
		void CoordinatedPartitionState<T>::incrementMachineLoad(int m, Edge<T> &e)
		{
			machines_load_edges[m] = machines_load_edges[m] + 1;
			int new_value = machines_load_edges.at(m);
			if (new_value > MAX_LOAD)
			{
				MAX_LOAD = new_value;
			}
		}
		template<typename T>
		int CoordinatedPartitionState<T>::getMinLoad()
		{
			int MIN_LOAD = std::numeric_limits<int>::max();
			auto machines_load_edges_it = machines_load_edges.begin();
			for (machines_load_edges_it; machines_load_edges_it != machines_load_edges.end(); ++machines_load_edges_it)
			{
				int loadi = *machines_load_edges_it;
				if (loadi < MIN_LOAD)
				{
					MIN_LOAD = loadi;
				}
			}
			return MIN_LOAD;
		}
		template<typename T>
		int CoordinatedPartitionState<T>::getMaxLoad()
		{
			return MAX_LOAD;
		}
		template<typename T>
		std::vector<int> CoordinatedPartitionState<T>::getMachines_load()
		{
			std::vector<int> result;
			for (int i = 0; i < machines_load_edges.size(); i++)
			{
				result.push_back(machines_load_edges[i]);
			}
			return result;
		}
		template<typename T>
		int CoordinatedPartitionState<T>::getTotalReplicas()
		{
			//TODO
			int result = 0;
			auto record_map_it = record_map.begin();
			for (record_map_it; record_map_it != record_map.end(); ++record_map_it)
			{
				int r = record_map_it->second.getReplicas();
				if (r > 0)
				{
					result += r;
				}
				else
				{
					result++;
				}
			}
			return result;
		}
		template<typename T>
		int CoordinatedPartitionState<T>::getNumVertices()
		{
			return record_map.size();
		}
		template<typename T>
		std::set<int> CoordinatedPartitionState<T>::getVertexIds()
		{
			//if (GLOBALS.OUTPUT_FILE_NAME!=null){ out.close(); }
			std::set<int> result;
			auto record_map_it = record_map.begin();
			for (record_map_it; record_map_it != record_map.end(); ++record_map_it)
			{
				result.insert(record_map_it->first);
			}
			return result;
		}
		template<typename T>
		void CoordinatedPartitionState<T>::incrementMachineLoadVertices(int m)
		{
			machines_load_vertices[m] = machines_load_vertices[m] + 1;
		}
		template<typename T>
		std::vector<int> CoordinatedPartitionState<T>::getMachines_loadVertices()
		{
			std::vector<int> result;
			for (int i = 0; i < machines_load_vertices.size(); i++)
			{
				result.push_back(machines_load_vertices.at(i));
			}
			return result;
		}
		template<typename T>
		class PartitionerThread
		{
		private:
			std::vector<Edge<T>> list;
			PartitionState<T> *state;
			PartitionStrategy<T> algorithm;

		public:
			PartitionerThread(std::vector<Edge<T>> &list, PartitionState<T> *state, PartitionStrategy<T> &algorithm, std::list<int> *ids);
			~PartitionerThread();

			void run();

			std::list<int> *id_partitions;
		};
		template<typename T>
		PartitionerThread<T>::PartitionerThread(std::vector<Edge<T>> &list, PartitionState<T> *state, PartitionStrategy<T> &algorithm, std::list<int> *ids)
		{
			this->list = list;
			this->state = state;
			this->algorithm = algorithm;
			this->id_partitions = ids;
		}
		template<typename T>
		PartitionerThread<T>::~PartitionerThread()
		{
		}
		template<typename T>
		void PartitionerThread<T>::run()
		{
			auto edge_it = list.begin();
			for (edge_it; edge_it != list.end(); ++edge_it)
			{
				algorithm.performStep(*edge_it, *state);
			}
		}

		template<typename T>
		class HDRF : public PartitionStrategy<T>
		{
		private:
			Globals GLOBALS;

		public:
			HDRF(Globals G);
			~HDRF();

			void performStep(Edge<T> &e, PartitionState<T> &Sstate);
		};
		template<typename T>
		HDRF<T>::HDRF(Globals G)
		{
			this->GLOBALS = G;
		}
		template<typename T>
		HDRF<T>::~HDRF()
		{
		}
		template<typename T>
		void HDRF<T>::performStep(Edge<T> &e, PartitionState<T> &state)
		{

			int P = GLOBALS.numberOfPartition;
			int epsilon = 1;
			int u = e.getU();
			int v = e.getV();

			Record u_record = state.getRecord(u);
			Record v_record = state.getRecord(v);

			//*** ASK FOR LOCK
			int sleep_time = 2;
			while (!u_record.getLock())
			{
				sleep(sleep_time);
				sleep = (int)pow(sleep_time, 2);
			}
			sleep_time = 2;
			while (!v_record.getLock())
			{
				sleep(sleep_time);
				sleep = (int)pow(sleep_time, 2);
				if (sleep_time > GLOBALS.SLEEP_LIMIT)
				{
					u_record.releaseLock();
					performStep(e, state);
					return;
				} //TO AVOID DEADLOCK
			}
			//*** LOCK TAKEN

			int machine_id = -1;

			//*** COMPUTE MAX AND MIN LOAD
			int MIN_LOAD = state.getMinLoad();
			int MAX_LOAD = state.getMaxLoad();

			//*** COMPUTE SCORES, FIND MIN SCORE, AND COMPUTE CANDIDATES PARITIONS
			std::list<int> candidates;
			double MAX_SCORE = 0.0;

			for (int m = 0; m < P; m++)
			{

				int degree_u = u_record.getDegree() + 1;
				int degree_v = v_record.getDegree() + 1;
				int SUM = degree_u + degree_v;
				double fu = 0;
				double fv = 0;
				if (u_record.hasReplicaInPartition(m))
				{
					fu = degree_u;
					fu /= SUM;
					fu = 1 + (1 - fu);
				}
				if (v_record.hasReplicaInPartition(m))
				{
					fv = degree_v;
					fv /= SUM;
					fv = 1 + (1 - fv);
				}
				int load = state.getMachineLoad(m);
				double bal = (MAX_LOAD - load);
				bal /= (epsilon + MAX_LOAD - MIN_LOAD);
				if (bal < 0)
				{
					bal = 0;
				}
				double SCORE_m = fu + fv + GLOBALS.lambda * bal;
				if (SCORE_m < 0)
				{
					std::cout << "ERRORE: SCORE_m<0" << std::endl;
					std::cout << "fu: " << fu << std::endl;
					std::cout << "fv: " << fv << std::endl;
					std::cout << "GLOBALS.LAMBDA: " << GLOBALS.lambda << std::endl;
					std::cout << "bal: " << bal << std::endl;
					exit(-1);
				}
				if (SCORE_m > MAX_SCORE)
				{
					MAX_SCORE = SCORE_m;
					candidates.clear();
					candidates.push_back(m);
				}
				else if (SCORE_m == MAX_SCORE)
				{
					candidates.push_back(m);
				}
			}

			//*** CHECK TO AVOID ERRORS
			if (candidates.empty())
			{
				std::cout << "ERROR: GreedyObjectiveFunction.performStep -> candidates.isEmpty()" << std::endl;
				std::cout << "MAX_SCORE: " << MAX_SCORE << std::endl;
				exit(-1);
			}

			//*** PICK A RANDOM ELEMENT FROM CANDIDATES
			/* initialize random seed: */
			srand(time(NULL));

			int choice = rand() % candidates.size();
			// TODOOOOOOOOO
			//machine_id = candidates.at(choice);

			try
			{
				CoordinatedPartitionState<T> &cord_state = dynamic_cast<CoordinatedPartitionState<T> &>(state);
				//NEW UPDATE RECORDS RULE TO UPFDATE THE SIZE OF THE PARTITIONS EXPRESSED AS THE NUMBER OF VERTICES THEY CONTAINS
				if (!u_record.hasReplicaInPartition(machine_id))
				{
					u_record.addPartition(machine_id);
					cord_state.incrementMachineLoadVertices(machine_id);
				}
				if (!v_record.hasReplicaInPartition(machine_id))
				{
					v_record.addPartition(machine_id);
					cord_state.incrementMachineLoadVertices(machine_id);
				}
			}
			catch (std::bad_cast)
			{
				// use employee's member functions
				//1-UPDATE RECORDS
				if (!u_record.hasReplicaInPartition(machine_id))
				{
					u_record.addPartition(machine_id);
				}
				if (!v_record.hasReplicaInPartition(machine_id))
				{
					v_record.addPartition(machine_id);
				}
			}

			//2-UPDATE EDGES
			state.incrementMachineLoad(machine_id, e);

			//3-UPDATE DEGREES
			u_record.incrementDegree();
			v_record.incrementDegree();

			//*** RELEASE LOCK
			u_record.releaseLock();
			v_record.releaseLock();
		}
	

		template<typename T>
		class Partitioner
		{
		private:
			std::vector<Edge<T>> dataset;
			PartitionStrategy<T> algorithm;
			Globals GLOBALS;

			CoordinatedPartitionState<T> startCoordinated();

		public:
			Partitioner(std::vector<Edge<T>> &dataset, Globals &G);
			~Partitioner();

			CoordinatedPartitionState<T> performCoordinatedPartition();
		};
		template<typename T>
		Partitioner<T>::Partitioner(std::vector<Edge<T>> &dataset, Globals &G)
		{
			this->GLOBALS = G;
			this->dataset = dataset;
			if (GLOBALS.partitionStategy == CXXGRAPH::PartitionAlgorithm::HDRF)
			{
				algorithm = new HDRF<T>(GLOBALS);
			}
		}
		template<typename T>
		CoordinatedPartitionState<T> Partitioner<T>::startCoordinated()
		{
			CoordinatedPartitionState state(GLOBALS);
			int processors = GLOBALS.threads;

			std::thread myThreads[processors];

			int n = dataset.size();
			int subSize = n / processors + 1;
			for (int t = 0; t < processors; t++)
			{
				int iStart = t * subSize;
				int iEnd = std::min((t + 1) * subSize, n);
				if (iEnd >= iStart)
				{
					std::vector<Edge<T>> list(dataset.begin() + iStart, dataset.begin() + iEnd);
					Runnable x = PartitionerThread<T>(list, state, algorithm, new std::list<int>());
					myThreads[t] = std::thread(&Runnable::run, &x);
				}
			}
			for (int t = 0; t < processors; t++)
			{
				myThreads[t].join();
			}
			return state;
		}
		template<typename T>
		Partitioner<T>::~Partitioner()
		{
		}
		template<typename T>
		CoordinatedPartitionState<T> Partitioner<T>::performCoordinatedPartition()
		{
			return startCoordinated();
		}
		
	}
	/////////////////////////

} // namespace CXXGRAPH
#endif // __CXXGRAPH_H__