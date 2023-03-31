#include "GameObject.hpp"
#include "Error.hpp"
#include "Camera.hpp"


GameObject::GameObject(){
}

GameObject::~GameObject(){
    
}

// Generate a game object.
void GameObject::MakeTexturedGameObject(std::string fileName, std::string normalMapFileName, std::vector<float>& vertices, std::vector<unsigned int>& indices, int id){
        m_id = id;
        piece_status = false;
        // Setup geometry
        // Be careful not to forget comma's after each line
        // (except the last line of course)!
        m_vertices = vertices;

        // Add data to the indices as you see fit
        m_indices = indices;

        m_geometry.Gen(m_vertices, m_indices);

        // Create a buffer and set the stride of information
        // NOTE: How we are leveraging our data structure in order to very cleanly
        //       get information into and out of our data structure.
        m_vertexBufferLayout.CreateNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                        m_geometry.GetIndicesSize(),
                                        m_geometry.GetBufferDataPtr(),
                                        m_geometry.GetIndicesDataPtr());


        // Load our actual texture
        // We are using the input parameter as our texture to load
        m_textureDiffuse.LoadTexture(fileName.c_str());

        // Load the normal map texture
        m_normalMap.LoadTexture(normalMapFileName.c_str());
        
        // Setup shaders
        std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");
        // Actually create our shader
        m_shader.CreateShader(vertexShader,fragmentShader);
       
}
// Set location
void GameObject::setLocation(float x, float y, float z){
        location_x = x;
        location_y = y;
        location_z = z;
}

// Location coordinate getters.
float GameObject::getLocX(){
        return location_x;
}

float GameObject::getLocY(){
        return location_y;
}
float GameObject::getLocZ(){
        return location_z;
}

// get the board ID
int GameObject::getBoardId() {
        return board_id;
}
// Check if it has a piece
bool GameObject::hasPiece() {
        return piece_status;
}

// set the piece status of the board piece.
void GameObject::boardSetter(bool status) {
        piece_status = status;
}

// Set a board id for the chess pieces or board pieces.
void GameObject::setBoardId(int id){
        board_id = id;
}

// Represents a unit of a board piece.
void GameObject::MakeTexturedQuad(std::string fileName, int id){
        m_id = id;
        piece_status = false;

        // Setup geometry
        // We are using a new abstraction which allows us
        // to create triangles shapes on the fly
        // Position and Texture coordinate 
        m_geometry.AddVertex(-0.375f, 0.0f, -0.375f, 0.0f, 0.0f);
        m_geometry.AddVertex( 0.375f, 0.0f, -0.375f, 1.0f, 0.0f);
    	m_geometry.AddVertex( 0.375f, 0.0f, 0.375f,  1.0f, 1.0f);
        m_geometry.AddVertex(-0.375f, 0.0f, 0.375f,  0.0f, 1.0f);

        m_geometry.AddVertex(-0.375f, -0.1f, -0.375f, 0.0f, 0.0f);
        m_geometry.AddVertex( 0.375f, -0.1f, -0.375f, 1.0f, 0.0f);
    	m_geometry.AddVertex( 0.375f, -0.1f, 0.375f,  1.0f, 1.0f);
        m_geometry.AddVertex(-0.375f, -0.1f, 0.375f,  0.0f, 1.0f);
            
        // Make our triangles and populate our
        // indices data structure	
        m_geometry.MakeTriangle(0,1,2);
        m_geometry.MakeTriangle(2,3,0);

        m_geometry.MakeTriangle(4,5,6);
        m_geometry.MakeTriangle(5,6,7);

        m_geometry.MakeTriangle(4,5,1);
        m_geometry.MakeTriangle(5,1,0);

        m_geometry.MakeTriangle(5,6,2);
        m_geometry.MakeTriangle(6,2,1);
        
        m_geometry.MakeTriangle(6,7,3);
        m_geometry.MakeTriangle(7,3,2);

        m_geometry.MakeTriangle(7,4,0);
        m_geometry.MakeTriangle(4,0,3);
        

        // This is a helper function to generate all of the geometry
        m_geometry.Gen();

        // Create a buffer and set the stride of information
        // NOTE: How we are leveraging our data structure in order to very cleanly
        //       get information into and out of our data structure.
        m_vertexBufferLayout.CreateNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                        m_geometry.GetIndicesSize(),
                                        m_geometry.GetBufferDataPtr(),
                                        m_geometry.GetIndicesDataPtr());

        // Load our actual texture
        // We are using the input parameter as our texture to load
        m_textureDiffuse.LoadTexture(fileName.c_str());

        // Load the normal map texture
        m_normalMap.LoadTexture("./../common/objects/white_texture/whiteNormal.ppm");

        
        // Setup shaders
        std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");
        // Actually create our shader
        m_shader.CreateShader(vertexShader,fragmentShader);
}

// TODO: In the future it may be good to 
// think about loading a 'default' texture
// if the user forgets to do this action!
void GameObject::LoadTexture(std::string fileName){
        // Load our actual textures
        m_textureDiffuse.LoadTexture(fileName);
}

// Bind everything we need in our object
// Generally this is called in update() and render()
// before we do any actual work with our object
void GameObject::Bind(){
        // Make sure we are updating the correct 'buffers'
        m_vertexBufferLayout.Bind();
        // Diffuse map is 0 by default, but it is good to set it explicitly
        m_textureDiffuse.Bind(0);
        // We need to set the texture slot explicitly for the normal map  
        m_normalMap.Bind(1);
        // Select our appropriate shader
        m_shader.Bind();
}

void GameObject::Update(unsigned int screenWidth, unsigned int screenHeight){
        // Call our helper function to just bind everything
        Bind();
        // TODO: Read and understand
        // For our object, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        m_shader.SetUniform1i("u_DiffuseMap",0);
        // If we want to load another texture, we assign it to another slot
        m_shader.SetUniform1i("u_NormalMap",1);  
         // Here we apply the 'view' matrix which creates perspective.
        // The first argument is 'field of view'
        // Then perspective
        // Then the near and far clipping plane.
        // Note I cannot see anything closer than 0.1f units from the screen.
        // TODO: In the future this type of operation would be abstracted away
        //       in a camera class.
        m_projectionMatrix = glm::perspective(45.0f,((float)screenWidth)/((float)screenHeight),0.1f,512.0f);

        // Set the uniforms in our current shader
        m_shader.SetUniformMatrix4fv("modelTransformMatrix",m_transform.GetTransformMatrix());
        m_shader.SetUniformMatrix4fv("view", &Camera::Instance().GetWorldToViewmatrix()[0][0]);
        m_shader.SetUniformMatrix4fv("projectionMatrix", &m_projectionMatrix[0][0]);

        // Create a first 'light'
        // Set in a light source position
        m_shader.SetUniform3f("lightPos",0.0f,0.0f,0.0f);	
        // Set a view and a vector
        m_shader.SetUniform3f("viewPos",0.0f,0.0f,50.0f);
        m_shader.SetUniform3f("colors", ((float) m_id)/255, m_selection_color, m_selection_color);

}
void GameObject::select() {
        m_selection_color = 50.0f;
}

void GameObject::unselect() {
        m_selection_color = 0.0f;
}

// Render our geometry
void GameObject::Render(){
    // Call our helper function to just bind everything
    Bind();
	//Render data
    glDrawElements(GL_TRIANGLES,
                   m_geometry.GetIndicesSize(), // The number of indicies, not triangles.
                   GL_UNSIGNED_INT,             // Make sure the data type matches
                        nullptr);               // Offset pointer to the data. 
                                                // nullptr because we are currently bound
}

// Returns the actual transform stored in our object
// which can then be modified
Transform& GameObject::GetTransform(){
    return m_transform; 
}
