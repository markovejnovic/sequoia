// Copyright 2021 Marko Vejnovic

#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

#include <string>
#include "./introspection.h"

#define CTX_BLAME_ME(ctx) ((ctx)->blame = std::string(__FUNCTION_NAME__));

class Context {
 public:
    Context();
    explicit Context(Context*);

    bool isDebug = true;
    std::string blame;
};

class AppThreadManager;
class ThreadContext {
 public:
    AppThreadManager *manager;
};

class WorkingContext: public Context, public ThreadContext {
 public:
    WorkingContext();
    explicit WorkingContext(Context*);
    std::string videoPath;
};

#endif  // _CONTEXT_HPP_
