// Copyright 2021 Marko Vejnovic

#include "context.hpp"

Context::Context() {}

Context::Context(Context* ctx) {
    this->isDebug = ctx->isDebug;
}

WorkingContext::WorkingContext() {}

WorkingContext::WorkingContext(Context* ctx) {
    this->isDebug = ctx->isDebug;
}
