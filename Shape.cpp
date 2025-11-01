#include "../../include/core/Shape.hpp"
#include <stdexcept>

Triangle::Triangle(float scale) : scale_(scale) {}
std::string Triangle::getType() const { return "Triangle"; }
float Triangle::getScale() const { return scale_; }

void Triangle::setScale(float scale) { scale_ = scale; }

nlohmann::json Triangle::toJson() const {
    nlohmann::json j;
    j["type"] = getType();
    j["scale"] = scale_;
    
    return j;
}

std::string Triangle::toString() const {
    return getType() + " (scale: " + std::to_string(scale_) + ")";
}

Circle::Circle(float scale) : scale_(scale) {}
std::string Circle::getType() const { return "Circle"; }
float Circle::getScale() const { return scale_; }
void Circle::setScale(float scale) { scale_ = scale; }

nlohmann::json Circle::toJson() const {
    nlohmann::json j;
    j["type"] = getType();
    j["scale"] = scale_;

    return j;
}

std::string Circle::toString() const {
    return getType() + " (scale: " + std::to_string(scale_) + ")";
}

Rectangle::Rectangle(float scale) : scale_(scale) {}
std::string Rectangle::getType() const { return "Rectangle"; }
float Rectangle::getScale() const { return scale_; }
void Rectangle::setScale(float scale) { scale_ = scale; }

nlohmann::json Rectangle::toJson() const {
    nlohmann::json j;
    j["type"] = getType();
    j["scale"] = scale_;

    return j;
}
std::string Rectangle::toString() const {
    return getType() + " (scale: " + std::to_string(scale_) + ")";
}

Ellipse::Ellipse(float scale) : scale_(scale) {}
std::string Ellipse::getType() const { return "Ellipse"; }
float Ellipse::getScale() const { return scale_; }
void Ellipse::setScale(float scale) { scale_ = scale; }

nlohmann::json Ellipse::toJson() const {
    nlohmann::json j;
    j["type"] = getType();
    j["scale"] = scale_;
    return j;
}

std::string Ellipse::toString() const {
    return getType() + " (scale: " + std::to_string(scale_) + ")";
}

std::unique_ptr<IShape> ShapeFactory::createShape(const std::string& type, float scale) {
    if (type == "Triangle") 
        return std::make_unique<Triangle>(scale);
    if (type == "Circle") 
        return std::make_unique<Circle>(scale);
    if (type == "Rectangle") 
        return std::make_unique<Rectangle>(scale);
    if (type == "Ellipse") 
        return std::make_unique<Ellipse>(scale);
    
    throw std::invalid_argument("Invalid shape type: " + type);
}

std::unique_ptr<IShape> ShapeFactory::fromJson(const nlohmann::json& j) {
    std::string type = j["type"];
    float scale = j["scale"];
    
    return createShape(type, scale);
}