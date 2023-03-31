#include "GameObjectManager.hpp"

// Constructor is empty
GameObjectManager::GameObjectManager(){

}

GameObjectManager& GameObjectManager::Instance(){
    static GameObjectManager* instance = new GameObjectManager();
    return *instance;
}

// Retrieve a reference to an object
GameObject& GameObjectManager::GetGameObject(unsigned int index){
    return *m_objects[index];
}

void GameObjectManager::AddGameObject(GameObject*& o){
    m_objects.push_back(o);
}

void GameObjectManager::RemoveAll(){
    for(int i=0; i < m_objects.size(); i++){
        delete m_objects[i];
    }
}


void GameObjectManager::UpdateAll(unsigned int screenWidth, unsigned int screenHeight){
    for(int i=0; i < m_objects.size(); i++){
        m_objects[i]->Update(screenWidth,screenHeight);
    }
}

void GameObjectManager::RenderAll(){
    for(int i=0; i < m_objects.size(); i++){
        m_objects[i]->Render();
    }
}
