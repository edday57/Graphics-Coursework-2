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
#include "SimpleMesh.h"
namespace
{
	constexpr char const* kWindowTitle = "COMP3811 - Coursework 2";
	
	void glfw_callback_error_( int, char const* );

	void glfw_callback_key_( GLFWwindow*, int key, int scancode, int action, int mode);


	void glfw_onMouseScroll_(GLFWwindow* window2, double deltaX, double deltaY);

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
int gWindowHeight = 720;
int gWindowWidth = 1280;
GLFWwindow* window = NULL;
const std::string texture2File = "textures/crackedSand.jpg";
const std::string texture1File = "textures/crate.jpg";

OrbitCamera orbitCam;
float gYaw = 0.0f;
float gPitch = 0.0f;
float gRadius = 10.0f;
const float MOUSE_SENSITIVITY = 0.25f;

FPSCamera fpsCamera(Vec3f{ 0.0f, 3.0f, 5.0f });
const double ZOOM_SENSITIVITY = -3.0;
double MOVE_SPEED = 5.0;
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

	double lastTime = glfwGetTime();
	float angle = 0.0f;




	ShaderProgram2 shaderProg;
	shaderProg.loadShaders("assets/directionalLighting.vert", "assets/directionalLighting.frag");

	ShaderProgram2 lightProg;
	lightProg.loadShaders("assets/lighting.vert", "assets/lighting.frag");

	//Model Positions
	Vec3f modelPos[] = {
		Vec3f{5.5f,-1.4f,-13.0f}, //tank
		Vec3f{-10.5f,-.0f,-10.0f}, //house
		Vec3f{-6.5f,-.0f,-5.5f}, //diamond ore
		Vec3f{ 0.0f, -.0f, -2.0f }, //crate
		Vec3f{ 0.0f, .0f, 0.0f }, //floor
		Vec3f{ -9.0f, .0f, -0.0f }, //barrel
		Vec3f{ -0.0f, .0f, 8.0f },
	};

	//Model scale
	Vec3f modelScale[] = {
		Vec3f{1.5f, 1.5f, 1.5f}, //tank
		Vec3f{.6f, .6f, .6f}, //crate
		Vec3f{1.f, 1.f, 1.f},  //diamond ore
		Vec3f{1.f, 1.f, 1.f},
		Vec3f{30.f, 1.f, 30.f}, //floor
		Vec3f{0.7f, 0.7f, 0.7f},
		Vec3f{1.2f, 1.2f, 1.2f},
	};

	Vec3f modelRotation[] = {
		Vec3f{0.0f, -15.0f, 0.0f}, //tank
		Vec3f{0.0f, 32.0f, 0.0f}, //crate
		Vec3f{0.0f, 34.0f, 0.0f},
		Vec3f{0.0f, 0.0f, 0.0f},
		Vec3f{0.0f, 0.0f, 0.0f},
		Vec3f{ 0.0f, .0f, 0.0f },
		Vec3f{ 0.0f, 180.0f, 0.0f },
	};

	//Load Meshes
	const int numModels = 7;
	Mesh mesh[numModels];
	Texture2D texture[numModels];
	mesh[0].loadOBJ("models/watchtower.obj");
	mesh[1].loadOBJ("models/cottage_obj.obj");
	mesh[2].loadOBJ("models/crate.obj");
	mesh[3].loadOBJ("models/woodcrate.obj");
	mesh[4].loadOBJ("models/floor.obj");
	mesh[5].loadOBJ("models/barrel.obj");
	mesh[6].loadOBJ("models/robot.obj");

	//texture[0].loadTexture("textures/tank1.jpg", true);
	//texture[1].loadTexture("textures/crate.jpg", true);
	texture[0].loadTexture("textures/watchtower.jpg", true);
	texture[1].loadTexture("textures/cottage_diffuse.png", true);
	texture[2].loadTexture("textures/crate.jpg", true);
	texture[3].loadTexture("textures/woodcrate_diffuse.jpg", true);
	texture[4].loadTexture("textures/crackedSand.jpg", true);
	texture[5].loadTexture("textures/barrel_diffuse.png", true);
	texture[6].loadTexture("textures/robot_diffuse.jpg", true);
	

	//Lighting
	Mesh lightMesh;
	lightMesh.loadOBJ("models/light.obj");
	Vec3f lightPos = { -5.f, 5.5f, 2.f };
	Vec3f lightCol = { 0.8f, 0.8f, 0.65f };
	Vec3f liightDirection = { 0.f, -0.9f, -0.17f };



	Texture2D texture1;
	texture1.loadTexture(texture1File, true);
	Texture2D texture2;
	texture2.loadTexture(texture2File, true);

	OGL_CHECKPOINT_ALWAYS();

	//SimpleMeshData armadillo = load_wavefront_obj("assets/Armadillo.obj");
	//GLuint vaoObj = create_vao(armadillo);
	//std::size_t vertexCountObj = armadillo.positions.size();

	// Main loop
	while( !glfwWindowShouldClose( window ) )
	{

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		angle += (float)deltaTime * 50.0f;
		lightPos.x = 8.0f * sinf(makeRadians(angle)) + 5.f;  // slide back and forth
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
			gWindowHeight = nheight;
			gWindowWidth = nwidth;

			glViewport( 0, 0, nwidth, nheight );
		}
	
		// Draw scene
		OGL_CHECKPOINT_DEBUG();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		Mat44f model(kIdentity44f), view(kIdentity44f), projection(kIdentity44f);

		//orbitCam.setLookAt(cubePos);
		//orbitCam.rotate(gYaw, gPitch);
		//orbitCam.setRadius(gRadius);



		//Orbit Cam
		//view = orbitCam.getViewMatrix();
		//projection = make_perspective_projection(makeRadians(45.0f), fbwidth / float(fbheight), 0.1f, 100.0f);

		//FPS Cam
		view = fpsCamera.getViewMatrix();
		projection = make_perspective_projection(makeRadians(fpsCamera.getFOV()), gWindowWidth / float(gWindowHeight), 0.1f, 100.0f);
		
		Vec3f viewPos;
		viewPos.x= fpsCamera.getPos().x;
		viewPos.y = fpsCamera.getPos().y;
		viewPos.z = fpsCamera.getPos().z;
		viewPos = { 0.f,0.f,0.f };
		//Use Shader
		//glUseProgram(prog.programId());
		shaderProg.use();
		shaderProg.setUniform("model", model);
		shaderProg.setUniform("view", view);
		shaderProg.setUniform("projection", projection);
		shaderProg.setUniform("viewPos", viewPos);

		shaderProg.setUniform("light.ambient", Vec3f{0.2f,0.2f,0.2f});
		shaderProg.setUniform("light.diffuse", lightCol);
		shaderProg.setUniform("light.specular", Vec3f{ .5f,0.5f,0.5f });
		shaderProg.setUniform("light.position", lightPos);
		//glUniform1i(glGetUniformLocation(shaderProg.getProgram(), "myTexture"), 0);
		//glUniform1i(glGetUniformLocation(shaderProg.getProgram(), "myTexture2"), 1);
		//shaderProg.setUniform("vertColor", Vec4f{ 0.8f, 0.6f, 0.2f, 1.0f });
		

		//Model loading
		for (int i = 0; i < numModels; i++) {
			model = kIdentity44f * make_translation(modelPos[i]) * make_scaling(modelScale[i].x, modelScale[i].y, modelScale[i].z) * make_rotation_x(makeRadians(modelRotation[i].x)) * make_rotation_y(makeRadians(modelRotation[i].y)) * make_rotation_z(makeRadians(modelRotation[i].z));
			shaderProg.setUniform("model", model);

			shaderProg.setUniform("material.ambient", Vec3f{0.4f,0.4f,0.4f});
			shaderProg.setUniformSampler("material.diffuseMap", 0);
			shaderProg.setUniform("material.specular", Vec3f{ 0.1f,0.1f,0.1f });
			shaderProg.setUniform("material.shininess", 10.f);
			if (i > 4) {
				shaderProg.setUniform("material.specular", Vec3f{ 0.8f,0.8f,0.7f });
				shaderProg.setUniform("material.shininess", 60.f);
			}
			texture[i].bind(0);
			mesh[i].draw();
			texture[i].unbind(0);
		}
		



		model = kIdentity44f * make_translation(lightPos);
		lightProg.use();
		lightProg.setUniform("lightColor", lightCol);
		lightProg.setUniform("view", view);
		lightProg.setUniform("model", model);
		lightProg.setUniform("projection", projection);
		lightMesh.draw();



		//texture[2].bind(0);
		//model = kIdentity44f;
		//glBindVertexArray(vaoObj);
		//glDrawArrays(GL_TRIANGLES, 0, vertexCountObj);
		//texture[2].unbind(0);


		glUseProgram(0);

		//TODO: draw frame

		OGL_CHECKPOINT_DEBUG();

		// Display results
		glfwSwapBuffers( window );
		lastTime = currentTime;
	}


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
		if (GLFW_KEY_Z == aKey && GLFW_PRESS == aAction)
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



	void glfw_onMouseScroll_(GLFWwindow* window2, double deltaX, double deltaY)
	{
		(void)deltaX;
		(void)window2;
		float fov = (float)fpsCamera.getFOV() + (float)deltaY * (float)ZOOM_SENSITIVITY;
		fov = clamp(fov, 1.0f, 120.0f);
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
			fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());
		//Left/right
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
		//Up/down
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			fpsCamera.move((float)MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			
			if (MOVE_SPEED == 10) {
				MOVE_SPEED = 5;
			}
			else {
				MOVE_SPEED = 10;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			if (MOVE_SPEED == 2.5) {
				MOVE_SPEED = 5;
			}
			else {
				MOVE_SPEED = 2.5;
			}
		}
			
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
		glEnable(GL_CULL_FACE);
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

