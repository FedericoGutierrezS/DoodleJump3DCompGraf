#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "Vector3.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

Vector3** DoTheImportThing(const std::string& pFile, int& faceAmount);

void drawFaces(Vector3** model, int faceAmount, GLuint textura);