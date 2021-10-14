#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <iostream>
#include <vector>
#include <OpenGL/gl.h>

typedef struct MyVertex
  {
    float x, y, z;
    float nx, ny, nz;
    float s0, t0;
  } TMyVertex;

std::vector<MyVertex> m_vertexes;
std::vector<GLuint> m_indexes;

int main(int argc, char *argv[])
{
    for(int i = 1; i < argc; i++)
    {
        std::cout << "Аргумент " << i << ": " << argv[i] << std::endl;


        const aiScene* scene = nullptr;
        Assimp::Importer importer;
        scene = importer.ReadFile(argv[i], NULL);
        if(scene == nullptr){
            std::cout << "initializeGL::Reading file failed!\n";
            exit(EXIT_FAILURE);
        }

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
                if(scene->mNumMeshes != 0 && scene->mMeshes[i]->HasNormals()){
                    //----- tmp -----------------
                    MyVertex tmpVertex;
                    aiVector3D* verteces = scene->mMeshes[i]->mVertices;
                    aiVector3D* normals = scene->mMeshes[i]->mNormals;
                    aiVector3D* texture_coords = scene->mMeshes[i]->mTextureCoords[0];
                    //---------------------------
                    for(unsigned int currentVertex = 0; currentVertex < scene->mMeshes[i]->mNumVertices; ++currentVertex){
                        tmpVertex.x = verteces[currentVertex].x;
                        tmpVertex.y = verteces[currentVertex].y;
                        tmpVertex.z = verteces[currentVertex].z;

                        tmpVertex.nx = normals[currentVertex].x;
                        tmpVertex.ny = normals[currentVertex].y;
                        tmpVertex.nz = normals[currentVertex].z;

                        tmpVertex.s0 = texture_coords[currentVertex].x;
                        tmpVertex.t0 = texture_coords[currentVertex].y;

                        //tmpVertex.s0 = verteces[currentVertex].x;
                        //tmpVertex.t0 = verteces[currentVertex].y;

                        //qDebug() << verteces[currentVertex].x << verteces[currentVertex].y << verteces[currentVertex].z;


                        m_vertexes.push_back(tmpVertex);
                    }
                }

                aiFace* faces = scene->mMeshes[i]->mFaces;
                for(unsigned int currentFace = 0; currentFace < scene->mMeshes[i]->mNumFaces; ++currentFace){
                    for(unsigned short currentFacePoint = 0; currentFacePoint < faces[currentFace].mNumIndices; ++currentFacePoint){
                        m_indexes.push_back((GLuint)faces[currentFace].mIndices[currentFacePoint]);
                    }
                }
            }



    }
}