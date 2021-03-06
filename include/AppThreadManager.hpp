#ifndef _APP_THREAD_MANAGER_HPP_
#define _APP_THREAD_MANAGER_HPP_

#include <vector>
#include <boost/thread.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include "context.hpp"
#include "mainui.hpp"
#include "ImageReader.hpp"
#include "BlockingSyncBuffer.hpp"

struct SharedState {
    cv::Mat* outImage = nullptr;
    float fps;
    float width;
    float height;
    int sm;
};

class AppThreadManager {
 public:
    AppThreadManager();
    ~AppThreadManager();

    /**
     * Spawns a UI thread. If one already exists, this function simply returns a
     * reference to it. Generates a Context automatically for it.
     */
    boost::thread* spawnUiThread(Context *appCtx);

    /**
     * Spawns a worker thread.
     */
    boost::thread* spawnWorkerThread(Context *appCtx);

    void finishAll();

    void updateFps(float fps) {
        std::function<void(SharedState* s)> updater = FpsUpdater(fps);
        this->stateBuffer->apply(updater);
    }

    float getFps() {
        SharedState* state = this->stateBuffer->readData();
        float fps = state->fps;
        delete state;
        return fps;
    }

    void updateMetrics(float width, float height, int sm) {
        std::function<void(SharedState* s)> updater =
            MetricUpdater(width, height, sm);
        this->stateBuffer->apply(updater);
    }

    float getWidth() {
        SharedState* state = this->stateBuffer->readData();
        float width = state->width;
        delete state;
        return width;
    }

    float getHeight() {
        SharedState* state = this->stateBuffer->readData();
        float height = state->height;
        delete state;
        return height;
    }

    float getSm() {
        SharedState* state = this->stateBuffer->readData();
        float sm = state->sm;
        delete state;
        return sm;
    }

    void setOutputData(cv::Mat* data) {
        std::function<void(SharedState* s)> updater = ImageBufferUpdater(data);
        this->stateBuffer->apply(updater);
    }

    cv::Mat* getOutputData(void) {
        return this->stateBuffer->readData()->outImage;
    }

    void freeData(void* target) {
        this->stateBuffer->freeData(target);
    }

 protected:
    class FpsUpdater {
     private:
        float fps;
     public:
        explicit FpsUpdater(float fps) { this->fps = fps; }
        void operator()(SharedState *s) const { s->fps = this->fps; }
    };

    class MetricUpdater {
     private:
        int sm;
        float width;
        float height;
     public:
        explicit MetricUpdater(float w, float h, int sm) :
            width(w), height(h), sm(sm) {}
        void operator()(SharedState *s) const {
            s->width = this->width;
            s->height = this->height;
            s->sm = this->sm;
        }
    };

    class ImageBufferUpdater {
     private:
         cv::Mat* data;
     public:
        explicit ImageBufferUpdater(cv::Mat* data) { this->data = data; }
        void operator()(SharedState *s) const { s->outImage = this->data; }
    };


 private:
    UI::MainUI* mainUi = nullptr;
    ImageReader* imageReader = nullptr;
    boost::thread* uiThread = nullptr;
    std::vector<boost::thread*> workers;

    BlockingSyncBuffer<SharedState> *stateBuffer;
};

#endif // _APP_THREAD_MANAGER_HPP_
