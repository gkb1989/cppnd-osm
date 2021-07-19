#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
  	//RouteModel::Node *start_node = &m_Model.FindClosestNode(start_x, start_y);
    //RouteModel::Node *end_node = &m_Model.FindClosestNode(end_x, end_y);
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    
    current_node->FindNeighbors(); //no need to reassign - method persists to member variable already
    for ( auto node : current_node->neighbors){
        node->parent = current_node;
        node->h_value = CalculateHValue(node);
        node->g_value = current_node->g_value + current_node->distance(*node); 

        
        open_list.push_back(node);
        node->visited = true;

    }

    return;
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

bool Compare(const RouteModel::Node *node1, const RouteModel::Node *node2) {
	float f_val1 = node1->h_value + node1->g_value;
  	float f_val2 = node2->h_value + node2->g_value;
  	return f_val1 < f_val2;
  
} 
  
void RoutePlanner::CellSort(std::vector<RouteModel::Node*> *openlist){
 
  std::sort(openlist->begin(), openlist->end(), Compare);
  return;
  
}

RouteModel::Node *RoutePlanner::NextNode() {
  
  
  RoutePlanner::CellSort(&open_list);
  RouteModel::Node *lowest_fval_node = open_list.front();
  open_list.erase(open_list.begin());
  return lowest_fval_node;	

  
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.

  	while (current_node != start_node) {
    
      path_found.push_back(*current_node);
      distance += current_node->distance(*(current_node->parent));
      current_node = current_node->parent;
    } 
  
  	path_found.push_back(*current_node);

  	std::reverse(path_found.begin(), path_found.end());
  
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
  	current_node = start_node;
  	current_node->visited = true;
  	open_list.push_back(current_node);
  	
  	std::cout << "Set the current node to the start node" << std::endl;
  
  	while (open_list.size() > 0) {
    
      	std::cout << "Entering the while loop" << std::endl;
      	std::cout << "Size of the open list is " << open_list.size() << std::endl;
      
      	if (current_node == end_node) {
          	std::cout << "Found the end node, constructing final path!" << std::endl;
      		auto path = ConstructFinalPath(current_node);
  			m_Model.path = path;
          	break;
        }
          
        std::cout << "Have not got to the end node yet!" << std::endl;
      	current_node = NextNode();
        std::cout << "NEXT NODE -->Size of the open list is " << open_list.size() << std::endl;
  		AddNeighbors(current_node);
        std::cout << "ADD NEIGHBORS -->Size of the open list is " << open_list.size() << std::endl;

			 	
    }
  	

}
