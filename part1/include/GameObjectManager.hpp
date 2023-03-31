/** @file GameObjectManager.hpp
 *  @brief Class to manage creation of objects 
 *  
 *  
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef OBJECTMANAGER_HPP
#define OBJECTMANAGER_HPP


#include "GameObject.hpp"

// Purpose:
// This class sets up a full graphics program using SDL
//
//
//
class GameObjectManager{
public:
    // Singleton pattern for having one single GameObjectManager
    // class at any given time.
    static GameObjectManager& Instance();

    // Desctructor
    ~GameObjectManager();
    // Add a new object
    void AddGameObject(GameObject*& o);
    // Retrieve a reference to an object
    GameObject& GetGameObject(unsigned int index);
    // Deletes all of the objects
    void RemoveAll();
    // Update all objects
    void UpdateAll(unsigned int screenWidth, unsigned int screenHeight);
    // Render All GameObjects
    void RenderAll();

private:
	// Constructor is private because we should
    // not be able to construct any other managers,
    // this how we ensure only one is ever created
    GameObjectManager();
    // GameObjects in our scene 
    std::vector<GameObject*> m_objects;
};

#endif
