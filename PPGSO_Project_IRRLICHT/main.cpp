#include <irrlicht.h>
#include <iostream>
#include "driverChoice.h"
#include "MyEventReceiver.h"
#include "CMyLightManager.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define FULLSCREEN

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "../Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

float t = 0.0;

int main()
{
	MyEventReceiver receiver;
	#ifdef FULLSCREEN
		IrrlichtDevice *device = createDevice( video::EDT_OPENGL, dimension2d<u32>(1366, 768), 32, true, false, false, 0);
	#else
		IrrlichtDevice *device = createDevice( video::EDT_OPENGL, dimension2d<u32>(743, 743), 16, false, false, false, 0);
	#endif

    if (!device)
        return 1;

	IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

	const float sunSize = (float) 10.0;

	IMeshSceneNode* sun = smgr->addSphereSceneNode(sunSize, 50, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (sun)
    {
		sun->setPosition(vector3df(0,0,30));
        sun->setMaterialFlag(EMF_LIGHTING, false);
        sun->setMaterialTexture( 0, driver->getTexture("../textures/sun_texture_big.BMP") );
		smgr->addLightSceneNode(sun, vector3df(0, 0, 0), SColorf(1.0f, 1.0f, 1.0f), sunSize, -1);
    }

	IMeshSceneNode* jupiter = smgr->addSphereSceneNode(sunSize, 20, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (jupiter)
    {
		const float jupiterSize = sunSize / (10.0f * sunSize);
		const float jAU = 5.4f;
        jupiter->setMaterialFlag(EMF_LIGHTING, true);
		jupiter->setParent(sun);
		jupiter->setScale(vector3df(jupiterSize, jupiterSize, jupiterSize));
		jupiter->setPosition(vector3df(2 * jAU + sunSize, 0, 0));
        jupiter->setMaterialTexture( 0, driver->getTexture("../textures/Jupiter.bmp") );
    }

	IMeshSceneNode* earth = smgr->addSphereSceneNode(sunSize, 10, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (earth)
    {
		const float earthSize = sunSize / (100.0f * sunSize);
		const float eAU = 1.0f;
        earth->setMaterialFlag(EMF_LIGHTING, true);
		earth->setParent(sun);
		earth->setScale(vector3df(earthSize, earthSize, earthSize));
		earth->setPosition(vector3df(2 * eAU + sunSize, 0, 0));
        earth->setMaterialTexture( 0, driver->getTexture("../textures/earth.bmp") );
    }

	IMeshSceneNode* moon = smgr->addSphereSceneNode(sunSize, 10, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (moon)
    {
		const float moonSize = sunSize * 75.0f / (416.0f * sunSize); //TODO
		const float mAU = 0.5f; //TODO
        moon->setMaterialFlag(EMF_LIGHTING, true);
		moon->setParent(earth);
		moon->setScale(vector3df(moonSize, moonSize, moonSize));
		moon->setPosition(vector3df(16 * mAU + sunSize, 0, 0));
        moon->setMaterialTexture( 0, driver->getTexture("../textures/moonTexture.BMP") );
    }

	IMeshSceneNode* pluto = smgr->addSphereSceneNode(sunSize, 5, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (pluto)
    {
		const float plutoSize = sunSize / (625.0f * sunSize);
		const float pAU = 40.0f;
        pluto->setMaterialFlag(EMF_LIGHTING, true);
		pluto->setParent(sun);
		pluto->setScale(vector3df(plutoSize, plutoSize, plutoSize));
		pluto->setPosition(vector3df(2 * pAU + sunSize, 0, 0));
        pluto->setMaterialTexture( 0, driver->getTexture("../textures/pluto.bmp") );
    }

	smgr->addCameraSceneNodeFPS(0, 100.0F, 0.005F, -1, 0, 0, false, 0.0F, false, true);

	u32 then = device->getTimer()->getTime();
    const f32 MOVEMENT_SPEED = (f32) 0.000001f;
	device->getCursorControl()->setVisible(false);

	CMyLightManager * myLightManager = new CMyLightManager(smgr);
    smgr->setLightManager(0); // This is the default: we won't do light management until told to do it.
    device->setEventReceiver(myLightManager);

	while(device->run())
    {
		// Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32) (now - then) / (f32) 1000.0f; // Time in seconds
        then = now;

		core::vector3df sunPosition = sun->getPosition();

		//Move camera
		if(receiver.IsKeyDown(irr::KEY_UP))
            sunPosition.Y += MOVEMENT_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_DOWN))
            sunPosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

		if(receiver.IsKeyDown(irr::KEY_LEFT))
            sunPosition.X -= MOVEMENT_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_RIGHT))
            sunPosition.X += (f32) (MOVEMENT_SPEED * frameDeltaTime);

		//Rotations
		t += (float) 0.1;
		sun->setPosition(sunPosition);
		
		//sun->setRotation(vector3df(0, t, 0));
		//jupiter->setRotation(vector3df(0, -t * (float) 5, 0));

		//render everything
		driver->beginScene(true, true, SColor(255,0,0,0));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
    }

    device->drop();

    return 0;
}