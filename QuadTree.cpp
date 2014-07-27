#include "QuadTree.h"
#include "SFML\Graphics\RenderTarget.hpp"
#include "SFML\Graphics\RectangleShape.hpp"
#include <iostream>

int QuadTree::MAX_LEVELS = 3;
int QuadTree::MAX_OBJECTS = 10;

QuadTree::QuadTree(int current_level, sf::FloatRect bounds)
	: level(current_level),
	bounds(bounds),
	nodesSplit(false)
{
}


QuadTree::~QuadTree()
{
}

void QuadTree::clear()
{
	objects.clear();
	if (nodesSplit)
	{
		for (int i = 0; i < 4; i++)
		{
			nodes[i]->clear();
		}
		nodes.clear();
		nodesSplit = false;
	}
}

void QuadTree::insert(Object object)
{
	if (nodesSplit)
	{
		sf::Vector2f x1(object.rect.left, object.rect.top);
		sf::Vector2f x2(object.rect.left + object.rect.width, object.rect.top);
		sf::Vector2f x3(object.rect.left + object.rect.width, object.rect.top + object.rect.height);
		sf::Vector2f x4(object.rect.left, object.rect.top + object.rect.height);

		for (std::size_t i = 0; i < 4; i++)
		{
			if (nodes[i]->pointInside(x1) ||
				nodes[i]->pointInside(x2) ||
				nodes[i]->pointInside(x3) ||
				nodes[i]->pointInside(x4))
			{
				nodes[0]->insert(object);
			}
		}
	}
	objects.push_back(object);
	if ((int)objects.size() > MAX_OBJECTS)
	{
		split();
	}
}

void QuadTree::split()
{
	if (level + 1 > MAX_LEVELS) return;

	float halfWidth = bounds.width / 2.f;
	float halfHeight = bounds.height / 2.f;
	float x = bounds.left;
	float y = bounds.top;
	
	nodesSplit = true;
	nodes.push_back(std::make_shared<QuadTree>(level + 1, sf::FloatRect(x, y, halfWidth, halfHeight)));
	nodes.push_back(std::make_shared<QuadTree>(level + 1, sf::FloatRect(x + halfWidth, y, halfWidth, halfHeight)));
	nodes.push_back(std::make_shared<QuadTree>(level + 1, sf::FloatRect(x + halfWidth, y + halfHeight, halfWidth, halfHeight)));
	nodes.push_back(std::make_shared<QuadTree>(level + 1, sf::FloatRect(x, y + halfHeight, halfWidth, halfHeight)));
	
	do
	{
		Object object = objects.back();
		objects.pop_back();

		sf::Vector2f x1(object.rect.left, object.rect.top);
		sf::Vector2f x2(object.rect.left + object.rect.width, object.rect.top);
		sf::Vector2f x3(object.rect.left + object.rect.width, object.rect.top + object.rect.height);
		sf::Vector2f x4(object.rect.left, object.rect.top + object.rect.height);

		for (std::size_t i = 0; i < 4; i++)
		{
			if (nodes[i]->pointInside(x1) ||
				nodes[i]->pointInside(x2) ||
				nodes[i]->pointInside(x3) ||
				nodes[i]->pointInside(x4))
			{
				nodes[i]->insert(object);
			}
		}

	} while (!objects.empty());
}

bool QuadTree::pointInside(const sf::Vector2f& point) const
{
	if (point.x < bounds.left) return false;
	if (point.y < bounds.top) return false;
	if (point.x >= bounds.left + bounds.width) return false;
	if (point.y >= bounds.top + bounds.height) return false;
	return true;
}

void QuadTree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!nodesSplit)
	{
		sf::RectangleShape shape;
		shape.setPosition(bounds.left, bounds.top);
		shape.setSize(sf::Vector2f(bounds.width, bounds.height));
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color::Red);
		target.draw(shape);

		for (std::size_t i = 0; i < objects.size(); i++)
		{
			sf::RectangleShape object;
			object.setPosition(objects[i].rect.left, objects[i].rect.top);
			object.setSize(sf::Vector2f(objects[i].rect.width, objects[i].rect.height));
			object.setFillColor(sf::Color::Transparent);
			object.setOutlineThickness(2);
			object.setOutlineColor(sf::Color::White);
			target.draw(object);
		}
		return;
	}
	for (std::size_t i = 0; i < 4; i++)
	{
		target.draw(*nodes[i], states);
	}
}
