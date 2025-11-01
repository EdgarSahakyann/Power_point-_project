#pragma once

#include <nlohmann/json.hpp>

class IShape {
public:
    virtual ~IShape() = default;
    virtual std::string getType() const = 0;
    virtual float getScale() const = 0;
    virtual void setScale(float scale) = 0;
    virtual nlohmann::json toJson() const = 0;
    virtual std::string toString() const = 0;
};