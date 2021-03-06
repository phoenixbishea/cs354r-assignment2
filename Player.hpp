
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
#include "Sound.hpp"

class Player
{
public:
    Player (Ogre::String name, Ogre::SceneManager* sceneMgr, BulletPhysics* physicsEngine, Sound* sound);

    ~Player ();

    // Updates the Player (movement...)
    void update (Ogre::Real elapsedTime, OIS::Keyboard* input, OIS::Mouse* mouse);

    // The three methods below returns the two camera-related nodes, 
    // and the current position of the Player (for the 1st person camera)
    Ogre::SceneNode* getSightNode ();

    Ogre::SceneNode* getCameraNode ();

    void updateAction(btCollisionWorld* world, btScalar dt);

    btPairCachingGhostObject* getGhostObject();
    btTransform& getWorldTransform();

    void setOgrePosition(Ogre::Vector3 vec);
    void setOgreOrientation(Ogre::Quaternion q);
    Ogre::Vector3 getOgrePosition();
    Ogre::Vector3 getOgreLookDirection();

    float getCollisionObjectHalfHeight();

protected:
    Ogre::String mName;
    btPairCachingGhostObject* ghost;
    btKinematicCharacterController* player;
    btRigidBody* paddleBody;
    Ogre::SceneNode* mMainNode;
  Ogre::SceneNode* mCannonNode;
    Ogre::SceneNode* mSightNode; // "Sight" node - The Player is supposed to be looking here
    Ogre::SceneNode* mCameraNode; // Node for the chase camera
    Ogre::Entity* mEntity; // Player entity
    Ogre::SceneManager* mSceneMgr;

    Sound* mSound;
};

#endif
