#pragma once

#include "../interfaces/ISerializer.hpp"

class JsonSerializer : public ISerializer {
public:
    void save(const ISlideRepository& repo, const std::string& filepath) const override;
    void load(ISlideRepository& repo, SlideFactory& factory, const std::string& filepath) override;
};