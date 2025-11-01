#pragma once

#include <string>
#include "../core/Slide.hpp"

class ISlideRepository {
public:
    virtual ~ISlideRepository() = default;
    virtual void addSlide(std::unique_ptr<Slide> slide) = 0;
    virtual Slide* getSlideById(int id) = 0;
    virtual void displayAll() const = 0;
    virtual nlohmann::json toJson() const = 0;
    virtual void clear() = 0;
    virtual void moveSlide(std::size_t fromIndex, std::size_t toIndex) = 0;
};
