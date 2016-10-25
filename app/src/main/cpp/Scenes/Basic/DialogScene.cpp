#include "Core/BackAnimation.hpp"
#include "DialogScene.hpp"
#include "Scenes/PlayScene.hpp"
#include "Data/UI_Params.hpp"

#define UIBUTTON_FONT_SCALE 0.6f
#define TEXT_FONT_SCALE 0.4f
#define BUTTON_MARGIN 0.2f

#define BUTTON_WIDTH 0.3f
#define BUTTON_HEIGHT 0.1f
#define UIBUTTON_SIZE BUTTON_WIDTH, BUTTON_HEIGHT
#define BUTTON_DISCOURAGE_COLOR 0.0f, 0.4f, 0.0f
#define LEFT_X center * 2 * 0.33f
#define RIGHT_X center * 2 * 0.67f
#define TEXT_Y 0.6f

DialogScene::DialogScene() {
    mLeftButtonId = mRightButtonId = -1;
    mTextBoxId = -1;
    mLeftButtonAction = mRightButtonAction = ACTION_RETURN;
    mButtonY = 0.5f;
    mText = mLeftButtonText = mRightButtonText = NULL;
}

DialogScene::~DialogScene() {
}

void DialogScene::CreateWidgetsSetText() {
    const char *text = mText;
    float center = 0.5f*SceneManager::GetInstance()->GetScreenAspect();

    if (mTextBoxId < 0) {
        mTextBoxId = NewWidget()->SetTransition(UiWidget::TRANS_FROM_TOP)->GetId();
    }

    GetWidgetById(mTextBoxId)->SetText(text)->SetFontScale(TEXT_FONT_SCALE)
            ->SetCenter(center, TEXT_Y);

    float height = TextRenderer::MeasureTextHeight(text, TEXT_FONT_SCALE);
    mButtonY = TEXT_Y - height * 0.5f - BUTTON_MARGIN - 0.5f * BUTTON_HEIGHT;
}

void DialogScene::CreateWidgetsSingleButton() {
    const char *text = mLeftButtonText;
    int action = mLeftButtonAction;
    float center = 0.5f * SceneManager::GetInstance()->GetScreenAspect();
    mLeftButtonId = NewWidget()->SetText(text)->SetTextColor(BUTTON_TEXT_COLOR)->SetBackColor(BUTTON_BACK)
            ->SetIsButton(true)->SetCenter(center, mButtonY)
            ->SetSize(UIBUTTON_SIZE)->SetTextColor(BUTTON_TEXT_COLOR)->SetFontScale(UIBUTTON_FONT_SCALE)
            ->SetTransition(UiWidget::TRANS_FROM_BOTTOM)->GetId();
    mLeftButtonAction = action;

    SetDefaultButton(mLeftButtonId);
}

void DialogScene::CreateWidgetsTwoButtons() {
    const char *leftText = mLeftButtonText;
    int leftAction = mLeftButtonAction;
    const char *rightText = mRightButtonText;
    int rightAction = mRightButtonAction;

    float center = 0.5f * SceneManager::GetInstance()->GetScreenAspect();
    mLeftButtonId = NewWidget()->SetText(leftText)->SetIsButton(true)->SetCenter(LEFT_X, mButtonY)
            ->SetTextColor(BUTTON_TEXT_COLOR)->SetBackColor(BUTTON_BACK)
            ->SetSize(UIBUTTON_SIZE)->SetTextColor(BUTTON_TEXT_COLOR)
            ->SetTransition(UiWidget::TRANS_FROM_LEFT)->GetId();
    mLeftButtonAction = leftAction;
    mRightButtonId = NewWidget()->SetText(rightText)->SetIsButton(true)
            ->SetTextColor(BUTTON_TEXT_COLOR)->SetBackColor(BUTTON_BACK)
            ->SetCenter(RIGHT_X, mButtonY)->SetSize(BUTTON_SIZE)
            ->SetTransition(UiWidget::TRANS_FROM_RIGHT)
            ->SetTextColor(BUTTON_TEXT_COLOR)->GetId();
    mRightButtonAction = rightAction;

    if (leftAction == ACTION_SIGN_OUT || leftAction == ACTION_PLAY_WITHOUT_SIGNIN) {
        GetWidgetById(mLeftButtonId)->SetTextColor(BUTTON_DISCOURAGE_COLOR);
    }
    if (rightAction == ACTION_SIGN_OUT || rightAction == ACTION_PLAY_WITHOUT_SIGNIN) {
        GetWidgetById(mRightButtonId)->SetTextColor(BUTTON_DISCOURAGE_COLOR);
    }

    // set up navigation between the buttons
    AddNav(mLeftButtonId, UI_DIR_DOWN, mRightButtonId);
    AddNav(mRightButtonId, UI_DIR_UP, mLeftButtonId);
}

void DialogScene::OnCreateWidgets() {
    if (mRightButtonText) {
        CreateWidgetsSetText();
        CreateWidgetsTwoButtons();
    } else {
        CreateWidgetsSetText();
        CreateWidgetsSingleButton();
    }
}

void DialogScene::RenderBackground() {
    RenderBackgroundAnimation(mShapeRenderer);
}

bool DialogScene::OnBackKeyPressed() {
    SceneManager *mgr = SceneManager::GetInstance();
    mgr->RequestNewScene(new WelcomeScene());
    return true;
}

void DialogScene::OnButtonClicked(int id) {
    SceneManager *mgr = SceneManager::GetInstance();

    int action;
    if (id == mLeftButtonId) {
        action = mLeftButtonAction;
    } else if (id == mRightButtonId) {
        action = mRightButtonAction;
    } else {
        return;
    }

    switch (action) {
        case ACTION_RETURN:
            mgr->RequestNewScene(new WelcomeScene());
            break;
        case ACTION_SIGN_IN:
            // note: we can't start playing directly because PlayScene expects the cloud
            // results to be ready when it constructs itself; therefore, WelcomeScene
            // has to make sure of that. So we can't jump directly to PlayScene from here.
            mgr->RequestNewScene(new WelcomeScene());
            break;
        case ACTION_PLAY_WITHOUT_SIGNIN:
            mgr->RequestNewScene(new PlayScene());
            break;
        case ACTION_SIGN_OUT:
            mgr->RequestNewScene(new WelcomeScene());
            break;
        default:
            // do nothing.
            break;
    }
}