#include "../../include/core/SlideRepository.hpp"
#include <iostream>

void SlideRepository::addSlide(std::unique_ptr<Slide> slide) {
    slides_.push_back(std::move(slide));
}

Slide* SlideRepository::getSlideById(int id) {
    for (auto& slide : slides_) {
        if (slide->getId() == id) {
            return slide.get();
        }
    }

    return nullptr;
}

void SlideRepository::displayAll() const {
    for (const auto& slide : slides_) {
        std::cout << slide->toString() << "-------------------\n";
    }
}

nlohmann::json SlideRepository::toJson() const {
    nlohmann::json j;
    j["language"] = "C++";
    nlohmann::json slidesJson = nlohmann::json::array();
    for (const auto& slide : slides_) {
        slidesJson.push_back(slide->toJson());
    }
 
    j["slides"] = slidesJson;
    return j;
}

void SlideRepository::clear() {
    slides_.clear();
}

void SlideRepository::moveSlide(std::size_t fromIndex, std::size_t toIndex) {
    if (fromIndex >= slides_.size() || toIndex >= slides_.size()) {
        throw std::out_of_range("Invalid index for moving slide");
    }
 
    auto slide = std::move(slides_[fromIndex]);
    slides_.erase(slides_.begin() + fromIndex);
    slides_.insert(slides_.begin() + toIndex, std::move(slide));
}