#include "HCI557Common.h"



CameraTypes _cam_type = TRACKBALL;


/* A trackball to move and rotate the camera view */
Trackball trackball( WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, true, true );

/*!
 A new camera manipulator object
 */
CameraManipulator cam_manipulator(WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f);


// variable too keep the glsl version.
int glsl_major = 0 , glsl_minor = 0;


// this is a helper variable to allow us to change the texture blend model
int g_change_texture_blend;
int _camheight = 100;
int _camera_d;
int _camera_roll;
// the current mouse position;
int g_current_mouse_x = 0;
int g_current_mouse_y = 0;
//human character movement
bool mw = false;
float movew = 0.0;

bool rot = false;
float rota = 0.0;
float deg = 0.0;

int cammode = 1;

int pokthrow = 1;
int ooo = 1;
int ppp = 1;

/*!
 Return the glsl major / minor version.
 */
int GLSLMajor(void)
{
    return glsl_major;
}

int GLSLMinor(void)
{
    return glsl_minor;
}


/**
@brief: returns the x and y position of the mouse.
*/
int GetMouseX(void)
{
    return g_current_mouse_x;
}


int GetMouseY(void)
{
    return g_current_mouse_y;
}



/*!
 Returns the camera matrix of the current active camera object
 */
glm::mat4 GetCurrentCameraMatrix(void)
{
    switch (_cam_type) {
        case CAMERA_MANIPULATOR:
            return cam_manipulator.getRotationMatrix();
            break;
        case TRACKBALL:
            return trackball.getRotationMatrix();
        default:
            break;
    }
}



/**
 Returns the translation matrix of the current camera
 */
glm::vec3 GetCurrentCameraTranslation(void)
{
    switch (_cam_type) {
    case CAMERA_MANIPULATOR:
        return cam_manipulator.getTranslationVec();
        break;
    case TRACKBALL: // constant distance
            return glm::vec3(0.0f,0.0f,65.0f);
 
    }
}


/**
 @brief Change the camera manipulator type
 @param m - the camera type.
 */
void SetCameraManipulator(CameraTypes m)
{
    _cam_type = m;
}


/*!
Set the distance of the camera to your center position
*/
void SetCameraDistance(float distance)
{
    cam_manipulator.setCameraDistance(distance);
}




/* In GLFW mouse callback */
void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods ){
    
    
    trackball.mouseButtonCallback( window, button, action, mods );
    
    cam_manipulator.camera_MouseButton_Callback(window, button, action, mods);
}

/* In GLFW curser callback */
void cursorCallback( GLFWwindow *window, double x, double y ) {
    
    g_current_mouse_x = x;
    g_current_mouse_y = y;
    
    trackball.cursorCallback( window, x, y );
    
    cam_manipulator.camera_MouseCursor_Callack(window, x, y );
    
}






// This is the callback we'll be registering with GLFW for errors.
// It'll just print out the error to the STDERR stream.
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}



// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 84 && action == GLFW_PRESS) // key t
    {
        g_change_texture_blend++;
        g_change_texture_blend = g_change_texture_blend%3;

    }
	//else if (key == 67 && action == GLFW_PRESS) // key t
	else if (key == GLFW_KEY_C) // key c               
	{
		_camheight++;

	}
	else if (key == GLFW_KEY_V) // key c               
	{
		_camheight--;

	}
	else if (key == GLFW_KEY_G) // key d               
	{
		_camera_d++;

	}
	else if (key == GLFW_KEY_F) // key d               
	{
		_camera_d--;

	}
	else if (key == GLFW_KEY_R) // key r               
	{
		_camera_roll++;

	}
	else if (key == GLFW_KEY_T) // key r               
	{
		_camera_roll--;

	}
	else if (key == GLFW_KEY_H) // key r               
	{
		if (action == GLFW_RELEASE) // key t
		{
			pokthrow = pokthrow*-1;


			cout << "Throwing";
		}

	}
	else if (key == GLFW_KEY_O) // key r               
	{
		if (action == GLFW_RELEASE) // key t
		{
			ooo = ooo*-1;


			cout << "changing throw1";
		}

	}
	else if (key == GLFW_KEY_P) // key r               
	{
		if (action == GLFW_RELEASE) // key t
		{
			ppp = ppp*-1;


			cout << "changing throw2";
		}

	}
	else if (key == GLFW_KEY_W) // key t
	{
		cout << "key w pressed";
		mw = true;
		//movew = movew - 0.2;
		movew = -0.04;
		if (action == GLFW_RELEASE) // key t
		{
			mw = false;
			movew = 0.0;
			cammode = cammode*-1;

		}

	}
	else if (key == GLFW_KEY_A) // key t
	{
		rot = true;
		//rota = rota - 0.08;
		rota = 0.008;
		deg = deg + 0.008;
		if (deg == 3.14)
		{
			deg = 0.0;
		}
		if (action == GLFW_RELEASE) // key t
		{
			rot = false;
			rota = 0.0;


		}

	}
	else if (key == GLFW_KEY_D) // key t
	{
		rot = true;
		//rota = rota - 0.08;
		rota = -0.008;
		deg = deg - 0.008;
		if (deg == -3.14)
		{
			deg = 0.0;
		}
		if (action == GLFW_RELEASE) // key t
		{
			rot = false;
			rota = 0.0;


		}

	}
	
}



/*!
 This function initializes the GLFW window
 */
// The handle to the window object
GLFWwindow* initWindow(void)
{
    // The handle to the window object
    GLFWwindow*         window = NULL;
    
    // Initialize GLFW, and if it fails to initialize for any reason, print it out to STDERR.
    if (!glfwInit()) {
        fprintf(stderr, "Failed initialize GLFW.");
        exit(EXIT_FAILURE);
    }
    
    // Set the error callback, as mentioned above.
    glfwSetErrorCallback(error_callback);
    
    // Set up OpenGL options.
    // Use GLFW verion 3.3,
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    // Create a window to put our stuff in.
    window = glfwCreateWindow(1200, 700, "PoKemon GO", NULL, NULL);
    
    // If the window fails to be created, print out the error, clean up GLFW and exit the program.
    if( window == NULL ){
        cout <<  "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible." << endl;
        glfwTerminate();
        system("pause");
        exit(-1);
    }
    
    
    
    // Use the window as the current context (everything that's drawn will be place in this window).
    glfwMakeContextCurrent(window);
    
    
    // Set the keyboard callback so that when we press ESC, it knows what to do.
    glfwSetKeyCallback(window, key_callback);
    
    
    // set the cursor callback
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    
    
    return window;
}



/*!
 This function initializes glew
 */
bool initGlew(void)
{
    
    // Makes sure all extensions will be exposed in GLEW and initialize GLEW.
    glewExperimental = GL_TRUE;
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        cout <<  "Failed to initialize GLEW\n" << endl;
        system("pause");
        return -1;
    }
    
    cout <<  "OpenGL version supported by this platform " << glGetString(GL_VERSION) <<  endl;
    
    
    // Check the GLSL version
    const char *verstr = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
    cout << "GLSL version supported by this platform " << string (verstr) << endl;
    sscanf(verstr, "%d.%d", &glsl_major, &glsl_minor);
    
    return true;
}




