#include <string.h>
#include <unistd.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GLFW/glfw3.h>
#include "termix.h"
#include "font.h"

// We can easily tweak this later, just prevents high CPU usage for now
#define FRAME_RATE 60

void tx_draw(GLFWwindow * window);
void tx_resize_callback(GLFWwindow * window, int width, int height);

// TODO: Move this into a struct somewhere
FT_Library ft;

static void print_sysinfo(void)
{
	puts("OpenGL information:");
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
}

tx_err tx_init(int argc, char * argv[])
{
	// Suppress unused warnings for now
	(void)argc;
	(void)argv;

	glfwSetErrorCallback(tx_glfw_error_callback);

	if (glfwInit() == 0)
	{
		fprintf(stderr, "termix: glfw initialization failed\n");
		tx_print_errors(); // Might be some stuff in the error log
		return E_GLFW;
	}

	/* Freetype init */
	if (FT_Init_FreeType(&ft) != 0)
	{
		fprintf(stderr, "termix: freetype initializaion failed\n");
		return E_FT;
	}

	// See http://www.glfw.org/docs/latest/window_guide.html#window_hints
	// Select OpenGL 2.1
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	// Set resizable and visible
	glfwWindowHint(GLFW_RESIZABLE, 1);
	glfwWindowHint(GLFW_VISIBLE, 1);

	return E_NONE;
}

tx_err tx_run(void)
{
	tx_err err = E_NONE;
	GLFWwindow * window = glfwCreateWindow(1280, 720, "Termix", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "termix: window creation failed\n");
		tx_print_errors();
		return E_GLFW;
	}

	// Binds the GL context to this thread
	glfwMakeContextCurrent(window);

	glfwSetWindowRefreshCallback(window, tx_draw);
	glfwSetWindowSizeCallback(window, tx_resize_callback);

#if !defined(NDEBUG)
	print_sysinfo();
#endif

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Blue, just for testing

	FT_Face face;
	char fontface_name[] = "/usr/share/fonts/truetype/FiraSans/FiraSans-Regular.ttf";
	if ((err = tx_load_fontface(&ft, &face, fontface_name)) != E_NONE)
	{
		tx_print_errors();
		return err;
	}

	FT_Set_Pixel_Sizes(face, 0, 10);
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER) != 0)
	{
		fprintf(stderr, "termix: could not load character 'X'\n");
		return E_FT;
	}

	if ((err = tx_unload_fontface(&face)) != E_NONE)
	{
		tx_print_errors();
		return err;
	}


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		tx_draw(window);

#if !defined(NDEBUG)
		if ((err = tx_geterr()) != E_NONE)
			break;
#endif
		// TODO: GLFW 3.2 contains a nice function which we could use here
		// Unfortunately not all package managers have updated to 3.2
		// See http://www.glfw.org/docs/latest/group__window.html#ga605a178db92f1a7f1a925563ef3ea2cf
		// 1000000 microseconds in a second
		usleep(1000000 / FRAME_RATE);
	}

	glfwDestroyWindow(window);

	return err;
}

tx_err tx_cleanup(void)
{
	tx_err err = E_NONE;
	glfwTerminate();

	if (FT_Done_FreeType(ft) != 0)
	{
		fprintf(stderr, "termix: could not cleanup freetype\n");
		return E_FT;
	}

	// Print errors to ensure that we don't leak memory from the log
	if ((err = tx_geterr()) != E_NONE)
	{
		tx_print_errors();
		return err;
	}

	return E_NONE;
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
