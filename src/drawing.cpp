#include "sequoiagfx/drawing.hpp"
#include "sequoiagfx/color.hpp"
#include "sequoiagfx/rectangle.hpp"

using Sequoia::Rectangle,
      Sequoia::Color;

/**
 * Draws a rectangle on a CV canvas. Assumes BGR.
 */
void Sequoia::drawRect(cv::Mat& target, Rectangle<int> rect, Color c) {
    cv::rectangle(target, sequoia2cvRect(rect),
            sequoia2cvColor(c, SEQUOIA_BGR));
}

void Sequoia::Image::overlay(std::vector<cv::Mat> inputs,
                             cv::Mat &output, float alpha) {
    output = inputs[0];

    for (size_t i = 1; i < inputs.size(); i++) {
        cv::addWeighted(inputs[i], alpha, output, (1 - alpha), 0, output);
    }
}
