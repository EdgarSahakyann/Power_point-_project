#include "../../include/serialization/SvgSerializer.hpp"
#include "../../include/core/Slide.hpp"
#include "../../include/core/SlideFactory.hpp"
#include "../../include/interfaces/ISlideRepository.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>

void SvgSerializer::save(const ISlideRepository& repo, const std::string& filepath) const {
    std::string svgContent = generateSvgContent(repo);
    
    std::ofstream outFile(filepath);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    
    outFile << svgContent;
    outFile.close();
    
    std::cout << "SVG exported successfully to: " << filepath << std::endl;
}

void SvgSerializer::load(ISlideRepository& repo, SlideFactory& factory, const std::string& filepath) {
    throw std::runtime_error("SVG format is export-only and does not support loading");
}

std::string SvgSerializer::generateSvgContent(const ISlideRepository& repo) const {
    const int SLIDE_WIDTH = 960;
    const int SLIDE_HEIGHT = 540;
    const int SLIDES_PER_ROW = 3;
    const int MARGIN = 40;
    const int SPACING = 30;
    
    std::ostringstream svg;
    
    int numSlides = 0;
    for (int i = 0; i < 1000; i++) {
        if (const_cast<ISlideRepository&>(repo).getSlideById(i)) {
            numSlides++;
        } else if (i > 0) {
            break; 
        }
    }
    
    if (numSlides == 0) {
        numSlides = 1;
    }
    
    int numRows = (numSlides + SLIDES_PER_ROW - 1) / SLIDES_PER_ROW;
    int totalWidth = SLIDES_PER_ROW * SLIDE_WIDTH + (SLIDES_PER_ROW + 1) * MARGIN;
    int totalHeight = numRows * SLIDE_HEIGHT + (numRows + 1) * MARGIN;
    
    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << totalWidth 
        << "\" height=\"" << totalHeight << "\" viewBox=\"0 0 " << totalWidth 
        << " " << totalHeight << "\">\n";
    svg << "  <style>\n";
    svg << "    .slide-bg { fill: white; stroke: #333; stroke-width: 2; }\n";
    svg << "    .slide-title { font-size: 18px; font-weight: bold; fill: #333; }\n";
    svg << "    .slide-text { font-size: 14px; fill: #666; }\n";
    svg << "    .shape-text { font-size: 12px; fill: #000; }\n";
    svg << "  </style>\n";
    
    int slideNum = 0;
    for (int i = 0; i < 1000 && slideNum < numSlides; i++) {
        Slide* slide = const_cast<ISlideRepository&>(repo).getSlideById(i);
        if (!slide) continue;
        
        int row = slideNum / SLIDES_PER_ROW;
        int col = slideNum % SLIDES_PER_ROW;
        int x = MARGIN + col * (SLIDE_WIDTH + MARGIN);
        int y = MARGIN + row * (SLIDE_HEIGHT + MARGIN);
        
        svg << "  <g id=\"slide-" << slide->getId() << "\">\n";
        svg << "    <rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << SLIDE_WIDTH 
            << "\" height=\"" << SLIDE_HEIGHT << "\" class=\"slide-bg\"/>\n";
        
        svg << "    <text x=\"" << (x + 15) << "\" y=\"" << (y + 30) 
            << "\" class=\"slide-title\">" << slide->getTitle() << "</text>\n";
        
        svg << "    <text x=\"" << (x + 15) << "\" y=\"" << (y + 55) 
            << "\" class=\"slide-text\">Theme: " << slide->getTheme() << "</text>\n";
        
        const auto& texts = slide->getTexts();
        int textY = y + 85;
        for (size_t j = 0; j < texts.size() && j < 3; j++) {
            svg << textToSvg(texts[j], slideNum, j);
            textY += 25;
        }
        
        const auto& shapes = slide->getShapes();
        int shapeX = x + 20;
        int shapeY = y + 250;
        int shapeSpacing = 120;
        
        for (size_t j = 0; j < shapes.size() && j < 6; j++) {
            if (j > 0 && j % 3 == 0) {
                shapeX = x + 20;
                shapeY += shapeSpacing;
            }
            svg << shapeToSvg(*shapes[j], slideNum, j);
            shapeX += shapeSpacing;
        }
        
        svg << "  </g>\n";
        slideNum++;
    }
    
    svg << "</svg>\n";
    return svg.str();
}

std::string SvgSerializer::shapeToSvg(const IShape& shape, int slideIndex, int shapeIndex) const {
    std::ostringstream svg;
    std::string type = shape.getType();
    float scale = shape.getScale();
    
    int row = slideIndex / 3;
    int col = slideIndex % 3;
    int baseX = 40 + col * 330 + (shapeIndex % 3) * 100;
    int baseY = 40 + row * 570 + 250 + (shapeIndex / 3) * 120;
    
    int size = static_cast<int>(40 * scale);
    
    if (type == "Circle") {
        svg << "    <circle cx=\"" << baseX << "\" cy=\"" << baseY << "\" r=\"" 
            << size << "\" fill=\"#87CEEB\" stroke=\"#4A90E2\" stroke-width=\"2\"/>\n";
        svg << "    <text x=\"" << (baseX - 15) << "\" y=\"" << (baseY + 25) 
            << "\" class=\"shape-text\">Circle</text>\n";
    } else if (type == "Rectangle") {
        svg << "    <rect x=\"" << (baseX - size) << "\" y=\"" << (baseY - size/2) 
            << "\" width=\"" << (size * 2) << "\" height=\"" << size 
            << "\" fill=\"#FFB6C1\" stroke=\"#FF1493\" stroke-width=\"2\"/>\n";
        svg << "    <text x=\"" << (baseX - 25) << "\" y=\"" << (baseY + 25) 
            << "\" class=\"shape-text\">Rect</text>\n";
    } else if (type == "Triangle") {
        int points = baseX;
        int py1 = baseY - size;
        int py2 = baseY + size;
        svg << "    <polygon points=\"" << baseX << "," << py1 << " " 
            << (baseX - size) << "," << py2 << " " 
            << (baseX + size) << "," << py2 
            << "\" fill=\"#90EE90\" stroke=\"#228B22\" stroke-width=\"2\"/>\n";
        svg << "    <text x=\"" << (baseX - 20) << "\" y=\"" << (baseY + 35) 
            << "\" class=\"shape-text\">Tri</text>\n";
    } else if (type == "Ellipse") {
        int rx = static_cast<int>(size * 1.5);
        int ry = size;
        svg << "    <ellipse cx=\"" << baseX << "\" cy=\"" << baseY << "\" rx=\"" 
            << rx << "\" ry=\"" << ry 
            << "\" fill=\"#DDA0DD\" stroke=\"#8B008B\" stroke-width=\"2\"/>\n";
        svg << "    <text x=\"" << (baseX - 20) << "\" y=\"" << (baseY + 25) 
            << "\" class=\"shape-text\">Ell</text>\n";
    }
    
    return svg.str();
}

std::string SvgSerializer::textToSvg(const Text& text, int slideIndex, int textIndex) const {
    std::ostringstream svg;
    
    int row = slideIndex / 3;
    int col = slideIndex % 3;
    int x = 40 + col * 330 + 15;
    int y = 40 + row * 570 + 85 + textIndex * 25;
    
    std::string displayText = text.content;
    if (displayText.length() > 40) {
        displayText = displayText.substr(0, 37) + "...";
    }
    
    svg << "    <text x=\"" << x << "\" y=\"" << y << "\" class=\"shape-text\" fill=\"" 
        << text.color << "\" font-family=\"" << text.font 
        << "\" font-size=\"" << (12 * text.size) << "\">" 
        << displayText << "</text>\n";
    
    return svg.str();
}