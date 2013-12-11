#include <irrlicht.h>
#include <iostream>
#include <math.h>
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

const float sunSize = (float) 10.0;
const float AU = (float) sunSize + (float) 10.0;
const f32 year = (f32) 0.00001;

bool AddObjects(IrrlichtDevice *device, IVideoDriver* driver, ISceneManager* smgr )
{
	IMeshSceneNode* sun = smgr->addSphereSceneNode(sunSize, 50, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (sun)
    {
		sun->setPosition(vector3df(0,0,30));
        sun->setMaterialFlag(EMF_LIGHTING, false);
        sun->setMaterialTexture( 0, driver->getTexture("../textures/sun_texture_big.BMP") );
		smgr->addLightSceneNode(sun, vector3df(0, 0, 0), SColorf(1.0f, 1.0f, 1.0f), sunSize, -1);
		ISceneNodeAnimator* orbit = smgr->createRotationAnimator(vector3df(0.f, 0.1f, 0.f));
		sun->addAnimator(orbit);
    }

	IMeshSceneNode* mercury = smgr->addSphereSceneNode(sunSize, 5, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (mercury)
    {
		//const float mercurySize = sunSize / (277.0f * sunSize);
		const float mercurySize = sunSize / (11.0f * sunSize);
        mercury->setMaterialFlag(EMF_LIGHTING, true);
		mercury->setParent(sun);
		mercury->setScale(vector3df(mercurySize, mercurySize, mercurySize));
		//mercury->setPosition(vector3df(0.38f * AU, 0, 0));
		mercury->setPosition(vector3df(0.78f * AU, 0, 0));
        mercury->setMaterialTexture( 0, driver->getTexture("../textures/sun_texture_big.bmp") );
		ISceneNodeAnimator* orbit = smgr->createFlyCircleAnimator(vector3df(0.f, 1, 0.f),0.38f * AU, year * (f32) 2.1f, vector3df(0.f, 1, 0.f), 100.0f, 0.0f);
		mercury->addAnimator(orbit);
    }

	IMeshSceneNode* venus = smgr->addSphereSceneNode(sunSize, 5, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (venus)
    {
		const float venusSize = sunSize / (113.0f * sunSize);
        venus->setMaterialFlag(EMF_LIGHTING, true);
		venus->setParent(sun);
		venus->setScale(vector3df(venusSize, venusSize, venusSize));
		venus->setPosition(vector3df(0.72f * AU, 0, 0));
        venus->setMaterialTexture( 0, driver->getTexture("../textures/venus.bmp") );
		ISceneNodeAnimator* orbit = smgr->createFlyCircleAnimator(vector3df(0.f, 1, 0.f),0.72f * AU, year * (f32) 1.5f, vector3df(0.f, 1, 0.f), 100.0f, 0.0f);
		venus->addAnimator(orbit);
    }

	IMeshSceneNode* earth = smgr->addSphereSceneNode(sunSize, 10, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (earth)
    {
		const float earthSize = sunSize / (100.0f * sunSize);
        earth->setMaterialFlag(EMF_LIGHTING, true);
		earth->setParent(sun);
		earth->setScale(vector3df(earthSize, earthSize, earthSize));
		earth->setPosition(vector3df(AU, 0, 0));
        earth->setMaterialTexture( 0, driver->getTexture("../textures/earth.bmp") );
		ISceneNodeAnimator* orbit = smgr->createFlyCircleAnimator(vector3df(0.f, 1, 0.f), AU, year, vector3df(0.f, 1, 0.f), 100.0f, 0.0f);
		earth->addAnimator(orbit);
	}

	IMeshSceneNode* moon = smgr->addSphereSceneNode(sunSize, 5, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (moon)
    {
		const float moonSize = sunSize * 75.0f / (416.0f * sunSize); //TODO
		const float mAU = 0.5f; //TODO
        moon->setMaterialFlag(EMF_LIGHTING, true);
		moon->setParent(earth);
		moon->setScale(vector3df(moonSize, moonSize, moonSize));
		moon->setPosition(vector3df(16 * mAU + sunSize, 0, 0));
        moon->setMaterialTexture( 0, driver->getTexture("../textures/moonTexture.BMP") );
		ISceneNodeAnimator* orbit = smgr->createFlyCircleAnimator(vector3df(0.f, 1, 0.f), 16 * mAU + sunSize, year * (f32) 365.0f, vector3df(0.f, 1, 0.f), 100.0f, 0.0f);
		moon->addAnimator(orbit);
    }

	IMeshSceneNode* mars = smgr->addSphereSceneNode(sunSize, 5, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (mars)
    {
		const float marsSize = sunSize / (208.0f * sunSize);
        mars->setMaterialFlag(EMF_LIGHTING, true);
		mars->setParent(sun);
		mars->setScale(vector3df(marsSize, marsSize, marsSize));
		mars->setPosition(vector3df(1.52f * AU, 0, 0));
        mars->setMaterialTexture( 0, driver->getTexture("../textures/mars.bmp") );
		ISceneNodeAnimator* orbit = smgr->createFlyCircleAnimator(vector3df(0.f, 1, 0.f),1.52f * AU, year / (f32) 1.8f, vector3df(0.f, 1, 0.f), 100.0f, 0.0f);
		mars->addAnimator(orbit);
    }

	IMeshSceneNode* jupiter = smgr->addSphereSceneNode(sunSize, 20, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (jupiter)
    {
		const float jupiterSize = sunSize / (10.0f * sunSize);
        jupiter->setMaterialFlag(EMF_LIGHTING, true);
		jupiter->setParent(sun);
		jupiter->setScale(vector3df(jupiterSize, jupiterSize, jupiterSize));
		jupiter->setPosition(vector3df(5.2f * AU, 0, 0));
        jupiter->setMaterialTexture( 0, driver->getTexture("../textures/Jupiter.bmp") );
		ISceneNodeAnimator* orbit = smgr->createFlyCircleAnimator(vector3df(0.f, 1, 0.f),5.2f * AU, year / (f32) 12.0f, vector3df(0.f, 1, 0.f), 100.0f, 0.0f);
		jupiter->addAnimator(orbit);
    }

	IMeshSceneNode* saturn = smgr->addSphereSceneNode(sunSize, 10, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (saturn)
    {
		const float saturnSize = sunSize / (11.4f * sunSize);
        saturn->setMaterialFlag(EMF_LIGHTING, true);
		saturn->setParent(sun);
		saturn->setScale(vector3df(saturnSize, saturnSize, saturnSize));
		saturn->setPosition(vector3df(9.5f * AU, 0, 0));
        saturn->setMaterialTexture( 0, driver->getTexture("../textures/saturn.bmp") );
		ISceneNodeAnimator* orbit = smgr->createFlyCircleAnimator(vector3df(0.f, 1, 0.f),9.5f * AU, year / (f32) 12.0f, vector3df(0.f, 1, 0.f), 100.0f, 0.0f);
		saturn->addAnimator(orbit);
    }

	IMeshSceneNode* uranus = smgr->addSphereSceneNode(sunSize, 10, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (uranus)
    {
		const float uranusSize = sunSize / (26.8f * sunSize);
        uranus->setMaterialFlag(EMF_LIGHTING, true);
		uranus->setParent(sun);
		uranus->setScale(vector3df(uranusSize, uranusSize, uranusSize));
		uranus->setPosition(vector3df(19.2f * AU, 0, 0));
        uranus->setMaterialTexture( 0, driver->getTexture("../textures/uran.bmp") );
		ISceneNodeAnimator* orbit = smgr->createFlyCircleAnimator(vector3df(0.f, 1, 0.f),19.2f * AU, year / (f32) 84.0f, vector3df(0.f, 1, 0.f), 100.0f, 0.0f);
		uranus->addAnimator(orbit);
    }

	IMeshSceneNode* neptune = smgr->addSphereSceneNode(sunSize, 5, NULL, -1, vector3df(0.0f, 0.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 1.0f, 1.0f));
	if (neptune)
    {
		const float neptuneSize = sunSize / (27.7f * sunSize);
        neptune->setMaterialFlag(EMF_LIGHTING, true);
		neptune->setParent(sun);
		neptune->setScale(vector3df(neptuneSize, neptuneSize, neptuneSize));
		neptune->setPosition(vector3df(30.0f * AU, 0, 0));
        neptune->setMaterialTexture( 0, driver->getTexture("../textures/neptun.bmp") );
		ISceneNodeAnimator* orbit = smgr->createFlyCircleAnimator(vector3df(0.f, 1, 0.f), 30.0f * AU, year / (f32) 165.0f, vector3df(0.f, 1, 0.f), 100.0f, 0.0f);
		neptune->addAnimator(orbit);
    }

	IMesh* star = smgr->getMesh("../objects/deathstar.3ds");
	IMesh* mesh = smgr->getMesh("../objects/asteroid.ply");
    if (!star || !mesh)
    {
        device->drop();
        return false;
    }
	
	IMeshSceneNode* asteroid = smgr->addMeshSceneNode(mesh);	
	IMeshSceneNode* dstar = smgr->addMeshSceneNode(star);
	if (dstar)
    {
		const float asteroidSize = sunSize / (5.0f * sunSize);
		const float pAU = 6.0f;
        dstar->setMaterialFlag(EMF_LIGHTING, true);
		dstar->setMaterialFlag(EMF_GOURAUD_SHADING, true);
		dstar->setScale(vector3df(asteroidSize, asteroidSize, asteroidSize));
		dstar->setPosition(vector3df(2 * pAU + sunSize, 0, 0));
		//ISceneNodeAnimator* crash = smgr->createCollisionResponseAnimator(0, asteroid, vector3df(30 ,60 ,30), vector3df(0, -10.0f, 0), vector3df(0,0,0), 0.0005);
		ISceneNodeAnimator* orbit = smgr->createRotationAnimator(vector3df(0.f, 0.2f, 0.f));
		dstar->addAnimator(orbit);
        dstar->setMaterialTexture( 0, driver->getTexture("../textures/ds.BMP") );
    }

	if (asteroid)
    {
		vector3df dstarPos = dstar->getPosition();
		dstarPos.Y += (f32) 2.5;
		const float asteroidSize = sunSize / (5.0f * sunSize);
        asteroid->setMaterialFlag(EMF_LIGHTING, true);
		asteroid->setMaterialFlag(EMF_GOURAUD_SHADING, true);
		asteroid->setScale(vector3df(asteroidSize, asteroidSize, asteroidSize));
		asteroid->setPosition(vector3df(10.0f * AU, 0, 0));
        asteroid->setMaterialTexture( 0, driver->getTexture("../textures/asteroid.BMP") );
		ISceneNodeAnimator* orbit = smgr->createFlyStraightAnimator(asteroid->getPosition(), dstarPos, 20 * 1000, true, false);
		asteroid->addAnimator(orbit);
    }

	return true;
}

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

	smgr->addCameraSceneNodeFPS(0, 100.0F, 0.005F, -1, 0, 0, false, 0.0F, false, true);
	device->getCursorControl()->setVisible(false);

	if(!AddObjects(device, driver, smgr)) return 1;

	CMyLightManager * myLightManager = new CMyLightManager(smgr);
    smgr->setLightManager(0); // This is the default: we won't do light management until told to do it.
    device->setEventReceiver(myLightManager);

	while(device->run())
    {
		//render everything
		driver->beginScene(true, true, SColor(255,0,0,0));
        smgr->drawAll();
        driver->endScene();
    }

    device->drop();

    return 0;
}