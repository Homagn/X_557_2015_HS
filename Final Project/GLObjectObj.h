#pragma once

// stl include
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLObject.h"
#include "HCI557Datatypes.h"


using namespace std;

class GLObjectObj : public GLObject
{
    // Ray intersection test has access to protected functions
    friend class RayIntersectionTest;
    
public:
	GLObjectObj::GLObjectObj(string filename);
    GLObjectObj();
    ~GLObjectObj();
	float contour[200][200];
	vector<GLuint>          _numloc;//stores the start indices of usage of different textures

	vector<string>          _bmploc;//stores the location of bmp files as string

	vector<GLuint>          _numloc1;//stores the start indices of usage of different textures

	vector<string>          _bmploc1;//stores the location of bmp files as string

	vector<GLuint>          _numloc2;//stores the start indices of usage of different textures

	vector<string>          _bmploc2;//stores the location of bmp files as string

	int _texturecounter = 0;//counts the index for each texture
	int _callnumber = 0;//counts the number of times draw was called

	int _texturecounter1 = 0;//counts the index for each texture
	int _callnumber1 = 0;//counts the number of times draw was called

	int _texturecounter2 = 0;//counts the index for each texture
	int _callnumber2 = 0;//counts the number of times draw was called

	int _objno=0;
    /*!
     Draw the objects
     */
    void draw();
	
	
	string GLObjectObj::getlocation(int objno);
	int getsize(int objno);
	void setobjno(int objno);
    /*!
     Init the geometry object
     */
    void init();
	void GLObjectObj::create_contour_map(void);
	inline glm::mat4& getModelMatrix(void){ return _modelMatrix; };
    
    /*!
    Returns the number of vertices
    */
    int size(void);
    
    
    /*!
    Returns a reference too the vertices.
    */
    vector<glm::vec3>& getVertices(void){return  _vertices; };
    
    /*!
    To update the vertices. 
    This function takes a vector of vertices and replaces the current vector.
    */
    void updateVertices(float* vertices);
    
private:
    
    /*!
     Load an obj model from a file
     */
	bool load_obj(const char* filename, vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<GLuint> &elements, vector<glm::vec2> &texuv, vector<GLuint> &numloc, vector<string> &bmploc, int objno);

    
    
    /*!
     Extract one obj face coded as:f v/vt/vn v/vt/vn v/vt/vn
     */
    bool extractNextFace3(string& in, string& out, int& pointIdx, int& texIdx, int& normalIdx );
    bool extractNextFace1(string& in, string& out, int& pointIdx0, int& pointIdx1, int& pointIdx2  );
    
    /*!
     Create the vertex buffer object for this element
     */
    virtual void initVBO();
    
    /*
     Inits the shader program for this object
     */
    virtual void initShader(void);
    
    
    // the program
    GLuint                  _program;
    

    // file and path of the loaded object
    string                  _file_and_path;
    bool                    _file_ok;
   
protected:
    // The data for the world
    vector<glm::vec3>       _vertices;
	
    vector<glm::vec4>       _vertex_colors;
    vector<glm::vec3>       _normals;
	vector<glm::vec2>       _texuv;
    vector<GLuint>          _elements;
	
	// The data for the pokemon
	vector<glm::vec3>       _vertices1;
	vector<glm::vec4>       _vertex_colors1;
	vector<glm::vec3>       _normals1;
	vector<glm::vec2>       _texuv1;
	vector<GLuint>          _elements1;

	// The data for the walking character
	vector<glm::vec3>       _vertices2;
	vector<glm::vec4>       _vertex_colors2;
	vector<glm::vec3>       _normals2;
	vector<glm::vec2>       _texuv2;
	vector<GLuint>          _elements2;
	
	
	

	vector<Vertex>          _vertex_points;
	vector<Vertex>          _normal_vectors;

	vector<Vertex>          _vertex_points1;
	vector<Vertex>          _normal_vectors1;

	vector<Vertex>          _vertex_points2;
	vector<Vertex>          _normal_vectors2;
	void GLObjectObj::make_model(vector<Vertex>& vp, vector<Vertex>& nv, int objno);
	
    
private:
    // obj file data
    string                  _material_file;
    string                  _model_name;
    
    
    int                     _num_vertices;
	int                     _num_vertices1;
	int                     _num_vertices2;
    
    unsigned int            _vaoID[2]; // Our Vertex Array Object
    
    unsigned int            _vboID[3]; // Our Vertex Buffer Object

	unsigned int            _vaoID1[2]; // Our Vertex Array Object

	unsigned int            _vboID1[3]; // Our Vertex Buffer Object

	unsigned int            _vaoID2[2]; // Our Vertex Array Object

	unsigned int            _vboID2[3]; // Our Vertex Buffer Object
    
    GLuint                  _elementbuffer;
};