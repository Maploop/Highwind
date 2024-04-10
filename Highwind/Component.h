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
	virtual void setPosition(glm::vec3 pos) { }
	virtual glm::vec3 getPosition() { return glm::vec3(0); }
	virtual void componentRender(Shader* shader) { }
	virtual void onChildAdded(Component* child) { }

	void addChild(Component* c)
	{
		children.push_back(c);
	}

	std::vector<Component*> getChildren()
	{
		return children;
	}
	
	bool hasChildren()
	{
		return children.size() > 0;
	}

	void renderAllChildren(Shader* shader)
	{
		for (auto& child : children)
		{
			child->componentRender(shader);
		}
	}
};