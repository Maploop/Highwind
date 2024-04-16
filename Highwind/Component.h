#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

class Component
{
private:
	std::vector<Component*> children;

public:
	virtual void move(glm::vec3 pos) { }
	virtual void set_position(glm::vec3 pos) { }
	virtual glm::vec3 get_position() { return glm::vec3(0); }
	virtual void component_render(Shader* shader) { }
	virtual void on_child_added(Component* child) { }

	void add_child(Component* c)
	{
		children.push_back(c);
	}

	std::vector<Component*> get_children()
	{
		return children;
	}
	
	bool has_children()
	{
		return children.size() > 0;
	}

	void render_children(Shader* shader)
	{
		for (auto& child : children)
		{
			child->component_render(shader);
		}
	}
};