#pragma once
#include "SFML\Graphics\Rect.hpp"
#include <vector>
#include <array>
#include <memory>
#include "SFML\Graphics\Drawable.hpp"

struct Object
{
	Object() 
	{
		hasMoved = false;
	}
	sf::FloatRect rect;
	bool hasMoved;
};
class QuadTree: public sf::Drawable
{
	static int MAX_LEVELS;
	static int MAX_OBJECTS;
public:
	QuadTree(int current_level, sf::FloatRect bounds);
	~QuadTree();

	void clear();
	void insert(Object object);
	
private:
	void split();
	bool pointInside(const sf::Vector2f& point) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	bool nodesSplit;
	int level;
	sf::FloatRect bounds;
	std::vector<Object> objects;
	std::vector<std::shared_ptr<QuadTree>> nodes;
};

