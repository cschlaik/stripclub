#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "gl/datatype/implicitShape.h"
#include "Canvas2D.h"
#include "Settings.h"
#include <vector>


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();
    void render(Camera *camera, int width, int height, Canvas2D* canvas);
    void renderTestSphere(Camera *camera, int width, int height, Canvas2D* canvas);

private:
    glm::vec4 generateRay(int x, int y, int width, int height, Camera* camera);
    glm::vec4 pixToFilm(int x, int y, int width, int height);
    glm::vec4 filmToWorld(glm::vec4 Pfilm, Camera* camera);
    void testIntersect(glm::vec4 ray);
    void setUpScene();
//    void renderTestSphere(Camera *camera, int width, int height, Canvas2D* canvas);

    std::unique_ptr<implicitShape> m_implicitShape;
    std::unique_ptr<implicitShape> m_implicitShape1;
    glm::vec4 findD(glm::vec4 Pworld, glm::vec4 Peye);
   // glm::vec4 findPeye(Camera* camera);
    glm::vec4 worldToObject(glm::vec4 worldVec, glm::mat4 modelMatrix);
//    std::vector<std::tuple<std::vector<CS123ScenePrimitive>, glm::mat4>> m_rayInfo;
    std::vector<std::tuple<std::vector<CS123ScenePrimitive>, glm::mat4, std::vector<QImage>>> m_rayInfo;
    bool approxEqual(float a, float b);
    bool inShadow;



};

#endif // RAYSCENE_H
