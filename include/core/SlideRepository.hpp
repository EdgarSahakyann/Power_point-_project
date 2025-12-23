#pragma once

#include "../interfaces/ISlideRepository.hpp"
#include <vector>
#include <memory>

class SlideRepository : public ISlideRepository {
public:
    void addSlide(std::unique_ptr<Slide> slide) override;
    Slide* getSlideById(int id) override;
    void displayAll() const override;
    nlohmann::json toJson() const override;
    void clear() override;
    void moveSlide(std::size_t fromIndex, std::size_t toIndex) override;
    void removeSlideById(int id) override;
    std::vector<Slide*> getAllSlides() override;

private:
    std::vector<std::unique_ptr<Slide>> slides_;
};

