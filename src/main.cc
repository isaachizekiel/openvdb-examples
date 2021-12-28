/* main.cc */

#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

/* The widthe and height of the window */
const GLuint WIDTH = 800, HEIGHT = 600;

/* In case a GLFW function fails, an error is reported to the GLFW error
 * callback. You can receive these reports with an error callback. */
static void
error_callback (int error, const char *description)
{
  fprintf (stderr, "Error: %s\n", description);
}

/* Each window has a large number of callbacks that can be set to receive all
 * the various kinds of events. To receive key press and release events,
 * create a key callback function. */
void
key_callback (GLFWwindow *window, int key, int scancode, int action, int mode)
{
  std::cout << key << std::endl;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose (window, GLFW_TRUE);
}

/* The main entry point of the program */
int
main ()
{
  /* Handle to the created combined window and context object */
  GLFWwindow *window;

  /* Callback functions must be set, so GLFW knows to call them.
   * The function to set the error callback is one of the few GLFW functions
   * that may be called before initialization,
   * which lets you be notified of errors both during and after initialization.
   */
  glfwSetErrorCallback (error_callback);

  /* Before you can use most GLFW functions, the library must be initialized.
   * On successful initialization, GLFW_TRUE is returned.
   * If an error occurred, GLFW_FALSE is returned. */
  if (!glfwInit ())
    exit (EXIT_FAILURE);

  /* By default, the OpenGL context GLFW creates may have any version.
   * You can require a minimum OpenGL version by setting the
   * GLFW_CONTEXT_VERSION_MAJOR and GLFW_CONTEXT_VERSION_MINOR hints before
   * creation. If the required minimum version is not supported on the machine,
   * context (and window) creation fails. */
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);

  /* The window and its OpenGL context are created with a single call to
   * glfwCreateWindow, which returns a handle to the created combined window
   * and context object */
  window = glfwCreateWindow (WIDTH, HEIGHT, "Hello GLFW", NULL, NULL);

  if (!window)
    {
      glfwTerminate ();
      exit (EXIT_FAILURE);
    }

  /* Before you can use the OpenGL API, you must have a current OpenGL context.
   * The context will remain current until you make another context current or
   * until the window owning the current context is destroyed. */
  glfwMakeContextCurrent (window);
  glfwSwapInterval (1);

  /* If you are using an extension loader library to access modern OpenGL
   * then this is when to initialize it, as the loader needs a
   * current context to load from. This example uses glad,
   * but the same rule applies to all such libraries */
  if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress))
    {
      fprintf (stderr, "Error: Failed to initialize OpenGL context\n");
      return -1;
    }

  /* This function sets the key callback of the specified window,
   * which is called when a key is pressed, repeated or released. */
  glfwSetKeyCallback (window, key_callback);

  /* Each window has a flag indicating whether the window should be closed. */
  while (!glfwWindowShouldClose (window))
    {
      // render here
      glClearColor (0.2f, 0.3f, 0.3f, 1.0f);
      glClear (GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers (window);
      glfwPollEvents ();
    }

  /* When a window and context is no longer needed, destroy it. */
  glfwDestroyWindow (window);

  /* When you are done using GLFW, typically just before the application exits,
   * you need to terminate GLFW. */
  glfwTerminate ();
  exit (EXIT_SUCCESS);
}
