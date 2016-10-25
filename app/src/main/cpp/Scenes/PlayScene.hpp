#ifndef CYBERBALL_PLAYSCENE_HPP
#define CYBERBALL_PLAYSCENE_HPP

#include "Core/Engine.hpp"
#include "GameObjects/ObstacleManager.hpp"
#include "GameObjects/Obstacle.hpp"
#include "Core/SoundManager.hpp"
#include "Rendering/ShapeRenderer.hpp"
#include "Rendering/TextRenderer.hpp"
#include "Core/Util.hpp"
#include "GameObjects/Ball.hpp"
#include "Scenes/Basic/ui_scene.hpp"

class OurShader;

/* This is the MAIN gameplay scene
 * Showing a ball controlled by user trying to dodge the obstacles */

class PlayScene : public UiScene {
public:
    PlayScene();
    virtual void OnStartGraphics();
    virtual void OnKillGraphics();
    virtual void DoFrame();
    virtual void OnPointerDown(int pointerId, const struct PointerCoords *coords);
    virtual void OnPointerUp(int pointerId, const struct PointerCoords *coords);
    virtual bool OnBackKeyPressed();
    virtual void OnScreenResized(int width, int height);
    virtual void OnKeyDown(int keyCode);
    virtual void OnPause();
    virtual void OnCreateWidgets();

protected:
    void Reset();
    // shaders
    OurShader *mOurShader;
    TrivialShader *mTrivialShader;

    // the wall texture
    Texture *mWallTexture;
    Texture *mBallTexture;
    Texture *mObsTexture;

    // shape and text renderers we use when rendering the HUD
    ShapeRenderer *mShapeRenderer;
    TextRenderer *mTextRenderer;

    // widgets
    int mGameOverTextId;
    int mRestartButtonId;
    int mQuitButtonId;
    int mStatsTableId;

    // Texure Indexes
    static const int BallId = 0;
    static const int ObsId = 1;
    static const int WallId = 2;

    // matrices
    glm::mat4 mViewMat, mProjMat;

    // player's position and direction
    glm::vec3 mCameraPos, mCameraDir;

    // bird position and direction
    glm::vec3 mBallPos;

    // live status
    bool isAlive;
    bool collision;
    bool isRecord;
    bool touchDown; // finger is on thescreen
    bool tonePlayed;

    // We store the score encrypted in mEncryptedScore, but have a
    // fake variable mFakeScore that stores a copy of it. This serves as a honeypot to
    // an attacker who's trying to crack the game using a memory editor.
    unsigned mFakeScore;
    unsigned mEncryptedScore;

    // to initalize ball and fill geometry arrays
    Ball *mBall;

    // vertex buffer and index buffer to render tunnel
    SimpleGeom *mTunnelGeom;

    // vertex buffer to render obstacles
    SimpleGeom *mCubeGeom;

    // vertex buffer and index buffer to render bird
    SimpleGeom *mBallGeom;

    // what is the first tunnel section that we are rendering
    int mFirstSection;
    int mTunColInd;
    int mBallColInd;

    int mNearestObsInd;

    // what is the number of the obstacle that we are rendering
    int mObstacleCounter;

    // how many obstacles have passed succesfully
    int mScore;
    int mBestScore;

    // circular buffer of obstacles (mObstacleCircBuf[mFirstObstacle...])
    static const int MAX_OBS = RENDER_TUNNEL_SECTION_COUNT + 1;
    int mFirstObstacle;
    int mCycleObsCount;
    float mObsThresh;
    Obstacle mObstacleCircBuf[MAX_OBS];

    // obstacle generator
    ObstacleGenerator mObstacleGen;

    // frame clock - it computes the deltas between successive frames so we can
    // update stuff properly
    DeltaClock mFrameClock;

    // sign (string) that we're currently showing (NULL if none)
    const char *mSignText;
    bool mSignExpires; // does the sign expire after a while?
    float mSignTimeLeft; // for how much longer the sign will still be on screen
    float mSignStartTime; // time when sign was shown

    // did we already show the instructions?
    bool mShowedHowto;

    // touch status bools
    bool hasTouched; // only one kick of the ball while POINTER_DOWN
    bool firstTap; // wait for the first tap to hide "TAP!" and turn on gravity

    // camera speed
    float mCameraSpeed;

    // Ball
    // float fallingConst = 65.0f;
    float mBallSpeed;
    float mRotateRatio;
    float mRotateSpeed;

    // timers
    float mClockShowStats;  // when will we show stats?
    float mClockGameExpire; // when will game expire?

    // name of the save file
    char *mSaveFileName;

    char* overStats;

    // get current score
    int GetScore() {
        return (int)(mEncryptedScore ^ 0x600673);
    }

    // set current score
    void SetScore(int s) {
        mFakeScore = (unsigned)s;
        mEncryptedScore = mFakeScore ^ 0x600673;
    }

    // add to current score
    void AddScore(int s) {
        SetScore(GetScore() + s);
    }

    // converts int to char array
    char* ConvInttoChar(int digit);

    void UpdateWidgetStates();

    virtual void OnButtonClicked(int id);

    // generate new obstacles as needed
    void GenObstacles();

    // renders the tunnel walls
    void RenderTunnel();

    // renders the obstacles
    void RenderObstacles();

    // renders the ball
    void RenderBall();

    // renders the HUD (score, signs, etc)
    void RenderHUD();

    // Shift tunnel sections and obstacles if needed
    // (this means discarding objects
    // the player has already past)
    void ShiftIfNeeded();

    // detect if the player hit obstacles
    void DetectCollisions(float previousY);

    // Game Over Widgets
    void ShowGameOverUI();
    bool overUIActive;

    // shows a text sign on the middle of the screen
    void ShowSign(const char* sign, float timeout) {
        mSignTimeLeft = timeout;
        mSignText = sign;
        mSignExpires = true;
        mSignStartTime = Clock();
    }

    Obstacle* GetObstacleAt(int i) {
        return &mObstacleCircBuf[(mFirstObstacle + i) % MAX_OBS];
    }

    // writes to the local save file
    void WriteSaveFile(int level);

    // loads progress from the local save file
    void LoadProgress();

    // saves progress to the local save file
    void SaveProgress();

    static unsigned char* GenerateTexutre(int TexId);

    // update projection matrix
    void UpdateProjectionMatrix();


    static float GetSectionCenterY(int i) {
        return (float)i * TUNNEL_SECTION_LENGTH;
    }

    static float GetSectionEndY(int i) {
        return GetSectionCenterY(i) + 0.5f * TUNNEL_SECTION_LENGTH;
    }

    static float GetSectionStartY(int i) {
        return GetSectionCenterY(i) - 0.5f * TUNNEL_SECTION_LENGTH;

    }

    float GetObsPositions(int i) {

        return mObsThresh + i*(OBS_DISTANCE_BETWEEN);

    }

};

#endif //CYBERBALL_PLAYSCENE_HPP
