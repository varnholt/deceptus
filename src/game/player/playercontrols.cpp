#include "playercontrols.h"

#include "constants.h"
#include "gamecontrollerintegration.h"
#include "framework/joystick/gamecontroller.h"


//----------------------------------------------------------------------------------------------------------------------
void PlayerControls::update(const sf::Time& /*dt*/)
{
   setWasMoving(isMoving());
   setWasMovingLeft(isMovingLeft());
   setWasMovingRight(isMovingRight());
}


//----------------------------------------------------------------------------------------------------------------------
void PlayerControls::addKeypressedCallback(const KeypressedCallback& callback)
{
   _keypressed_callbacks.push_back(callback);
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::hasFlag(int32_t flag) const
{
   return _keys_pressed & flag;
}


//----------------------------------------------------------------------------------------------------------------------
void PlayerControls::forceSync()
{
   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
   {
      _keys_pressed |= KeyPressedJump;
   }

   if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
   {
      _keys_pressed |= KeyPressedLook;
   }

   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
   {
      _keys_pressed |= KeyPressedUp;
   }

   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
   {
      _keys_pressed |= KeyPressedDown;
   }

   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
   {
      _keys_pressed |= KeyPressedLeft;
   }

   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
   {
      _keys_pressed |= KeyPressedRight;
   }

   if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
   {
      _keys_pressed |= KeyPressedRun;
   }

   if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
   {
      _keys_pressed |= KeyPressedFire;
   }
}


//----------------------------------------------------------------------------------------------------------------------
void PlayerControls::keyboardKeyPressed(sf::Keyboard::Key key)
{
   if (GameControllerIntegration::getCount() > 0)
   {
      return;
   }

   if (key == sf::Keyboard::Space)
   {
      _keys_pressed |= KeyPressedJump;
   }

   if (key == sf::Keyboard::LShift)
   {
      _keys_pressed |= KeyPressedLook;
   }

   if (key == sf::Keyboard::Up)
   {
      _keys_pressed |= KeyPressedUp;
   }

   if (key == sf::Keyboard::Down)
   {
      _keys_pressed |= KeyPressedDown;
   }

   if (key == sf::Keyboard::Left)
   {
      _keys_pressed |= KeyPressedLeft;
   }

   if (key == sf::Keyboard::Right)
   {
      _keys_pressed |= KeyPressedRight;
   }

   if (key == sf::Keyboard::LAlt)
   {
      _keys_pressed |= KeyPressedRun;
   }

   if (key == sf::Keyboard::LControl)
   {
      _keys_pressed |= KeyPressedFire;
   }

   for (auto& callback : _keypressed_callbacks)
   {
      callback(key);
   }
}


//----------------------------------------------------------------------------------------------------------------------
void PlayerControls::keyboardKeyReleased(sf::Keyboard::Key key)
{
   if (GameControllerIntegration::getCount() > 0)
   {
      return;
   }

   if (key == sf::Keyboard::LShift)
   {
      _keys_pressed &= ~KeyPressedLook;
   }

   if (key == sf::Keyboard::Up)
   {
      _keys_pressed &= ~KeyPressedUp;
   }

   if (key == sf::Keyboard::Down)
   {
      _keys_pressed &= ~KeyPressedDown;
   }

   if (key == sf::Keyboard::Left)
   {
      _keys_pressed &= ~KeyPressedLeft;
   }

   if (key == sf::Keyboard::Right)
   {
      _keys_pressed &= ~KeyPressedRight;
   }

   if (key == sf::Keyboard::Space)
   {
      _keys_pressed &= ~KeyPressedJump;
   }

   if (key == sf::Keyboard::LAlt)
   {
      _keys_pressed &= ~KeyPressedRun;
   }

   if (key == sf::Keyboard::LControl)
   {
      _keys_pressed &= ~KeyPressedFire;
   }
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::isLookingAround() const
{
  if (_keys_pressed & KeyPressedLook)
  {
    return true;
  }

  if (isControllerUsed())
  {
    return isControllerButtonPressed(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
  }

  return false;
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::isControllerUsed() const
{
  return !_joystick_info.getAxisValues().empty();
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::isControllerButtonPressed(int buttonEnum) const
{
  auto pressed = false;

  auto gji = GameControllerIntegration::getInstance(0);
  if (gji != nullptr)
  {
     auto buttonId = gji->getController()->getButtonId(static_cast<SDL_GameControllerButton>(buttonEnum));
     pressed = (_joystick_info.getButtonValues()[static_cast<size_t>(buttonId)]);
  }

  return pressed;
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::isFireButtonPressed() const
{
  if (_keys_pressed & KeyPressedFire)
  {
    return true;
  }

  if (isControllerUsed())
  {
    return isControllerButtonPressed(SDL_CONTROLLER_BUTTON_X);
  }

  return false;
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::isJumpButtonPressed() const
{
  if (_keys_pressed & KeyPressedJump)
  {
    return true;
  }

  if (isControllerUsed())
  {
     return isControllerButtonPressed(SDL_CONTROLLER_BUTTON_A);
  }

  return false;
}



//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::isMovingLeft() const
{
  if (isControllerUsed())
  {
     const auto& axisValues = _joystick_info.getAxisValues();
     int axisLeftX = GameControllerIntegration::getInstance(0)->getController()->getAxisIndex(SDL_CONTROLLER_AXIS_LEFTX);
     auto xl = axisValues[static_cast<size_t>(axisLeftX)] / 32767.0f;
     auto hatValue = _joystick_info.getHatValues().at(0);
     auto dpadLeftPressed = hatValue & SDL_HAT_LEFT;
     auto dpadRightPressed = hatValue & SDL_HAT_RIGHT;

     if (dpadLeftPressed)
     {
        xl = -1.0f;
     }
     else if (dpadRightPressed)
     {
        xl = 1.0f;
     }

     if (fabs(xl) >  0.3f)
     {
        if (xl < 0.0f)
        {
           return true;
        }
     }
  }
  else
  {
     if (_keys_pressed & KeyPressedLeft)
     {
        return true;
     }
  }

  return false;
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::isMovingRight() const
{
  if (isControllerUsed())
  {
     const auto& axisValues = _joystick_info.getAxisValues();
     int axisLeftX = GameControllerIntegration::getInstance(0)->getController()->getAxisIndex(SDL_CONTROLLER_AXIS_LEFTX);
     auto xl = axisValues[static_cast<size_t>(axisLeftX)] / 32767.0f;
     auto hatValue = _joystick_info.getHatValues().at(0);
     auto dpadLeftPressed = hatValue & SDL_HAT_LEFT;
     auto dpadRightPressed = hatValue & SDL_HAT_RIGHT;

     if (dpadLeftPressed)
     {
        xl = -1.0f;
     }
     else if (dpadRightPressed)
     {
        xl = 1.0f;
     }

     if (fabs(xl)> 0.3f)
     {
        if (xl > 0.0f)
        {
           return true;
        }
     }
  }
  else
  {
     if (_keys_pressed & KeyPressedRight)
     {
        return true;
     }
  }

  return false;
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::isMoving() const
{
   return isMovingLeft() || isMovingRight();
}


//----------------------------------------------------------------------------------------------------------------------
int PlayerControls::getKeysPressed() const
{
   return _keys_pressed;
}


//----------------------------------------------------------------------------------------------------------------------
void PlayerControls::setKeysPressed(int keysPressed)
{
   _keys_pressed = keysPressed;
}


//----------------------------------------------------------------------------------------------------------------------
const GameControllerInfo& PlayerControls::getJoystickInfo() const
{
   return _joystick_info;
}


//----------------------------------------------------------------------------------------------------------------------
void PlayerControls::setJoystickInfo(const GameControllerInfo &joystickInfo)
{
   _joystick_info = joystickInfo;
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::wasMoving() const
{
   return _was_moving;
}


//----------------------------------------------------------------------------------------------------------------------
void PlayerControls::setWasMoving(bool was_moving)
{
   _was_moving = was_moving;
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::wasMovingLeft() const
{
   return _was_moving_left;
}


//----------------------------------------------------------------------------------------------------------------------
void PlayerControls::setWasMovingLeft(bool was_moving_left)
{
   _was_moving_left = was_moving_left;
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::wasMovingRight() const
{
   return _was_moving_right;
}


//----------------------------------------------------------------------------------------------------------------------
void PlayerControls::setWasMovingRight(bool was_moving_right)
{
   _was_moving_right = was_moving_right;
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::changedToIdle() const
{
   return wasMoving() && !isMoving();
}


//----------------------------------------------------------------------------------------------------------------------
bool PlayerControls::changedToMoving() const
{
   return !wasMoving() && isMoving();
}
