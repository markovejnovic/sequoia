// Copyright 2021 Marko Vejnovic

#include <boost/thread.hpp>
#include "AppThreadManager.hpp"
#include "mainui.hpp"
#include "ImageProcessor.hpp"
#include "config.hpp"

AppThreadManager::AppThreadManager() {
    this->stateBuffer = new BlockingSyncBuffer<SharedState>();
}

AppThreadManager::~AppThreadManager() {
    delete this->stateBuffer;
}

boost::thread* AppThreadManager::spawnUiThread(Context* appCtx) {
    this->mainUi = new UI::MainUI(appCtx->blame);
    UIContext *uiCtx = new UIContext(appCtx);
    uiCtx->manager = this;

    if (this->uiThread == nullptr) {
        this->uiThread = new boost::thread(
            std::bind(&UI::MainUI::main, &*this->mainUi, uiCtx));
    }

    return this->uiThread;
}

boost::thread* AppThreadManager::spawnWorkerThread(Context *appCtx) {
    if (this->imageReader == nullptr) {
        WorkingContext *ctx = new WorkingContext(appCtx);
        ctx->manager = this;
        ctx->videoPath = Configuration::getInstance().videoSource;

        this->imageReader = new ImageReader();
        this->workers.push_back(new boost::thread(std::bind(
            &ImageReader::main, &*this->imageReader, ctx)));
        return this->workers[0];
    }

    // TODO: Undefined behavior. Only one thread supported.
    // Fix this ASAP.
}

void AppThreadManager::finishAll() {
    this->uiThread->join();
    for (auto worker : workers) {
        worker->join();
    }
}
