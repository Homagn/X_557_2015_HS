//
//  main_spotlight.cpp
//  HCI 557 Spotlight example
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Sphere3D.h"
#include "Textures.h"
//#include "Texture.h"

#include "GLObjectObj.h"
#include "RayIntersectionTest.h"

using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;
extern int g_change_texture_blend;
//character movement variables
extern bool mw;
extern float movew;

extern bool rot;
extern float rota;
extern float deg;

GLObjectObj* loadedModel1;
glm::mat4 tranform3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 25.0f, -10.0f));
glm::mat4 tranform99;
glm::mat4 poketran = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 6.0, 0.0));
//camera
extern float ch;
extern float cd;
extern float cr;

extern int cammode;
extern int pokthrow;
extern int ooo;
extern int ppp;
extern glm::vec3 cra;

glm::mat4 movecharacter(glm::mat4 transformT, float zz, float yy)
{

	transformT = transformT*glm::translate(glm::mat4(1.0f), glm::vec3(zz*sin(yy), 0.0f, zz*cos(yy)));
	//tranform3 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f - zz*sin(yy), 25.0f, -10.0f - zz*cos(yy)));
	transformT = transformT*glm::rotate(glm::mat4(1.0f), yy, glm::vec3(0.0f, 1.0f, 0.0f));
	//tranform3 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + zz*sin(yy), 25.0f, -10.0f + zz*cos(yy)));
	//transformT = transformT*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
	
	return transformT;
	
}
void rotatecharacter(float zz,float yy)
{
	tranform3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f , 25.0f, -10.0f));
	tranform3 = tranform3*glm::rotate(glm::mat4(1.0f), yy, glm::vec3(0.0f, 1.0f, 0.0f));
	tranform3 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + zz*sin(yy), 25.0f, -10.0f + zz*cos(yy)));
	tranform3 = tranform3*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
}



int main(int argc, const char * argv[])
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Init glfw, create a window, and init glew

	// Init the GLFW Window
	window = initWindow();


	// Init the glew api
	initGlew();
	SetCameraManipulator(CameraTypes::CAMERA_MANIPULATOR);



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Create some models

	// coordinate system
	CoordSystem* cs = new CoordSystem(40.0);


	// create an apperance object.
	vector<GLAppearance*> appearance;
	GLAppearance* apperance_0 = new GLAppearance("../data/shaders/multi_texture.vs", "../data/shaders/multi_texture.fs");
	GLAppearance* apperance_1 = new GLAppearance("../data/shaders/multi_texture.vs", "../data/shaders/multi_texture.fs");
	GLAppearance* apperance_2 = new GLAppearance("../data/shaders/multi_texture.vs", "../data/shaders/multi_texture.fs");
	GLAppearance* apperance_3 = new GLAppearance("../data/shaders/multi_texture.vs", "../data/shaders/multi_texture.fs");
	GLAppearance* apperance_4 = new GLAppearance("../data/shaders/multi_texture.vs", "../data/shaders/multi_texture.fs");
	appearance.push_back(apperance_0);// The fs and vs are capable of handling only 3 textures at once, so we need a vector of appearances
	GLDirectLightSource  light_source;
	light_source._lightPos = glm::vec4(20.0, 20.0, 0.0, 0.0);
	light_source._ambient_intensity = 0.2;
	light_source._specular_intensity = 5.5;
	light_source._diffuse_intensity = 2.0;
	light_source._attenuation_coeff = 0.0;

	// add the light to this apperance object
	apperance_0->addLightSource(light_source);
	apperance_1->addLightSource(light_source);
	apperance_2->addLightSource(light_source);//shirt color
	apperance_3->addLightSource(light_source);//skin color
	apperance_4->addLightSource(light_source);//pant color
	// Create a material object
	GLMaterial material_0;
	material_0._diffuse_material = glm::vec3(0.0, 0.0, 0.0);
	material_0._ambient_material = glm::vec3(0.0, 0.0, 0.0);
	material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_0._shininess = 1000.0;
	material_0._transparency = 1.0;

	GLMaterial material_1;//shirt material
	material_1._diffuse_material = glm::vec3(1.0, 0.0, 0.0);
	material_1._ambient_material = glm::vec3(1.0, 0.0, 0.0);
	material_1._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_1._shininess = 1000.0;
	material_1._transparency = 1.0;

	GLMaterial material_2;//skin material
	material_2._diffuse_material = glm::vec3(1.2, 0.50, 0.30);
	material_2._ambient_material = glm::vec3(1.2, 0.50, 0.30);
	material_2._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_2._shininess = 1000.0;
	material_2._transparency = 1.0;

	GLMaterial material_3;//pant material
	material_3._diffuse_material = glm::vec3(0.0, 0.0, 1.0);
	material_3._ambient_material = glm::vec3(0.0, 0.0, 1.0);
	material_3._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_3._shininess = 1000.0;
	material_3._transparency = 1.0;

	// Add the material to the apperance object
	apperance_0->setMaterial(material_0);
	apperance_1->setMaterial(material_0);
	apperance_2->setMaterial(material_1);
	apperance_3->setMaterial(material_2);
	apperance_4->setMaterial(material_3);
	//load texture here




	// If you want to change appearance parameters after you init the object, call the update function
	apperance_0->updateLightSources();
	apperance_1->updateLightSources();
	apperance_2->updateLightSources();
	apperance_3->updateLightSources();
	apperance_4->updateLightSources();
	//change here

	
	
	GLObjectObj* loadedModel1 = new GLObjectObj("data/World/pokemonworld.obj");
	loadedModel1->setApperance(*apperance_1);
	//loadedModel1->setobjno(0);
	loadedModel1->init();
	loadedModel1->setobjno(0);
	//loadedModel1->setobjno(1);

	GLObjectObj* loadedModel111 = new GLObjectObj("data/World/collision_map.obj");
	loadedModel111->setApperance(*apperance_3);
	//loadedModel1->setobjno(0);
	loadedModel111->init();
	loadedModel111->create_contour_map();

	GLObjectObj* loadedModel22 = new GLObjectObj("data/human/tracker.obj");
	loadedModel22->setApperance(*apperance_4);
	//loadedModel1->setobjno(0);
	loadedModel22->init();
	

	GLObjectObj* loadedModel2 = new GLObjectObj("data/001 - Bulbasaur/Bulbasaur.obj");
	loadedModel2->setApperance(*apperance_1);
	loadedModel2->init();
	int size = loadedModel2->getsize(0);

	GLObjectObj* loadedModel20 = new GLObjectObj("data/007 - Squirtle/Squirtle.obj");
	loadedModel2->setApperance(*apperance_0);
	loadedModel2->init();
	
	GLObjectObj* loadedModel3 = new GLObjectObj("data/human/torso.obj");
	loadedModel3->setApperance(*apperance_2);
	loadedModel3->init();

	GLObjectObj* loadedModel4 = new GLObjectObj("data/human/head.obj");//
	loadedModel4->setApperance(*apperance_3);
	loadedModel4->init();

	GLObjectObj* loadedModel5 = new GLObjectObj("data/human/legright.obj");
	loadedModel5->setApperance(*apperance_4);
	loadedModel5->init();

	GLObjectObj* loadedModel6 = new GLObjectObj("data/human/legleft.obj");
	loadedModel6->setApperance(*apperance_4);
	loadedModel6->init();

	GLObjectObj* loadedModel7 = new GLObjectObj("data/human/footright.obj");
	loadedModel7->setApperance(*apperance_3);
	loadedModel7->init();

	GLObjectObj* loadedModel8 = new GLObjectObj("data/human/footleft.obj");
	loadedModel8->setApperance(*apperance_3);
	loadedModel8->init();

	GLObjectObj* loadedModel9 = new GLObjectObj("data/human/elbowright.obj");
	loadedModel9->setApperance(*apperance_3);
	loadedModel9->init();

	GLObjectObj* loadedModel10 = new GLObjectObj("data/human/elbowleft.obj");
	loadedModel10->setApperance(*apperance_3);
	loadedModel10->init();

	GLObjectObj* loadedModel11 = new GLObjectObj("data/human/handright.obj");
	loadedModel11->setApperance(*apperance_3);
	loadedModel11->init();

	GLObjectObj* loadedModel12 = new GLObjectObj("data/human/handleft.obj");
	loadedModel12->setApperance(*apperance_3);
	loadedModel12->init();

	GLSphere* pokeball = new GLSphere();
	pokeball->GLSphere1(-8.4, 10, 0, 1, 20, 20);
	//loading the texture
	// Add a texture
	GLMultiTexture* texture = new GLMultiTexture();
	
	
	texture->changemode(0);
	
	
	for (int i = 0; i < 1; i++)//can be extended for several textures into groups of 3
	{
	string inpstring1 = loadedModel20->getlocation(0);
	string inpstring2 = loadedModel20->getlocation(0);
	string inpstring3 = loadedModel20->getlocation(0);
	cout << inpstring1;
	cout << inpstring2;
	cout << inpstring3;

	
	int texid1 = texture->loadAndCreateTextures(inpstring1, inpstring2, inpstring3, 0);//vertex shader and fragment shader code must be able to handle upto 20 textures
    }
	apperance_0->setTexture(texture);
	GLMultiTexture* texture1 = new GLMultiTexture();
	
	for (int i = 0; i < 1; i++)//can be extended for several textures into groups of 3
	{
		string inpstring1 = loadedModel2->getlocation(0);
		string inpstring2 = loadedModel2->getlocation(0);
		string inpstring3 = loadedModel1->getlocation(0);
		cout << inpstring1;
		cout << inpstring2;
		cout << inpstring3;
		int texid2 = texture1->loadAndCreateTextures(inpstring1, inpstring2, inpstring3, 1);//vertex shader and fragment shader code must be able to handle upto 20 textures
	}
	//int texid = texture->loadAndCreateTexture("../../data/textures/texture_earth_128x128_a.bmp");
	
	apperance_1->setTexture(texture1);
	apperance_2->setTexture(texture1);//set the dummy texture
	apperance_3->setTexture(texture1);//set the dummy texture
	apperance_4->setTexture(texture1);//set the dummy texture
	//************************************************************************************************
	// Finalize the appearance object
	//apperance_0->finalize();
	apperance_1->finalize();
	apperance_2->finalize();
	apperance_3->finalize();
	apperance_4->finalize();
    
	glm::mat4 tranform = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	tranform = tranform*glm::scale(glm::vec3(25.0, 25.0f, 25.0f));
    loadedModel1->setMatrix(tranform);

	glm::mat4 tranform1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0, 0.0f));
	tranform1 = tranform1*glm::scale(glm::vec3(30.0, 30.0f, 30.0f));
	loadedModel2->setMatrix(tranform1);

	glm::mat4 tranform2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 25.0f, -10.0f));
	tranform2 = tranform2*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
	loadedModel3->setMatrix(tranform2);

	//tranform3 = tranform2;
	loadedModel4->setMatrix(tranform3);

	loadedModel5->setMatrix(tranform3);
	
	loadedModel6->setMatrix(tranform3);

	loadedModel7->setMatrix(tranform3);

	loadedModel8->setMatrix(tranform3);

	loadedModel9->setMatrix(tranform3);

	loadedModel10->setMatrix(tranform3);

	loadedModel11->setMatrix(tranform3);

	loadedModel12->setMatrix(tranform3);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.5f, 0.6f, 0.6f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(20.0f, 20.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Blending
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
	float k = 0.0;
	float j = 0.0;
	float n = 0.0;
	bool thrown = true;
	bool ballthrow = false;
	float kk = 0.0;
	float jjj = 0.0;

	float ffff = 0.0;
	float hhh = 0.0;
	bool lockff = false;
	bool lockhh = false;

	float p = 0.0;
	float h = 0.0;//human
	float m = 0.006;//human
	float r = 0.0;//human
	float rl = 0.0;//human

	bool pokecaught = false;
	bool pokecaughtsq = true;
	bool respawn = false;
	glm::vec3 pokepos;
	glm::vec3 pokepos1;
	
	glm::mat4 tranform10 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + movew*sin(rota), 25.0f, -10.0f + movew*cos(rota)));
	//tranform10 = tranform10*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + movew*sin(rota), 25.0f, -10.0f + movew*cos(rota)));
	//tranform10 = tranform10*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f - movew*sin(rota), 25.0f, -10.0f - movew*cos(rota)));
	//tranform10 = tranform10*glm::rotate(glm::mat4(1.0f), rota, glm::vec3(0.0f, 1.0f, 0.0f));
	//tranform10 = tranform10*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + movew*sin(rota), 25.0f, -10.0f + movew*cos(rota)));
	tranform10 = tranform10*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
	tranform10 = movecharacter(tranform10,movew, rota);
	tranform99 = tranform10;

	int patience = 0;
	int jj = 0;
	//glm::mat4 camera_transformation = glm::lookAt(glm::vec3(1.5f, 0.0f, 2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 camera_transformation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f , 30.0f, 0.0f));
	glm::mat4 object_transformation = glm::mat4();
	glm::mat4 camera_matrix = glm::mat4();
	glm::mat4 camera_delta = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    while(!glfwWindowShouldClose(window))
    {
		jj++;
		if (jj == 300)
		{
			jj = 0;
		}
		if (thrown == true)
		{
			k = k + 1;
			n = n + 0.0314;
		}
		if (ballthrow == true)
		{
			kk = kk + 1;
		}
		if (kk == 200)
		{
			kk = 0.0;
			ballthrow = false;
			lockff = false;
			lockhh = false;
		}
		if (k == 200)
		{
			k = 0.0;
			n = 0.0;
			thrown = false;
			
		}
		if (thrown == false)
		{
			p = p + 1;
		}
		if (p == 2000)
		{
			p = 0;
			thrown = true;

		}

			 j = (0.2*k - 0.5 * 0.0002 * k*k)*(200-k)*0.02;
			
			 jjj = (0.2*kk - 0.5 * 0.0002 * kk*kk)*(200 - kk)*0.02;
			 /*if (kk > 100)
			 {
				 jjj = jjj*-1;
			 }*/
		
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
		SetTrackballLocation(GetCurrentCameraMatrix(), GetCurrentCameraTranslation(), glm::vec3(sin(8*deg), 0.0f, cos(8*deg)));
        
        // draw the objects

		//change here
		//glm::mat4 tranform1 = glm::rotate(glm::mat4(1.0f), k, glm::vec3(1.0f, 0.0f, 0.0f));
		if (respawn == false)
		{
			glm::mat4 tranform1 = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 22 + j, 0.0f));
			tranform1 = tranform1*glm::rotate(glm::mat4(1.0f), 3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
			tranform1 = tranform1*glm::rotate(glm::mat4(1.0f), n, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform1 = tranform1*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
			pokepos.x = tranform1[3][0];
			pokepos.y = tranform1[3][1];
			pokepos.z = tranform1[3][2];
			loadedModel2->setMatrix(tranform1);
		}
		if (respawn == true)
		{
			glm::mat4 tranform1 = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 22 + j, -20.0f));
			tranform1 = tranform1*glm::rotate(glm::mat4(1.0f), 1.14f, glm::vec3(0.0f, 1.0f, 0.0f));
			tranform1 = tranform1*glm::rotate(glm::mat4(1.0f), n, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform1 = tranform1*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
			pokepos.x = tranform1[3][0];
			pokepos.y = tranform1[3][1];
			pokepos.z = tranform1[3][2];
			loadedModel2->setMatrix(tranform1);
		}

		glm::mat4 tranform100 = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 22 + j, 0.0f));
		tranform100 = tranform100*glm::rotate(glm::mat4(1.0f), 3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
		tranform100 = tranform100*glm::rotate(glm::mat4(1.0f), n, glm::vec3(1.0f, 0.0f, 0.0f));
		tranform100 = tranform100*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
		pokepos1.x = tranform100[3][0];
		pokepos1.y = tranform100[3][1];
		pokepos1.z = tranform100[3][2];
		loadedModel20->setMatrix(tranform100);


		/////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////
		h = h + m;
		r = h;
		rl = h;
		if (h > 0.8)
		{
			m=m*-1;
			//h = 0;
		}
		if (h < -0.8)
		{
			m = m*-1;
			//h = 0;
		}
		if (h < 0)
		{
			r = 0;
			rl = -h;
		}

		tranform10 = movecharacter(tranform10, movew, rota);

		
		
		if (mw == true)
		{
			int s1 = 0;
			int s2 = 0;

			if (tranform[3][1] > 25 && tranform[3][1] < 30)
			{
				s1 = 0;
				s2 = 20;
			}
			else if (tranform[3][1] > 30 && tranform[3][1] < 100)
			{
				s1 = 90;
				s2 = 60;
			}
			else if (tranform[3][1] < 20 && tranform[3][1] > 10)
			{
				s1 = 0;
				s2 = 90;
			}
			if (jj % 4 == 0)
			{
				tranform3 = tranform10;

				glm::vec3 s(tranform3[3][0], tranform3[3][1]+90, tranform3[3][2]);
				glm::vec3 e(tranform10[3][0] , tranform10[3][1] - 90, tranform10[3][2] );
				vector<glm::vec3> res;

				// perform the ray intersectiont test.
				RayIntersectionTest::intersect(s, e, *loadedModel111, res);

				// pick the first result if one is available
				if (res.size() > 0) {
					glm::vec3 position = res[0];

					//tranform3 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -position[1]+14, 0.0f));
					//tranform3[3][0] = position[0];
					tranform3[3][1] = position[1];
					poketran[3][1] = position[1];
					//tranform3[3][2] = position[2];
					
				}
			}
			/*h = h * 3;
			r = r * 3;
			rl = rl * 3;*/
			/*glm::vec3 s(tranform3[3][0], tranform3[3][1], tranform3[3][2]);
			int xxx = s.x+4.0;
			int yyy = s.z-1.0;
			float zzz = loadedModel111->contour[yyy][xxx];
			float diff = tranform3[3][2]-zzz;
			if (diff > 0)
			{
				tranform3 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, diff-25, 0.0f));
			}
*/
				

			loadedModel4->setMatrix(tranform3);
			glm::mat4 tranform4 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 14.0, 2.0));
			tranform4 = tranform4*glm::rotate(glm::mat4(1.0f), h, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform4 = tranform4*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -14.0, -2.0));
			loadedModel9->setMatrix(tranform4);//elbow right

			glm::mat4 tranform5 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 14.0, 2.0));
			tranform5 = tranform5*glm::rotate(glm::mat4(1.0f), -h, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform5 = tranform5*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -14.0, -2.0));
			loadedModel10->setMatrix(tranform5);//elbow left



			tranform4 = tranform4*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 10.0, 1.2));
			tranform4 = tranform4*glm::rotate(glm::mat4(1.0f), r, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform4 = tranform4*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -10.0, -1.2));
			loadedModel11->setMatrix(tranform4);//hand right

			tranform5 = tranform5*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 10.0, 1.2));
			tranform5 = tranform5*glm::rotate(glm::mat4(1.0f), r, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform5 = tranform5*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -10.0, -1.2));
			loadedModel12->setMatrix(tranform5);//hand left



			glm::mat4 tranform6 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 9.6, 1.0));
			tranform6 = tranform6*glm::rotate(glm::mat4(1.0f), -h / 4, glm::vec3(0.0f, 1.0f, 0.0f));
			tranform6 = tranform6*glm::translate(glm::mat4(1.0f), glm::vec3(1.0, -9.6, -1.0));
			loadedModel3->setMatrix(tranform6);//torso

			glm::mat4 tranform7 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 7.2, 1.2));
			tranform7 = tranform7*glm::rotate(glm::mat4(1.0f), -h / 2, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform7 = tranform7*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -7.2, -1.2));
			loadedModel5->setMatrix(tranform7);//leg right



			glm::mat4 tranform8 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 7.2, 1.2));
			tranform8 = tranform8*glm::rotate(glm::mat4(1.0f), h / 2, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform8 = tranform8*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -7.2, -1.2));
			loadedModel6->setMatrix(tranform8);//leg left

			tranform8 = tranform8*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 2.6, -0.2));
			tranform8 = tranform8*glm::rotate(glm::mat4(1.0f), -rl, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform8 = tranform8*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -2.6, 0.2));
			loadedModel8->setMatrix(tranform8);//foot left

			glm::mat4 tranform9 = tranform7*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 2.6, -0.2));
			tranform9 = tranform9*glm::rotate(glm::mat4(1.0f), -rl, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform9 = tranform9*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -2.6, 0.2));
			loadedModel7->setMatrix(tranform9);//foot right
		}
		else
		{
			int s1 = 0;
			int s2 = 0;

			if (tranform[3][1] > 25 && tranform[3][1] < 30)
			{
				s1 = 0;
				s2 = 20;
			}
			else if (tranform[3][1] > 30 && tranform[3][1] < 100)
			{
				s1 = 90;
				s2 = 60;
			}
			else if (tranform[3][1] < 20 && tranform[3][1] > 10)
			{
				s1 = 0;
				s2 = 90;
			}
			if (jj % 4 == 0)
			{
				

				glm::vec3 s(tranform10[3][0], tranform10[3][1]+90, tranform10[3][2]);
				glm::vec3 e(tranform10[3][0] , tranform10[3][1] - 90, tranform10[3][2] );
				vector<glm::vec3> res;

				// perform the ray intersectiont test.
				RayIntersectionTest::intersect(s, e, *loadedModel111, res);

				// pick the first result if one is available
				if (res.size() > 0) {
					glm::vec3 position = res[0];

					//tranform3 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -position[1]+14, 0.0f));
					//tranform3[3][0] = position[0];
					tranform10[3][1] = position[1];
					//tranform3[3][2] = position[2];

				}
			}
			
			
			loadedModel3->setMatrix(tranform10);
			loadedModel4->setMatrix(tranform10);
			loadedModel5->setMatrix(tranform10);
			loadedModel6->setMatrix(tranform10);
			loadedModel7->setMatrix(tranform10);
			loadedModel8->setMatrix(tranform10);
			loadedModel9->setMatrix(tranform10);
			loadedModel10->setMatrix(tranform10);
			loadedModel11->setMatrix(tranform10);
			loadedModel12->setMatrix(tranform10);
			
			glm::mat4 tranform22 = tranform10;
			if (ooo > 0)
			{
				
				tranform22 = tranform22*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 24.2, 0));

				tranform22 = tranform22*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 8.0));
				float fff = rl * 3 - 1.4;
				if (ballthrow==true&&lockff == false)
				{
					ffff = fff;
					lockff = true;
				}
				tranform22 = tranform22*glm::rotate(glm::mat4(1.0f), fff, glm::vec3(0.0f, 1.0f, 0.0f));
				tranform22 = tranform22*glm::translate(glm::mat4(1.0f), glm::vec3(-0.0, 0.0, -8.0));
				tranform22 = tranform22*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -24.2, 0));
				tranform99 = tranform22;
				loadedModel22->setMatrix(tranform22);
			}
			else if (ooo < 0)
			{
				if (ppp < 0)
				{
					ooo = ooo*-1;
					ballthrow = true;
				}
				
				tranform22 = tranform99*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 24.2, 0));
				tranform22 = tranform22*glm::rotate(glm::mat4(1.0f), -rl * 3, glm::vec3(1.0f, 0.0f, 0.0f));
				tranform22 = tranform22*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -24.2, 0));
				loadedModel22->setMatrix(tranform22);
				if (ballthrow == true && lockhh == false)
				{
					hhh = -rl*3;
					lockhh = true;
				}
			}
			
		}

		if (pokthrow < 0)
		{
			//change character stance here
			
			glm::mat4 tranform7 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 7.2, 1.2));
			tranform7 = tranform7*glm::rotate(glm::mat4(1.0f), -0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform7 = tranform7*glm::rotate(glm::mat4(1.0f), 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
			tranform7 = tranform7*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -7.2, -1.2));
			//tranform7 = tranform7*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
			loadedModel5->setMatrix(tranform7);//leg right



			glm::mat4 tranform8 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 7.2, 1.2));
			tranform8 = tranform8*glm::rotate(glm::mat4(1.0f), 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform8 = tranform8*glm::rotate(glm::mat4(1.0f), -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
			tranform8 = tranform8*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -7.2, -1.2));
			//tranform8 = tranform8*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
			loadedModel6->setMatrix(tranform8);//leg left

			tranform8 = tranform8*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 2.6, -0.2));
			tranform8 = tranform8*glm::rotate(glm::mat4(1.0f), -0.4f, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform8 = tranform8*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -2.6, 0.2));
			//tranform8 = tranform8*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
			loadedModel8->setMatrix(tranform8);//foot left

			glm::mat4 tranform9 = tranform7*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 2.6, -0.2));
			tranform9 = tranform9*glm::rotate(glm::mat4(1.0f), -0.4f, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform9 = tranform9*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -2.6, 0.2));
			//tranform9 = tranform9*glm::scale(glm::vec3(2.0, 2.0f, 2.0f));
			loadedModel7->setMatrix(tranform9);//foot right

			glm::mat4 tranform5 = tranform3*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 14.0, 2.0));
			tranform5 = tranform5*glm::rotate(glm::mat4(1.0f), -0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform5 = tranform5*glm::rotate(glm::mat4(1.0f), -0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
			tranform5 = tranform5*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -14.0, -2.0));
			loadedModel10->setMatrix(tranform5);//elbow left

			tranform5 = tranform5*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 10.0, 1.2));
			tranform5 = tranform5*glm::rotate(glm::mat4(1.0f), 0.4f, glm::vec3(1.0f, 0.0f, 0.0f));
			tranform5 = tranform5*glm::rotate(glm::mat4(1.0f), -0.4f, glm::vec3(0.0f, 0.0f, 1.0f));
			tranform5 = tranform5*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -10.0, -1.2));
			loadedModel12->setMatrix(tranform5);//hand left




		}
		bool ret = texture1->setTextureBlendMode(3);
		apperance_2->updateTextures();
		apperance_3->updateTextures();
		loadedModel3->draw();
		loadedModel4->draw();
		loadedModel5->draw();
		loadedModel6->draw();
		loadedModel7->draw();
		loadedModel8->draw();
		loadedModel9->draw();
		loadedModel10->draw();
		loadedModel11->draw();
		loadedModel12->draw();
		
		poketran = movecharacter(tranform3, movew, rota);
		if (ballthrow == true)
		{
			int correction = 1;
			int correction1 = 1;
			int correction2 = 1;
			if (sin(ffff) < 0)
			{
				correction = correction*-1;

			}
			if (sin(hhh) < 0)
			{
				correction1 = correction1*-1;
			}
			if (cos(hhh) < 0)
			{
				correction2 = correction2*-1;
			}
			poketran = poketran*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, jjj*sin(ffff)*correction, 0.0));
			poketran = poketran*glm::translate(glm::mat4(1.0f), glm::vec3(kk*0.1*cos(hhh), 0.0, kk*0.1*sin(hhh)));
		}
		//poketran = poketran*glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -1.2));
		float aa = poketran[3][0];
		float bb = poketran[3][1];
		float cc = poketran[3][2];

		float aaa = pokepos.x;
		float bbb = pokepos.y;
		float ccc = pokepos.z;

		float a = pokepos1.x;
		float b = pokepos1.y;
		float c = pokepos1.z;

		float collis = (aaa - aa)*(aaa - aa) + (ccc - cc)*(ccc - cc);
		float collis1 = (a - aa)*(a - aa) + (c - cc)*(c - cc);
		if (collis < 20)
		{

			pokecaught = true;
		}
		if (collis1 < 20)
		{

			pokecaughtsq = true;
		}
		pokeball->setMatrix(poketran);
		
		if (pokthrow < 0)
		{
			loadedModel22->draw();
			pokeball->draw();
		}

		
        cs->draw();
		loadedModel1->setobjno(0);
		ret = texture1->setTextureBlendMode(2);
		apperance_1->updateTextures();

		loadedModel1->draw();
		ret = texture1->setTextureBlendMode(0);
		apperance_1->updateTextures();
		loadedModel1->setobjno(0);
		size = 2;
		if (pokecaught == false)
		{
			patience = 0;
			pokecaughtsq = false;
			for (int i = 0; i < size; i++)
			{
				loadedModel2->draw();
				//loadedModel20->draw();
				bool ret = texture1->setTextureBlendMode(size - 1 - i);
				//if (ret)
				apperance_1->updateTextures();
			}
		}
		else if (pokecaught == true)
		{
			patience++;
			if (patience == 2000)
			{
				pokecaught = false;
				respawn = true;
			}
		}

		//if (pokecaughtsq == false)
		//{
		//	pokecaught = true;
		//	for (int i = 0; i < size; i++)
		//	{
		//		loadedModel2->draw();
		//		bool ret = texture1->setTextureBlendMode(size - 1 - i);
		//		//if (ret)
		//		apperance_1->updateTextures();
		//	}
		//}
		loadedModel1->setobjno(0);
		//object_transformation = loadedModel3->getModelMatrix();
		if (cammode < 0)
		{
			object_transformation = tranform3;
			// Add the camera and a camera delta
			//camera_matrix = camera_delta *  camera_transformation * object_transformation;
			camera_matrix =camera_transformation * object_transformation;
			// set the view matrix
			//camera_matrix = camera_transformation * glm::inverse(object_transformation);
			ch = camera_matrix[3][0];
			cd = camera_matrix[3][1];
			cr = camera_matrix[3][2];
			//SetViewAsMatrix(camera_matrix);
			cra.x = 0;
			cra.y = camera_matrix[3][2];
			cra.z = 0;
		}
		if (cammode > 0)
		{
			

			// set the view matrix
			ch = 100;
			cd = 50;
			cr = 50;
		}
		//SetTrackballLocation(GetCurrentCameraMatrix(), GetCurrentCameraTranslation(), glm::vec3(ch, cd, cr));
		/*loadedModel1->draw();
		ret = texture->setTextureBlendMode(0);
		if (ret)apperance_0->updateTextures();*/
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

