#pragma once

#include "constants.h"
#include "deathshader.h"
#include "extramanager.h"
#include "extratable.h"
#include "gamenode.h"
#include "joystick/gamecontrollerinfo.h"
#include "playerjump.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Box2D/Box2D.h"

#include <deque>
#include <functional>
#include <memory>
#include <set>

class Animation;
class GameContactListener;
class Weapon;
struct WeaponSystem;

const sf::Vector2f vector2fZero;

class Player : public GameNode
{

   class PlayerAABBQueryCallback : public b2QueryCallback
   {
      public:
         std::set<b2Body*> mBodies;

      public:
         bool ReportFixture(b2Fixture* fixture)
         {
            // foundBodies.push_back(fixture->GetBody());
            mBodies.insert(fixture->GetBody());

            // keep going to find all fixtures in the query area
            return true;
         }
   };

   struct PositionedAnimation
   {
      sf::Vector2f mPosition;
      std::shared_ptr<Animation> mAnimation;
   };

   enum class Edge
   {
      None,
      Left,
      Right
   };

   enum class Dash
   {
     None,
     Left,
     Right
   };

   struct PlayerSpeed
   {
      b2Vec2 currentVelocity;
      float velocityMax = 0.0f;
      float acceleration = 0.0f;
      float deceleration = 0.0f;
   };


public:

   Player(GameNode* parent = nullptr);
   virtual ~Player() = default;

   static Player* getCurrent();

   void initialize();
   void initializeLevel();
   void initializeController();
   void draw(sf::RenderTarget& target);

   void update(const sf::Time& dt);
   void keyboardKeyPressed(sf::Keyboard::Key key);
   void keyboardKeyReleased(sf::Keyboard::Key key);

   void controllerRunButtonPressed();
   void controllerRunButtonReleased();
   bool isLookingAround() const;
   bool isControllerUsed() const;
   bool isControllerButtonPressed(int buttonEnum) const;
   bool isFireButtonPressed() const;
   bool isJumpButtonPressed() const;

   void fire();
   void die();
   void reset();
   DeathReason checkDead() const;

   bool isMovingRight() const;
   bool isMovingLeft() const;
   bool isMoving() const;
   bool isPointingRight() const;
   bool isPointingLeft() const;

   void setStartPixelPosition(float x, float y);

   int getKeysPressed() const;
   void setKeysPressed(int keys);

   b2Vec2 getBodyPosition() const;
   const sf::Vector2f& getPixelPositionf() const;
   const sf::Vector2i& getPixelPositioni() const;
   void setPixelPosition(float x, float y);

   float getBeltVelocity() const;
   void setBeltVelocity(float beltVelocity);
   bool isOnBelt() const;
   void setOnBelt(bool onBelt);
   void applyBeltVelocity(float &desiredVel);

   const sf::IntRect& getPlayerPixelRect() const;


   b2Body* getBody() const;
   void setWorld(const std::shared_ptr<b2World>& world);
   void updatePixelPosition();
   void updatePreviousBodyState();
   void updatePlayerPixelRect();
   void setBodyViaPixelPosition(float x, float y);
   void setFriction(float f);


   bool getVisible() const;
   void setVisible(bool visible);


   b2Body* getPlatformBody() const;
   void setPlatformBody(b2Body* body);
   void setGroundBody(b2Body* body);

   bool isInAir() const;
   bool isClimbing() const;
   bool isInWater() const;
   bool isOnPlatform() const;
   bool isOnGround() const;
   bool isDead() const;
   bool isCrouching() const;

   void setInWater(bool inWater);

   int getZ() const;
   void setZ(int z);

   int getId() const;

   const GameControllerInfo& getJoystickInfo() const;
   void setJoystickInfo(const GameControllerInfo& joystickInfo);

   void impulse(float intensity);
   void damage(int damage, const sf::Vector2f& force = vector2fZero);

   void updateClimb();

   std::shared_ptr<ExtraManager> getExtraManager() const;


private:

   void createPlayerBody();

   void updateAnimation(const sf::Time& dt);
   void updatePixelCollisions();
   void updateAtmosphere();
   void updateVelocity();
   void updatePlatformMovement(const sf::Time& dt);
   void updateFire();
   void updateFootsteps();
   void updatePortal();
   void updatePlayerOrientation();
   void updateDash(Dash dir = Dash::None);
   void updateCrouch();
   void updateHardLanding();
   void updateGroundAngle();

   bool isDashActive() const;
   void resetDash();

   void createBody();
   void createFeet();
   void setCrouching(bool enabled);

   bool isClimbableEdge(b2ChainShape* shape, int currIndex);
   void removeClimbJoint();
   bool edgeMatchesMovement(const b2Vec2 &edgeDir);
   float getMaxVelocity() const;
   float getVelocityFromController(const PlayerSpeed& speed) const;
   float getVelocityFromKeyboard(const PlayerSpeed& speed) const;
   float getDesiredVelocity(const PlayerSpeed& speed) const;
   float getDesiredVelocity() const;
   float getDeceleration() const;
   float getAcceleration() const;

   void playDustAnimation();

private:

   std::shared_ptr<WeaponSystem> mWeaponSystem;
   std::shared_ptr<ExtraManager> mExtraManager;

   std::shared_ptr<b2World> mWorld;
   b2Body* mBody = nullptr;
   b2Fixture* mBodyFixture = nullptr;
   b2Joint* mClimbJoint = nullptr;

   GameControllerInfo mJoystickInfo;
   int mKeysPressed = 0;
   bool mControllerRunPressed = false;

   sf::Vector2f mPixelPositionf;
   sf::Vector2i mPixelPositioni;
   sf::Texture mTexture;
   sf::Sprite mSprite;
   sf::Vector2u mSpritePrev;
   sf::Vector2u mSpriteAnim;
   sf::IntRect mPlayerPixelRect;

   sf::Time mTime;
   sf::Clock mClock;
   sf::Clock mPortalClock;
   sf::Clock mDamageClock;
   bool mDamageInitialized = false;

   int mAnimSpeed = 50;

   bool mPointsToLeft = false;
   bool mVisible = true;
   bool mCrouching = false;
   bool mInWater = false;
   bool mDead = false;

   b2Vec2 mPositionPrevious;
   b2Vec2 mVelocityPrevious;
   b2Body* mPlatformBody = nullptr;
   b2Body* mGroundBody = nullptr;
   b2Vec2 mGroundNormal;
   float mNextFootStepTime = 0.0f;

   int mZ = 0;
   int mId = 0;

   bool mHardLanding = false;
   int32_t mHardLandingCycles = 0;

   float mBeltVelocity = 0.0f;
   bool mIsOnBelt = false;

   int mDashSteps = 0;
   Dash mDashDir = Dash::None;

   std::shared_ptr<Animation> mIdleRightAligned;
   std::shared_ptr<Animation> mIdleLeftAligned;
   std::shared_ptr<Animation> mSwimRightAligned;
   std::shared_ptr<Animation> mSwimLeftAligned;
   std::shared_ptr<Animation> mRunRightAligned;
   std::shared_ptr<Animation> mRunLeftAligned;
   std::shared_ptr<Animation> mDashRightAligned;
   std::shared_ptr<Animation> mDashLeftAligned;
   std::shared_ptr<Animation> mCrouchRightAligned;
   std::shared_ptr<Animation> mCrouchLeftAligned;


   std::shared_ptr<Animation> mJumpInitRightAligned;
   std::shared_ptr<Animation> mJumpUpRightAligned;
   std::shared_ptr<Animation> mJumpMidairRightAligned;
   std::shared_ptr<Animation> mJumpDownRightAligned;
   std::shared_ptr<Animation> mJumpLandingRightAligned;

   std::shared_ptr<Animation> mJumpInitLeftAligned;
   std::shared_ptr<Animation> mJumpUpLeftAligned;
   std::shared_ptr<Animation> mJumpMidairLeftAligned;
   std::shared_ptr<Animation> mJumpDownLeftAligned;
   std::shared_ptr<Animation> mJumpLandingLeftAligned;

   int mJumpAnimationReference = 0;

   std::vector<std::shared_ptr<Animation>> mAnimations;
   std::shared_ptr<Animation> mCurrentCycle;

   PlayerJump mJump;

   std::deque<PositionedAnimation> mLastAnimations;

   static Player* sCurrent;
};

