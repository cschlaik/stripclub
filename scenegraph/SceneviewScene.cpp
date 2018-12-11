#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"

#include "gl/datatype/shape.h"
#include "gl/datatype/cube.h"
#include "gl/datatype/cone.h"
#include "gl/datatype/cylinder.h"
#include "gl/datatype/sphere.h"
#include "gl/datatype/torus.h"
#include "gl/datatype/special1.h"
#include "CS123SceneData.h"

//#include "CS123XmlSceneParser.h"

using namespace CS123::GL;


SceneviewScene::SceneviewScene() :

    m_shape(nullptr)
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::render(SupportCanvas3D *context) {
    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setSceneUniforms(context);
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

}

void SceneviewScene::setSceneUniforms(SupportCanvas3D *context) {
    Camera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void SceneviewScene::setLights()
{
 // Set up the lighting for scene using m_phongShader

    //for each light in list of lights that i made
    for(int i = 0; i < m_lights.size(); i++){
        m_phongShader->setLight(m_lights[i]);
    }
}

void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //iterate through vector of tuples
    for(int i = 0; i < m_info.size(); i++){

        //first element in tuple is vector of primitives from each node
        std::vector<CS123ScenePrimitive> primitiveList = std::get<0>(m_info[i]);

        //iterate over vector of primitives at each node
        for(int j = 0; j < primitiveList.size(); j++){
            CS123ScenePrimitive primitive = primitiveList[j];

            //switch case determining primitive type and then drawing a primitive of that type using shapes code
            switch(primitive.type){

            case PrimitiveType::PRIMITIVE_CONE:
                m_shape = std::make_unique<Cone>();
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                m_shape = std::make_unique<Cylinder>();
                break;
            case PrimitiveType::PRIMITIVE_TORUS:
                m_shape = std::make_unique<Torus>();
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                m_shape = std::make_unique<Sphere>();
                break;
            case PrimitiveType::PRIMITIVE_MESH:
                m_shape = std::make_unique<Special1>();
                break;
            case PrimitiveType::PRIMITIVE_CUBE:
                m_shape = std::make_unique<Cube>();
                break;
            }

            //multiply ambient and diffuse object colors
            primitive.material.cDiffuse *= m_global.kd;
            primitive.material.cAmbient *= m_global.ka;

            //this sets the uniform
            m_phongShader->applyMaterial(primitive.material);

            //setUniform
            m_phongShader->setUniform("m", std::get<glm::mat4>(m_info[i]));
            m_shape->draw();

        }

    }

}

void SceneviewScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.
}

