#ifndef Renderer_hpp
#define Renderer_hpp

#include "Player.hpp"
#include "ExtendedCamera.hpp"

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include "BulletPhysics.hpp"

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class Renderer
  : public Ogre::WindowEventListener,
{
public:
  Renderer();
  virtual ~Renderer();

private:

  Ogre::Root* mRoot;
  Ogre::RenderWindow* mWindow;
  Ogre::SceneManager* mSceneMgr;
  Ogre::Camera* mCamera;
  ExtendedCamera* mExCamera;
};

#endif

bool Renderer::init()
{
  mRoot = new Ogre::Root("plugins.cfg");
  initResources();

  if (!initWindow())
  {
    return false;
  }

  // Set default mipmap level (NB some APIs ignore this)
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

  // Get the SceneManager, in this case a generic one
  mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

  // Create the camera
  mCamera = mSceneMgr->createCamera("MainCam");
  mExCamera = new ExtendedCamera("ExtendedCamera", mSceneMgr, mCamera);

  initViewports();

  return true;
}

void Renderer::initResources()
{
  // Load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load("resources.cfg");

  // Go through all sections & settings in the file
  Ogre::String name, locType;
  Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

  while (secIt.hasMoreElements())
  {
    Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator it;

    for (it = settings->begin(); it != settings->end(); ++it)
    {
      locType = it->first;
      name = it->second;

      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
    }
  }

  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

bool Renderer::initWindow()
{
  // Show the configuration dialog and initialise the system.
  // You can skip this and use root.restoreConfig() to load configuration
  // settings if you were sure there are valid ones saved in ogre.cfg.
  if (mRoot->restoreConfig() || mRoot->showConfigDialog())
  {
    // If returned true, user clicked OK so initialise.
    // Here we choose to let the system create a default rendering window by passing 'true'.
    mWindow = mRoot->initialise(true, "Render Window");
    return true;
  }
  else
  {
    return false;
  }
}

//---------------------------------------------------------------------------
void Renderer::initViewports()
{
  // Create one viewport, entire window
  Ogre::Viewport* vp = mWindow->addViewport(mCamera);
  vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

  // Alter the camera aspect ratio to match the viewport
  mCamera->setAspectRatio(
    Ogre::Real(vp->getActualWidth()) /
    Ogre::Real(vp->getActualHeight()));
}

//---------------------------------------------------------------------------

// Adjust mouse clipping area
void GameManager::windowResized(Ogre::RenderWindow* rw)
{
  int left, top;
  unsigned int width, height, depth;

  rw->getMetrics(width, height, depth, left, top);

  const OIS::MouseState& ms = mMouse->getMouseState();
  ms.width = width;
  ms.height = height;
}

//---------------------------------------------------------------------------
// Unattach OIS before window shutdown
void GameManager::windowClosed(Ogre::RenderWindow* rw)
{
  // Only close for window that created OIS
  if(rw == mWindow)
  {
    if(mInputMgr)
    {
      mInputMgr->destroyInputObject(mMouse);
      mInputMgr->destroyInputObject(mKeyboard);

      OIS::InputManager::destroyInputSystem(mInputMgr);
      mInputMgr = 0;
    }
  }
}