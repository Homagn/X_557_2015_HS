

#include "GLObjectObj.h"

#include <algorithm>

bool lock1 = false;
GLObjectObj::GLObjectObj(string filename):
_file_and_path(filename)
{
    
    _file_ok = false;
	_texturecounter = -1;
	if (_objno == 0)
		_file_ok = load_obj(filename.c_str(), _vertices, _normals, _elements, _texuv, _numloc, _bmploc, _objno);
	
	else if (_objno == 1)
		_file_ok = load_obj(filename.c_str(), _vertices1, _normals1, _elements1, _texuv1, _numloc1, _bmploc1, _objno);
	
	else if (_objno == 2)
		_file_ok = load_obj(filename.c_str(), _vertices2, _normals2, _elements2, _texuv2, _numloc2, _bmploc2, _objno);
	//_file_ok = load_obj(filename.c_str(), _vertices2, _normals2, _elements2, _texuv2, _numloc2, _bmploc2, _objno);
	
   
}

GLObjectObj::GLObjectObj()
{

}


GLObjectObj::~GLObjectObj()
{

}

void GLObjectObj::setobjno(int objno)
{
	_objno = objno;
	
}

/*!
 Init the geometry object
 */
void GLObjectObj::init()
{
    if(_file_ok)
    {
        initShader();
        initVBO();
    }
}

string GLObjectObj::getlocation(int objno)
{
	if (objno == 0)
		_texturecounter++;
	else if (objno == 1)
		_texturecounter1++;
	else if (objno == 2)
		_texturecounter2++;

	if (_texturecounter > _bmploc.size() - 1)
		_texturecounter = 0;
	else if (_texturecounter1 > _bmploc1.size() - 1)
		_texturecounter1 = 0;
	else if (_texturecounter2 > _bmploc2.size() - 1)
		_texturecounter2 = 0;

	if (objno==0)
		return _bmploc[_texturecounter];
	else if (objno==1)
		return _bmploc1[_texturecounter1];
	else if (objno == 2)
		return _bmploc2[_texturecounter2];
	
}

int GLObjectObj::getsize(int objno)
{
	if (objno == 0)
		return _bmploc.size();
	else if (objno == 1)
		return _bmploc1.size();
	else if (objno == 2)
		return _bmploc1.size();
	
}

bool GLObjectObj::extractNextFace1(string& in, string& out, int& pointIdx0, int& pointIdx1, int& pointIdx2 )
{
    GLuint a,b,c;
    
    
    // remove the first space
    int idv1 = in.find_first_of(" ");
    string v1(in.substr(idv1+1, in.length()-idv1)); // remove it
    
    sscanf(v1.c_str(), " %d %d %d", &a, &b, &c);

	pointIdx0 = a;
	pointIdx1 = b;
	pointIdx2 = c;
    
    return true;
}




bool GLObjectObj::extractNextFace3(string& in, string& out, int& pointIdx, int& texIdx, int& normalIdx )
{
    GLuint a,b,c;
    
    // first
    int idv1 = in.find_first_of(" ");
    string v1(in.substr(idv1+1, in.length()-idv1));
    
    sscanf(v1.c_str(), " %d/%d/%d", &a, &b, &c);
	if (lock1==true)
	{
		b = c;
		c = 1;
	}
    
    int idv2 = v1.find_first_of(" ");
    if(idv2 != -1)
    {
        out = v1.substr(idv2, v1.length()-idv2); // with blank
    }
    else
    {
        out = "";
    }
    
    /*
    int idx = v1.find_first_of("/");
    istringstream c1(v1.substr(0,idx));
    
    
    c1 >> a;
    
    // second
    string v2 = v1.substr(idx+1, v1.length()-idx);
    
    int idv2 = v2.find_first_of("/");
    istringstream c2(v2.substr(0,idv2));
    c2 >> b;
    
    
    // third
    string v3 = v2.substr(idv2+1, v2.length()-idv2);
    
    int idv3 = v3.find_first_of(" ");
    istringstream c3(v3.substr(0,idv3));
    
    c3 >> c;
    
    if(idv3 != -1)
        out = v3.substr(idv3, in.length()-idv3); // with blank
    else
        out = "";
     */

    pointIdx = a;
    texIdx = b;
    normalIdx = c;
    
    return true;

}




bool GLObjectObj::load_obj(const char* filename, vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<GLuint> &elements, vector<glm::vec2> &texuv, vector<GLuint> &numloc, vector<string> &bmploc, int objno)
{
    ifstream in(filename, ios::in);
    if (!in)
    {
        cerr << "Cannot open " << filename << endl;
        return false;
    }
    
	
    
    vector<glm::vec3> temp_normals;
    vector<glm::vec3> temp_vertices;
    vector<glm::vec3> temp_index_normal2point;
    vector<glm::vec3> temp_index_triangle;
    vector<glm::vec3> temp_index_textures;
	vector< glm::vec2 > temp_uvs;
	
	int num = 0;
    string line;
	bool lock = false;
	
	int cc = 0;
    while (getline(in, line))
	{
		cc++;
        if (line.substr(0,2) == "v ")
        {
			if (lock == false)
			{
				numloc.push_back(num);
				lock = true;
			}
            istringstream s(line.substr(2));
            glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;// v.w = 1.0f;
            temp_vertices.push_back(v);
            _vertex_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
			
            
        }
        else if (line.substr(0,2) == "f ")
        {
			num = num + 3;
			
            int idx = line.find_first_of("/");
            if(idx == -1)
            {
                istringstream s(line.substr(2));
                GLushort a,b,c;
                s >> a; s >> b; s >> c;
                cout << a << " : " << b << " : " << c << endl;;
                a--; b--; c--;
            
                elements.push_back(a); elements.push_back(b); elements.push_back(c);
            }
            else
            {
                
                size_t n = std::count(line.begin(), line.end(), '/');
                if(n == 0)
                {
                    string new_line;
                    int p0, p1, p2;
                    extractNextFace1(line, new_line, p0, p1, p2 );
                    
                    temp_index_triangle.push_back(glm::vec3(p0, p1, p2));
                    temp_index_normal2point.push_back(glm::vec3(p0, p1, p2)); // same normal vectors.
                    
                }
                else if(n == 3)
                {
                    string new_line;
                    int p0, p1, p2;
                    extractNextFace1(line, new_line, p0, p1, p2 );
                    
                    temp_index_triangle.push_back(glm::vec3(p0, p1, p2));
                    temp_index_normal2point.push_back(glm::vec3(p0, p1, p2)); // same normal vectors.
					if (lock1 == true)
					{
						temp_index_textures.push_back(glm::vec3(p0, p1, p2));
						cout << "started initializing human torso";
						lock1 = false;
					}

                
                }
                else if(n==6)
                {
                    string new_line;
                    int p0, t0,  n0;
                    int p1, t1,  n1;
                    int p2, t2,  n2;
                    int p3, t3,  n3;
                    
                    // first
                    extractNextFace3(line, new_line, p0, t0, n0 );
                    
                    // second
                    extractNextFace3(new_line, new_line, p1, t1, n1 );
                    
                    // third
                    extractNextFace3(new_line, new_line, p2, t2, n2 );
                    
                    // Quads and not triangles
                    if(new_line.length() > 2)
                    {
                        // fourth
                        extractNextFace3(new_line, new_line, p3, t3, n3 );
                    }

                    // remember the normal vectors
                    temp_index_triangle.push_back(glm::vec3(p0, p1, p2));
                    temp_index_normal2point.push_back(glm::vec3(n0, n1, n2));
                    temp_index_textures.push_back(glm::vec3(t0, t1, t2));
                    
                    elements.push_back(p0);
                    elements.push_back(p1);
                    elements.push_back(p2);
                    
                }
                
               
            }
        }
        else if (line.substr(0,3) == "vn ")
        {
            istringstream s(line.substr(3));
            glm::vec3 n; s >> n.x; s >> n.y; s >> n.z;
            temp_normals.push_back(n);
        }
		else if (line.substr(0, 3) == "vt ")
		{
			istringstream s(line.substr(3));
			glm::vec2 n; s >> n.x; s >> n.y; 
			temp_uvs.push_back(n);
			
		}
        else if (line.substr(0,7) == "usemtl ")
        {
            istringstream s(line.substr(7));
            //s >> _material_file;
			string append;
			s >> append;


			//change here

			if (strcmp(filename, "data/001 - Bulbasaur/Bulbasaur.obj")==0){
				append = "data/001 - Bulbasaur/" + append + ".bmp";
			}
			else if (strcmp(filename , "data/World/pokemonworld.obj")==0)
			{
				append = "data/World/" + append + ".bmp";
			}
			else{
				append = "data/human/" + append + ".bmp";
			}
			bmploc.push_back(append);
			
			lock = false;
			
			
            /* ignoring this line */
        }
        else if (line.substr(0,2) == "hu")
        {
            istringstream s(line.substr(2));
            s >> _model_name;
			lock1 = true;
            /* ignoring this line */
        }
        else if (line[0] == '#')
        {
            /* ignoring this line */
        }
        else
        {
            /* ignoring this line */
        }
    }
	numloc.push_back(num);
	cout << num;
    
    /////////////////////////////////////////////
    // assign normals to points and points to triangles.
    
    vertices.clear();
    normals.clear();
    for(int i=0; i<temp_index_triangle.size(); i++)
    {
        glm::vec3 pointIdx = temp_index_triangle[i];
        glm::vec3 normalIdx = temp_index_normal2point[i];
		glm::vec3 texuvIdx;
		
			
		if (temp_index_textures.size()>1)
			texuvIdx = temp_index_textures[i];
		
			
        // the three points of one triangle
        // -1 since obj does not use the index 0
        vertices.push_back(temp_vertices[ pointIdx.x-1 ]);
        vertices.push_back(temp_vertices[ pointIdx.y-1 ]);
        vertices.push_back(temp_vertices[ pointIdx.z-1 ]);
		
			
		if (temp_index_textures.size() > 1)
		{
			texuv.push_back(temp_uvs[texuvIdx.x - 1]);
			texuv.push_back(temp_uvs[texuvIdx.y - 1]);
			texuv.push_back(temp_uvs[texuvIdx.z - 1]);
		}
		
			
        normals.push_back(temp_normals[normalIdx.x-1]);
        normals.push_back(temp_normals[normalIdx.y-1]);
        normals.push_back(temp_normals[normalIdx.z-1]);
	
    }
    
	cout << "loaded";
    
    
    
    
    
    if(normals.size() > 0) return true; // loaded normal vectors
    
    normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
    for (int i = 0; i < elements.size(); i+=3)
    {
        GLushort ia = elements[i];
        GLushort ib = elements[i+1];
        GLushort ic = elements[i+2];
        glm::vec3 normal = glm::normalize(glm::cross(
                                                     glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
                                                     glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
        normals[ia] = normals[ib] = normals[ic] = normal;
        
        
       // _vertex_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
      //  _vertex_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
       // _vertex_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
    }
	cout << "loaded one object";
    return true;
}


/*!
 Create the vertex buffer object for this element
 */
void GLObjectObj::initVBO()
{
	cout << _vertices.size();
	cout << _vertices1.size();
	int objno = _objno;
	if (objno == 0){
		cout << "entered object 0";
		_num_vertices = _vertices.size();
		_vertex_points.clear();
		_normal_vectors.clear();

		//---------------------------------------------------------
		// This creates the box geometry
		make_model(_vertex_points, _normal_vectors, objno);

		_num_vertices = _vertex_points.size();

		glUseProgram(_program);

		glGenVertexArrays(2, _vaoID); // Create our Vertex Array Object


		//create memory for the vertices, etc.
		int valb = 0;
		int val = 0;


		int height = 2;//number of vaos

		float** vertices = new float*[height];
		for (int i = 0; i < height; ++i)
			vertices[i] = new float[_num_vertices * 5];

		float** normals = new float*[height];
		for (int i = 0; i < height; ++i)
			normals[i] = new float[_num_vertices * 3];

		//float(* vertices)[2] = new float[_num_vertices * 5][2];
		//float(* normals)[2] = new float[_num_vertices * 3][2];


		int locpos[2];
		int locnorm[2];
		int tex_idx[2];
		cout << _num_vertices;
		
		//cout << "\n";
		//cout << _numloc[2];

		//change here

		for (int j = 0; j < 2; j++)
		{


			if (j == 0)
			{
				val = _vertex_points.size();
			}
			if (j == 1)
			{
				val = _vertex_points.size();
			}




			// copy the data to the vectors
			for (int i = 0; i < _num_vertices; i++)
			{
				Vertex v = _vertex_points[i];
				vertices[j][(i * 5)] = v.x(); vertices[j][(i * 5) + 1] = v.y(); vertices[j][(i * 5) + 2] = v.z();
				vertices[j][(i * 5) + 3] = v.u(); vertices[j][(i * 5) + 4] = v.v();

				Vertex n = _normal_vectors[i];
				normals[j][(i * 3)] = n.x(); normals[j][(i * 3) + 1] = n.y(); normals[j][(i * 3) + 2] = n.z();

			}


			glBindVertexArray(_vaoID[j]); // Bind our Vertex Array Object so we can use it


			glGenBuffers(3, _vboID); // Generate our Vertex Buffer Object


			// vertices
			glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]); // Bind our Vertex Buffer Object
			glBufferData(GL_ARRAY_BUFFER, val * 5 * sizeof(GLfloat), vertices[j], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

			locpos[j] = glGetAttribLocation(_program, "in_Position");
			glVertexAttribPointer((GLuint)locpos[j], 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0); // Set up our vertex attributes pointer
			glEnableVertexAttribArray(locpos[j]); //

			//textures
			tex_idx[j] = glGetAttribLocation(_program, "in_TexCoord");
			glVertexAttribPointer((GLuint)tex_idx[j], 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat),
				(const GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(tex_idx[j]);

			// normals
			locnorm[j] = glGetAttribLocation(_program, "in_Normal");
			glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]); // Bind our Vertex Buffer Object
			glBufferData(GL_ARRAY_BUFFER, val * 3 * sizeof(GLfloat), normals[j], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

			glVertexAttribPointer((GLuint)locnorm[j], 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
			glEnableVertexAttribArray(locnorm[j]); //


			valb = val;

		}
		glBindVertexArray(0); // Disable our Vertex Buffer Object
		cout << "VBO initialization was successful";
	}
	if (objno == 1){
		cout << "entered object 1";
		_num_vertices1 = _vertices1.size();
		_vertex_points1.clear();
		_normal_vectors1.clear();

		//---------------------------------------------------------
		// This creates the box geometry
		make_model(_vertex_points1, _normal_vectors1, objno);

		_num_vertices1 = _vertex_points1.size();

		glUseProgram(_program);

		glGenVertexArrays(2, _vaoID1); // Create our Vertex Array Object


		//create memory for the vertices, etc.
		int valb = 0;
		int val = 0;


		int height = 2;//number of vaos

		float** vertices = new float*[height];
		for (int i = 0; i < height; ++i)
			vertices[i] = new float[_num_vertices * 5];

		float** normals = new float*[height];
		for (int i = 0; i < height; ++i)
			normals[i] = new float[_num_vertices * 3];

		//float(* vertices)[2] = new float[_num_vertices * 5][2];
		//float(* normals)[2] = new float[_num_vertices * 3][2];


		int locpos[2];
		int locnorm[2];
		int tex_idx[2];
		cout << _num_vertices;
		
		//cout << "\n";
		//cout << _numloc[2];

		//change here

		for (int j = 0; j < 2; j++)
		{


			if (j == 0)
			{
				val = _vertex_points1.size();
			}
			if (j == 1)
			{
				val = _vertex_points1.size();
			}




			// copy the data to the vectors
			for (int i = 0; i < _num_vertices1; i++)
			{
				Vertex v = _vertex_points1[i];
				vertices[j][(i * 5)] = v.x(); vertices[j][(i * 5) + 1] = v.y(); vertices[j][(i * 5) + 2] = v.z();
				vertices[j][(i * 5) + 3] = v.u(); vertices[j][(i * 5) + 4] = v.v();

				Vertex n = _normal_vectors1[i];
				normals[j][(i * 3)] = n.x(); normals[j][(i * 3) + 1] = n.y(); normals[j][(i * 3) + 2] = n.z();

			}


			glBindVertexArray(_vaoID1[j]); // Bind our Vertex Array Object so we can use it


			glGenBuffers(3, _vboID1); // Generate our Vertex Buffer Object


			// vertices
			glBindBuffer(GL_ARRAY_BUFFER, _vboID1[0]); // Bind our Vertex Buffer Object
			glBufferData(GL_ARRAY_BUFFER, val * 5 * sizeof(GLfloat), vertices[j], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

			locpos[j] = glGetAttribLocation(_program, "in_Position");
			glVertexAttribPointer((GLuint)locpos[j], 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0); // Set up our vertex attributes pointer
			glEnableVertexAttribArray(locpos[j]); //

			//textures
			tex_idx[j] = glGetAttribLocation(_program, "in_TexCoord");
			glVertexAttribPointer((GLuint)tex_idx[j], 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat),
				(const GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(tex_idx[j]);

			// normals
			locnorm[j] = glGetAttribLocation(_program, "in_Normal");
			glBindBuffer(GL_ARRAY_BUFFER, _vboID1[1]); // Bind our Vertex Buffer Object
			glBufferData(GL_ARRAY_BUFFER, val * 3 * sizeof(GLfloat), normals[j], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

			glVertexAttribPointer((GLuint)locnorm[j], 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
			glEnableVertexAttribArray(locnorm[j]); //


			valb = val;

		}
		glBindVertexArray(0); // Disable our Vertex Buffer Object
		cout << "initialization was successful";
	}
	if (objno == 2){
		_num_vertices2 = _vertices2.size();
		_vertex_points2.clear();
		_normal_vectors2.clear();

		//---------------------------------------------------------
		// This creates the box geometry
		make_model(_vertex_points2, _normal_vectors2, objno);

		_num_vertices2 = _vertex_points2.size();

		glUseProgram(_program);

		glGenVertexArrays(2, _vaoID2); // Create our Vertex Array Object


		//create memory for the vertices, etc.
		int valb = 0;
		int val = 0;


		int height = 2;//number of vaos

		float** vertices = new float*[height];
		for (int i = 0; i < height; ++i)
			vertices[i] = new float[_num_vertices * 5];

		float** normals = new float*[height];
		for (int i = 0; i < height; ++i)
			normals[i] = new float[_num_vertices * 3];

		//float(* vertices)[2] = new float[_num_vertices * 5][2];
		//float(* normals)[2] = new float[_num_vertices * 3][2];


		int locpos[2];
		int locnorm[2];
		int tex_idx[2];
		cout << _num_vertices;
		cout << "\n";
		cout << _numloc[0];
		cout << "\n";
		cout << _numloc[1];
		//cout << "\n";
		//cout << _numloc[2];

		//change here

		for (int j = 0; j < 2; j++)
		{


			if (j == 0)
			{
				val = _vertex_points1.size();
			}
			if (j == 1)
			{
				val = _vertex_points1.size();
			}




			// copy the data to the vectors
			for (int i = 0; i < _num_vertices2; i++)
			{
				Vertex v = _vertex_points2[i];
				vertices[j][(i * 5)] = v.x(); vertices[j][(i * 5) + 1] = v.y(); vertices[j][(i * 5) + 2] = v.z();
				vertices[j][(i * 5) + 3] = v.u(); vertices[j][(i * 5) + 4] = v.v();

				Vertex n = _normal_vectors2[i];
				normals[j][(i * 3)] = n.x(); normals[j][(i * 3) + 1] = n.y(); normals[j][(i * 3) + 2] = n.z();

			}


			glBindVertexArray(_vaoID2[j]); // Bind our Vertex Array Object so we can use it


			glGenBuffers(3, _vboID2); // Generate our Vertex Buffer Object


			// vertices
			glBindBuffer(GL_ARRAY_BUFFER, _vboID2[0]); // Bind our Vertex Buffer Object
			glBufferData(GL_ARRAY_BUFFER, val * 5 * sizeof(GLfloat), vertices[j], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

			locpos[j] = glGetAttribLocation(_program, "in_Position");
			glVertexAttribPointer((GLuint)locpos[j], 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0); // Set up our vertex attributes pointer
			glEnableVertexAttribArray(locpos[j]); //

			//textures
			tex_idx[j] = glGetAttribLocation(_program, "in_TexCoord");
			glVertexAttribPointer((GLuint)tex_idx[j], 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat),
				(const GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(tex_idx[j]);

			// normals
			locnorm[j] = glGetAttribLocation(_program, "in_Normal");
			glBindBuffer(GL_ARRAY_BUFFER, _vboID2[1]); // Bind our Vertex Buffer Object
			glBufferData(GL_ARRAY_BUFFER, val * 3 * sizeof(GLfloat), normals[j], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

			glVertexAttribPointer((GLuint)locnorm[j], 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
			glEnableVertexAttribArray(locnorm[j]); //


			valb = val;

		}
		glBindVertexArray(0); // Disable our Vertex Buffer Object
		cout << "initialization was successful";
	}

}


void GLObjectObj::make_model(vector<Vertex>& vp, vector<Vertex>& nv, int objno)
{

	
	// TODO: add a better vertex generation code, one which incorporates the number of rows and colums.

	// allocate the memory.
	if (objno == 0){
		int numvert = _vertices.size();//stores the number of vertices after the object has been loaded
		vp = vector<Vertex>(numvert);
		nv = vector<Vertex>(numvert);

		for (int i = 0; i < numvert; i++)
		{
			vp[i].x() = _vertices[i].x;
			vp[i].y() = _vertices[i].y;
			vp[i].z() = _vertices[i].z;
			if (_texuv.size() != 0)
			{
				vp[i].u() = _texuv[i].x;
				vp[i].v() = _texuv[i].y;
			}
			else if (_texuv.size() == 0)
			{
				vp[i].u() = 0;
				vp[i].v() = 0;
			}
		}



		for (int i = 0; i < numvert; i++) {
			nv[i].x() = _normals[i].x;
			nv[i].y() = _normals[i].y;
			nv[i].z() = _normals[i].z;
		}
	}
	if (objno == 1){
		int numvert = _vertices1.size();//stores the number of vertices after the object has been loaded
		vp = vector<Vertex>(numvert);
		nv = vector<Vertex>(numvert);

		for (int i = 0; i < numvert; i++)
		{
			vp[i].x() = _vertices1[i].x;
			vp[i].y() = _vertices1[i].y;
			vp[i].z() = _vertices1[i].z;
			vp[i].u() = _texuv1[i].x;
			vp[i].v() = _texuv1[i].y;
		}



		for (int i = 0; i < numvert; i++) {
			nv[i].x() = _normals1[i].x;
			nv[i].y() = _normals1[i].y;
			nv[i].z() = _normals1[i].z;
		}
	}
	if (objno == 2){
		int numvert = _vertices2.size();//stores the number of vertices after the object has been loaded
		vp = vector<Vertex>(numvert);
		nv = vector<Vertex>(numvert);

		for (int i = 0; i < numvert; i++)
		{
			vp[i].x() = _vertices2[i].x;
			vp[i].y() = _vertices2[i].y;
			vp[i].z() = _vertices2[i].z;
			vp[i].u() = _texuv2[i].x;
			vp[i].v() = _texuv2[i].y;
		}



		for (int i = 0; i < numvert; i++) {
			nv[i].x() = _normals2[i].x;
			nv[i].y() = _normals2[i].y;
			nv[i].z() = _normals2[i].z;
		}
	}
	cout << "\n";
	cout << "model successfully made";

}


void GLObjectObj::create_contour_map(void)
{
	for (int i = 0; i < _vertices.size(); i++)

	{
		float x = _vertices[i].x;
		float y = _vertices[i].y;
		int xx = (int)x;
		int yy = (int)y;
		int zz = (_vertices[i].z) / 3;
		contour[xx][yy] = zz;
	}
	for (int i = 0; i < 200; i++)
	{

	}

}
/*
 Inits the shader program for this object
 */
void GLObjectObj::initShader(void)
{
    
    if(!_apperance.exists())return;
    
    // This loads the shader program from a file
    _program = _apperance.getProgram();
    
    
    glUseProgram(_program);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Vertex information / names
    
    glBindAttribLocation(_program, 0, "in_Position");
    glBindAttribLocation(_program, 1, "in_Normal");
	glBindAttribLocation(_program, 2, "in_TexCoord");
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Define the model view matrix.
    _modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    addModelViewMatrixToProgram(_program);
    
    
    
    glUseProgram(0);
	cout << "shader successfully initialized";
}




/*!
 Draw the objects
 */
void GLObjectObj::draw()
{
	int objno = _objno;
	if (objno == 0)
	{
		if (_callnumber < _bmploc.size() - 1)
		{

			glUseProgram(_program);

			// Bind the buffer and switch it to an active buffer
			glBindVertexArray(_vaoID[_callnumber]);


			// this changes the camera location
			glm::mat4 rotated_view = rotatedViewMatrix();
			glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
			glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
			glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //


			// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

			// Draw the triangles
			int texturenumber = 0;
			glDrawArrays(GL_TRIANGLES, _numloc[_callnumber], _numloc[_callnumber + 1]);
			glBindVertexArray(0);
			_callnumber++;
		}
		else if (_callnumber == _bmploc.size() - 1)
		{
			glUseProgram(_program);

			int texturenumber = 0;
			glBindVertexArray(_vaoID[_callnumber]);

			// this changes the camera location
			glm::mat4 rotated_view = rotatedViewMatrix();
			glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
			glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
			glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //

			/*glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 1);*/
			glDrawArrays(GL_TRIANGLES, _numloc[_callnumber], _numloc[_callnumber + 1]);
			// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbuffer);
			//glDrawElements( GL_TRIANGLES, _elements.size(), GL_UNSIGNED_INT,(void*)0 );


			// Unbind our Vertex Array Object
			glBindVertexArray(0);
			_callnumber = 0;
		}
	}
	else if (objno == 1)
	{
		if (_callnumber1 < _bmploc1.size() - 1)
		{

			glUseProgram(_program);

			// Bind the buffer and switch it to an active buffer
			glBindVertexArray(_vaoID1[_callnumber1]);


			// this changes the camera location
			glm::mat4 rotated_view = rotatedViewMatrix();
			glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
			glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
			glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //


			// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

			// Draw the triangles
			int texturenumber = 0;
			glDrawArrays(GL_TRIANGLES, _numloc1[_callnumber1], _numloc1[_callnumber1 + 1]);
			glBindVertexArray(0);
			_callnumber1++;
		}
		else if (_callnumber1 == _bmploc1.size() - 1)
		{
			glUseProgram(_program);

			int texturenumber = 0;
			glBindVertexArray(_vaoID1[_callnumber1]);

			// this changes the camera location
			glm::mat4 rotated_view = rotatedViewMatrix();
			glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
			glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
			glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //

			/*glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 1);*/
			glDrawArrays(GL_TRIANGLES, _numloc1[_callnumber1], _numloc1[_callnumber1 + 1]);
			// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbuffer);
			//glDrawElements( GL_TRIANGLES, _elements.size(), GL_UNSIGNED_INT,(void*)0 );


			// Unbind our Vertex Array Object
			glBindVertexArray(0);
			_callnumber1 = 0;
		}
	}
	else if (objno == 2)
	{
		if (_callnumber2 < _bmploc2.size() - 1)
		{

			glUseProgram(_program);

			// Bind the buffer and switch it to an active buffer
			glBindVertexArray(_vaoID2[_callnumber2]);


			// this changes the camera location
			glm::mat4 rotated_view = rotatedViewMatrix();
			glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
			glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
			glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //


			// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

			// Draw the triangles
			int texturenumber = 0;
			glDrawArrays(GL_TRIANGLES, _numloc2[_callnumber2], _numloc2[_callnumber2 + 1]);
			glBindVertexArray(0);
			_callnumber2++;
		}
		else if (_callnumber2 == _bmploc2.size() - 1)
		{
			glUseProgram(_program);

			int texturenumber = 0;
			glBindVertexArray(_vaoID2[_callnumber2]);

			// this changes the camera location
			glm::mat4 rotated_view = rotatedViewMatrix();
			glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
			glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
			glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //

			/*glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 1);*/
			glDrawArrays(GL_TRIANGLES, _numloc2[_callnumber2], _numloc2[_callnumber2 + 1]);
			// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementbuffer);
			//glDrawElements( GL_TRIANGLES, _elements.size(), GL_UNSIGNED_INT,(void*)0 );


			// Unbind our Vertex Array Object
			glBindVertexArray(0);
			_callnumber2 = 0;
		}
	}
    
}




/*!
Returns the number of vertices
*/
int GLObjectObj::size(void)
{
    return _num_vertices;
}


/*!
To update the vertices.
This function takes a vector of vertices and replaces the current vector.
*/
void GLObjectObj::updateVertices(float* vertices)
{

    glBindVertexArray(_vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    // vertices
    int locPos = glGetAttribLocation(_program, "in_Position");
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * 3 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locPos); //

}
