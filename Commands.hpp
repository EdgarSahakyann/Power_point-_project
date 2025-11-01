#pragma once

#include "../interfaces/ICommand.hpp"
#include "../core/Slide.hpp"
#include "../core/SlideRepository.hpp"
#include "../serialization/JsonSerializer.hpp"
#include "../core/SlideFactory.hpp"

class CreateSlideCommand : public ICommand {
public:
    CreateSlideCommand(SlideFactory& factory, ISlideRepository& repo, const std::string& title,
                      const std::string& content, const std::string& theme);
    void execute() override;

private:
    SlideFactory& factory_;
    ISlideRepository& repo_;
    std::string title_;
    std::string content_;
    std::string theme_;
};

class AddTextCommand : public ICommand {
public:
    AddTextCommand(ISlideRepository& repo, int slideId, const std::string& textContent, float size,
                   const std::string& font, const std::string& color, float lineWidth);
    void execute() override;

private:
    ISlideRepository& repo_;
    int slideId_;
    std::string textContent_;
    float size_;
    std::string font_;
    std::string color_;
    float lineWidth_;
};

class AddShapeCommand : public ICommand {
public:
    AddShapeCommand(ISlideRepository& repo, unsigned slideId, const std::string& shapeType, float scale);
    void execute() override;

private:
    ISlideRepository& repo_;
    int slideId_;
    std::string shapeType_;
    float scale_;
};

class MoveSlideCommand : public ICommand {
public:
    MoveSlideCommand(ISlideRepository& repo, std::size_t fromIndex, std::size_t toIndex);
    void execute() override;

private:
    ISlideRepository& repo_;
    std::size_t fromIndex_;
    std::size_t toIndex_;
};

class RemoveTextCommand : public ICommand {
public:
    RemoveTextCommand(ISlideRepository& repo, int slideId, std::size_t index);
    void execute() override;

private:
    ISlideRepository& repo_;
    int slideId_;
    std::size_t index_;
};

class RemoveShapeCommand : public ICommand {
public:
    RemoveShapeCommand(ISlideRepository& repo, int slideId, std::size_t index);
    void execute() override;

private:
    ISlideRepository& repo_;
    int slideId_;
    std::size_t index_;
};

class SaveCommand : public ICommand {
public:
    SaveCommand(JsonSerializer& serializer, ISlideRepository& repo, const std::string& filepath);
    void execute() override;

private:
    JsonSerializer& serializer_;
    ISlideRepository& repo_;
    std::string filepath_;
};

class LoadCommand : public ICommand {
public:
    LoadCommand(JsonSerializer& serializer, ISlideRepository& repo, SlideFactory& factory, const std::string& filepath);
    void execute() override;

private:
    JsonSerializer& serializer_;
    ISlideRepository& repo_;
    SlideFactory& factory_;
    std::string filepath_;
};

class DisplayCommand : public ICommand {
public:
    explicit DisplayCommand(ISlideRepository& repo);
    void execute() override;

private:
    ISlideRepository& repo_;
};
