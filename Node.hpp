#pragma once

#include <list>
#include "geom/Coord.hpp"

enum Status { Unvisited, Open, Closed };

class Node : public std::enable_shared_from_this<Node> {
   private:
   public:
	Coord coord;
	Status status = Status::Unvisited;
	std::shared_ptr<Node> parent;
	std::list<std::shared_ptr<Node>> children;

	Node() {}
	//~Node() {}
	Node(Coord coord, std::shared_ptr<Node> parent) {
		this->coord = coord;
		this->parent = parent;
	}

	void removeChild(std::shared_ptr<Node> child) { this->children.remove(child); }

	void printChildren() {
		for (const auto& child : this->children) {
			printf("(%.2f, %.2f) ", child->coord.x, child->coord.y);
		}
		printf("\n");
	}
};
