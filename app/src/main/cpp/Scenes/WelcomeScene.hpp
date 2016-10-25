#ifndef CYBERBALL_WELCOMESCENE_HPP
#define CYBERBALL_WELCOMESCENE_HPP

#include "Core/Engine.hpp"
#include "Rendering/OurShader.hpp"
#include "Scenes/Basic/ui_scene.hpp"
#include "Core/Util.hpp"

/* The "welcome scene" (main menu) */
class WelcomeScene : public UiScene {
protected:
    // IDs for our widgets:
    int mPlayButtonId;
    int mAboutButtonId;

    virtual void RenderBackground();
    virtual void OnButtonClicked(int id);

    void UpdateWidgetStates();

public:
    WelcomeScene();
    ~WelcomeScene();

    virtual void OnCreateWidgets();
    virtual void OnStartGraphics();
    virtual void OnKillGraphics();

    virtual void DoFrame();
};

#endif //CYBERBALL_WELCOMESCENE_HPP
