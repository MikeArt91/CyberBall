#include "Core/BackAnimation.hpp"
#include "Scenes/Basic/DialogScene.hpp"
#include "PlayScene.hpp"
#include "WelcomeScene.hpp"

#include "Data/UI_Strings.hpp"
#include "Data/UI_Params.hpp"

WelcomeScene::WelcomeScene() {
}

WelcomeScene::~WelcomeScene() {
}

void WelcomeScene::RenderBackground() {
    RenderBackgroundAnimation(mShapeRenderer);
}

void WelcomeScene::OnButtonClicked(int id) {
    SceneManager *mgr = SceneManager::GetInstance();

    if (id == mPlayButtonId) {
        mgr->RequestNewScene(new PlayScene());
    } else if (id == mAboutButtonId) {
        mgr->RequestNewScene((new DialogScene())->SetText(TEXT_ABOUT)->SetSingleButton(S_OK,
                                                                                       DialogScene::ACTION_RETURN));
    }
}

void WelcomeScene::DoFrame() {
    // update widget states based on signed-in status
    UpdateWidgetStates();

    // draw the UI
    UiScene::DoFrame();
}

void WelcomeScene::UpdateWidgetStates() {
    // Build navigation
    AddNav(mPlayButtonId, UI_DIR_DOWN, mAboutButtonId);

    AddNav(mAboutButtonId, UI_DIR_UP, mPlayButtonId);

}

void WelcomeScene::OnStartGraphics() {
    UiScene::OnStartGraphics();
}

void WelcomeScene::OnCreateWidgets() {

    // create widgets
    float maxX = SceneManager::GetInstance()->GetScreenAspect();
    float center = 0.5f * maxX;

    // create a "shadow" around title
    NewWidget()->SetText(S_TITLE)->SetCenter(TITLE_POS)->SetTextColor(TITLE_SHADOW)
            ->SetFontScale(TITLE_FONT_SCALE*1.03f)->SetTransition(UiWidget::TRANS_FROM_TOP);

    // create the static title
    NewWidget()->SetText(S_TITLE)->SetCenter(TITLE_POS)->SetTextColor(TITLE_COLOR)
            ->SetFontScale(TITLE_FONT_SCALE)->SetTransition(UiWidget::TRANS_FROM_TOP);

    // create the "play" button
    mPlayButtonId = NewWidget()->SetText(S_PLAY)->SetTextColor(BUTTON_TEXT_COLOR)->SetBackColor(BUTTON_BACK)
            ->SetCenter(BUTTON_PLAY_POS)->SetSize(BUTTON_PLAY_SIZE)
            ->SetFontScale(BUTTON_PLAY_FONT_SCALE)->SetIsButton(true)
            ->SetTransition(UiWidget::TRANS_SCALE)->GetId();

    // about button
    mAboutButtonId = NewWidget()->SetTextColor(BUTTON_TEXT_COLOR)->SetText(S_ABOUT)->SetBackColor(BUTTON_BACK_ADD)
            ->SetCenter(BUTTON_ABOUT_POS)->SetSize(BUTTON_SIDEBUTTON_SIZE)
            ->SetFontScale(BUTTON_FONT_SCALE)->SetIsButton(true)
            ->SetTransition(UiWidget::TRANS_FROM_RIGHT)->GetId();

    // "Play" button is the default button
    SetDefaultButton(mPlayButtonId);

    // enable/disable widgets as appropriate to signed in state
    UpdateWidgetStates();
}

void WelcomeScene::OnKillGraphics() {
    UiScene::OnKillGraphics();
}
