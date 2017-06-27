#include "al/core/gl/al_GLEW.hpp"

#include <iostream>
#include <cstdlib> // exit, EXIT_FAILURE

using namespace al;

bool& glew_loaded() {
  static bool b;
  return b;
}

void glew::init() {
  // static bool inited = false;
  if (glew_loaded()) {
    std::cout << "GLEW already initialized" << std::endl;
    return;
  }
#if GLEW_VERSION < 2
  glewExperimental=GL_TRUE; // MUST if glew version is 1.13 or lower (apt!!!!)
#endif
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cout << "Glew Error: " << glewGetErrorString(err) << std::endl;
    exit(EXIT_FAILURE); // FIXME? is this recommended way of terminating?
  }
  std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
  // inited = true;/
  glew_loaded() = true;
}


bool gl::loaded() {
  return glew_loaded();
}

void gl::default_gl_settings() {
	glDisable(GL_LINE_SMOOTH); // acts weirdly depending on OS / version
	glLineWidth(1); // only 1 is guaranteed to be supported...
}