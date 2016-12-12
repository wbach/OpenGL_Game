#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

namespace Utils
{
	static void AddVectorToNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const glm::vec3 vector)
	{
		rapidxml::xml_node<>* x = document.allocate_node(rapidxml::node_element, "x", document.allocate_string(std::to_string(vector.x).c_str()));
		rapidxml::xml_node<>* y = document.allocate_node(rapidxml::node_element, "y", document.allocate_string((std::to_string(vector.y).c_str())));
		rapidxml::xml_node<>* z = document.allocate_node(rapidxml::node_element, "z", document.allocate_string((std::to_string(vector.z).c_str())));
		node->append_node(x);
		node->append_node(y);
		node->append_node(z);
	}
	static void AddVectorToNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const glm::vec2 vector)
	{
		rapidxml::xml_node<>* x = document.allocate_node(rapidxml::node_element, "x", document.allocate_string(std::to_string(vector.x).c_str()));
		rapidxml::xml_node<>* y = document.allocate_node(rapidxml::node_element, "y", document.allocate_string((std::to_string(vector.y).c_str())));
		node->append_node(x);
		node->append_node(y);
	}
}