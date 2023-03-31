#include "OBJReader.h"
#include <cmath>

// Initialize OBJReader with file names.
OBJReader::OBJReader(char** argv, int argc) {
    for (int i = 1; i < argc; i++) {
        fileNames.push_back(argv[i]);
    }
    read(1); // Default read the first argument (obj file).
    createVertexIndexBufffer();
}

OBJReader::OBJReader(std::string fileName) {
    read(0, fileName);
    createVertexIndexBufffer();
}

// Function for reading through obj file and getting geometric data.
// Returns true for a successful read or false otherwise.
bool OBJReader::read(int option, std::string fileName)  {
    // option is the user's choice for which argument(file name) to run.
    // if (option > fileNames.size()) {
    //     std::cout << "Outside of possible options!\n";
    //     return false;
    // }
    // Helper values and initialization.
    scale = 0.4;
    std::ifstream inFile;
    if (fileName == "") {
        fileName = fileNames[option - 1];
    }
    int i = 0;
    int j = 0;
    std::string line;
    std::string val;
    std::string faceVal;
    std::stringstream characterBuffer; 
    std::stringstream lineBuffer;
    vertexIndices.clear();
    vertexNormals.clear();
    textures.clear();
    vertices.clear(); 

    inFile.open(fileName);
    char vec_delim = ' '; // General delimiter
    char face_delim = '/'; // Face delimiter to parse through the '/'s.
    
    if (inFile.is_open()){
        // Reading each line in the obj file.
        while(getline(inFile, line)) {
            lineBuffer << line;
            i = 0;
            // Parsing through each line in the obj file.
            while(std::getline(lineBuffer, val, vec_delim)){
                characterBuffer << val;
                if (i > 0) {
                    if (line.at(0) == 'v') {
                        // Parsing vertex normals.
                        if (line.at(1) == 'n') {
                            vertexNormals.push_back(std::stof(val));
                        // Parsing vertices.
                        } else if (line.at(1) == ' ') {
                            vertices.push_back(std::stof(val));
                        } else if (line.at(1) == 't') {
                            textures.push_back(std::stof(val));
                        }
                      // Parsing faces.
                    } else if (line.at(0) == 'f') {
                        j = 0;
                        while (std::getline(characterBuffer, faceVal, face_delim)) {
                            if (faceVal != "") {
                                // Parsing vertex indices.
                                if (j == 0) {
                                    vertexIndices.push_back(std::stoi(faceVal) - 1);
                                // Parsing texture indices.
                                } else if (j == 1) {
                                    textureIndices.push_back(std::stoi(faceVal) - 1);
                                // Parsing vertex normal indices.
                                } else if (j == 2) {
                                    normalIndices.push_back(std::stoi(faceVal) - 1);
                                }
                            }
                            j++;
                        }
                        // Parsing file that points the .mtl file.
                    } else if (line.substr(0,6) == "mtllib") {
                        int index = fileName.find_last_of('/');
                        workingDirectory = fileName.substr(0, index+1);
                        mtlFileName = workingDirectory + line.substr(7, line.length());
                        read(1, mtlFileName);
                        // parsing the .mtl file to read the .ppm file to be used for texturing.   
                    } else if (line.substr(0,6) == "map_Kd") {
                        int index = fileName.find_last_of('/');
                        workingDirectory = fileName.substr(0, index+1);
                        ppmFileName = workingDirectory + line.substr(7, line.length());
                    } else if (line.substr(0,8) == "map_Bump") {
                        int index = fileName.find_last_of('/');
                        workingDirectory = fileName.substr(0, index + 1);
                        normalMapFileName = workingDirectory + line.substr(9, line.length()); 
                    }
                } 
                i++;
                characterBuffer.str("");
                characterBuffer.clear();    
            }
            
            lineBuffer.str("");
            lineBuffer.clear();
        }
    }

    inFile.close();
    return true;
}

// Function to compare whether two floats are equal.
bool floatEquality(float a, float b, float epsilon = 0.0000000001f) {
    return (fabs(a - b) < epsilon);
}

// Function to use vertex and texture indices to create a single 
// 5D vertice vector and a single indexBuffer.
void OBJReader::createVertexIndexBufffer() {
    vertexTextures.clear();
    unsigned int vert_counter = 0;
    unsigned int norm_counter = 0;
    unsigned int texture_counter = 0;
    int index = 0;
    int existence = 0;
    float x, y, z, xn, yn, zn, s, t = 0;
    for (unsigned int i = 0; i < textureIndices.size(); i++) {
        // vertex, normal and texture indices.
        vert_counter = vertexIndices[i] * 3;
        norm_counter = normalIndices[i] * 3;
        texture_counter = textureIndices[i] * 2;
        // acquire vertices, normals and textures at respective indices.
        x = vertices[vert_counter];
        y = vertices[vert_counter + 1]; 
        z = vertices[vert_counter + 2];

        xn = vertexNormals[norm_counter];
        yn = vertexNormals[norm_counter + 1]; 
        zn = vertexNormals[norm_counter + 2];

        s = textures[texture_counter];
        t = textures[texture_counter + 1];
        // check if current vertex layout exists in the new vector.
        existence = exists(x, y, z, xn, yn, zn, s, t);
        if (existence == - 1) {
            // if not present add new x, y,z, xn, yn, zn coordinates and texture points s and t.
            vertexTextures.push_back(x);
            vertexTextures.push_back(y);
            vertexTextures.push_back(z);
            vertexTextures.push_back(xn);
            vertexTextures.push_back(yn);
            vertexTextures.push_back(zn);
            vertexTextures.push_back(s);
            vertexTextures.push_back(t);
            // create an index buffer using the new vertex + normal + texture coordinates.
            indexBuffer.push_back(index);
            index++;
        } else {
            // if points already exist in the new vector for vertices and textures just append
            // the respective position to the index buffer.
            indexBuffer.push_back(existence);
        }
    }
}

// check for existence of the coordinates in the new layout of vertices and textures.
int OBJReader::exists(float x, float y, float z, float xn, float yn, float zn, float s, float t) {
    unsigned int j = 0;
    for (unsigned int i = 0; i < vertexTextures.size(); i = i + 8) {
        if (floatEquality(x, vertexTextures[i]) &&
        floatEquality(y, vertexTextures[i + 1]) &&
        floatEquality(z, vertexTextures[i + 2]) &&
        floatEquality(xn, vertexTextures[i + 3]) &&
        floatEquality(yn, vertexTextures[i + 4]) &&
        floatEquality(zn, vertexTextures[i + 5]) &&
        floatEquality(s, vertexTextures[i + 6]) &&
        floatEquality(t, vertexTextures[i + 7])) {
            return j;
        }
        j++;
    }
    return -1; // if not found.
}

// Function to print the vertices in a formatted manner.
void OBJReader::printVertices() {
    for (unsigned int i = 0; i < vertexTextures.size(); i = i + 5) {
        std::cout << vertexTextures[i] << " " << vertexTextures[i+1] << " " << vertexTextures[i+2] << " " << vertexTextures[i+3] << " " << vertexTextures[i+4] << " " << "\n";
    }
}