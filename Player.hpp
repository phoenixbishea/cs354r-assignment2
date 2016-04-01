#ifndef Player_hpp
#define Player_hpp

#include <OgreEntity.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreSceneManager.h>
#include <OgreSubMesh.h>
#include <OgreMeshManager.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "BulletPhysics.hpp"
#include "NetManager.h"

#include "PlayerInputComponent.hpp"
#include "PlayerPhysicsComponent.hpp"
#include "PlayerGraphicsComponent.hpp"
#include "PlayerCameraComponent.hpp"
#include "PlayerNetworkComponent.hpp"

#include "PlayerData.hpp"

class Player
{
public:
    Player(Ogre::SceneManager* graphics,
           BulletPhysics* physics,
           Ogre::Camera* camera,
           const Vector& initialPosition = Vector(0.0f, 0.0f, 0.0f),
           bool networkedPlayer = false)
        : mPhysics(new PlayerPhysicsComponent(data, physics, initialPosition)),
          mGraphics(new PlayerGraphicsComponent(data, graphics, initialPosition)),
          mNetworkedPlayer(networkedPlayer)
        {
            if (!networkedPlayer)
            {
                mCamera = new PlayerCameraComponent(graphics, camera);
                mInput = new PlayerInputComponent();
                mNetwork = new PlayerNetworkComponent();
            }
        }

    bool update(BulletPhysics* physics, OIS::Keyboard* keyboard, OIS::Mouse* mouse, float elapsedTime)
    {
        if (mInput) mInput->update(data, keyboard, mouse, elapsedTime);
        else if (mNetwork) mNetwork->update(data, mGraphics->cannonNode);

        if(!mPhysics->update(data, physics, elapsedTime, mNetworkedPlayer)) return false;

        mGraphics->update(data);

        if (mCamera) mCamera->update(data);

        return true;
    }

    btTransform getWorldTransform()
    {
        return mPhysics->getTransform();
    }

    Vector getCannonDirection()
    {
        return Vector(mGraphics->cannonNode->_getDerivedOrientation() * Ogre::Vector3(0, 0, -1));
    }

    void serializeData(char* buf, int playerNum)
    {
        // Put DC_PINFO at the start of the string
        memcpy(buf, STR_PINFO.c_str(), STR_PINFO.length());

        int* buf_int = (int*) (buf + 4);

        // Put the player number in the string
        *buf_int++ = playerNum;

        float* buf_float = (float*) buf_int;

        // Put the player's x, y, and z
        btVector3 position = this->getWorldTransform().getOrigin();
        *buf_float++ = position.x();
        *buf_float++ = position.y();
        *buf_float++ = position.z();

        // Put the player's rotation with respect to 0, 1, 0
        btQuaternion orientation = this->getWorldTransform().getRotation();
        *buf_float++ = orientation.w();

        // Put the player's horizontal pitch
        Ogre::Real pitch = mGraphics->cannonNode->getOrientation().getPitch().valueDegrees();
        *buf_float = pitch;
    }

    static bool unSerializeData(char* buf, int& playerNum, Vector& playerPosition, float& orientation, float& pitch)
    {
        std::string temp(buf);
        //Check to make sure PLIN is in the message
        if(temp.substr(0,STR_PINFO.length())!=STR_PINFO)
            return false;

        int* buf_int = (int*) (buf + 4);

        // Put the player number in the string
        playerNum = *buf_int++;
        float* buf_float = (float *)buf_int;

        // Put the player's x, y, and z
        playerPosition.setX(*buf_float++);
        playerPosition.setY(*buf_float++);
        playerPosition.setZ(*buf_float++);

        // Put the player's rotation with respect to 0, 1, 0
        orientation = *buf_float++;

        // Put the player's horizontal pitch
        pitch = *buf_float;
        return true;
    }

    void readDummyData(const Vector& position, float orientation, float pitch)
    {
        data.position = position;
        // update player obj.position
        // update the player obj.orientation
        // update playerGraphicsComponent->cannonNode from graphics component with proper pitch
    }
private:
    PlayerInputComponent* mInput;
    PlayerPhysicsComponent* mPhysics;
    PlayerGraphicsComponent* mGraphics;
    PlayerCameraComponent* mCamera;
    PlayerNetworkComponent* mNetwork;

	PlayerData data;

    bool mNetworkedPlayer; 
};

#endif
