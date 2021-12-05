#include "mainui.hpp"

#include <boost/lexical_cast.hpp>
#include "AppThreadManager.hpp"
#include "ocv2ray.hpp"

using namespace UI;

void MainUI::drawDecisionButtons(UIContext *context) {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 10);
    GuiStatusBar(Rectangle {
            0,
            static_cast<float>(this->screenHeight - DEF_STATUSBAR_HEIGHT),
            static_cast<float>(this->screenWidth),
            DEF_STATUSBAR_HEIGHT,
        }, this->getStatusbarText().c_str());

    float hAvailable = this->screenHeight
        - DEF_STATUSBAR_HEIGHT - 2 * DEF_MARGIN;
    float wAvailable = this->screenWidth - 2 * DEF_MARGIN;

    float buttonW = wAvailable / 2 - DEF_MARGIN;
    float buttonH = hAvailable / 4 - DEF_MARGIN;
    float buttonX = static_cast<float>(this->screenWidth / 2 + DEF_MARGIN);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 52);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(GREEN));
    GuiButton(Rectangle { buttonX, 0 + DEF_MARGIN, buttonW, buttonH },
        "Quality OK");

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(RED));
    GuiButton(Rectangle { buttonX, buttonH + 2 * DEF_MARGIN, buttonW, buttonH },
        "Misclassification");
}

void MainUI::drawImage(UIContext* context) {
    cv::Mat* outputImage = context->manager->getOutputData();
    if (outputImage != nullptr && outputImage->isContinuous()) {
        Image rayImg = ocvMatToRayImage(*outputImage);
        context->manager->freeData(outputImage);
        ImageResize(&rayImg,
                    this->screenWidth / 2 - 2 * DEF_MARGIN,
                    this->screenHeight - 2 * DEF_MARGIN - DEF_STATUSBAR_HEIGHT);
        this->image = LoadTextureFromImage(rayImg);
        free(rayImg.data);
    }

    DrawTexture(this->image, DEF_MARGIN, DEF_MARGIN, WHITE);
}

MainUI::MainUI(std::string windowTitle) {
    this->windowTitle = windowTitle;
}

void MainUI::init(UIContext* ctx) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(this->screenWidth, this->screenHeight, windowTitle.c_str());
    SetTargetFPS(60);
}

void MainUI::handleInternalEvents(UIContext* context) {
    if (IsWindowResized() && !IsWindowFullscreen()) {
        this->screenWidth = GetScreenWidth();
        this->screenHeight = GetScreenHeight();
    }
}

int MainUI::loadData(UIContext* context) {
    this->fps = context->manager->getFps();

    return 0;
}

std::string MainUI::getStatusbarText() {
    return "FPS: " + boost::lexical_cast<std::string>(this->fps);
}

void MainUI::handleEvents(UIContext* context) {
    this->handleInternalEvents(context);

    if (this->loadData(context) != 0) {
        return;
    }
}

void MainUI::main(UIContext *context) {
    this->init(context);

    while (!WindowShouldClose()) {
        this->handleEvents(context);

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        MainUI::loop(context);

        EndDrawing();
    }

    CloseWindow();
    MainUI::loop(context);
}

void MainUI::loop(UIContext *context) {
    this->drawDecisionButtons(context);
    this->drawImage(context);
}
