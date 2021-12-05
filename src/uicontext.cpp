#include "uicontext.hpp"

UIContext::UIContext() {}

UIContext::UIContext(Context* ctx) {
    this->isDebug = ctx->isDebug;
}
