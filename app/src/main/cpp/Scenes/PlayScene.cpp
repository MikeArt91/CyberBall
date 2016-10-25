#include <cstdio>
#include <string>
#include "Core/BackAnimation.hpp"
#include "Rendering/AsciiToGeom.hpp"
#include "Data/GameConsts.hpp"
#include "Rendering/OurShader.hpp"
#include "PlayScene.hpp"
#include "Core/Util.hpp"
#include "WelcomeScene.hpp"

#include "Scenes/Basic/ui_scene.hpp"

#include "Data/Cube.hpp"
#include "Data/UI_Strings.hpp"
#include "Data/Tunnel.hpp"
#include "Data/UI_Params.hpp"

static const float OBS_COLORS[] = COLORS_OBS;
static const float LIGHT_COLORS[] = COLORS_LIGHT;
static const float TUNNEL_COLORS[] = COLORS_TUNNEL;
static const float BALL_COLORS[] = COLORS_BALL;

static void _get_color(int style, float *r, float *g, float *b) {
    style = Clamp(style, 1, 6);
    *r = OBS_COLORS[style * 3];
    *g = OBS_COLORS[style * 3 + 1];
    *b = OBS_COLORS[style * 3 + 2];
}

PlayScene::PlayScene() : UiScene() {

    mOurShader = NULL;
    mTrivialShader = NULL;
    mTextRenderer = NULL;
    mShapeRenderer = NULL;

    mBall = NULL;

    mBallGeom = NULL;
    mCubeGeom = NULL;
    mTunnelGeom = NULL;

    mWallTexture = NULL;
    mBallTexture = NULL;
    mObsTexture = NULL;

    mCameraDir = glm::vec3(-1.0f, 0.0f, 0.0f); // in what direction camera is looking?

    // Resetting initial positions and bools
    Reset();

    // fix camera and ball poistions in current axis
    mCameraPos.x = 6.0f;
    mCameraPos.z = 0.0f;
    mBallPos.x = BALL_POS_X;

    mFrameClock.SetMaxDelta(MAX_DELTA_T);

    // How many obstacles will be in a "trash zone" behind ball?
    float forNearestInd = floorf((SHIFT_THRESH-BALL_CENTER_THRESH)/OBS_DISTANCE_BETWEEN);

    // Index of the obstacle parsed in  DetectCollisions()
    mNearestObsInd = (int) forNearestInd;

    /*
   * where do I put the program???
   */
    const char *savePath = "/data/data/com.mikeart.cyberball/files";
    int len = strlen(savePath) + strlen(SAVE_FILE_NAME) + 3;
    mSaveFileName = new char[len];
    strcpy(mSaveFileName, savePath);
    strcat(mSaveFileName, "/");
    strcat(mSaveFileName, SAVE_FILE_NAME);
    LOGD("Save file name: %s", mSaveFileName);

    LoadProgress();

}

// Resetting initial positions and bools
void PlayScene::Reset() {

    // color of the tunnel
    mTunColInd = 3*Random(3);

    // ball colors
    mBallColInd = 3*Random(2);

    mBallPos.z = 0.0f;
    mRotateSpeed = 4.0f;

    mCycleObsCount = 0;
    mFirstObstacle = 0;
    mObstacleCounter = 0;
    mFirstSection = 0;

    mScore = 0;

    mSignText = NULL;
    mSignTimeLeft = 0.0f;

    isAlive = true;
    collision = false;
    mShowedHowto = false;
    isRecord = false;
    firstTap = false;
    touchDown = false;
    tonePlayed = false;

    overUIActive = false;

    mCameraSpeed = 0.0f;
    mBallSpeed = 0.0f;
    mObsThresh = 0.0f;

    SetScore(0);

    ResetTransitionTimer();

    mFrameClock.Reset();
}

void PlayScene::OnStartGraphics() {
    // build shaders
    mOurShader = new OurShader();
    mOurShader->Compile();
    mTrivialShader = new TrivialShader();
    mTrivialShader->Compile();

    UiScene::OnStartGraphics();

    // build projection matrix
    UpdateProjectionMatrix();

    // initialize Ball
    mBall = new Ball(BALL_RADIUS, BALL_SLICES, BALL_FIRST_COLOR, BALL_SECOND_COLOR);

    // create and fill coordinates array for the ball
    GLfloat BALL_GEOM[mBall->GetGeomSize()];
    GLushort BALL_GEOM_INDICES[mBall->GetIndSize()];

    mBall->GenBallGeom(BALL_GEOM);
    mBall->GenBallInd(BALL_GEOM_INDICES);

    // build tunnel geometry
    mTunnelGeom = new SimpleGeom(
            new VertexBuf(TUNNEL_GEOM, sizeof(TUNNEL_GEOM),GEOM_STRIDE),
            new IndexBuf(TUNNEL_GEOM_INDICES, sizeof(TUNNEL_GEOM_INDICES)));
    mTunnelGeom->vbuf->SetColorsOffset(GEOM_COLOR_OFFSET);
    mTunnelGeom->vbuf->SetTexCoordsOffset(GEOM_TEXCOORD_OFFSET);

    // build cube geometry (to draw obstacles)
    mCubeGeom = new SimpleGeom(new VertexBuf(CUBE_GEOM, sizeof(CUBE_GEOM),GEOM_STRIDE));
    mCubeGeom->vbuf->SetColorsOffset(GEOM_COLOR_OFFSET);
    mCubeGeom->vbuf->SetTexCoordsOffset(GEOM_TEXCOORD_OFFSET);

    // build ball geometry
    mBallGeom = new SimpleGeom(
            new VertexBuf(BALL_GEOM, sizeof(BALL_GEOM),GEOM_STRIDE),
            new IndexBuf(BALL_GEOM_INDICES, sizeof(BALL_GEOM_INDICES)));
    mBallGeom->vbuf->SetColorsOffset(GEOM_COLOR_OFFSET);
    mBallGeom->vbuf->SetTexCoordsOffset(GEOM_TEXCOORD_OFFSET);

    // make the wall texture
    mWallTexture = new Texture();
    mWallTexture->InitFromRawRGB(TEXTURE_SIZE, TEXTURE_SIZE, false,
                                 GenerateTexutre(WallId));

    // make the ball texture
    mBallTexture = new Texture();
    mBallTexture->InitFromRawRGB(TEXTURE_SIZE, TEXTURE_SIZE, false,
                                 GenerateTexutre(BallId));

    // make the obstacle texture
    mObsTexture = new Texture();
    mObsTexture->InitFromRawRGB(TEXTURE_SIZE, TEXTURE_SIZE, false,
                                GenerateTexutre(ObsId));

    // reset frame clock so the animation doesn't jump
    mFrameClock.Reset();

    // create text renderer and shape renderer
    mTextRenderer = new TextRenderer(mTrivialShader);
    mShapeRenderer = new ShapeRenderer(mTrivialShader);
}

void PlayScene::OnKillGraphics() {
    CleanUp(&mTextRenderer);
    CleanUp(&mShapeRenderer);
    CleanUp(&mOurShader);
    CleanUp(&mTrivialShader);
    CleanUp(&mTunnelGeom);
    CleanUp(&mCubeGeom);
    CleanUp(&mBall);
    CleanUp(&mBallGeom);
    CleanUp(&mWallTexture);
    CleanUp(&mObsTexture);
    UiScene::OnKillGraphics();
}

// create widgets for UI
void PlayScene::OnCreateWidgets() {
    float aspect = SceneManager::GetInstance()->GetScreenAspect();
    float center = aspect*0.5f;

    mGameOverTextId = NewWidget()->SetCenter(OVER_POS)->SetTextColor(TITLE_COLOR)
            ->SetFontScale(OVER_FONT_SCALE)->SetTransition(UiWidget::TRANS_SCALE)->GetId();

    mStatsTableId = NewWidget()->SetCenter(STATS_POS)->SetFontScale(STATS_FONT_SCALE)
            ->SetSize(STATS_SIZE)->SetIsButton(true)->SetEnabled(0)->SetBackColor(STATS_BACK)
            ->SetTextColor(BUTTON_TEXT_COLOR)->SetTransition(UiWidget::TRANS_SCALE)->GetId();

    mRestartButtonId = NewWidget()->SetCenter(BUTTON_RESTART_POS)->SetFontScale(BUTTON_RESTART_FONT_SCALE)
            ->SetSize(BUTTON_RESTART_SIZE)->SetText(S_RESTART)->SetIsButton(true)
            ->SetBackColor(BUTTON_BACK)->SetTextColor(BUTTON_TEXT_COLOR)
            ->SetTransition(UiWidget::TRANS_FROM_LEFT)->GetId();

    mQuitButtonId = NewWidget()->SetCenter(BUTTON_QUIT_POS)->SetFontScale(BUTTON_QUIT_FONT_SCALE)
            ->SetSize(BUTTON_QUIT_SIZE)->SetText(S_QUIT)->SetIsButton(true)
            ->SetBackColor(BUTTON_BACK_ADD)->SetTextColor(BUTTON_TEXT_COLOR)
            ->SetTransition(UiWidget::TRANS_FROM_RIGHT)->GetId();

    SetDefaultButton(mRestartButtonId);
    UpdateWidgetStates();
}

// navigation rule
void PlayScene::UpdateWidgetStates() {
    AddNav(mRestartButtonId, UI_DIR_DOWN, mQuitButtonId);
    AddNav(mQuitButtonId, UI_DIR_UP, mRestartButtonId);
}

void PlayScene::OnButtonClicked(int id) {
    SceneManager *mgr = SceneManager::GetInstance();

    if (id == mRestartButtonId) {
        // reset positions and bools
        Reset();
    } else if (id == mQuitButtonId) {
        // return to welcome scene
        mgr->RequestNewScene(new WelcomeScene());
    }
}


void PlayScene::DoFrame() {
    float deltaT = mFrameClock.ReadDelta();
    float previousY = mBallPos.y;

    // clear screen
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set Z-axis looking UP
    glm::vec3 upVec = glm::vec3(0.0f, 0.0f, 1.0f);

    // set up view matrix according to player's ship position and direction
    mViewMat = glm::lookAt(mCameraPos, mCameraPos + mCameraDir, upVec);

    // render tunnel walls
    RenderTunnel();

    // render obstacles
    RenderObstacles();

    // render ball
    RenderBall();

    // deduct from the time remaining to remove a sign from the screen
    if (mSignText && mSignExpires) {
        if (mSignTimeLeft < 0.0f) {
            mSignText = NULL;
        }
    }

    // did we already show the howto?
    if (!mShowedHowto) {
        mShowedHowto = true;
        ShowSign(S_HOWTO, SIGN_DURATION);
    }

    // waiting for the First Tap
    if (!firstTap)
    {
        mBallSpeed=0;
        // shift obstacles while waiting
        // mObsThresh = ceilf(mBallPos.y);
        mObsThresh = mBallPos.y;
    } else {
        mObsThresh = ceilf(mObsThresh);
        // "Oh, there goes gravity!"
        mBallSpeed -= GRAVITY_CONST*deltaT;
        // Time to remove "Tap!" Sign
        mSignTimeLeft -= deltaT;
    }

    // update camera speed
    float targetSpeed = CAMERA_SPEED;
    float accel = mCameraSpeed >= 0.0f ? CAMERA_ACCELERATION_POSITIVE_SPEED :
                  CAMERA_ACCELERATION_NEGATIVE_SPEED;
    mCameraSpeed = Approach(mCameraSpeed, targetSpeed, deltaT * accel);

    // update ball rotation speed
    mRotateSpeed -= ROTATION_ACCELERATION*deltaT;

    // move camera and player, enable collision detection
    if (isAlive) {
        // show signs and current score
        RenderHUD();

        // update camera position
        mCameraPos.y += deltaT * mCameraSpeed;

        // distance between Ball and Camera (Y axis)
        mBallPos.y = mCameraPos.y-BALL_CENTER_THRESH;

        // update ball vertical speed and rotate ratio
        mBallPos.z += deltaT*mBallSpeed;
        mRotateRatio +=deltaT*mRotateSpeed;

        if (touchDown && !hasTouched) {
            mBallSpeed += BALL_KICK;
            mRotateSpeed += ROTATION_KICK;
            hasTouched = 1;
            SfxMan::GetInstance()->PlayTone(TONE_TOUCH);
        }

        DetectCollisions(previousY);

    }

    // GAME OVER!
    if(!isAlive) {
        // show UiScene with statistics and buttons
        ShowGameOverUI();

        // focus camera on a crash poisition
        mCameraPos.y = Approach(mCameraPos.y, mBallPos.y, deltaT * accel/5.0f);

        // play proper sound
        if(SfxMan::GetInstance()->IsIdle() && !tonePlayed) {
            SfxMan::GetInstance()->PlayTone(isRecord ? TONE_HIGH_SCORE : TONE_GAME_OVER);
            tonePlayed=true;
        }

        if (Clock() > mClockGameExpire) {
            SceneManager::GetInstance()->RequestNewScene(new WelcomeScene());
        }
    }

    // make sure camera and ball didn't leave tunnel
    mCameraPos.x = Clamp(mCameraPos.x, MIN_X, MAX_X);
    mCameraPos.z = Clamp(mCameraPos.z, MIN_Z, MAX_Z);
    mBallPos.z = Clamp(mBallPos.z, MIN_Z, MAX_Z);
    mBallSpeed = Clamp(mBallSpeed, DOWN_SPEED_LIMIT, UP_SPEED_LIMIT); // 25

    // limit rotation speed
    mRotateSpeed = Clamp(mRotateSpeed, BALL_MIN_ROTATE, BALL_MAX_ROTATE);

    // shift sections if needed
    ShiftIfNeeded();

    // generate more obstacles!
    GenObstacles();

}

void PlayScene::RenderTunnel() {
    glm::mat4 modelMat;
    glm::mat4 mvpMat;
    int i, oi;
    float lightYpos;
    float R = TUNNEL_COLORS[mTunColInd];
    float G = TUNNEL_COLORS[mTunColInd+1];
    float B = TUNNEL_COLORS[mTunColInd+2];

    mOurShader->BeginRender(mTunnelGeom->vbuf);
    mOurShader->SetTexture(mWallTexture);

    for (i = mFirstSection, oi = 0; i <= mFirstSection + RENDER_TUNNEL_SECTION_COUNT; ++i, ++oi) {
        float segCenterY = GetSectionCenterY(i);
        modelMat = glm::translate(glm::mat4(1.0), glm::vec3(0.0, segCenterY, 0.0));
        mvpMat = mProjMat * mViewMat * modelMat;

        // -TUNNEL_SECTION_LENGTH/2 is needed because initial position of the ball is half section forward
        lightYpos = mBallPos.y - GetSectionStartY(i)-TUNNEL_SECTION_LENGTH/2;

        // add some coefficient to z coordinate to make light more inertial
        mOurShader->EnablePointLight
                (glm::vec3(mBallPos.x+9.0f, lightYpos, mBallPos.z*1.05f),
                 LIGHT_COLORS[3*!isAlive], LIGHT_COLORS[3*!isAlive+1], LIGHT_COLORS[3*!isAlive+2]);

        mOurShader->SetTintColor(R,G,B);

        // render tunnel section
        mOurShader->Render(mTunnelGeom->ibuf, &mvpMat);
    }
    mOurShader->EndRender();
}

void PlayScene::RenderBall() {
    glm::mat4 modelMat;
    glm::mat4 mvpMat;
    float R = BALL_COLORS[mBallColInd];
    float G = BALL_COLORS[mBallColInd+1];
    float B = BALL_COLORS[mBallColInd+2];

    mOurShader->BeginRender(mBallGeom->vbuf);
    mOurShader->SetTexture(mBallTexture);

    modelMat = glm::translate(glm::mat4(1.0f), mBallPos);

    // rotation of the ball defined by mRotateRatio
    modelMat = glm::rotate(modelMat, mRotateRatio, glm::vec3(0.0f, 0.0f, 1.0f));
    mvpMat = mProjMat * mViewMat * modelMat;

    // add blinking effect if crashed
    if (isAlive) {
        mOurShader->SetTintColor(R,G,B);
    } else {
        mOurShader->SetTintColor(SineWave(R/1.4f, R, 0.5f, 0.0f),
                                 SineWave(G/1.4f, G, 0.5f, 0.0f),
                                 SineWave(B/1.4f, B, 0.5f, 0.0f));
    }

    mOurShader->Render(mBallGeom->ibuf, &mvpMat);
    mOurShader->EndRender();

}


void PlayScene::RenderObstacles() {
    int i, oi;
    int r, c;
    float red, green, blue;
    glm::mat4 modelMat;
    glm::mat4 mvpMat;

    mOurShader->BeginRender(mCubeGeom->vbuf);
    mOurShader->SetTexture(mObsTexture);

    for (i = 0, oi =0 ; i < mCycleObsCount; i++, oi++) {
        Obstacle *o = GetObstacleAt(i);

        float posY = GetObsPositions(mObstacleCounter + i);

        if (o->style == Obstacle::STYLE_NULL) {
            // don't render null obstacles
            continue;
        }

        for (r = 0; r < OBS_GRID_SIZE; r++) {
            for (c = 0; c < OBS_GRID_SIZE; c++) {
                if (o->grid[c][r]) {
                    // set up matrices
                    modelMat = glm::translate(glm::mat4(1.0f), o->GetBoxCenter(c, r, posY));
                    modelMat = glm::scale(modelMat, o->GetBoxSize(c, r));
                    mvpMat = mProjMat * mViewMat * modelMat;

                    // set up color
                    _get_color(o->style, &red, &green, &blue);
                    mOurShader->SetTintColor(red, green, blue);

                    // render box
                    mOurShader->Render(&mvpMat);
                }
            }
        }
    }
    mOurShader->EndRender();
}

void PlayScene::GenObstacles() {
    while (mCycleObsCount < MAX_OBS) {
        // generate a new obstacle
        int index = (mFirstObstacle + mCycleObsCount) % MAX_OBS;

        if (mCycleObsCount < OBS_START_IND) {
            // generate an empty obstacle
            mObstacleCircBuf[index].Reset();
            mObstacleCircBuf[index].style = Obstacle::STYLE_NULL;
        } else {
            // generate a normal obstacle
            mObstacleGen.Generate(&mObstacleCircBuf[index]);
        }
        mCycleObsCount++;

    }
}

void PlayScene::ShiftIfNeeded() {
    // is it time to discard a section and shift forward?
    while (mCameraPos.y > GetSectionEndY(mFirstSection) + SHIFT_THRESH) {
        // shift to the next turnnel section
        mFirstSection++;
    }

    // is it time to discard an obstacle and shift forward?
    while (mCameraPos.y > GetObsPositions(mObstacleCounter) + SHIFT_THRESH) {
        // discard first obstacle out of view
        if (mCycleObsCount > 0) {
            // discarding first object (shifting) is easy because it's a circular buffer!
            mFirstObstacle = (mFirstObstacle + 1) % MAX_OBS;
            mObstacleCounter++;
            mCycleObsCount-=1;
        }
    }
}

// Render Score, Signs, etc
void PlayScene::RenderHUD() {
    float aspect = SceneManager::GetInstance()->GetScreenAspect();
    glm::mat4 modelMat;

    glDisable(GL_DEPTH_TEST);

    int score = GetScore();

    mTextRenderer->SetFontScale(SCORE_FONT_SCALE);
    mTextRenderer->RenderText(ConvInttoChar(score), aspect/2, SCORE_POS_Y);

    float scaleFactor = SineWave(1.0f, SIGN_PULSE_AMOUNT, SIGN_PULSE_PERIOD, 0.0f);

    // render current sign
    if (mSignText) {
        modelMat = glm::mat4(1.0f);
        float t = Clock() - mSignStartTime;
        if (t < SIGN_ANIM_DUR) {
            float scale = t / SIGN_ANIM_DUR;
            modelMat = glm::scale(modelMat, glm::vec3(1.0f, scale, 1.0f));
        } else if (mSignTimeLeft < SIGN_ANIM_DUR) {
            float scale = mSignTimeLeft / SIGN_ANIM_DUR;
            modelMat = glm::scale(modelMat, glm::vec3(1.0f, scale, 1.0f));
        }

        mTextRenderer->SetMatrix(modelMat);
        mTextRenderer->SetFontScale(scaleFactor*SIGN_FONT_SCALE);
        mTextRenderer->RenderText(mSignText, aspect * 0.5f, 0.5f);
        mTextRenderer->ResetMatrix();
    }

    glEnable(GL_DEPTH_TEST);
}

void PlayScene::DetectCollisions(float previousY) {

    Obstacle *o = GetObstacleAt(mNearestObsInd);

    float obsCenter = GetObsPositions(mObstacleCounter+mNearestObsInd);
    float obsMin = obsCenter - (OBS_BOX_SIZE/2);
    float obsMax = obsCenter + (OBS_BOX_SIZE/2);
    float minHeight = o->GetMinCutY();
    float maxHeight = o->GetMaxCutY();

    float curY =  mBallPos.y;

    // upgrade score if passed the MIDDLE of an obstacle
    // not so precise but makes the game less frustrating
    if ((previousY <= obsCenter) && (curY > obsCenter))
    {
        // do not count empty obstacles
        if(mObstacleCounter+mNearestObsInd>=OBS_START_IND) {
            mScore++;
            SetScore(mScore);
            SaveProgress();
        }
        LOGI("SCORE: %i", mScore );
    }

    // check if hit the walls
    if (mBallPos.z<=MIN_Z || mBallPos.z>=MAX_Z) {
        collision=true;
    }

    // check if hit an obstacle
    // 1) check Y position
    if (curY+BALL_RADIUS >= obsMin && curY-BALL_RADIUS <= obsMax) {
        // 2) check Z position
        if ((maxHeight - mBallPos.z) < BALL_RADIUS || (mBallPos.z - minHeight) < BALL_RADIUS) {
            collision = true;
        }
    }

    // what to do if collided?
    if (collision){
        // crashed against obstacle
        LOGI("CRASHED! Ball Position: %f ROW: %i", curY, o->GetRowAt(mBallPos.z));
        LOGI("GAME OVER! Score %i Best %i", mScore, mBestScore);

        isAlive = false;

        // check if player has beat the record
        isRecord = (mScore == mBestScore && mScore != 0);

        // set Clocks for Stats and Game Expiring
        mClockShowStats = Clock() + GAME_OVER_STATS;
        mClockGameExpire = Clock() + GAME_OVER_EXPIRE;
    }
}


void PlayScene::ShowGameOverUI() {

    // forming text fot statistics
    int resLen = 15 + numOfDigits(mScore) + numOfDigits(mBestScore);
    overStats = new char[resLen];

    strcpy(overStats,"SCORE: ");

    // Add spaces to align score and best score numbers
    int def = numOfDigits(mBestScore)- numOfDigits(mScore);
    while(def) {
        strcat(overStats," ");
        resLen++;
        def--;
    };

    strcat(overStats,ConvInttoChar(mScore));
    strcat(overStats,"\n");
    strcat(overStats," BEST: ");
    strcat(overStats,ConvInttoChar(mBestScore));
    overStats[resLen] = '\0';

    GetWidgetById(mStatsTableId)->SetText(overStats);
    GetWidgetById(mGameOverTextId)->SetText(isRecord ? S_HIGH_SCORE : S_GAME_OVER);

    // enable UiScene when time is right
    if(Clock() > mClockShowStats) {
        overUIActive = true;
        UpdateWidgetStates();
        SetWaitScreen(false);
        UiScene::DoFrame();
    }

}

void PlayScene::OnPointerDown(int pointerId, const struct PointerCoords *coords) {

    touchDown = true;
    firstTap = true;

    if(overUIActive){ UiScene::OnPointerDown(pointerId,coords);}
}

void PlayScene::OnPointerUp(int pointerId, const struct PointerCoords *coords) {

    touchDown = false;
    hasTouched = false;

    if(overUIActive){ UiScene::OnPointerUp(pointerId,coords);}
}

void PlayScene::LoadProgress() {
    // try to load save file
    mBestScore = 0;

    LOGD("Attempting to load: %s", mSaveFileName);
    FILE *f = fopen(mSaveFileName, "r");

    if (f) {
        LOGD("File found. Loading data.");
        if (1 != fscanf(f, "v1 %d", &mBestScore)) {
            LOGE("Error parsing save file.");
            mBestScore = 0;
        } else {
            LOGD("Loaded. Score = %d", mBestScore);
        }
        fclose(f);
    } else {
        LOGD("Save file not present.");
    }

}

void PlayScene::WriteSaveFile(int level) {
    LOGD("Saving best score %d to file: %s", level, mSaveFileName);
    FILE *f = fopen(mSaveFileName, "w");
    if (!f) {
        LOGE("Error writing to save game file.");
        return;
    }
    fprintf(f, "v1 %d", level);
    fclose(f);
    LOGD("Save file written.");
}

void PlayScene::SaveProgress() {
    if (mScore <= mBestScore) {
        // nothing to do
        LOGD("No need to save score, current = %d, saved = %d", mScore, mBestScore);
        return;
    }

    mBestScore = mScore;

    LOGD("Saving progress to LOCAL FILE: score %d", mScore);
    WriteSaveFile(mScore);

}

unsigned char* PlayScene::GenerateTexutre(int TexId) {
    static unsigned char pixel_data[TEXTURE_SIZE * TEXTURE_SIZE * 3];
    unsigned char *p;
    int x, y;
    for (y = 0, p = pixel_data; y < TEXTURE_SIZE; y++) {
        for (x = 0; x < TEXTURE_SIZE; x++, p += 3) {
            switch (TexId) {
                case BallId: // Ball
                    // solid color if ball
                    p[0] = p[1] = p[2] = 192;
                    break;
                case ObsId: // Obstacle
                    // solid color with mesh
                    p[0] = p[1] = p[2] = 128 + ((x > 2 && y > 2) ? 32+Random(64) : 0);
                    break;
                case WallId: // Tunnel Walls
                    // generates mesh
                    p[0] = p[1] = p[2] = (x > 2 && y > 2) ? 0 : 128;
                    break;
                default:
                    // some noise
                    p[0] = p[1] = p[2] = 128 + ((x > 2 && y > 2) ? Random(128) : 0);
            }
        }
    }
    return pixel_data;
}

char* PlayScene::ConvInttoChar(int digit) {
    float units;
    int length = numOfDigits(digit);
    char* result;
    result = new char[length+1];

    for (int i = 0, units = pow(10,length-1); i < length; i++, units /= 10) {
        result[i] = '0' + (digit / (int) units) % 10;
    }

    result[length]='\0';
    return result;
}

void PlayScene::UpdateProjectionMatrix() {
    SceneManager *mgr = SceneManager::GetInstance();
    mProjMat = glm::perspective(RENDER_FOV, mgr->GetScreenAspect(), RENDER_NEAR_CLIP,
                                RENDER_FAR_CLIP);
}

void PlayScene::OnScreenResized(int width, int height) {
    UpdateProjectionMatrix();
}

// return to welcome scene if back pressed after game is over
bool PlayScene::OnBackKeyPressed() {
    SceneManager *mgr = SceneManager::GetInstance();
    if(overUIActive){
    mgr->RequestNewScene(new WelcomeScene());
    return true;
    } else {return  true;}
}

void PlayScene::OnKeyDown(int keyCode) {

}

void PlayScene::OnPause() {

}