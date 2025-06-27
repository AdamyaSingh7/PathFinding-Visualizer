#include "button.h"

// Constructor: initialize text first, then shape defaults
Button::Button(const std::string& label, sf::Vector2f position, sf::Vector2f size, const sf::Font& font)
: text(font, label, 16)
, isSelected(false)
{
    // Configure shape
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color(180, 180, 180)); // default color
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.f);

    // Configure text color
    text.setFillColor(sf::Color::Black);

    // Center text inside the button
    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f textPos(
        position.x + (size.x - bounds.size.x) / 2.f,
        position.y + (size.y - bounds.size.y) / 2.f
    );
    text.setPosition(textPos);
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
}

bool Button::isHovered(const sf::Vector2i& mousePos) const {
    return shape.getGlobalBounds().contains(sf::Vector2f(mousePos));
}

bool Button::isClicked(const sf::Vector2i& mousePos) const {
    return isHovered(mousePos);
}

void Button::setSelected(bool selected) {
    isSelected = selected;
    updateAppearance();
}

void Button::updateAppearance() {
    shape.setFillColor(isSelected ? sf::Color(100, 200, 250) : sf::Color(180, 180, 180));
}