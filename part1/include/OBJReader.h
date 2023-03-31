#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

// This class is responsible for parsing obj files and storing geometric data.
class OBJReader {
public:
    // Vertices and Normals
    std::vector<float> vertices;
    std::vector<float> vertexNormals;
    std::vector<float> textures;
    std::vector<float> vertexTextures;
    // Indices
    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> textureIndices;
    std::vector<unsigned int> normalIndices;
    std::vector<unsigned int> indexBuffer;

    std::string ppmFileName;
    std::string normalMapFileName;

    // Takes the argument and argument size to parse through filename/s 
    // provided.
    OBJReader(char** argv, int argc);
    OBJReader(std::string fileName);
    
    bool read(int option, std::string fileName = "");
    
private:
    // Scale to translate object into scope
    float scale;
    // specific filename and all fileNames that may have been passed.
    std::string fileName;
    std::string workingDirectory;
    std::string mtlFileName;
    std::vector<std::string> fileNames;
    void createVertexIndexBufffer();
    int exists(float x, float y, float z, float xn, float yn, float zn, float s, float t);
    void printVertices();
};