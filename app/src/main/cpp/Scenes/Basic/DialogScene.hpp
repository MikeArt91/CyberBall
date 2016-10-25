#ifndef CYBERBALL_DIALOGSCENE_HPP
#define CYBERBALL_DIALOGSCENE_HPP

#include "Core/Engine.hpp"
#include "ui_scene.hpp"
#include "Scenes/WelcomeScene.hpp"

/* Dialog Scene. Shows a message and buttons. When a button is clicked, performs
 * a given action. */
class DialogScene : public UiScene {
protected:
    // text to be shown
    const char *mText;
    const char *mLeftButtonText;
    const char *mRightButtonText;

    // IDs for buttons
    int mLeftButtonId;
    int mRightButtonId;

    // ID for the text box
    int mTextBoxId;

    // action for left button and right button
    int mLeftButtonAction, mRightButtonAction;

    // y position of buttons
    float mButtonY;

    virtual void OnCreateWidgets();
    virtual void RenderBackground();
    virtual void OnButtonClicked(int id);
    virtual bool OnBackKeyPressed();

public:
    // (action) return to main screen
    static const int ACTION_RETURN = 1000;

    // (action) sign in with Google
    static const int ACTION_SIGN_IN = 1001;

    // (action) play without signing in
    static const int ACTION_PLAY_WITHOUT_SIGNIN = 1002;

    // (action) sign out
    static const int ACTION_SIGN_OUT = 1003;

    DialogScene();
    ~DialogScene();
    DialogScene* SetText(const char *text) {
        mText = text;
        return this;
    }

    DialogScene* SetSingleButton(const char *text, int action) {
        mLeftButtonText = text;
        mLeftButtonAction = action;
        mRightButtonText = NULL;
        return this;
    }

private:
    void CreateWidgetsSetText();
    void CreateWidgetsSingleButton();
    void CreateWidgetsTwoButtons();
};

#endif //CYBERBALL_DIALOGSCENE_HPP
