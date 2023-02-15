#ifndef GUI_H
#define GUI_H

// Bring in OpenGL
// Windows
#ifdef WIN32
#include <windows.h> // Must have for Windows platform builds
#include "glee.h" // OpenGL Extension "autoloader"
#include <gl\gl.h> // Microsoft OpenGL headers
#include <gl\glu.h> // OpenGL Utilities
#include "glut.h"// Glut (Free-Glut on Windows)
#include <GL/glew.h>
#endif

// Mac OS X
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION // Defined before OpenGL and GLUT includes to avoid deprecation messages
//#include <Carbon/Carbon.h> // Brings in most Apple specific stuff
#include <OpenGL/gl.h> // Apple OpenGL haders (version depends on OS X SDK version)
#include <OpenGL/gl3.h> // Required
#include <GL/glew.h>
#define Sleep(x) // Just ignore sleep on Apple
#endif

// Linux
#ifdef linux
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#endif