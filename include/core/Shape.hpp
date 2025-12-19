#pragma once

#include <string>
#include <memory>
#include "../interfaces/IShape.hpp"

class Triangle : public IShape {
public:
    explicit Triangle(float scale = 1.0f);
    std::string getType() const override;
    float getScale() const override;
    void setScale(float scale) override;
    nlohmann::json toJson() const override;
    std::string toString() const override;

private:
    float scale_;
};

class Circle : public IShape {
public:
    explicit Circle(float scale = 1.0f);
    std::string getType() const override;
    float getScale() const override;
    void setScale(float scale) override;
    nlohmann::json toJson() const override;
    std::string toString() const override;

private:
    float scale_;
};

class Rectangle : public IShape {
public:
    explicit Rectangle(float scale = 1.0f);
    std::string getType() const override;
    float getScale() const override;
    void setScale(float scale) override;
    nlohmann::json toJson() const override;
    std::string toString() const override;

private:
    float scale_;
};

class Ellipse : public IShape {
public:
    explicit Ellipse(float scale = 1.0f);
    std::string getType() const override;
    float getScale() const override;
    void setScale(float scale) override;
    nlohmann::json toJson() const override;
    std::string toString() const override;
    
private:
    float scale_;
};

class ShapeFactory {
public:
    static std::unique_ptr<IShape> createShape(const std::string& type, float scale = 1.0f);
    static std::unique_ptr<IShape> fromJson(const nlohmann::json& j);
};