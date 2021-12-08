#ifndef _SEQUOIA_UNITS_HPP_
#define _SEQUOIA_UNITS_HPP_

#include <cmath>

namespace Sequoia::Units {
    float in2f(float in) { return in / 12.0; }
    float f2in(float f) { return f * 12.0; }

    /**
     * Returns the surface measure of a given width and length, width in inches,
     * length in feet. This is the NHLA standard definition.
     */
    int inf2sm(float w, float l) {
        return static_cast<int>(std::round(w * l / 12.0));
    }

    /**
     * Returns the sufrace measure of a given width and length, both in feet
     */
    int ff2sm(float w, float l) { return inf2sm(f2in(w), l); }

    /**
     * Returns the surface measure given inches.
     * @see inf2sm
     */
    int inin2sm(float w, float l) { return inf2sm(w, in2f(l)); }
} // namespace Sequoia::Units

#endif  //  _SEQUOIA_UNITS_HPP_
