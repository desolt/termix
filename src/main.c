#include <stdio.h>
#include <GLFW/glfw3.h>

int main(void)
{
	if (!glfwInit())
	{
		fprintf(stderr, "GLFW initialization failed\n");
		return -1;
	}

	// Select OpenGL 2.1
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	GLFWwindow *window = glfwCreateWindow(640, 480, "Unnamed Window", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "Window creation failedn");
		return -1;
	}

	glfwMakeContextCurrent(window); // Binds the GL context to this thread
	printf("Hello termix!\n");
	printf("OpenGL information:\n");
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Blue
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	return 0;
}
