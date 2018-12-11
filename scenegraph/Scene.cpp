#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"

#include "glm/gtx/transform.hpp"
#include "iostream"
#include "CS123SceneData.h"
#include <memory>

#include <QGLWidget>
#include <QImage>



Scene::Scene()
{
}

//copy constructor
Scene::Scene(Scene &scene) :

    m_info(0)

{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    setGlobal(global);


    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.
    m_info = scene.m_info;
    m_lights = scene.m_lights;
    m_global = scene.m_global;

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {

    //set up global data
    std::unique_ptr<CS123SceneGlobalData> globalData = std::make_unique<CS123SceneGlobalData>();
    parser->getGlobalData(*globalData);
    sceneToFill->setGlobal(*globalData);

    //iterate through lights and set them up
    for(int i=0; i < parser->getNumLights(); i++){
        std::unique_ptr<CS123SceneLightData> lightData = std::make_unique<CS123SceneLightData>();
        parser->getLightData(i, *lightData);
        sceneToFill->addLight(*lightData);
    }

    //get root of scene tree
    CS123SceneNode* root = parser->getRootNode();
    //set parent matrix to be identity matrix so that it is not null but does not affect transformations
    glm::mat4 parentMatrix = glm::mat4();

    //call this method, which will traverse tree, recursively passing in parent's matrix to use in calculating cumulative
    //transformation matrix
    sceneToFill->preorder(root, parentMatrix);
}

//traverse the tree in preorder, so that each child is visited after its parent and can use its parent's translation matrix
void Scene::preorder(CS123SceneNode* node, glm::mat4 parentMatrix){
    if (node == NULL)
        return;

    //here i iterate through the vector of pointers, making copies of the actual objects
    std::vector<CS123ScenePrimitive*> primitivePointers = node->primitives;
    std::vector<CS123ScenePrimitive> primitives;
    std::vector<QImage> textures = std::vector<QImage>();
    for(CS123ScenePrimitive* prim : primitivePointers){
        CS123ScenePrimitive myPrim = *prim;
        //primitives.push_back(*prim);
        primitives.push_back(myPrim);
        if(myPrim.material.textureMap.isUsed){
            QImage image = QImage(myPrim.material.textureMap.filename.data());
            textures.push_back(image);
        }
        else{
            //push back null image
            textures.push_back(QImage());
        }
    }

    std::vector<CS123SceneTransformation*> transformation = node->transformations;
    //start each transformation matrix as an identity matrix to avoid any of these ever being null
    glm::mat4 cumulative = glm::mat4();

    //iterate through all of the node's transformations. switch case determines what type of transformation, then uses appropriate method to
    //convert vec3 to mat4, and update variable which is later used to calculate composite matrix
    for(int i=0; i < transformation.size(); i++){
        switch(transformation[i]->type) {
        case TRANSFORMATION_TRANSLATE:
            cumulative *= glm::translate(transformation[i]->translate);
            break;
        case TRANSFORMATION_SCALE:
            cumulative *= glm::scale(transformation[i]->scale);
            break;
        case TRANSFORMATION_ROTATE:
            cumulative *= glm::rotate(transformation[i]->angle, transformation[i]->rotate);
            break;
        case TRANSFORMATION_MATRIX:
            cumulative *= transformation[i]->matrix;
            break;

        }
    }

    //calculating composite matrix
    cumulative = parentMatrix*cumulative;



    //make a tuple of the primitive(s) and composition transformation matrix and add it to vector of tuples
//    std::tuple<std::vector<CS123ScenePrimitive>, glm::mat4> tuple = std::make_tuple(primitives, cumulative);
    std::tuple<std::vector<CS123ScenePrimitive>, glm::mat4, std::vector<QImage>> tuple = std::make_tuple(primitives, cumulative, textures);
    m_info.push_back(tuple);

    //recursively call preorder on children, after dealing with current node
    std::vector<CS123SceneNode*> children = node->children;
    if(node->children.empty()){
        return;
    }
    //if node has children
    else{
        for(int i = 0; i < children.size(); i++){
            //pass in composite matrix, to be used by child
            preorder(children[i], cumulative);
        }
    }
}

//i deal with this directly in preorder()
void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_lights.push_back(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_global = global;
}

