#pragma once

#include "../interfaces/ISerializer.hpp"
#include <string>

class SvgSerializer : public ISerializer {
public:
    void save(const ISlideRepository& repo, const std::string& filepath) const override;
    void load(ISlideRepository& repo, SlideFactory& factory, const std::string& filepath) override;

private:
    std::string generateSvgContent(const ISlideRepository& repo) const;
    std::string shapeToSvg(const IShape& shape, int slideIndex, int shapeIndex) const;
    std::string textToSvg(const Text& text, int slideIndex, int textIndex) const;
};
