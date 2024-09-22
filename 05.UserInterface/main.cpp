/** Example 005 User Interface

This tutorial shows how to use the built in User Interface of
the Irrlicht Engine. It will give a brief overview and show
how to create and use windows, buttons, scroll bars, static
texts, and list boxes.

As always, we include the header files, and use the irrlicht
namespaces. We also store a pointer to the Irrlicht device,
a counter variable for changing the creation position of a window,
and a pointer to a listbox.
*/
#include <irrlicht.h>
#include <vector>
#include <SDL2/SDL.h>

#include <codecvt>
#include <strstream>

#include "driverChoice.h"
#include "exampleHelper.h"

#include "gui_freetype_font.h"
#include "rc_stdlib.h"
#include "rc_gfx.h"
#include "rc_gfx3D.h"
#include "camera.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

void drawDebugInfo(int overlay_canvas, int view_canvas)
{
	rc_setActiveCanvas(view_canvas);
	double cam_x, cam_y, cam_z;
	rc_getCameraPosition(&cam_x, &cam_y, &cam_z);
	std::string cam_pos = rc_intern_str(cam_x) + ", " + rc_intern_str(cam_y) + ", " + rc_intern_str(cam_z);

	double rot_x, rot_y, rot_z;
	rc_getCameraRotation(&rot_x, &rot_y, &rot_z);
	std::string cam_rot = rc_intern_str(rot_x) + ", " + rc_intern_str(rot_y) + ", " + rc_intern_str(rot_z);

	double mouse_x, mouse_y, mb1, mb2, mb3;
	rc_getMouse(&mouse_x, &mouse_y, &mb1, &mb2, &mb3);
	std::string mouse_info = "";
	mouse_info += rc_intern_str(mouse_x) + ", ";
	mouse_info += rc_intern_str(mouse_y) + "   ";
	mouse_info += "LEFT = " + rc_intern_str(mb1) + ", ";
	mouse_info += "MIDDLE = " + rc_intern_str(mb2) + ", ";
	mouse_info += "RIGHT = " + rc_intern_str(mb3) + ", ";

	rc_setActiveCanvas(overlay_canvas);
	rc_clearCanvas();
	rc_setColor(rc_rgb(255,255,255));

	rc_drawText("Position: " + cam_pos, 10, 10);
	rc_drawText("Rotation: " + cam_rot, 10, 30);
	rc_drawText("Mouse Info: " + mouse_info, 10, 50);
}

void test_matrix1()
{
	irr::core::matrix4 m;
	m.setTranslation(irr::core::vector3df(44, 55, 66));
	m.setRotationDegrees(irr::core::vector3df(20, 70, 30));

	std::cout << "rot euler = " << m.getTranslation().X << ", " << m.getTranslation().Y << ", " << m.getTranslation().Z << std::endl;

	std::cout << std::endl << "debug output 1" << std::endl;
	printIrrMatrix(m);

	irr::core::vector3df rot = m.getRotationDegrees();

	std::cout << std::endl;

	std::cout << "ROT = " << rot.X << ", " << rot.Y << ", " << rot.Z << std::endl;

	irr::core::matrix4 m2;
	m2.setRotationDegrees(irr::core::vector3df(11, 12, 14));
	m2.setTranslation(irr::core::vector3df(23, 22, 19));
	m2.setScale(irr::core::vector3df(2,3,4));

	std::cout << std::endl;

	std::cout << std::endl << "debug output 2" << std::endl;
	printIrrMatrix(m2);

	m = m * m2;

	std::cout << std::endl;

	std::cout << std::endl << "debug output 3" << std::endl;
	printIrrMatrix(m);

	std::cout << std::endl;

	//printMatrix(m);
}

void test_matrix2()
{
	double x;
	double y;
	double z;

	int m = DimMatrix(NEW_MATRIX, 4, 4);
	setIdentityMatrix(m, 4);
	rc_setMatrixTranslation(m, 44, 55, 66);
	rc_setMatrixRotation(m, 20, 70, 30);

	rc_getMatrixTranslation(m, &x, &y, &z);

	std::cout << "rot euler = " << x << ", " << y << ", " << z << std::endl;

	std::cout << std::endl << "debug output 1" << std::endl;
	printRCMatrix(m);

	rc_getMatrixRotation(m, &x, &y, &z);

	std::cout << std::endl;

	std::cout << "!!ROT = " << x << ", " << y << ", " << z << std::endl;

	int m2 = DimMatrix(NEW_MATRIX, 4, 4);
	setIdentityMatrix(m2, 4);
	rc_setMatrixRotation(m2, 11, 12, 14);
	rc_setMatrixTranslation(m2, 23, 22, 19);
	rc_setMatrixScale(m2, 2,3,4);

	std::cout << std::endl;

	std::cout << std::endl << "debug output 2" << std::endl;
	printRCMatrix(m2);

	int mC = DimMatrix(NEW_MATRIX, 4, 4);

	MultiplyMatrix(m, m2, mC);

	std::cout << std::endl << "debug output 3" << std::endl;
	printRCMatrix(mC);

	std::cout << std::endl;

	//printMatrix(m);
}

int main()
{

    rcbasic_init();

    rc_windowOpen("testing", 640, 480, false, true);


	SDL_Event event;
	bool quit = false;

	std::cout << "test start" << std::endl;

	uint32_t canvas1 = rc_canvasOpen(640, 480, 0, 0, 640, 480, 0);
	uint32_t canvas2 = rc_canvasOpen(640, 480, 0, 0, 640, 480, 0);

	rc_setCanvasZ(canvas2, 0);

	rc_setCanvas3D(canvas1, true);

    std::string fnt = "NotoSansJP-VariableFont_wght.ttf";
    rc_loadFont(fnt, 12);


    int mesh1 = rc_loadMesh("../../media/sydney.md2");

    int actor1 = rc_createMeshActor(mesh1);
    int actor1_texture = rc_loadImage("../../media/sydney.bmp");
    int mat = rc_createMaterial();
    rc_setMaterialTexture(mat, 0, actor1_texture);
    rc_setMaterialLighting(mat, false);
	rc_setActorMaterial(actor1, 0, mat);

	int a_mat = rc_getActorMaterial(actor1, 0);
	rc_setMaterialLighting(a_mat, true);

    //rc_setActorTexture(actor1, 0, actor1_texture);
    //rc_setActorMaterialFlag(actor1, EMF_LIGHTING, false);

    rc_setActorSolid(actor1, true);
    rc_setActorCollisionShape(actor1, RC_NODE_SHAPE_TYPE_CAPSULE, 25);
    rc_translateActor(actor1, 0, 150, 0);

	int level = rc_loadMeshFromArchive("../../media/map-20kdm2.pk3", "20kdm2.bsp");
	int actor2 = -1;

	if (level >= 0)
	{
		actor2 = rc_createMeshOctreeActor(level);
		rc_setActorSolid(actor2, true);
		rc_setActorCollisionShape(actor2, RC_NODE_SHAPE_TYPE_TRIMESH, 0);
	}

	double ax, ay, az;

	rc_setActiveCanvas(canvas1);
	rc_setActorPosition(actor1, 1160, 399, 2122);
	rc_setActorRotation(actor1, 0, 0, 0);
	//rc_setActorCollisionShape(actor1, rc_actor[actor1].physics.shape_type, 1);
	//rc_setActorSolid(actor1, true);
	//rc_setActorMassProperties(actor1, 1, 0, 0, 0);
	//rc_applyActorTorqueImpulseWorld(actor1, 0, 0, 0);

	rc_setCameraPosition(984, 488, 2303);
	rc_setCameraRotation(23, 1216, 0);

	bool init = true;
	int i = 0;

	//rc_setActorSolid(actor1, true);

	while(rc_update())
	{

		rc_setActorAngularVelocityWorld(actor1, 0, 0, 0);

		if(rc_key(SDLK_p) && init)
		{
			//rc_setActorSolid(actor1, true);
			double x, y, z;
			rc_getActorLocalInertia(actor1, &x, &y, &z);
			btVector3 v;
			double mass = 8;
			rc_actor[actor1].physics.rigid_body->getPointer()->getCollisionShape()->calculateLocalInertia(mass, v);
			std::cout << "Set Mass: " << x << ", " << y << ", " << z << std::endl;
			std::cout << "Set Vect: " << v.getX() << ", " << v.getY() << ", " << v.getZ() << std::endl;
			//rc_physics3D.world->getPointer()->removeRigidBody(rc_actor[actor1].physics.rigid_body->getPointer());
			//rc_actor[actor1].physics.rigid_body->getPointer()->setMassProps(mass, v);
			//rc_physics3D.world->getPointer()->addRigidBody(rc_actor[actor1].physics.rigid_body->getPointer());


			rc_setActorMassProperties(actor1, mass, v.getX(), v.getY(), v.getZ());
			rc_setActorGravity(actor1, 0, -100, 0);
			//init = false;
		}


		if(rc_key(SDLK_ESCAPE))
			break;

		if(rc_key(SDLK_w))
        {
            rc_setActiveCanvas(canvas1);
            rc_translateCamera(0,0,10);
        }
        else if(rc_key(SDLK_s))
        {
            rc_setActiveCanvas(canvas1);
            rc_translateCamera(0,0,-10);
        }

        if(rc_key(SDLK_a))
        {
            rc_setActiveCanvas(canvas1);
            rc_translateCamera(-10,0,0);
        }
        else if(rc_key(SDLK_d))
        {
            rc_setActiveCanvas(canvas1);
            rc_translateCamera(10,0,0);
        }

        if(rc_key(SDLK_r))
		{
			rc_setActiveCanvas(canvas1);

			double crx, cry, crz;
            rc_getCameraPosition(&crx, &cry, &crz);

            rc_setCameraPosition(crx, cry+10, crz);

			//rc_translateCameraW(0, 10, 0);
		}
		else if(rc_key(SDLK_f))
		{
			rc_setActiveCanvas(canvas1);
			rc_translateCameraW(0, -10, 0);
		}


        if(rc_key(SDLK_UP))
        {
            rc_setActiveCanvas(canvas1);
            rc_rotateCamera(1, 0, 0);
        }
        else if(rc_key(SDLK_DOWN))
        {
            rc_setActiveCanvas(canvas1);
			rc_rotateCamera(-1, 0, 0);
        }

        if(rc_key(SDLK_LEFT))
        {
            rc_setActiveCanvas(canvas1);

            double crx, cry, crz;
            rc_getCameraRotation(&crx, &cry, &crz);

			rc_rotateCamera(-1*crx, 0, 0);
			rc_rotateCamera(0, -1, 0);
			rc_rotateCamera(crx, 0, 0);
        }
        else if(rc_key(SDLK_RIGHT))
        {
            rc_setActiveCanvas(canvas1);

            double crx, cry, crz;
            rc_getCameraRotation(&crx, &cry, &crz);

			rc_setCameraRotation(crx, cry+1, crz);
			//rc_rotateCamera(-1*crx, 0, 0);
			//rc_rotateCamera(0, 1, 0);
			//rc_rotateCamera(crx, 0, 0);
        }

        if(rc_key(SDLK_g))
		{
			rc_setActorAngularVelocityLocal(actor1, 0, 10, 0);
			//rc_translateActorWorld(actor1, 0, -5, 0);
			//rc_applyActorTorqueImpulseLocal(actor1, 0, 30, 0);
			//rc_applyActorTorqueWorld(actor1, 0, 120, 0);
			//rc_rotateActor(actor1, 0, -5, 0);
			//ay += 5;
			//rc_setActorRotation(actor1, ax, ay, az);
		}

		if(rc_key(SDLK_b))
		{
			rc_setActorLinearVelocityLocal(actor1, 80, 0, 0);
			//rc_setActorAngularFactor(actor1, 0, 0, 0);
			//rc_applyActorCentralForceWorld(actor1, 0, 0, 10);
		}

		if(rc_key(SDLK_n))
		{
			rc_setActorLinearVelocityLocal(actor1, 0, 120, 0);
			//rc_setActorAngularFactor(actor1, 0, 0, 0);
			//rc_applyActorCentralForceWorld(actor1, 0, 0, 10);
		}


        drawDebugInfo(canvas2, canvas1);

	}

	std::cout << "test end" << std::endl;

	SDL_DestroyWindow(rc_window);
	SDL_Quit();
	device->drop();

	return 0;
}

/*
**/
