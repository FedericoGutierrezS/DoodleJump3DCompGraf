#include "mesh.h"

Vector3** DoTheImportThing(const std::string& pFile, int& faceAmount) {
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_SortByPType);
	// If the import failed, report it

	// Now we can access the file's contents.

	// We're done. Everything will be cleaned up by the importer destructor

	faceAmount = scene->mMeshes[0]->mNumFaces;
	Vector3** model = new Vector3 * [3];
	model[0] = new Vector3[faceAmount * 3];
	model[1] = new Vector3[faceAmount * 3];
	model[2] = new Vector3[faceAmount * 3];
	for (int i = 0; i < faceAmount; i++) {
		//VERTICES
		model[0][i].setX(scene->mMeshes[0]->mVertices[scene->mMeshes[0]->mFaces[i].mIndices[0]].x);
		model[0][i].setY(scene->mMeshes[0]->mVertices[scene->mMeshes[0]->mFaces[i].mIndices[0]].y);
		model[0][i].setZ(scene->mMeshes[0]->mVertices[scene->mMeshes[0]->mFaces[i].mIndices[0]].z);
		model[0][faceAmount + i].setX(scene->mMeshes[0]->mVertices[scene->mMeshes[0]->mFaces[i].mIndices[1]].x);
		model[0][faceAmount + i].setY(scene->mMeshes[0]->mVertices[scene->mMeshes[0]->mFaces[i].mIndices[1]].y);
		model[0][faceAmount + i].setZ(scene->mMeshes[0]->mVertices[scene->mMeshes[0]->mFaces[i].mIndices[1]].z);
		model[0][faceAmount * 2 + i].setX(scene->mMeshes[0]->mVertices[scene->mMeshes[0]->mFaces[i].mIndices[2]].x);
		model[0][faceAmount * 2 + i].setY(scene->mMeshes[0]->mVertices[scene->mMeshes[0]->mFaces[i].mIndices[2]].y);
		model[0][faceAmount * 2 + i].setZ(scene->mMeshes[0]->mVertices[scene->mMeshes[0]->mFaces[i].mIndices[2]].z);
		//NORMALES
		model[1][i].setX(scene->mMeshes[0]->mNormals[scene->mMeshes[0]->mFaces[i].mIndices[0]].x);
		model[1][i].setY(scene->mMeshes[0]->mNormals[scene->mMeshes[0]->mFaces[i].mIndices[0]].y);
		model[1][i].setZ(scene->mMeshes[0]->mNormals[scene->mMeshes[0]->mFaces[i].mIndices[0]].z);
		model[1][faceAmount + i].setX(scene->mMeshes[0]->mNormals[scene->mMeshes[0]->mFaces[i].mIndices[1]].x);
		model[1][faceAmount + i].setY(scene->mMeshes[0]->mNormals[scene->mMeshes[0]->mFaces[i].mIndices[1]].y);
		model[1][faceAmount + i].setZ(scene->mMeshes[0]->mNormals[scene->mMeshes[0]->mFaces[i].mIndices[1]].z);
		model[1][faceAmount * 2 + i].setX(scene->mMeshes[0]->mNormals[scene->mMeshes[0]->mFaces[i].mIndices[2]].x);
		model[1][faceAmount * 2 + i].setY(scene->mMeshes[0]->mNormals[scene->mMeshes[0]->mFaces[i].mIndices[2]].y);
		model[1][faceAmount * 2 + i].setZ(scene->mMeshes[0]->mNormals[scene->mMeshes[0]->mFaces[i].mIndices[2]].z);
		//UVs
		model[2][i].setX(scene->mMeshes[0]->mTextureCoords[0][scene->mMeshes[0]->mFaces[i].mIndices[0]].x);
		model[2][i].setY(scene->mMeshes[0]->mTextureCoords[0][scene->mMeshes[0]->mFaces[i].mIndices[0]].y);
		model[2][i].setZ(scene->mMeshes[0]->mTextureCoords[0][scene->mMeshes[0]->mFaces[i].mIndices[0]].z);
		model[2][faceAmount + i].setX(scene->mMeshes[0]->mTextureCoords[0][scene->mMeshes[0]->mFaces[i].mIndices[1]].x);
		model[2][faceAmount + i].setY(scene->mMeshes[0]->mTextureCoords[0][scene->mMeshes[0]->mFaces[i].mIndices[1]].y);
		model[2][faceAmount + i].setZ(scene->mMeshes[0]->mTextureCoords[0][scene->mMeshes[0]->mFaces[i].mIndices[1]].z);
		model[2][faceAmount * 2 + i].setX(scene->mMeshes[0]->mTextureCoords[0][scene->mMeshes[0]->mFaces[i].mIndices[2]].x);
		model[2][faceAmount * 2 + i].setY(scene->mMeshes[0]->mTextureCoords[0][scene->mMeshes[0]->mFaces[i].mIndices[2]].y);
		model[2][faceAmount * 2 + i].setZ(scene->mMeshes[0]->mTextureCoords[0][scene->mMeshes[0]->mFaces[i].mIndices[2]].z);

	}
	return model;
};

void drawFaces(Vector3** model, int faceAmount, GLuint textura) {
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, textura);
	for (int i = 0; i < faceAmount; i++) {
		glBegin(GL_TRIANGLES);
		glTexCoord3f(model[2][i].getX(), model[2][i].getY(), model[2][i].getZ());
		glNormal3f(model[1][i].getX(), model[1][i].getY(), model[1][i].getZ());
		glVertex3f(model[0][i].getX(), model[0][i].getY(), model[0][i].getZ());
		glTexCoord3f(model[2][faceAmount + i].getX(), model[2][faceAmount + i].getY(), model[2][faceAmount + i].getZ());
		glNormal3f(model[1][faceAmount + i].getX(), model[1][faceAmount + i].getY(), model[1][faceAmount + i].getZ());
		glVertex3f(model[0][faceAmount + i].getX(), model[0][faceAmount + i].getY(), model[0][faceAmount + i].getZ());
		glTexCoord3f(model[2][faceAmount * 2 + i].getX(), model[2][faceAmount * 2 + i].getY(), model[2][faceAmount * 2 + i].getZ());
		glNormal3f(model[1][faceAmount * 2 + i].getX(), model[1][faceAmount * 2 + i].getY(), model[1][faceAmount * 2 + i].getZ());
		glVertex3f(model[0][faceAmount * 2 + i].getX(), model[0][faceAmount * 2 + i].getY(), model[0][faceAmount * 2 + i].getZ());
		glEnd();
	}
	glPopMatrix();
}