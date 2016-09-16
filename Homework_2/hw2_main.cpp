//
//  main.cpp
//  OpenGL4Test
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


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"

// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;




static const string vs_string =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"in vec3 in_Position;                                               \n"
"                                                                   \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"    pass_Color = in_Color;                                         \n"
"}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string  =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";




/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix




// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Fill this functions with your model code.

// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];

unsigned int vboID[4];

/*!
 ADD YOUR CODE TO CREATE THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
unsigned int createTriangleStripModel(void)
{
    // use the vertex array object vaoID[0] for this model representation
	float* vertices = new float[129];  // Vertices for our square
	float *colors = new float[129]; // Colors for our vertices

	// FRONT
	vertices[0] = 2.0; vertices[1] = 0.0; vertices[2] = -3.0; // Top left corner  0
	colors[0] = 1.0; colors[1] = 0.0; colors[2] = 0.0; // Top left corner

	vertices[3] = 0.0; vertices[4] = 0.0; vertices[5] = -3.0; // Bottom left corner  1
	colors[3] = 1.0; colors[4] = 1.0; colors[5] = 1.0; // Bottom left corner

	vertices[6] = 2.0; vertices[7] = -1.0; vertices[8] = -3.0; // Top Right corner   2
	colors[6] = 0.0; colors[7] = 1.0; colors[8] = 0.0; // Top Right corner

	vertices[9] = 0.0; vertices[10] = -1.0; vertices[11] = -3.0; // Bottom right corner  3
	colors[9] = 0.0; colors[10] = 0.0; colors[11] = 1.0; // Bottom right corner

	// RIGHT SIDE
	vertices[12] = 2.0; vertices[13] = -1.0; vertices[14] = -4.0; // Top right corner  4
	colors[12] = 1.0; colors[13] = 1.0; colors[14] = 0.0; // Top right corner

	vertices[15] = 0.0; vertices[16] = -1.0; vertices[17] = -4.0; // Bottom right corner  5
	colors[15] = 1.0; colors[16] = 0.0; colors[17] = 1.0; // Bottom right corner

	// BACK
	vertices[18] = 2.0; vertices[19] = 0.0; vertices[20] = -4.0; // Top left  corner  6
	colors[18] = 0.0; colors[19] = 0.5; colors[20] = 0.5; // Top left corner

	vertices[21] = 0.0; vertices[22] = 0.0; vertices[23] = -4.0; // Bottom left  corner  7
	colors[21] = 0.5; colors[22] = 0.5; colors[23] = 0.0; // Bottom left corner

	// LEFT
	vertices[24] = 0.0; vertices[25] = 0.0; vertices[26] = -3.0; // Top left  corner  8
	colors[24] = 1.0; colors[25] = 0.0; colors[26] = 0.0; // Top left corner

	vertices[27] = 2.0; vertices[28] = 0.0; vertices[29] = -4.0; // Bottom left  corner   9
	colors[27] = 1.0; colors[28] = 0.0; colors[29] = 0.0; // Bottom left corner

	// BOTTOM
	vertices[30] = 2.0; vertices[31] = 0.0; vertices[32] = -3.0; // Top left  corner   10
	colors[30] = 1.0; colors[31] = 0.0; colors[32] = 0.0; // Top left corner

	vertices[33] = 0.0; vertices[34] = -1.0; vertices[35] = -4.0; // Bottom left  corner  11
	colors[33] = 1.0; colors[34] = 1.0; colors[35] = 0.0; // Bottom left corner

	//TOP
	vertices[36] = 0.0; vertices[37] = -1.0; vertices[38] = -3.0; // Top left  corner  12
	colors[36] = 0.0; colors[37] = 1.0; colors[38] = 0.0; // Top left corner

	vertices[39] = 0.0; vertices[40] = 0.0; vertices[41] = -4.0; // Bottom left  corner  13
	colors[39] = 0.0; colors[40] = 0.3; colors[41] = 0.0; // Bottom left corner

	vertices[42] = 0.0; vertices[43] = 0.0; vertices[44] = -3.0; // Top left  corner  14
	colors[42] = 0.0; colors[43] = 0.8; colors[44] = 0.0; // Top left corner

	vertices[45] = 2.0; vertices[46] = -1.0; vertices[47] = -3.0; // Bottom left  corner  15
	colors[45] = 0.0; colors[46] = 0.5; colors[47] = 0.0; // Bottom left corner

	
	
	
	
	
	
	
	// FRONT
	vertices[48] = 2.0; vertices[49] = 0.0; vertices[50] = -3.0; // Top left corner  16
	colors[48] = 1.0; colors[49] = 0.0; colors[50] = 0.0; // Top left corner

	vertices[51] = 2.0; vertices[52] = -1.0; vertices[53] = -4.0; // Bottom left corner  17
	colors[51] = 1.0; colors[52] = 1.0; colors[53] = 1.0; // Bottom left corner

	vertices[54] = 2.0; vertices[55] = 2.0; vertices[56] = -4.0; // Top Right corner  18
	colors[54] = 0.0; colors[55] = 1.0; colors[56] = 0.0; // Top Right corner

	vertices[57] = 3.0; vertices[58] = 2.0; vertices[59] = -4.0; // Bottom right corner  19
	colors[57] = 0.0; colors[58] = 0.0; colors[59] = 1.0; // Bottom right corner

	// RIGHT SIDE
	vertices[60] = 2.0; vertices[61] = 2.0; vertices[62] = -3.0; // Top right corner  20
	colors[60] = 1.0; colors[61] = 1.0; colors[62] = 0.0; // Top right corner

	vertices[63] = 3.0; vertices[64] = 2.0; vertices[65] = -3.0; // Bottom right corner  21
	colors[63] = 1.0; colors[64] = 0.0; colors[65] = 1.0; // Bottom right corner

	// BACK
	vertices[66] = 2.0; vertices[67] = 0.0; vertices[68] = -1.0; // Top left  corner  22
	colors[66] = 0.0; colors[67] = 0.5; colors[68] = 0.5; // Top left corner

	vertices[69] = 3.0; vertices[70] = 0.0; vertices[71] = -1.0; // Bottom left  corner  23
	colors[69] = 0.5; colors[70] = 0.5; colors[71] = 0.0; // Bottom left corner

	// LEFT
	vertices[72] = 2.0; vertices[73] = -1.0; vertices[74] = -1.0; // Top left  corner  24
	colors[72] = 1.0; colors[73] = 0.0; colors[74] = 0.0; // Top left corner

	vertices[75] = 3.0; vertices[76] = -1.0; vertices[77] = -1.0; // Bottom left  corner  25
	colors[75] = 1.0; colors[76] = 0.0; colors[77] = 0.0; // Bottom left corner

	// BOTTOM
	vertices[78] = 2.0; vertices[79] = -1.0; vertices[80] = -3.0; // Top left  corner  26
	colors[78] = 1.0; colors[79] = 0.0; colors[80] = 0.0; // Top left corner

	vertices[81] = 3.0; vertices[82] = -1.0; vertices[83] = -3.0; // Bottom left  corner  27
	colors[81] = 1.0; colors[82] = 1.0; colors[83] = 0.0; // Bottom left corner

	//TOP
	vertices[84] = 2.0; vertices[85] = -1.0; vertices[86] = -4.0; // Top left  corner  28
	colors[84] = 0.0; colors[85] = 1.0; colors[86] = 0.0; // Top left corner

	vertices[87] = 3.0; vertices[88] = -1.0; vertices[89] = -4.0; // Bottom left  corner  29
	colors[87] = 0.0; colors[88] = 0.3; colors[89] = 0.0; // Bottom left corner

	vertices[90] = 2.0; vertices[91] = 2.0; vertices[92] = -4.0; // Top left  corner  30
	colors[90] = 0.0; colors[91] = 0.8; colors[92] = 0.0; // Top left corner

	vertices[93] = 3.0; vertices[94] = 2.0; vertices[95] = -4.0; // Bottom left  corner  31
	colors[93] = 0.0; colors[94] = 0.5; colors[95] = 0.0; // Bottom left corner





	// FRONT
	vertices[96] = 3.0; vertices[97] = -1.0; vertices[98] = -4.0; // Top left corner  32
	colors[96] = 1.0; colors[97] = 0.0; colors[98] = 0.0; // Top left corner

	vertices[99] = 3.0; vertices[100] = 0.0; vertices[101] = -1.0; // Bottom left corner  33
	colors[99] = 1.0; colors[100] = 1.0; colors[101] = 1.0; // Bottom left corner

	vertices[102] = 3.0; vertices[103] = -1.0; vertices[104] = -1.0; // Top Right corner  34
	colors[102] = 0.0; colors[103] = 1.0; colors[104] = 0.0; // Top Right corner

	vertices[105] = 2.0; vertices[106] = -1.0; vertices[107] = -1.0; // Bottom right corner  35
	colors[105] = 0.0; colors[106] = 0.0; colors[107] = 1.0; // Bottom right corner

	// RIGHT SIDE
	vertices[108] = 2.0; vertices[109] = -1.0; vertices[110] = -3.0; // Top right corner  36
	colors[108] = 1.0; colors[109] = 1.0; colors[110] = 0.0; // Top right corner

	vertices[111] = 2.0; vertices[112] = -1.0; vertices[113] = -1.0; // Bottom right corner  37
	colors[111] = 1.0; colors[112] = 0.0; colors[113] = 1.0; // Bottom right corner

	// BACK
	vertices[114] = 2.0; vertices[115] = 2.0; vertices[116] = -3.0; // Top left  corner  38
	colors[114] = 0.0; colors[115] = 0.5; colors[116] = 0.5; // Top left corner

	vertices[117] = 2.0; vertices[118] = 0.0; vertices[119] = -1.0; // Bottom left  corner  39
	colors[117] = 0.5; colors[118] = 0.5; colors[119] = 0.0; // Bottom left corner




	vertices[120] = 2.0; vertices[121] = 0.0; vertices[122] = -3.0; // Bottom right corner  40
	colors[120] = 1.0; colors[121] = 0.0; colors[122] = 0.0; // Bottom right corner

	// BACK
	vertices[123] = 2.0; vertices[124] = 2.0; vertices[125] = -4.0; // Top left  corner  41
	colors[123] = 1.0; colors[124] = 0.5; colors[125] = 0.1; // Top left corner

	vertices[126] = 2.0; vertices[127] = 2.0; vertices[128] = -3.0; // Bottom left  corner  42
	colors[126] = 1.0; colors[127] = 0.5; colors[128] = 0.5; // Bottom left corner
	


	glGenVertexArrays(2, &vaoID[0]); // Create our Vertex Array Object
	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it


	glGenBuffers(2, vboID); // Generate our Vertex Buffer Object

	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 129 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(0); // Disable our Vertex Array Object


	//Color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 129 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(1); // Enable the second vertex attribute array

	glBindVertexArray(0); // Disable our Vertex Buffer Object


	delete[] vertices; // Delete our vertices from memory
    



    //TODO:
    vaoID[0];
    
    return 1;
}

/*!
 ADD YOUR CODE TO CREATE A MODEL USING PRIMITIVES OF YOUR CHOISE TO THIS FUNCTION
 */
unsigned int createPolygonModel(void)
{
    // use the vertex array object vaoID[1] for this model representation

    //TODO:
	float* vertices = new float[300];  // Vertices for our square
	float *colors = new float[300]; // Colors for our vertices
    vaoID[1];
	//test
	//vertices[0] = -3.0; vertices[1] = 0.0; vertices[2] = -5.0;  // Bottom left  corner
	//colors[0] = 1.0; colors[1] = 0.5; colors[2] = 0.5; // Bottom left corner
//vertices[3] = -5.0; vertices[4] = 0.0; vertices[5] = -5.0;  // Bottom left  corner
	//colors[3] = 1.0; colors[4] = 0.5; colors[5] = 0.5; // Bottom left corner
//vertices[6] = -5.0; vertices[7] = -1.0; vertices[8] = -5.0;  // Bottom left  corner
	//colors[6] = 1.0; colors[7] = 0.5; colors[8] = 0.5; // Bottom left corner

	vertices[0] = 2.0-3.0; vertices[1] = 0.0; vertices[2] = -3.0; // Top left corner  0
	colors[0] = 1.0; colors[1] = 0.0; colors[2] = 0.0; // Top left corner

	vertices[3] = 0.0 - 3.0; vertices[4] = 0.0; vertices[5] = -3.0; // Bottom left corner  1
	colors[3] = 1.0; colors[4] = 1.0; colors[5] = 1.0; // Bottom left corner

	vertices[6] = 2.0 - 3.0; vertices[7] = -1.0; vertices[8] = -3.0; // Top Right corner   2
	colors[6] = 0.0; colors[7] = 1.0; colors[8] = 0.0; // Top Right corner

	vertices[9] = 0.0 - 3.0; vertices[10] = 0.0; vertices[11] = -3.0; // Bottom left corner  1
	colors[9] = 1.0; colors[10] = 1.0; colors[11] = 1.0; // Bottom left corner

	vertices[12] = 2.0 - 3.0; vertices[13] = -1.0; vertices[14] = -3.0; // Top Right corner   2
	colors[12] = 0.0; colors[13] = 1.0; colors[14] = 0.0; // Top Right corner

	vertices[15] = 0.0 - 3.0; vertices[16] = -1.0; vertices[17] = -3.0; // Bottom right corner  3
	colors[15] = 0.0; colors[16] = 0.0; colors[17] = 1.0; // Bottom right corner

	vertices[18] = 2.0 - 3.0; vertices[19] = -1.0; vertices[20] = -3.0; // Top Right corner   2
	colors[18] = 0.0; colors[19] = 1.0; colors[20] = 0.0; // Top Right corner

	vertices[21] = 0.0 - 3.0; vertices[22] = -1.0; vertices[23] = -3.0; // Bottom right corner  3
	colors[21] = 0.0; colors[22] = 0.0; colors[23] = 1.0; // Bottom right corner

	// RIGHT SIDE
	vertices[24] = 2.0 - 3.0; vertices[25] = -1.0; vertices[26] = -4.0; // Top right corner  4
	colors[24] = 1.0; colors[25] = 1.0; colors[26] = 0.0; // Top right corner

	vertices[27] = 0.0 - 3.0; vertices[28] = -1.0; vertices[29] = -3.0; // Bottom right corner  3
	colors[27] = 0.0; colors[28] = 0.0; colors[29] = 1.0; // Bottom right corner

	// RIGHT SIDE
	vertices[30] = 2.0 - 3.0; vertices[31] = -1.0; vertices[32] = -4.0; // Top right corner  4
	colors[30] = 1.0; colors[31] = 1.0; colors[32] = 0.0; // Top right corner

	vertices[33] = 0.0 - 3.0; vertices[34] = -1.0; vertices[35] = -4.0; // Bottom right corner  5
	colors[33] = 1.0; colors[34] = 0.0; colors[35] = 1.0; // Bottom right corner

	vertices[36] = 2.0 - 3.0; vertices[37] = -1.0; vertices[38] = -4.0; // Top right corner  4
	colors[36] = 1.0; colors[37] = 1.0; colors[38] = 0.0; // Top right corner

	vertices[39] = 0.0 - 3.0; vertices[40] = -1.0; vertices[41] = -4.0; // Bottom right corner  5
	colors[39] = 1.0; colors[40] = 0.0; colors[41] = 1.0; // Bottom right corner

	// BACK
	vertices[42] = 2.0 - 3.0; vertices[43] = 0.0; vertices[44] = -4.0; // Top left  corner  6
	colors[42] = 0.0; colors[43] = 0.5; colors[44] = 0.5; // Top left corner

	vertices[45] = 0.0 - 3.0; vertices[46] = -1.0; vertices[47] = -4.0; // Bottom right corner  5
	colors[45] = 1.0; colors[46] = 0.0; colors[47] = 1.0; // Bottom right corner

	// BACK
	vertices[48] = 2.0 - 3.0; vertices[49] = 0.0; vertices[50] = -4.0; // Top left  corner  6
	colors[48] = 0.0; colors[49] = 0.5; colors[50] = 0.5; // Top left corner

	vertices[51] = 0.0 - 3.0; vertices[52] = 0.0; vertices[53] = -4.0; // Bottom left  corner  7
	colors[51] = 0.5; colors[52] = 0.5; colors[53] = 0.0; // Bottom left corner

	vertices[54] = 2.0 - 3.0; vertices[55] = 0.0; vertices[56] = -4.0; // Top left  corner  6
	colors[54] = 0.0; colors[55] = 0.5; colors[56] = 0.5; // Top left corner

	vertices[57] = 0.0 - 3.0; vertices[58] = 0.0; vertices[59] = -4.0; // Bottom left  corner  7
	colors[57] = 0.5; colors[58] = 0.5; colors[59] = 0.0; // Bottom left corner

	// LEFT
	vertices[60] = 0.0 - 3.0; vertices[61] = 0.0; vertices[62] = -3.0; // Top left  corner  8
	colors[60] = 1.0; colors[61] = 0.0; colors[62] = 0.0; // Top left corner

	vertices[63] = 0.0 - 3.0; vertices[64] = 0.0; vertices[65] = -3.0; // Top left  corner  8
	colors[63] = 1.0; colors[64] = 0.0; colors[65] = 0.0; // Top left corner

	vertices[66] = 2.0 - 3.0; vertices[67] = 0.0; vertices[68] = -4.0; // Bottom left  corner   9
	colors[66] = 1.0; colors[67] = 0.0; colors[68] = 0.0; // Bottom left corner

	// BOTTOM
	vertices[69] = 2.0 - 3.0; vertices[70] = 0.0; vertices[71] = -3.0; // Top left  corner   10
	colors[69] = 1.0; colors[70] = 0.0; colors[71] = 0.0; // Top left corner

	vertices[72] = 0.0 - 3.0; vertices[73] = -1.0; vertices[74] = -4.0; // Bottom left  corner  11
	colors[72] = 1.0; colors[73] = 1.0; colors[74] = 0.0; // Bottom left corner

	//TOP
	vertices[75] = 0.0 - 3.0; vertices[76] = -1.0; vertices[77] = -3.0; // Top left  corner  12
	colors[75] = 0.0; colors[76] = 1.0; colors[77] = 0.0; // Top left corner

	vertices[78] = 0.0 - 3.0; vertices[79] = 0.0; vertices[80] = -4.0; // Bottom left  corner  13
	colors[78] = 0.0; colors[79] = 0.3; colors[80] = 0.0; // Bottom left corner

	vertices[81] = 0.0 - 3.0; vertices[82] = -1.0; vertices[83] = -3.0; // Top left  corner  12
	colors[81] = 0.0; colors[82] = 1.0; colors[83] = 0.0; // Top left corner

	vertices[84] = 0.0 - 3.0; vertices[85] = 0.0; vertices[86] = -4.0; // Bottom left  corner  13
	colors[84] = 0.0; colors[85] = 0.3; colors[86] = 0.0; // Bottom left corner

	vertices[87] = 0.0 - 3.0; vertices[88] = 0.0; vertices[89] = -3.0; // Top left  corner  14
	colors[87] = 0.0; colors[88] = 0.8; colors[89] = 0.0; // Top left corner

	vertices[90] = 2.0 - 3.0; vertices[91] = -1.0; vertices[92] = -3.0; // Bottom left  corner  15
	colors[90] = 0.0; colors[91] = 0.5; colors[92] = 0.0; // Bottom left corner








	// FRONT
	vertices[93] = 2.0 - 3.0; vertices[94] = 0.0; vertices[95] = -3.0; // Top left corner  16
	colors[93] = 1.0; colors[94] = 0.0; colors[95] = 0.0; // Top left corner

	vertices[96] = 2.0 - 3.0; vertices[97] = -1.0; vertices[98] = -4.0; // Bottom left corner  17
	colors[96] = 1.0; colors[97] = 1.0; colors[98] = 1.0; // Bottom left corner

	vertices[99] = 2.0 - 3.0; vertices[100] = 0.0; vertices[101] = -3.0; // Top left corner  16
	colors[99] = 1.0; colors[100] = 0.0; colors[101] = 0.0; // Top left corner

	vertices[102] = 2.0 - 3.0; vertices[103] = -1.0; vertices[104] = -4.0; // Bottom left corner  17
	colors[102] = 1.0; colors[103] = 1.0; colors[104] = 1.0; // Bottom left corner

	vertices[105] = 2.0 - 3.0; vertices[106] = 2.0; vertices[107] = -4.0; // Top Right corner  18
	colors[105] = 0.0; colors[106] = 1.0; colors[107] = 0.0; // Top Right corner

	vertices[108] = 2.0 - 3.0; vertices[109] = 2.0; vertices[110] = -4.0; // Top Right corner  18
	colors[108] = 0.0; colors[109] = 1.0; colors[110] = 0.0; // Top Right corner

	vertices[111] = 3.0 - 3.0; vertices[112] = 2.0; vertices[113] = -4.0; // Bottom right corner  19
	colors[111] = 0.0; colors[112] = 0.0; colors[113] = 1.0; // Bottom right corner

	// RIGHT SIDE
	vertices[114] = 2.0 - 3.0; vertices[115] = 2.0; vertices[116] = -3.0; // Top right corner  20
	colors[114] = 1.0; colors[115] = 1.0; colors[116] = 0.0; // Top right corner

	vertices[117] = 3.0 - 3.0; vertices[118] = 2.0; vertices[119] = -4.0; // Bottom right corner  19
	colors[117] = 0.0; colors[118] = 0.0; colors[119] = 1.0; // Bottom right corner

	// RIGHT SIDE
	vertices[120] = 2.0 - 3.0; vertices[121] = 2.0; vertices[122] = -3.0; // Top right corner  20
	colors[120] = 1.0; colors[121] = 1.0; colors[122] = 0.0; // Top right corner

	vertices[123] = 3.0 - 3.0; vertices[124] = 2.0; vertices[125] = -3.0; // Bottom right corner  21
	colors[123] = 1.0; colors[124] = 0.0; colors[125] = 1.0; // Bottom right corner

	vertices[126] = 2.0 - 3.0; vertices[127] = 2.0; vertices[128] = -3.0; // Top right corner  20
	colors[126] = 1.0; colors[127] = 1.0; colors[128] = 0.0; // Top right corner

	vertices[129] = 3.0 - 3.0; vertices[130] = 2.0; vertices[131] = -3.0; // Bottom right corner  21
	colors[129] = 1.0; colors[130] = 0.0; colors[131] = 1.0; // Bottom right corner


	// BACK
	vertices[132] = 2.0 - 3.0; vertices[133] = 0.0; vertices[134] = -1.0; // Top left  corner  22
	colors[132] = 0.0; colors[133] = 0.5; colors[134] = 0.5; // Top left corner

	vertices[135] = 3.0 - 3.0; vertices[136] = 2.0; vertices[137] = -3.0; // Bottom right corner  21
	colors[135] = 1.0; colors[136] = 0.0; colors[137] = 1.0; // Bottom right corner


	// BACK
	vertices[138] = 2.0 - 3.0; vertices[139] = 0.0; vertices[140] = -1.0; // Top left  corner  22
	colors[138] = 0.0; colors[139] = 0.5; colors[140] = 0.5; // Top left corner

	vertices[141] = 3.0 - 3.0; vertices[142] = 0.0; vertices[143] = -1.0; // Bottom left  corner  23
	colors[141] = 0.5; colors[142] = 0.5; colors[143] = 0.0; // Bottom left corner

	vertices[144] = 2.0 - 3.0; vertices[145] = 0.0; vertices[146] = -1.0; // Top left  corner  22
	colors[144] = 0.0; colors[145] = 0.5; colors[146] = 0.5; // Top left corner

	vertices[147] = 3.0 - 3.0; vertices[148] = 0.0; vertices[149] = -1.0; // Bottom left  corner  23
	colors[147] = 0.5; colors[148] = 0.5; colors[149] = 0.0; // Bottom left corner

	// LEFT
	vertices[150] = 2.0 - 3.0; vertices[151] = -1.0; vertices[152] = -1.0; // Top left  corner  24
	colors[150] = 1.0; colors[151] = 0.0; colors[152] = 0.0; // Top left corner

	vertices[153] = 3.0 - 3.0; vertices[154] = 0.0; vertices[155] = -1.0; // Bottom left  corner  23
	colors[153] = 0.5; colors[154] = 0.5; colors[155] = 0.0; // Bottom left corner

	// LEFT
	vertices[156] = 2.0 - 3.0; vertices[157] = -1.0; vertices[158] = -1.0; // Top left  corner  24
	colors[156] = 1.0; colors[157] = 0.0; colors[158] = 0.0; // Top left corner

	vertices[159] = 3.0 - 3.0; vertices[160] = -1.0; vertices[161] = -1.0; // Bottom left  corner  25
	colors[159] = 1.0; colors[160] = 0.0; colors[161] = 0.0; // Bottom left corner

	vertices[162] = 2.0 - 3.0; vertices[163] = -1.0; vertices[164] = -1.0; // Top left  corner  24
	colors[162] = 1.0; colors[163] = 0.0; colors[164] = 0.0; // Top left corner

	vertices[165] = 3.0 - 3.0; vertices[166] = -1.0; vertices[167] = -1.0; // Bottom left  corner  25
	colors[165] = 1.0; colors[166] = 0.0; colors[167] = 0.0; // Bottom left corner

	// BOTTOM
	vertices[168] = 2.0 - 3.0; vertices[169] = -1.0; vertices[170] = -3.0; // Top left  corner  26
	colors[168] = 1.0; colors[169] = 0.0; colors[170] = 0.0; // Top left corner

	vertices[171] = 3.0 - 3.0; vertices[172] = -1.0; vertices[173] = -1.0; // Bottom left  corner  25
	colors[171] = 1.0; colors[172] = 0.0; colors[173] = 0.0; // Bottom left corner

	// BOTTOM
	vertices[174] = 2.0 - 3.0; vertices[175] = -1.0; vertices[176] = -3.0; // Top left  corner  26
	colors[174] = 1.0; colors[175] = 0.0; colors[176] = 0.0; // Top left corner

	vertices[177] = 3.0 - 3.0; vertices[178] = -1.0; vertices[179] = -3.0; // Bottom left  corner  27
	colors[177] = 1.0; colors[178] = 1.0; colors[179] = 0.0; // Bottom left corner

	vertices[180] = 2.0 - 3.0; vertices[181] = -1.0; vertices[182] = -3.0; // Top left  corner  26
	colors[180] = 1.0; colors[181] = 0.0; colors[182] = 0.0; // Top left corner

	vertices[183] = 3.0 - 3.0; vertices[184] = -1.0; vertices[185] = -3.0; // Bottom left  corner  27
	colors[183] = 1.0; colors[184] = 1.0; colors[185] = 0.0; // Bottom left corner

	//TOP
	vertices[186] = 2.0 - 3.0; vertices[187] = -1.0; vertices[188] = -4.0; // Top left  corner  28
	colors[186] = 0.0; colors[187] = 1.0; colors[188] = 0.0; // Top left corner

	vertices[189] = 3.0 - 3.0; vertices[190] = -1.0; vertices[191] = -3.0; // Bottom left  corner  27
	colors[189] = 1.0; colors[190] = 1.0; colors[191] = 0.0; // Bottom left corner

	//TOP
	vertices[192] = 2.0 - 3.0; vertices[193] = -1.0; vertices[194] = -4.0; // Top left  corner  28
	colors[192] = 0.0; colors[193] = 1.0; colors[194] = 0.0; // Top left corner

	vertices[195] = 3.0 - 3.0; vertices[196] = -1.0; vertices[197] = -4.0; // Bottom left  corner  29
	colors[195] = 0.0; colors[196] = 0.3; colors[197] = 0.0; // Bottom left corner

	vertices[198] = 2.0 - 3.0; vertices[199] = -1.0; vertices[200] = -4.0; // Top left  corner  28
	colors[198] = 0.0; colors[199] = 1.0; colors[200] = 0.0; // Top left corner

	vertices[201] = 3.0 - 3.0; vertices[202] = -1.0; vertices[203] = -4.0; // Bottom left  corner  29
	colors[201] = 0.0; colors[202] = 0.3; colors[203] = 0.0; // Bottom left corner

	vertices[204] = 2.0 - 3.0; vertices[205] = 2.0; vertices[206] = -4.0; // Top left  corner  30
	colors[204] = 0.0; colors[205] = 0.8; colors[206] = 0.0; // Top left corner

	vertices[207] = 3.0 - 3.0; vertices[208] = 2.0; vertices[209] = -4.0; // Bottom left  corner  31
	colors[207] = 0.0; colors[208] = 0.5; colors[209] = 0.0; // Bottom left corner



	// FRONT
	vertices[210] = 3.0 - 3.0; vertices[211] = -1.0; vertices[212] = -4.0; // Top left corner  32
	colors[210] = 1.0; colors[211] = 0.0; colors[212] = 0.0; // Top left corner

	vertices[213] = 3.0 - 3.0; vertices[214] = 0.0; vertices[215] = -1.0; // Bottom left corner  33
	colors[213] = 1.0; colors[214] = 1.0; colors[215] = 1.0; // Bottom left corner

	vertices[216] = 3.0 - 3.0; vertices[217] = -1.0; vertices[218] = -1.0; // Top Right corner  34
	colors[216] = 0.0; colors[217] = 1.0; colors[218] = 0.0; // Top Right corner


	// RIGHT SIDE
	vertices[219] = 2.0 - 3.0; vertices[220] = -1.0; vertices[221] = -3.0; // Top right corner  36
	colors[219] = 1.0; colors[220] = 1.0; colors[221] = 0.0; // Top right corner

	vertices[222] = 2.0 - 3.0; vertices[223] = -1.0; vertices[224] = -1.0; // Bottom right corner  37
	colors[222] = 1.0; colors[223] = 0.0; colors[224] = 1.0; // Bottom right corner

	// BACK
	vertices[225] = 2.0 - 3.0; vertices[226] = 2.0; vertices[227] = -3.0; // Top left  corner  38
	colors[225] = 0.0; colors[226] = 0.5; colors[227] = 0.5; // Top left corner

	vertices[228] = 2.0 - 3.0; vertices[229] = -1.0; vertices[230] = -1.0; // Bottom right corner  37
	colors[228] = 1.0; colors[229] = 0.0; colors[230] = 1.0; // Bottom right corner

	// BACK
	vertices[231] = 2.0 - 3.0; vertices[232] = 2.0; vertices[233] = -3.0; // Top left  corner  38
	colors[231] = 0.0; colors[232] = 0.5; colors[233] = 0.5; // Top left corner

	vertices[234] = 2.0 - 3.0; vertices[235] = 0.0; vertices[236] = -1.0; // Bottom left  corner  39
	colors[234] = 0.5; colors[235] = 0.5; colors[236] = 0.0; // Bottom left corner




	vertices[237] = 2.0 - 3.0; vertices[238] = 0.0; vertices[239] = -3.0; // Bottom right corner  40
	colors[237] = 1.0; colors[238] = 0.0; colors[239] = 0.0; // Bottom right corner

	// BACK
	vertices[240] = 2.0 - 3.0; vertices[241] = 2.0; vertices[242] = -4.0; // Top left  corner  41
	colors[240] = 1.0; colors[241] = 0.5; colors[242] = 0.1; // Top left corner

	vertices[243] = 2.0 - 3.0; vertices[244] = 2.0; vertices[245] = -3.0; // Bottom left  corner  42
	colors[243] = 1.0; colors[244] = 0.5; colors[245] = 0.5; // Bottom left corner

	vertices[246] = 3.0 - 3.0; vertices[247] = 0.0; vertices[248] = -1.0; // Bottom right corner  40
	colors[246] = 1.0; colors[247] = 0.0; colors[248] = 0.0; // Bottom right corner

	// BACK
	vertices[249] = 3.0 - 3.0; vertices[250] = -1.0; vertices[251] = -1.0; // Top left  corner  41
	colors[249] = 1.0; colors[250] = 0.0; colors[251] = 0.0; // Top left corner

	vertices[252] = 3.0 - 3.0; vertices[253] = -1.0; vertices[254] = -4.0; // Bottom left  corner  42
	colors[252] = 1.0; colors[253] = 0.0; colors[254] = 0.0; // Bottom left corner

	vertices[255] = 3.0 - 3.0; vertices[256] = 2.0; vertices[257] = -4.0; // Bottom right corner  40
	colors[255] = 1.0; colors[256] = 0.0; colors[257] = 0.0; // Bottom right corner

	// BACK
	vertices[258] = 3.0 - 3.0; vertices[259] = 2.0; vertices[260] = -3.0; // Top left  corner  41
	colors[258] = 1.0; colors[259] = 0.5; colors[260] = 0.1; // Top left corner

	vertices[261] = 3.0 - 3.0; vertices[262] = 0.0; vertices[263] = -1.0; // Bottom left  corner  42
	colors[261] = 1.0; colors[262] = 0.5; colors[263] = 0.5; // Bottom left corner
	
	vertices[264] = 3.0 - 3.0; vertices[265] = 2.0; vertices[266] = -4.0; // Bottom right corner  40
	colors[264] = 1.0; colors[265] = 0.0; colors[266] = 0.0; // Bottom right corner

	// BACK
	vertices[267] = 2.0 - 3.0; vertices[268] = 2.0; vertices[269] = -4.0; // Top left  corner  41
	colors[267] = 1.0; colors[268] = 0.5; colors[269] = 0.1; // Top left corner

	vertices[270] = 3.0 - 3.0; vertices[271] = -1.0; vertices[272] = -4.0; // Bottom left  corner  42
	colors[270] = 1.0; colors[271] = 0.5; colors[272] = 0.5; // Bottom left corner

	vertices[273] = 2.0 - 3.0; vertices[274] = 2.0; vertices[275] = -4.0; // Bottom right corner  40
	colors[273] = 1.0; colors[274] = 0.0; colors[275] = 0.0; // Bottom right corner

	// BACK
	vertices[276] = 2.0 - 3.0; vertices[277] = 2.0; vertices[278] = -3.0; // Top left  corner  41
	colors[276] = 1.0; colors[277] = 0.5; colors[278] = 0.1; // Top left corner

	vertices[279] = 2.0 - 3.0; vertices[280] = 0.0; vertices[281] = -1.0; // Bottom left  corner  42
	colors[279] = 1.0; colors[280] = 0.5; colors[281] = 0.5; // Bottom left corner

	vertices[282] = 2.0 - 3.0; vertices[283] = 0.0; vertices[284] = -3.0; // Bottom right corner  40
	colors[282] = 1.0; colors[283] = 0.0; colors[284] = 0.0; // Bottom right corner

	// BACK
	vertices[285] = 2.0 - 3.0; vertices[286] = 0.0; vertices[287] = -1.0; // Top left  corner  41
	colors[285] = 1.0; colors[286] = 0.5; colors[287] = 0.1; // Top left corner

	vertices[288] = 2.0 - 3.0; vertices[289] = -1.0; vertices[290] = -1.0; // Bottom left  corner  42
	colors[288] = 1.0; colors[289] = 0.5; colors[290] = 0.5; // Bottom left corner

	vertices[291] = 2.0 - 3.0; vertices[292] = 0.0; vertices[293] = -3.0; // Bottom right corner  40
	colors[291] = 1.0; colors[292] = 0.0; colors[293] = 0.0; // Bottom right corner

	// BACK
	vertices[294] = 2.0 - 3.0; vertices[295] = -1.0; vertices[296] = -3.0; // Top left  corner  41
	colors[294] = 1.0; colors[295] = 0.5; colors[296] = 0.1; // Top left corner

	vertices[297] = 2.0 - 3.0; vertices[298] = -1.0; vertices[299] = -1.0; // Bottom left  corner  42
	colors[297] = 1.0; colors[298] = 0.5; colors[299] = 0.5; // Bottom left corner 


	glGenVertexArrays(2, &vaoID[1]); // Create our Vertex Array Object
	glBindVertexArray(vaoID[1]); // Bind our Vertex Array Object so we can use it


	glGenBuffers(2, vboID); // Generate our Vertex Buffer Object

	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(0); // Disable our Vertex Array Object


	//Color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(1); // Enable the second vertex attribute array

	glBindVertexArray(0); // Disable our Vertex Buffer Object


	delete[] vertices; // Delete our vertices from memory

    
    return 1;
}



/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderTriangleStripModel(void)
{

    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(vaoID[0]);
        
	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY TRIANGLES / VERTICES YOU HAVE.
	// COMPLETE THE LINE
    // Draw the triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0 , 43 );


    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}



/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderPolygonModel(void)
{

    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(vaoID[1]);
        

	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY POLYGONS YOU HAVE.
	// COMPLETE THE LINE
    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0 , 82);
	glDrawArrays(GL_TRIANGLES, 82, 83);
	//glDrawArrays(GL_TRIANGLES, 84, 85);
    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the two models
 */
void setupScene(void) {
    
    createTriangleStripModel();
    renderTriangleStripModel();
	createPolygonModel();
	renderPolygonModel();
    
}




int main(int argc, const char * argv[])
{
	
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    static const string vertex_code = vs_string;
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = fs_string;
    static const char * fs_source = fragment_code.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);
    
    //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    // It controls the virtual camera
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    
    projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
    
    
	 //// The Shader Program ends here
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    


    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");
    

    
    // this creates the scene
    setupScene();
    
    int i=0;

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // this draws the coordinate system
		coordinate_system_renderer->draw();
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This generate the object
        // Enable the shader program
        glUseProgram(program);
        
        // this changes the camera location
        glm::mat4 rotated_view = viewMatrix * GetRotationMatrix();
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
        

        // This moves the model to the right
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        

		// This line renders your triangle strip model
        renderTriangleStripModel();
        

        // This moves the model to the left
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        
		// This line renders your Ppolygon model
        renderPolygonModel();
        
        
		// disable the shader program
        glUseProgram(0);


        //// This generate the object
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
	// delete the coordinate system object
	delete coordinate_system_renderer;

    // Program clean up when the window gets closed.
    glDeleteVertexArrays(2, vaoID);
    glDeleteProgram(program);
}

