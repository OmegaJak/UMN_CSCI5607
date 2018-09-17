//HW 0 - Moving Square
//Starter code for the first homework assignment.
//This code assumes SDL2 and OpenGL are both properly installed on your system

#include "glad/glad.h"  //Include order can matter here
#if defined(__APPLE__) || defined(__linux__)
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_opengl.h>
#else
	#include <SDL.h>
	#include <SDL_opengl.h>
#endif
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>
using namespace std;
using namespace std::chrono;

//Name of image texture
string textureName = "test.ppm";

//Globals to store the state of the square (position, width, and angle)
float g_pos_x = 0.0f;
float g_pos_y = 0.0f;
float g_size = 0.6f;
float g_angle = 0;
unsigned char* g_img_data;

const float default_color[] = { 1.0f, 1.0f, 1.0f };
const float translating_color[] = { 1.0f, 0.0f, 0.0f };
const float scaling_color[] = { 0.0f, 1.0f, 0.0f };
const float rotating_color[] = { 0.0f, 0.0f, 1.0f };
const float* g_background_color = default_color;

float vertices[] = {  //These values should be updated to match the square's state when it changes
//    X      Y     R     G     B     U     V
	 0.3f,  0.3f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
	 0.3f, -0.3f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
	-0.3f,  0.3f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left 
	-0.3f, -0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
}; 

int window_width = 800;
int window_height = 800;
float aspect_ratio = 1;
int lightness_increment = 15;

float g_mouse_down = false;
float g_clicked_x = -1;
float g_clicked_y = -1;
float g_lastCenter_x = -1;
float g_lastCenter_y = -1;
float g_clicked_angle = -1;
float g_clicked_size = -1;

void mouseClicked(float mx, float my);
void mouseDragged(float mx, float my);
float distanceFrom(float x1, float y1, float x2, float y2);
void updateTextureData(int img_w, int img_h);

bool g_bTranslate = false;
bool g_bRotate = false;
bool g_bScale = false;
bool g_fun_mode = false;
bool g_motion_indicators = false;

//////////////////////////
///  Begin your code here
/////////////////////////

//Inputs are output variables for returning the image width and heigth
unsigned char* loadImage(int& img_w, int& img_h){

	//Open the texture image file
	ifstream ppmFile;
	ppmFile.open(textureName.c_str());
	if (!ppmFile){
		printf("ERROR: Texture file '%s' not found.\n",textureName.c_str());
		exit(1);
	}

	//Check that this is an ASCII PPM (first line is P3)
	string PPM_style;
	ppmFile >> PPM_style; //Read the first line of the header    
	if (PPM_style != "P3") {
		printf("ERROR: PPM Type number is %s. Not an ASCII (P3) PPM file!\n",PPM_style.c_str());
		exit(1);
	}

	//Read in the texture width and height
	ppmFile >> img_w >> img_h;
	unsigned char* img_data = new unsigned char[4*img_w*img_h];

	//Check that the 3rd line is 255 (ie., this is an 8 bit/pixel PPM)
	int maximum;
	ppmFile >> maximum;
	if (maximum != 255) {
		printf("ERROR: Maximum size is (%d) not 255.\n",maximum);
		exit(1);
	}

	int y_coord = img_h - 1;
	int red, green, blue;
	int pixel_index = 0;
	int base_index;
	int array_width = img_w * 4;
	while (ppmFile >> red >> green >> blue) {
		base_index = y_coord * array_width + (pixel_index % img_w) * 4;
		img_data[base_index] = red;
		img_data[base_index + 1] = green;
		img_data[base_index + 2] = blue;
		img_data[base_index + 3] = 255; // Alpha
		pixel_index++;
		if (pixel_index % img_w == 0) {
			y_coord--;
		}
	}

	return img_data;
}

//TODO: Account for rotation by g_angle
void updateVertices() {
	float r = sqrtf(g_size * g_size * 2); // c = sqrt(a^2 + b^2), but a == b
	float vx = r * cosf(g_angle + M_PI_4);
	float vy =  r * sinf(g_angle + M_PI_4);
	vertices[0] = g_pos_x + vx;  //Top right x
	vertices[1] = g_pos_y + vy;  //Top right y
	
	vx = r * cosf(g_angle - M_PI_4);
	vy = r * sinf(g_angle - M_PI_4);
	vertices[7] = g_pos_x + vx;  //Bottom right x
	vertices[8] = g_pos_y + vy;  //Bottom right y
	
	vx = r * cosf(g_angle + M_PI - M_PI_4);
	vy = r * sinf(g_angle + M_PI - M_PI_4);
	vertices[14] =  g_pos_x + vx;  //Top left x
	vertices[15] =  g_pos_y + vy;  //Top left y
	
	vx = r * cosf(g_angle + M_PI + M_PI_4);
	vy = r * sinf(g_angle + M_PI + M_PI_4);
	vertices[21] =  g_pos_x + vx;  //Bottom left x
	vertices[22] =  g_pos_y + vy;  //Bottom left y

	// Adjust for aspect ratio changes
	for (int i = 0; i <= 21; i += 7) {
		vertices[i] /= aspect_ratio;
	}
}

//TODO: Choose between translate, rotate, and scale based on where the user clicked
// I've implemented the logic for translate and scale, but you need to add rotate
void mouseClicked(float m_x, float m_y){   
	printf("Clicked at %f, %f\n",m_x,m_y);
	g_clicked_x = m_x;
	g_clicked_y = m_y;
	g_lastCenter_x = g_pos_x;
	g_lastCenter_y = g_pos_y;
	g_clicked_angle = g_angle;
	g_clicked_size = g_size;
	
	g_bTranslate = false;
	g_bRotate = false;
	g_bScale = false;
	
	// x and y is the click position normalized to size of the square, with (-1,-1) at one corner (1,1) the other. (0, 0) is the center
	float x = (m_x * aspect_ratio) - g_pos_x;
	float y = m_y - g_pos_y;
	x = x / g_size;
	y = y / g_size;
	
	// Account for rotation
	float s = sin(-g_angle);
	float c = cos(-g_angle);
	float newX = x * c - y * s;
	y = x * s + y * c;
	x = newX;
	
	if (x > 1.05 || y > 1.05 || x < -1.05 || y < -1.05)
		return;

	bool isOnLeft = x < -0.9;
	bool isOnRight = x > 0.9;
	bool isOnTop = y > 0.9;
	bool isOnBottom = y < -0.9;
	if (x < .9 && x > -.9 && y < .9 && y > -.9) {
		printf("Translate!\n");
		g_bTranslate = true;
		g_background_color = translating_color;
	} else if ((isOnBottom && (isOnLeft || isOnRight)) || (isOnTop && (isOnLeft || isOnRight))) {
		printf("Rotate!\n");
		g_bRotate = true;
		g_background_color = rotating_color;
	} else {
		printf("Scale!\n");
		g_bScale = true;
		g_background_color = scaling_color;
	}
}

void mouseReleased() {
	g_background_color = default_color;
}

void mouseDragged(float m_x, float m_y) {
	m_x = m_x * aspect_ratio;

	if (g_bTranslate) {
		g_pos_x = m_x - (g_clicked_x * aspect_ratio) + g_lastCenter_x;
		g_pos_y = m_y - g_clicked_y + g_lastCenter_y;
	}
	
	if (g_bScale) { // Compute the new size, g_size, based on the mouse positions
		float initialDistance = distanceFrom(g_lastCenter_x, g_lastCenter_y, g_clicked_x * aspect_ratio, g_clicked_y); // Could calculate just once per click and store in a global but meh
		float currentDistance = distanceFrom(g_lastCenter_x, g_lastCenter_y, m_x, m_y);
		g_size = g_clicked_size * (currentDistance / initialDistance);
	}
	
	if (g_bRotate) { // Compute the new angle, g_angle, based on the mouse positions
		float currentAngle = atan2(m_y - g_pos_y, m_x - g_pos_x);
		printf("CurrentAngle: %f\n", (currentAngle / M_PI) * 180);
		float initialAngle = atan2(g_clicked_y - g_lastCenter_y, (g_clicked_x * aspect_ratio) - g_lastCenter_x);
		printf("initialAngle: %f\n", (initialAngle / M_PI) * 180);

		g_angle = g_clicked_angle + currentAngle - initialAngle;
		printf("g_angle: %f\n", (g_angle / M_PI) * 180);
	}
	
	updateVertices();
}

void funModeUpdate() {
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	float angle_amt = sin(((ms.count() % 10000) / 10000.0) * M_PI * 2);
	//printf("Sine: %f\n", angle_amt);
	g_angle += angle_amt / 250;

	updateVertices();
}

void updateBrightness(int increment, int img_w, int img_h) {
	for (int i = 0; i < img_w * img_h * 4; i++) {
		g_img_data[i] = fmaxf(fminf(255, g_img_data[i] + increment), 0);
	}

	updateTextureData(img_w, img_h);
}

float distanceFrom(float x1, float y1, float x2, float y2) {
	return sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

/////////////////////////////
/// ... below is OpenGL specific code,
///     we will cover it in detail around Week 9,
///     but you should try to poke around a bit righ now.
///     I've annotated some parts with "TODO: Test ..." check those out.
////////////////////////////

// Shader sources
const GLchar* vertexSource =
	"#version 150 core\n"
	"in vec2 position;"
	"in vec3 inColor;"
	"in vec2 inTexcoord;"
	"out vec3 Color;"
	"out vec2 texcoord;"
	"void main() {"
	"   Color = inColor;"
	"   gl_Position = vec4(position, 0.0, 1.0);"
	"   texcoord = inTexcoord;"
	"}";
	 
const GLchar* fragmentSource =
	"#version 150 core\n"
	"uniform sampler2D tex0;"
	"in vec2 texcoord;"
	"out vec3 outColor;"
	"void main() {"
	"   outColor = texture(tex0, texcoord).rgb;"
	"}";
	 
bool fullscreen = false;

void updateTextureData(int img_w, int img_h) {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_w, img_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, g_img_data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

int main(int argc, char *argv[]){
	SDL_Init(SDL_INIT_VIDEO);  //Initialize Graphics (for OpenGL)
	
	//Ask SDL to get a fairly recent version of OpenGL (3.2 or greater)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	 
	//Create a window (offsetx, offsety, width, height, flags)
	SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 100, 100, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	//TODO: Test your understanding: Try changing the title of the window to something more personalized.
	 
	//The above window cannot be resized which makes some code slightly easier.
	//Below show how to make a full screen window or allow resizing
	//SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 0, 0, window_width, window_height, SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL);
	//SDL_Window* window = SDL_CreateWindow("My OpenGL Program", 100, 100, window_width, window_height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
	//SDL_Window* window = SDL_CreateWindow("My OpenGL Program",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,0,0,SDL_WINDOW_FULLSCREEN_DESKTOP|SDL_WINDOW_OPENGL); //Boarderless window "fake" full screen
	 
	updateVertices(); //set initial position of the square to match it's state
	 
	//Create a context to draw in
	SDL_GLContext context = SDL_GL_CreateContext(window);
	 
	//OpenGL functions using glad library
	if (gladLoadGLLoader(SDL_GL_GetProcAddress)){
		printf("OpenGL loaded\n");
		printf("Vendor:   %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version:  %s\n", glGetString(GL_VERSION));
	} else {
		printf("ERROR: Failed to initialize OpenGL context.\n");
		return -1;
	}

	//// Allocate Texture 0 (Created in Load Image) ///////
	GLuint tex0;
	glGenTextures(1, &tex0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

	//What to do outside 0-1 range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR
	//TODODONE: Test your understanding: Try GL_LINEAR instead of GL_NEAREST on the 4x4 test image. What is happening?


	int img_w, img_h;
	g_img_data = loadImage(img_w,img_h);
	printf("Loaded Image of size (%d,%d)\n",img_w,img_h);
	//memset(img_data,0,4*img_w*img_h); //Load all zeros
	//Load the texture into memory
	updateTextureData(img_w, img_h);
	//// End Allocate Texture ///////


	//Build a Vertex Array Object. This stores the VBO and attribute mappings in one object
	GLuint vao;
	glGenVertexArrays(1, &vao); //Create a VAO
	glBindVertexArray(vao); //Bind the above created VAO to the current context


	//Allocate memory on the graphics card to store geometry (vertex buffer object)
	GLuint vbo;
	glGenBuffers(1, &vbo);  //Create 1 buffer called vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //Set the vbo as the active array buffer (Only one buffer can be active at a time)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); //upload vertices to vbo
	//GL_STATIC_DRAW means we won't change the geometry, GL_DYNAMIC_DRAW = geometry changes infrequently
	//GL_STREAM_DRAW = geom. changes frequently.  This effects which types of GPU memory is used


	//Load the vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	//Let's double check the shader compiled 
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (!status){
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf("Vertex Shader Compile Failed. Info:\n\n%s\n",buffer);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//Double check the shader compiled 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (!status){
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf("Fragment Shader Compile Failed. Info:\n\n%s\n",buffer);
	}

	//Join the vertex and fragment shaders together into one program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor"); // set output
	glLinkProgram(shaderProgram); //run the linker

	glUseProgram(shaderProgram); //Set the active shader (only one can be used at a time)


	//Tell OpenGL how to set fragment shader input 

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	//               Attribute, vals/attrib., type, normalized?, stride, offset
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib); //Binds the VBO current GL_ARRAY_BUFFER 

	GLint colAttrib = glGetAttribLocation(shaderProgram, "inColor");
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(colAttrib);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "inTexcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(5*sizeof(float)));
		

	//Event Loop (Loop forever processing each event as fast as possible)
	SDL_Event windowEvent;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&windowEvent)) {  //Process input events (e.g., mouse & keyboard)
			//List of keycodes: https://wiki.libsdl.org/SDL_Keycode - You can catch many special keys
			//Scancode referes to a keyboard position, keycode referes to the letter (e.g., EU keyboards)
			switch (windowEvent.type) {
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYUP:
					if (windowEvent.key.keysym.sym == SDLK_ESCAPE)
						done = true; //Exit event loop

					if (windowEvent.key.keysym.sym == SDLK_f) //If "f" is pressed
						fullscreen = !fullscreen;

					if (windowEvent.key.keysym.sym == SDLK_EQUALS) {
						printf("Brighten!\n");
						updateBrightness(lightness_increment, img_w, img_h);
					}
					
					if (windowEvent.key.keysym.sym == SDLK_MINUS) {
						printf("Darken!\n");
						updateBrightness(-lightness_increment, img_w, img_h);
					}

					if (windowEvent.key.keysym.sym == SDLK_r) {
						printf("Reset!\n");
						g_pos_x = 0.0f;
						g_pos_y = 0.0f;
						g_size = 0.6f;
						g_angle = 0;
						g_fun_mode = false;
						updateVertices();
					}

					if (windowEvent.key.keysym.sym == SDLK_s) {
						if (!g_fun_mode) {
							printf("Fun mode activated!\n");
						} else {
							printf("Fun mode deactivated!\n");
						}

						g_fun_mode = !g_fun_mode;
					}

					if (windowEvent.key.keysym.sym == SDLK_g) {
						if (g_motion_indicators) {
							printf("Motion indicators deactivated!");
						} else {
							printf("Motion indicators activated!");
						}

						g_motion_indicators = !g_motion_indicators;
					}
					break;
				case SDL_WINDOWEVENT:
					// Docs & example found here: https://wiki.libsdl.org/SDL_WindowEventID#SDL_WindowEventID-1
					if (windowEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
						window_width = windowEvent.window.data1;
						window_height = windowEvent.window.data2;
						aspect_ratio = window_width/(float)window_height;
						printf("Resized! Width: %i, Height %i\n", window_width, window_height);
						glViewport(0, 0, window_width, window_height);
						updateVertices();
					}
					break;
			};
			
			SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); //Set to full screen 
		}
		
		
		int mx, my;
		if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT)) { //Is the mouse down?
			if (g_mouse_down == false){
				mouseClicked(2*mx/(float)window_width - 1, 1-2*my/(float)window_height);
			} else {
				mouseDragged(2*mx/(float)window_width-1, 1-2*my/(float)window_height);
			}
			g_mouse_down = true;
		} else {
			if (g_mouse_down = true)
				mouseReleased();
				
			g_mouse_down = false;
		}

		if (g_fun_mode) {
			funModeUpdate();
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); //upload vertices to vbo

		if (!g_motion_indicators) g_background_color = default_color; // Override changes if motion indicators are off

		// Clear the screen to white
		glClearColor(g_background_color[0], g_background_color[1], g_background_color[2], 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
					
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //Draw the two triangles (4 vertices) making up the square
		//TODO: Test your understanding: What shape do you expect to see if you change the above 4 to 3?  Guess, then try it!

		SDL_GL_SwapWindow(window); //Double buffering
	}

	delete [] g_img_data;
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);


	//Clean Up
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}
