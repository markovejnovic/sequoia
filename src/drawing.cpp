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
