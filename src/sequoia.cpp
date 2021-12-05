#include "config.hpp"
#include "context.hpp"
#include "mainui.hpp"
#include "AppThreadManager.hpp"

#ifdef SEQUOIA_STATIC_DEBUG
#include "staticdbg.h"
#endif

#define APP_NAME "Sequoia"

int main(int argc, char **argv) {
#ifdef SEQUOIA_STATIC_DEBUG
    signal(SIGSEGV, segfault_handler);
#endif
    Context *appCtx = new Context();

    Configuration& appCfg = loadConfig(appCtx);
    appCtx->isDebug = appCfg.isDebug;
    appCtx->blame = APP_NAME;

    AppThreadManager* threadManager = new AppThreadManager();
    threadManager->spawnUiThread(appCtx);
    threadManager->spawnWorkerThread(appCtx);

    threadManager->finishAll();

    return 0;
}
