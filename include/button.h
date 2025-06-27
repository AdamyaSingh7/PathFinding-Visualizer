#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(const std::string& label, sf::Vector2f position, sf::Vector2f size, const sf::Font& font);

    void draw(sf::RenderWindow& window) const;
    bool isClicked(const sf::Vector2i& mousePos) const;
    bool isHovered(const sf::Vector2i& mousePos) const;

    void setSelected(bool selected);       // Set selected state
    void updateAppearance();               // Update color based on state

private:
    sf::RectangleShape shape;
    sf::Text text;
    bool isSelected = false;
};
