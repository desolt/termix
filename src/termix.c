#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <GLFW/glfw3.h>
#include "termix.h"
#include "err.h"

// We can easily tweak this later, just prevents high CPU usage for now
#define FRAME_RATE 60

void tx_draw(GLFWwindow * window);
void tx_resize_callback(GLFWwindow * window, int width, int height);

static void print_sysinfo(void)
{
	printf("OpenGL information:\n");
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
}

int tx_init(int argc, char * argv[])
{
	// Suppress unused warnings for now
	(void)argc;
	(void)argv;

	glfwSetErrorCallback(tx_error_callback);

	if (!glfwInit())
	{
		fprintf(stderr, "termix: glfw initialization failed\n");
		tx_print_errors();
		return -1;
	}

	// See http://www.glfw.org/docs/latest/window_guide.html#window_hints
	// Select OpenGL 2.1
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	// Set resizable and visible
	glfwWindowHint(GLFW_RESIZABLE, 1);
	glfwWindowHint(GLFW_VISIBLE, 1);

	return 0;
}

int tx_run(void)
{
	GLFWwindow * window = glfwCreateWindow(1280, 720, "Termix", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "termix: window creation failed\n");
		tx_print_errors();
		return -1;
	}

	// Binds the GL context to this thread
	glfwMakeContextCurrent(window);

	glfwSetWindowRefreshCallback(window, tx_draw);
	glfwSetWindowSizeCallback(window, tx_resize_callback);

#if !defined(NDEBUG)
	print_sysinfo();
#endif

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Blue, just for testing
	if (tx_geterr() != 0)
	{
		tx_print_errors();
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		tx_draw(window);

#if !defined(NDEBUG)
		if (tx_geterr() != 0)
		{
			break;
		}
#endif
		// GLFW 3.2 contains a nice function which we could use here
		// Unfortunately not all package managers have updated to 3.2
		// See http://www.glfw.org/docs/latest/group__window.html#ga605a178db92f1a7f1a925563ef3ea2cf
		usleep(1000000 / FRAME_RATE);
	}

	glfwDestroyWindow(window);

	if (tx_geterr() != 0)
	{
		tx_print_errors();
		return -1;
	}

	return 0;
}

// Called on window resize
void tx_resize_callback(GLFWwindow * window, int width, int height)
{
	tx_draw(window);
	// TODO: Register window changing size
	(void)width;
	(void)height;
}

// Called during the render loop or on refresh
void tx_draw(GLFWwindow * window)
{
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Drawing goes here...

	// Display
	glfwSwapBuffers(window);
}
