/** @file GameObject.hpp
 *  @brief Sets up an OpenGL camera..
 *  
 *  More...
 *
 *  @author Mike and Samuel
 *  @bug No known bugs.
 */
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>

#include <vector>
#include <string>

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "Geometry.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Purpose:
// An abstraction to create multiple Game objects
//
//
class GameObject{
public:
    // GameObject Constructor
    GameObject();
    // GameObject destructor
    ~GameObject();
    // Load a texture
    void LoadTexture(std::string fileName);
    // Create a textured GameObject.
    void MakeTexturedGameObject(std::string fileName, std::string normalMapFileName, std::vector<float>& vertices, std::vector<unsigned int>& indices, int id);
    // Create a textured quad
    void MakeTexturedQuad(std::string fileName, int id);
    // Updates and transformations applied to object
    void Update(unsigned int screenWidth, unsigned int screenHeight);
    // How to draw the object
    void Render();

    // Get Locations of objects
    float getLocX();
    float getLocY();
    float getLocZ();

    // Get the specific ID of the board itself for boards or the board ID on which the chess piece is sitting
    int getBoardId();

    // Set location of object
    void setLocation(float x, float y, float z);
    // Returns an objects transform
    Transform& GetTransform();

    // Returns if a board has a piece.
    bool hasPiece();
    // Sets whether a board piece has a chess piece on top
    void boardSetter(bool status);

    // Set Board ID of chess or board pieces.
    void setBoardId(int id);

    // Makes RGB changes to create the effect of a selection
    void select();
    // Makes RGB changes to revert the selection effects.
    void unselect();
protected:
	// Helper method for when we are ready to draw or update our object
	void Bind();

    // GameObject vertices
    std::vector<GLfloat> m_vertices;
    // GameObject indicies
    std::vector<GLuint> m_indices;

    // Game Object ID
    int m_id;

    // Location attributes
    float location_x;
    float location_y;
    float location_z;

    // Variable for whether there is a chess piece on the specific board piece.
    bool piece_status;

    // The board id for chess or board pieces
    int board_id;
    // variable to modify colors to create a selection effect
    float m_selection_color = 0.0f;

    // For now we have one shader per object.
    Shader m_shader;
    // For now we have one buffer per object.
    VertexBufferLayout m_vertexBufferLayout;
    // For now we have one texture per object
    Texture m_textureDiffuse;
    // NOTE: that we have a normal map per object as well!
    Texture m_normalMap;
    // Store the objects transformations
    Transform m_transform; 
    // Store the 'camera' projection
    glm::mat4 m_projectionMatrix;
    // Store the objects Geometry
	Geometry m_geometry;
};


#endif
