#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define GLEW_STATIC
#define MUSIC

#pragma comment(lib, "bass")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glew32s")
#pragma comment(lib, "glfw3")

#include "bass.h"
#include "glew.h"
#include "glfw3.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float points[] = {
	-1.0f,-1.0f,0.0f,
	-1.0f,1.0f,0.0f,
	1.0f,1.0f,0.0f,

	1.0f,-1.0f,0.0f,
	-1.0f,-1.0f,0.0f,
	1.0f,1.0f,0.0f
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

char* loadShader(char* shader) {
	FILE *f;
	fopen_s(&f, shader, "rb");

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* string = (char*)malloc(fsize);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = '\0';

	return string;
}

int WinMain(HINSTANCE hInsDtance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	GLFWwindow* window;
	static float time = 0.0;
	static float frameTime = 0.0;
	static HSTREAM handle;
	int	vsid, fsid;
	unsigned int pid;

	char* vertexshader = loadShader("Data\\vertexshader.glsl");
	char* fragmentshader = loadShader("Data\\fragmentshader.glsl");

	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "pc-demo-engine", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();

#ifdef MUSIC
	BASS_Init(-1, 44100, 0, 0, 0);
	handle = BASS_StreamCreateFile(FALSE, "Data\\soundtrack.mp3", 0, 0, 0);
	BASS_ChannelPlay(handle, 0);
#endif

	glEnable(GL_TEXTURE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	vsid = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertexshader);
	fsid = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragmentshader);
	glGenProgramPipelines(1, &pid);
	glBindProgramPipeline(pid);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	while (!glfwWindowShouldClose(window)) {
		time = (float)glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgramStages(pid, GL_VERTEX_SHADER_BIT, vsid);
		glUseProgramStages(pid, GL_FRAGMENT_SHADER_BIT, fsid);

		//glProgramUniform1fEXT(vsid, 1, time);
		glProgramUniform3fEXT(vsid, 0, 1.0f, 1.0f, 1.0f);
		glProgramUniform1fEXT(fsid, 0, time);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(points));

		glfwPollEvents();
		glfwSwapBuffers(window);
		frameTime = (float)glfwGetTime() - time;
	}

#ifdef MUSIC
	BASS_Stop();
	BASS_StreamFree(handle);
	BASS_Free();
#endif

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}