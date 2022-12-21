#include <glad.h>
#include <GLFW/glfw3.h>

#include <typeinfo>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>

#include "../support/error.hpp"
#include "../support/program.hpp"
#include "../support/checkpoint.hpp"
#include "../support/debug_output.hpp"

#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"

#include "defaults.hpp"
#include "Texture2D.h"
#include <iostream>
#include "ShaderProgram2.h"
#include "Camera.h"
#include "Mesh.h"
namespace
{
	constexpr char const* kWindowTitle = "COMP3811 - Coursework 2";
	
	void glfw_callback_error_( int, char const* );

	void glfw_callback_key_( GLFWwindow*, int key, int scancode, int action, int mode);

	void glfw_onMouseMove_(GLFWwindow* window, double posX, double posY);

	void glfw_onMouseScroll_(GLFWwindow* window, double deltaX, double deltaY);

	void update(double elapsedTime);

	struct State_
	{
		ShaderProgram* prog;

		struct CamCtrl_
		{
			bool cameraActive;
			bool actionZoomIn, actionZoomOut;

			float phi, theta;
			float radius;

			float lastX, lastY;
		} camControl;
	};

	struct GLFWCleanupHelper
	{
		~GLFWCleanupHelper();
	};
	struct GLFWWindowDeleter
	{
		~GLFWWindowDeleter();
		GLFWwindow* window;
	};
	bool initOpenGL();


}
bool gWireframe = false;
const int gWindowHeight = 720;
const int gWindowWidth = 1280;
GLFWwindow* window = NULL;
const std::string texture2File = "textures/crackedSand.jpg";
const std::string texture1File = "textures/crate.jpg";

OrbitCamera orbitCam;
float gYaw = 0.0f;
float gPitch = 0.0f;
float gRadius = 10.0f;
const float MOUSE_SENSITIVITY = 0.25f;

FPSCamera fpsCamera(Vec3f{ 0.0f, 0.0f, 5.0f });
const double ZOOM_SENSITIVITY = -3.0;
const double MOVE_SPEED = 5.0;
const float MOUSE_SENSITIVITY_FPS = 0.1f;



int main() try
{
	// Initialize GLFW
	if( GLFW_TRUE != glfwInit() )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwInit() failed with '%s' (%d)", msg, ecode );
	}

	if (!initOpenGL()) {
		glfwTerminate();
	}

	// Ensure that we call glfwTerminate() at the end of the program.
	GLFWCleanupHelper cleanupHelper;

	// Get actual framebuffer size.
	// This can be different from the window size, as standard window
	// decorations (title bar, borders, ...) may be included in the window size
	// but not be part of the drawable surface area.
	int iwidth, iheight;
	glfwGetFramebufferSize(window, &iwidth, &iheight);

	glViewport(0, 0, iwidth, iheight);

	// Other initialization & loading
	OGL_CHECKPOINT_ALWAYS();


	GLfloat vertex_position[] = {
		// position		 // tex coords

	   // front face
	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
	   -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		// back face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		 // left face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		 // right face
		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		  // top face
		 -1.0f,  1.0f, -1.0f, 0.0f, 8.0f,
		  1.0f,  1.0f,  1.0f, 8.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 8.0f, 8.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 8.0f,
		 -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		  1.0f,  1.0f,  1.0f, 8.0f, 0.0f,

		  // bottom face
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	};

	Vec3f cubePos = { 0.0f, 0.0f, -2.0f };
	Vec3f floorPos = { 0.0f, -1.0f, 0.0f };
	float cubeAngle = 0.0f;
	double lastTime = glfwGetTime();

	GLfloat vertex_color[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,  // First Triangle
		0, 2, 3   // Second Triangle
	};



	//Create buffers
	GLuint vbo, vao;

	//Position and tex
	glGenBuffers(1, &vbo); //create buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_position), vertex_position, GL_STATIC_DRAW); //Move to GPU


	//glGenBuffers(1, &vbo2); //create buffer
	//glBindBuffer(GL_ARRAY_BUFFER, vbo2); // bind buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_color), vertex_color, GL_STATIC_DRAW); //Move to GPU

	glGenVertexArrays(1, &vao); //create vao
	glBindVertexArray(vao); //bind vao

	// Position attribute
	//glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	

	// Texture Coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); //5 stride, 3 float offset
	glEnableVertexAttribArray(1);

	//glGenBuffers(1, &ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Color attribute
	//glBindBuffer(GL_ARRAY_BUFFER, vbo2); // bind buffer
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	//glEnableVertexAttribArray(1);

	ShaderProgram2 shaderProg;
	shaderProg.loadShaders("assets/default.vert", "assets/default.frag");


	//Model Positions
	Vec3f modelPos[] = {
		Vec3f{-2.5f,-1.0f,5.0f}, //tank
		Vec3f{4.5f,-0.5f,-2.0f}, //crate
		Vec3f{3.5f,-2.0f,10.0f}
	};

	//Model scale
	Vec3f modelScale[] = {
		Vec3f{1.f, 1.f, 1.f}, //tank
		Vec3f{0.5f, 0.5f, 0.5f}, //crate
		Vec3f{1.f, 1.f, 1.f}
	};

	Vec3f modelRotation[] = {
		Vec3f{90.0f, 180.0f, 180.0f}, //tank
		Vec3f{0.0f, 0.0f, 0.0f}, //crate
		Vec3f{0.0f, 0.0f, 0.0f}
	};

	//Load Meshes
	const int numModels = 3;
	Mesh mesh[numModels];
	Texture2D texture[numModels];
	mesh[0].loadOBJ("models/tank1.obj");
	mesh[1].loadOBJ("models/crate.obj");
	mesh[2].loadOBJ("models/FantasyHouse.obj");

	texture[0].loadTexture("textures/tank1.jpg", true);
	texture[1].loadTexture("textures/crate.jpg", true);
	texture[2].loadTexture("textures/FantasyHouse.png", true);



	Texture2D texture1;
	texture1.loadTexture(texture1File, true);
	Texture2D texture2;
	texture2.loadTexture(texture2File, true);

	OGL_CHECKPOINT_ALWAYS();

	// Main loop
	while( !glfwWindowShouldClose( window ) )
	{

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		// Let GLFW process events
		glfwPollEvents();
		
		update(deltaTime);
		// Check if window was resized.
		float fbwidth, fbheight;
		{
			int nwidth, nheight;
			glfwGetFramebufferSize( window, &nwidth, &nheight );

			fbwidth = float(nwidth);
			fbheight = float(nheight);

			if( 0 == nwidth || 0 == nheight )
			{
				// Window minimized? Pause until it is unminimized.
				// This is a bit of a hack.
				do
				{
					glfwWaitEvents();
					glfwGetFramebufferSize( window, &nwidth, &nheight );
				} while( 0 == nwidth || 0 == nheight );
			}

			glViewport( 0, 0, iwidth, iheight );
		}
	
		// Draw scene
		OGL_CHECKPOINT_DEBUG();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture1.bind(0);
		//texture2.bind(1);
		cubeAngle += (float)(deltaTime * 20.0f);
		if (cubeAngle >= 360) cubeAngle = 0.0f;

		Mat44f model(kIdentity44f), view(kIdentity44f), projection(kIdentity44f);

		orbitCam.setLookAt(cubePos);
		orbitCam.rotate(gYaw, gPitch);
		orbitCam.setRadius(gRadius);

		model = model*make_translation(cubePos);

		//Orbit Cam
		//view = orbitCam.getViewMatrix();
		//projection = make_perspective_projection(makeRadians(45.0f), fbwidth / float(fbheight), 0.1f, 100.0f);

		//FPS Cam
		view = fpsCamera.getViewMatrix();
		projection = make_perspective_projection(makeRadians(fpsCamera.getFOV()), fbwidth / float(fbheight), 0.1f, 100.0f);
		
		//Use Shader
		//glUseProgram(prog.programId());
		shaderProg.use();
		shaderProg.setUniform("model", model);
		shaderProg.setUniform("view", view);
		shaderProg.setUniform("projection", projection);
		//glUniform1i(glGetUniformLocation(shaderProg.getProgram(), "myTexture"), 0);
		//glUniform1i(glGetUniformLocation(shaderProg.getProgram(), "myTexture2"), 1);
		//shaderProg.setUniform("vertColor", Vec4f{ 0.8f, 0.6f, 0.2f, 1.0f });
		
		//Bind vao
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		

		texture2.bind(0);
		model = model * make_translation(floorPos) * make_scaling(100.0f, 0.01f, 100.f);
		shaderProg.setUniform("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Unbind VAO
		glBindVertexArray(0);
		for (int i = 0; i < numModels; i++) {
			model = kIdentity44f * make_scaling(modelScale[i].x, modelScale[i].y, modelScale[i].z) * make_translation(modelPos[i]) * make_rotation_x(makeRadians(modelRotation[i].x))*make_rotation_y(makeRadians(modelRotation[i].y))* make_rotation_z(makeRadians(modelRotation[i].z));
			shaderProg.setUniform("model", model);
			texture[i].bind(0);
			mesh[i].draw();
			texture[i].unbind(0);
		}

		glUseProgram(0);

		//TODO: draw frame

		OGL_CHECKPOINT_DEBUG();

		// Display results
		glfwSwapBuffers( window );
		lastTime = currentTime;
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	return 0;
}
catch( std::exception const& eErr )
{
	std::fprintf( stderr, "Top-level Exception (%s):\n", typeid(eErr).name() );
	std::fprintf( stderr, "%s\n", eErr.what() );
	std::fprintf( stderr, "Bye.\n" );
	return 1;
}


namespace
{
	void glfw_callback_error_( int aErrNum, char const* aErrDesc )
	{
		std::fprintf( stderr, "GLFW error: %s (%d)\n", aErrDesc, aErrNum );
	}

	void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int )
	{
		if( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction )
		{
			glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
			return;
		}
		if (GLFW_KEY_Q == aKey && GLFW_PRESS == aAction)
		{
			gWireframe = !gWireframe;
			if (gWireframe) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
	}

	void glfw_onMouseMove_(GLFWwindow* window, double posX, double posY)
	{
	//	static Vec2f lastMousePosition = Vec2f{ 0,0 };
	//	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1)
	//	{
	//		gYaw -= ((float)posX - lastMousePosition.x) * MOUSE_SENSITIVITY;
	//		gPitch += ((float)posY - lastMousePosition.y) * MOUSE_SENSITIVITY;
	//	}
	//	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1)
	//	{
	//		float dx = 0.01f * ((float)posX - lastMousePosition.x);
	//		float dy = 0.01f * ((float)posY - lastMousePosition.y);
	//		gRadius += dx - dy;
	//	}
	//	lastMousePosition.x = (float)posX;
	//	lastMousePosition.y = (float)posY;
	}

	void glfw_onMouseScroll_(GLFWwindow* window, double deltaX, double deltaY)
	{
		double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;
		fov = clamp(fov, 1.0, 120.0);
		fpsCamera.setFOV((float)fov);
	}

	void update(double elapsedTime)
	{
		//Cam orientation
		double mouseX, mouseY;

		//Get current mouse position change
		glfwGetCursorPos(window, &mouseX, &mouseY);

		//Rotate camera the amount that the mouse has moved * sensitivity
		fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY_FPS, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY_FPS);
		//Keep mouse in center of screen
		glfwSetCursorPos(window, gWindowWidth / 2.0, gWindowHeight / 2);
	
		//Forward/back
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());
		//Left/right
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
		//Up/down
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
	}

	bool initOpenGL() {
		glfwSetErrorCallback(&glfw_callback_error_);

		//glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
		//glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

		//glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#	if !defined(NDEBUG)
		// When building in debug mode, request an OpenGL debug context. This
		// enables additional debugging features. However, this can carry extra
		// overheads. We therefore do not do this for release builds.
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#	endif // ~ !NDEBUG
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		window = glfwCreateWindow(
			gWindowWidth,
			gWindowHeight,
			kWindowTitle,
			nullptr, nullptr
		);
		if (window == NULL)
		{
			std::fprintf(stderr, "Error creating window");
			return false;
		}
		glfwMakeContextCurrent(window); //use window created above


		// Set up drawing stuff

		//glfwSwapInterval(1); // V-Sync is on.

		// Set up event handling
		glfwSetKeyCallback(window, &glfw_callback_key_);
		glfwSetCursorPosCallback(window, glfw_onMouseMove_);
		glfwSetScrollCallback(window, &glfw_onMouseScroll_);

		//Hide cursor, unlimited movement
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, gWindowWidth / 2.0, gWindowHeight / 2.0);

		// Initialize GLAD
		// This will load the OpenGL API. We mustn't make any OpenGL calls before this!
		if (!gladLoadGLLoader((GLADloadproc)&glfwGetProcAddress))
		{
			std::fprintf(stderr, "Error creating window");
			return false;
		}
		std::printf("RENDERER %s\n", glGetString(GL_RENDERER));
		std::printf("VENDOR %s\n", glGetString(GL_VENDOR));
		std::printf("VERSION %s\n", glGetString(GL_VERSION));
		std::printf("SHADING_LANGUAGE_VERSION %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));



		// Ddebug output
#	if !defined(NDEBUG)
		setup_gl_debug_output();
#	endif // ~ !NDEBUG

		// Global GL state
		OGL_CHECKPOINT_ALWAYS();

		// TODO: global GL setup goes here
		glEnable(GL_FRAMEBUFFER_SRGB);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
		glViewport(0, 0, gWindowWidth, gWindowHeight);

		return true;
	}
}

namespace
{
	GLFWCleanupHelper::~GLFWCleanupHelper()
	{
		
		glfwTerminate();
	}

	GLFWWindowDeleter::~GLFWWindowDeleter()
	{
		if( window )
			glfwDestroyWindow( window );
	}
}

