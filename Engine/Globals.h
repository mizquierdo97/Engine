#ifndef _GLOBALS
#define _GLOBALS

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <filesystem>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

namespace fs = std::experimental::filesystem;
typedef unsigned int uint;

void log(const char file[], int line, const char* format, ...);
std::string GetExtension(std::string path);
double FloatPrecision(double value, double precision);
std::string GetFileName(std::string path);
std::string GetFileNameExtension(std::string path);
void NormalizePath(char * full_path);
std::string NormalizePath(std::string full_path);
bool ExistsFile(const std::string& name);
std::string GetFolderPath(std::string path);
void ShaderLog(uint shader);
void ProgramLog(uint program);
#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)


// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }


#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI




enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Engine"
#define ASSETS_PATH "assets/"
#define LIBRARY_PATH "Library/"
#define MESHES_PATH "Library/Meshes/"
#define MATERIALS_PATH "Library/Materials/"


#endif