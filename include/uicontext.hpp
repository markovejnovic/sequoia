#ifndef _UI_CONTEXT_HPP_
#define _UI_CONTEXT_HPP_

#include "context.hpp"

/**
 * Creates a UI context for use with the UI libraries.
 */
class UIContext : public Context, public ThreadContext {
 public:
    UIContext();
    explicit UIContext(Context* ctx);
};

#endif  // _UI_CONTEXT_HPP_
