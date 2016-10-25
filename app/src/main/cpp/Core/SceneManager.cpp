#include "Core/Log.hpp"
#include "Scenes/Basic/Scene.hpp"
#include "SceneManager.hpp"

static SceneManager _sceneManager;

SceneManager::SceneManager() {
    mCurScene = NULL;

    // start with non-bogus (though not accurate) values
    mScreenWidth = 320;
    mScreenHeight = 240;

    mSceneToInstall = NULL;

    mHasGraphics = false;
}

void SceneManager::RequestNewScene(Scene *newScene) {
    LOGD("SceneManager: requesting new scene %p", newScene);
    mSceneToInstall = newScene;
}

void SceneManager::InstallScene(Scene *newScene) {
    LOGD("SceneManager: installing scene %p.", newScene);

    // kill graphics, if we have them.
    bool hadGraphics = mHasGraphics;
    if (mHasGraphics) {
        KillGraphics();
    }

    // If we have an existing scene, uninstall it.
    if (mCurScene) {
        mCurScene->OnUninstall();
        delete mCurScene;
        mCurScene = NULL;
    }

    // install the new scene
    mCurScene = newScene;
    if (mCurScene) {
        mCurScene->OnInstall();
    }

    // if we had graphics before, start them again.
    if (hadGraphics) {
        StartGraphics();
    }
}

Scene* SceneManager::GetScene() {
    return mCurScene;
}

void SceneManager::DoFrame() {
    if (mSceneToInstall) {
        InstallScene(mSceneToInstall);
        mSceneToInstall = NULL;
    }

    if (mHasGraphics && mCurScene) {
        mCurScene->DoFrame();
    }
}

void SceneManager::KillGraphics() {
    if (mHasGraphics) {
        LOGD("SceneManager: killing graphics.");
        mHasGraphics = false;
        if (mCurScene) {
            mCurScene->OnKillGraphics();
        }
    }
}

void SceneManager::StartGraphics() {
    if (!mHasGraphics) {
        LOGD("SceneManager: starting graphics.");
        mHasGraphics = true;
        if (mCurScene) {
            LOGD("SceneManager: calling mCurScene->OnStartGraphics.");
            mCurScene->OnStartGraphics();
        }
    }
}


void SceneManager::SetScreenSize(int width, int height) {
    if (mScreenWidth != width || mScreenHeight != height) {
        mScreenWidth = width;
        mScreenHeight = height;

        if (mCurScene && mHasGraphics) {
            mCurScene->OnScreenResized(width, height);
        }
    }
}

SceneManager* SceneManager::GetInstance() {
    return &_sceneManager;
}

void SceneManager::OnPointerDown(int pointerId, const struct PointerCoords *coords) {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnPointerDown(pointerId, coords);
    }
}

void SceneManager::OnPointerUp(int pointerId, const struct PointerCoords *coords) {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnPointerUp(pointerId, coords);
    }
}

void SceneManager::OnPointerMove(int pointerId, const struct PointerCoords *coords) {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnPointerMove(pointerId, coords);
    }
}

bool SceneManager::OnBackKeyPressed() {
    if (mHasGraphics && mCurScene) {
        return mCurScene->OnBackKeyPressed();
    }
    return false;
}

void SceneManager::OnKeyDown(int ourKeycode) {
    MY_ASSERT(ourKeycode >= 0 && ourKeycode < OURKEY_COUNT);
    if (mHasGraphics && mCurScene) {
        mCurScene->OnKeyDown(ourKeycode);

        // if our "escape" key (normally corresponding to joystick button B or Y)
        // was pressed, handle it as a back key
        if (ourKeycode == OURKEY_ESCAPE) {
            mCurScene->OnBackKeyPressed();
        }
    }
}

void SceneManager::OnKeyUp(int ourKeycode) {
    MY_ASSERT(ourKeycode >= 0 && ourKeycode < OURKEY_COUNT);
    if (mHasGraphics && mCurScene) {
        mCurScene->OnKeyUp(ourKeycode);
    }
}

void SceneManager::UpdateJoy(float joyX, float joyY) {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnJoy(joyX, joyY);
    }
}

void SceneManager::OnPause() {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnPause();
    }
}

void SceneManager::OnResume() {
    if (mHasGraphics && mCurScene) {
        mCurScene->OnResume();
    }
}