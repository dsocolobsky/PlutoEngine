#include <irrlicht.h>
#include <iostream>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// Neccessary to load the .dll
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
	// Ask use to choice Graphic Driver
	
	video::E_DRIVER_TYPE driverType; // Declares the Driver
	// Asks for Driver
	std::cout << "Select the driver you want: " << std::endl;
	std::cout << "	(a)OpenGL (b)Direct3D 9 (c)Direct3D 8" << std::endl;
	std::cout << "	(d)Burning's Software (e)Software (f)Null" << std::endl;
	
	char i; // Used for option
	std::cin >> i; // We ask

	switch(i)
	{
		case 'a': driverType = video::EDT_OPENGL;   break;
        case 'b': driverType = video::EDT_DIRECT3D9; break;
        case 'c': driverType = video::EDT_DIRECT3D8; break;
        case 'd': driverType = video::EDT_BURNINGSVIDEO; break;
        case 'e': driverType = video::EDT_SOFTWARE; break;
        case 'f': driverType = video::EDT_NULL;     break;
        default: return 1;
	}
	
	// Create device
	IrrlichtDevice *device = createDevice(driverType, dimension2d<u32>(640,480));

	// If there's an error with the device, or this doesn't exist: Exit with an error
	if (!device)
		return 1;

	//Get a pointer to the video driver and the SceneManager to make code shorter
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// Load Quake3 Map, wich is just a renamed .zip file
	device->getFileSystem()->addZipFileArchive("media/map-20kdm2.pk3");

	// We load the mesh to 'mesh' wich is a pointer type IAnimatedMesh
	scene::IAnimatedMesh* mesh = smgr->getMesh("media/20kdm2.bsp");
	scene::ISceneNode* node = 0; // Node for optimization
	if(mesh)
	{
		// Octree scene node for more optimization
		node = smgr->addOctreeSceneNode(mesh->getMesh(0),0,-1,1024);
		// Try this:
		// node = smgr->addMeshSceneNode(mesh->getMesh(0));
	}

	// We move the node a little bit
	if(node)
		node->setPosition(core::vector3df(-1300,-144,-1249));

	// We create the user-controlled camera
	smgr->addCameraSceneNodeFPS(); // FPS-Like Camera
	device->getCursorControl()->setVisible(false); // Hide the cursor

	// Code for drawing the window
	int lastFPS = -1; // Last FPS to keep track of current FPS

	//Main Loop
	while(device->run())
	{
		if(device->isWindowActive())
		{
			// We begin the scene
			driver->beginScene(true,true,video::SColor(255,200,200,200));
			smgr->drawAll(); // We draw all from the Scene Manager
			driver->endScene(); // We end the scene

			int fps = driver->getFPS();
			if(lastFPS != fps)
			{
				core::stringw str = L"FPS: [";
				str += fps;
				str += "]";

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		} else
		{
			device->yield();
		}
		device->drop();
		return 0;
	}
}