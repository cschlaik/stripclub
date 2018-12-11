#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"

#include <iostream>

#include "CamtransCamera.h"
#include "gl/datatype/implicitCone.h"
#include "gl/datatype/implicitCube.h"
#include "gl/datatype/implicitCylinder.h"
#include "gl/datatype/implicitMesh.h"
#include "gl/datatype/implicitShape.h"
#include "gl/datatype/implicitSpecial1.h"
#include "gl/datatype/implicitSphere.h"
#include "gl/datatype/implicitTorus.h"

#include <QGLWidget>
#include <QImage>
#include <math.h>


RayScene::RayScene(Scene &scene) :
    Scene(scene),
    m_implicitShape(nullptr),
    m_implicitShape1(nullptr),
    inShadow(false)

{
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.
    m_rayInfo = scene.m_info;

}

RayScene::~RayScene()
{

}

void RayScene::render(Camera *camera, int width, int height, Canvas2D* canvas){
    //loop over every pixel on canvas
    BGRA *pix = canvas->data();
    float pi = 3.14159265358979323846;

    //transform camera position from camera to world space
    glm::vec4 Peye = filmToWorld(glm::vec4(0.f,0.f,0.f,1.f), camera);

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            //store the current pixel (find pixel index from its row and col and considering canvas width)
            BGRA *pixel = &pix[y*canvas->width() + x];

            float smallestT = -1;
            glm::vec4 objNormal;
            glm::vec4 objectIntersectionPoint;
            glm::vec4 intersectionPoint;
            glm::mat4 transformation;
            CS123ScenePrimitive shape;
            QImage texture;
            int index;
            for(int a=0; a < m_rayInfo.size(); a++){
                //first element in tuple is vector of primitives from each node
                std::vector<CS123ScenePrimitive> primitiveList = std::get<0>(m_rayInfo[a]);
                glm::mat4 transformationMatrix = std::get<1>(m_rayInfo[a]);
                std::vector<QImage> textures = std::get<2>(m_rayInfo[a]);

                //iterate over vector of primitives at each node
                for(int j = 0; j < primitiveList.size(); j++){
                    CS123ScenePrimitive primitive = primitiveList[j];

                    //switch case determining primitive type and then drawing a primitive of that type using shapes code
                    switch(primitive.type){

                    case PrimitiveType::PRIMITIVE_CONE:
                        m_implicitShape = std::make_unique<implicitCone>();
                        break;
                    case PrimitiveType::PRIMITIVE_CYLINDER:
                        m_implicitShape = std::make_unique<implicitCylinder>();
                        break;
                    case PrimitiveType::PRIMITIVE_TORUS:
                        m_implicitShape = std::make_unique<implicitTorus>();
                        break;
                    case PrimitiveType::PRIMITIVE_SPHERE:
                        m_implicitShape = std::make_unique<implicitSphere>();
                        break;
                    case PrimitiveType::PRIMITIVE_MESH:
                        m_implicitShape = std::make_unique<implicitSpecial1>();
                        break;
                    case PrimitiveType::PRIMITIVE_CUBE:
                        m_implicitShape = std::make_unique<implicitCube>();
                        break;
                    }

                    //finds film plane point of current pixel, then finds world point from film plane point
                    glm::vec4 Pworld = filmToWorld(pixToFilm(x, y, width, height), camera);
                    //find direction vector, in world space
                    glm::vec4 d = findD(Pworld, Peye);

                    //compare all the shapes a ray intersects to find which one is closest
                    if(m_implicitShape->intersect(worldToObject(Peye, transformationMatrix), worldToObject(d, transformationMatrix))==true){
                        float T = m_implicitShape->getT();
                        //if this is the first shape being looked at
                        if(smallestT < 0){
                            smallestT = T;
                        }
                        if(T <= smallestT){
                            smallestT = T;
                            objNormal = m_implicitShape->getNormal();
                            //returns object space intersection point, so multiply by transformation matrix to get in wold space
                            objectIntersectionPoint = m_implicitShape->getIntersection();
                            intersectionPoint = transformationMatrix*objectIntersectionPoint;

                            shape = primitive;
                            transformation = transformationMatrix;
                            texture = textures[j];
                            index = j;
                        }
                    }
                }
            }
            //after iterating over primitives
            //if ray intersects nothinb, set to black
            if(smallestT < 0){
                pixel->b = 0;
                pixel->g = 0;
                pixel->r = 0;
            }
            //if intersects
            else{
                int imageWidth;
                int imageHeight;
                glm::vec2 st;

                //if shape has a texture
                if(!texture.isNull()){

                    imageWidth = (int)texture.width();
                    imageHeight = (int)texture.height();

                    float x = objectIntersectionPoint.x;
                    float y = objectIntersectionPoint.y;
                    float z = objectIntersectionPoint.z;

                    glm::vec2 uv;
                    //figureing out uv coords for each shape
                    //this could be made more efficient as it is repetitive
                    //for cube
                    if(shape.type == PrimitiveType::PRIMITIVE_CUBE){
                        if(approxEqual(x, 0.5)){
                            uv = glm::vec2(-z + 0.5,  -y + 0.5);
                        }
                        if(approxEqual(x, -0.5)){
                            uv = glm::vec2(z + 0.5,  -y + 0.5);
                        }
                        if(approxEqual(y, 0.5)){
                            uv = glm::vec2(x + 0.5,  z + 0.5);
                        }
                        if(approxEqual(y, -0.5)){
                            uv = glm::vec2(-x + 0.5,  z + 0.5);
                        }
                        if(approxEqual(z, 0.5)){
                            uv = glm::vec2(x + 0.5,  -y + 0.5);
                        }
                        if(approxEqual(z, -0.5)){
                            uv = glm::vec2(-x + 0.5,  -y + 0.5);
                        }
                    }

                    //cylinder
                    if(shape.type == PrimitiveType::PRIMITIVE_CYLINDER){
                        //caps
                        if(approxEqual(y, 0.5)){
                            uv = glm::vec2(x + 0.5,  z + 0.5);
                        }
                        else if(approxEqual(y, -0.5)){
                            uv = glm::vec2(-x + 0.5,  z + 0.5);
                        }
                        //cylinder body
                        else{
                            float theta = atan2(z, x);
                            float u;
                            if(theta < 0){
                                u = (-theta)/(2*pi);
                            }
                            else{
                                u = 1 - (theta/(2*pi));
                            }
                            uv = glm::vec2(u, -y + 0.5);
                        }
                    }

                    //cone
                    if(shape.type == PrimitiveType::PRIMITIVE_CONE){
                        //cap
                        if(approxEqual(y, -0.5)){
                            uv = glm::vec2(-x + 0.5,  z + 0.5);
                        }
                        //cone body
                        else{
                            float theta = atan2(z, x);
                            float u;
                            if(theta < 0){
                                u = (-theta)/(2*pi);
                            }
                            else{
                                u = 1 - (theta/(2*pi));
                            }
                            uv = glm::vec2(u, -y + 0.5);
                        }
                    }

                    //sphere
                    if(shape.type == PrimitiveType::PRIMITIVE_SPHERE){
                        float theta = atan2(z, x);
                        float u;
                        if(theta < 0){
                            u = (-theta)/(2*pi);
                        }
                        else{
                            u = 1 - (theta/(2*pi));
                        }

                        float v;
                        if(approxEqual(y, 0.5) || approxEqual(y, -0.5)){
                            v = 0.5;
                        }
                        else{
                            float phi = asin(-y/0.5);
                            v = (phi/pi) + 0.5;
                        }
                        uv = glm::vec2(u, v);

                    }

                    float u = uv[0];
                    float v = uv[1];
                    int repeatU = (int)shape.material.textureMap.repeatU;
                    int repeatV = (int)shape.material.textureMap.repeatV;
                    st = glm::vec2(fmod((u*repeatU*imageWidth), imageWidth), fmod((v*repeatV*imageHeight), imageHeight));

                }

                //if ray intersects, get object normal at point of intersection, transform to world space(done above)
                glm::vec3 objNormal3 = glm::vec3(objNormal[0], objNormal[1], objNormal[2]);

                glm::mat3 M3x3 = glm::mat3(transformation);
                glm::mat3 M3x3invTrans = glm::transpose(glm::inverse(M3x3));

                glm::vec3 worldNormal3 = M3x3invTrans*objNormal3;
                glm::vec4 worldNormal = glm::vec4(worldNormal3, 0.0f);
                worldNormal = glm::normalize(worldNormal);

                //use normal to find pixel color
                //following determination of pixel color using lighting equation, update pixel color
                glm::vec4 Lm;

                //intersectionPoint set as point
                intersectionPoint[3] = 1.0f;

                glm::vec4 cumulative;
                //ambient is constant for scene
                glm::vec4 ambient = (255/3)*m_global.ka*shape.material.cAmbient;
                float lightDist;
                //iterate over each light in scene
                for(int i = 0; i < m_lights.size(); i++){
                    //calculate Lm, the unit length incoming light vector from light i, according to type of light
                    if(m_lights[i].type == LightType::LIGHT_DIRECTIONAL){
                        Lm = glm::normalize(-m_lights[i].dir);
                    }
                    else{
                        glm::vec4 lightPos = glm::vec4(m_lights[i].pos[0], m_lights[i].pos[1], m_lights[i].pos[2], 1.0f);
                        Lm = glm::normalize(lightPos - intersectionPoint);
                        //first cast ray from point of intersection to light. Peye will be intersection point and direction will be (?) Lm
                        //record distance from intersection point to light
                        lightDist = sqrt(pow((lightPos.x - intersectionPoint.x), 2) + pow((lightPos.y - intersectionPoint.y), 2));
                    }


                    //iterate over primitives in the scene again (this is repetitive code :( )
                    for(int k=0; k < m_rayInfo.size(); k++){
                        //first element in tuple is vector of primitives from each node
                        std::vector<CS123ScenePrimitive> primitiveList = std::get<0>(m_rayInfo[k]);
                        glm::mat4 transformationMatrix = std::get<1>(m_rayInfo[k]);

                        //then iterate over other primitives in the scene
                        for(int j = 0; j < primitiveList.size(); j++){
                            CS123ScenePrimitive primitive = primitiveList[j];
//                            if(j == index){
//                                continue;
//                            }

                            //switch case determining primitive type and then drawing a primitive of that type using shapes code
                            switch(primitive.type){

                            case PrimitiveType::PRIMITIVE_CONE:
                                m_implicitShape1 = std::make_unique<implicitCone>();
                                break;
                            case PrimitiveType::PRIMITIVE_CYLINDER:
                                m_implicitShape1 = std::make_unique<implicitCylinder>();
                                break;
                            case PrimitiveType::PRIMITIVE_TORUS:
                                m_implicitShape1 = std::make_unique<implicitTorus>();
                                break;
                            case PrimitiveType::PRIMITIVE_SPHERE:
                                m_implicitShape1 = std::make_unique<implicitSphere>();
                                break;
                            case PrimitiveType::PRIMITIVE_MESH:
                                m_implicitShape1 = std::make_unique<implicitSpecial1>();
                                break;
                            case PrimitiveType::PRIMITIVE_CUBE:
                                m_implicitShape1 = std::make_unique<implicitCube>();
                                break;
                            }

                            inShadow = false;

                            glm::vec4 intersectionEpsilon = intersectionPoint + (glm::normalize(Lm)*0.0001f);
                            if(m_implicitShape1->intersect(worldToObject(intersectionEpsilon, transformationMatrix), worldToObject(Lm, transformationMatrix))==true){
                                //for directional light, if intersects object at any point, intersectionPoint is in shadow
                                if(m_lights[i].type == LightType::LIGHT_DIRECTIONAL){
                                    inShadow = true;
                                }
                                //for point lights, if intersects another object and distance of that intersection is less
                                //than distance to light, intersectionPoint is in shadow
                                else{
                                    glm::vec4 shapeIntersection = transformationMatrix*m_implicitShape1->getIntersection();
                                    //compare their distances to that of light
                                    float shapeDist = sqrt(pow(intersectionPoint.x - shapeIntersection.x, 2) + pow(intersectionPoint.y - shapeIntersection.y, 2));


                                    //if any distances are smaller, only count the ambient light in the lighting eqn
                                    if (shapeDist < lightDist){
                                        //only count ambient
                                        inShadow = true;
                                        //can break as soon as we find any shape that casts shadow
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    //direction of reflected light for current light, calculated from incoming ray and normal
                    glm::vec4 Ri = (-Lm) - 2.f*worldNormal*glm::dot((-Lm), worldNormal);
                    //ksOsgamma(Ri dot V)^n
                    //Peye normalized gives V, normalized line of sight
                    //shininess is specular exponent
                    glm::vec4 specular = (255/3)*m_global.ks*shape.material.cSpecular*pow(glm::max(0.f, glm::dot(Ri, glm::normalize(Peye))), shape.material.shininess);

                    //N dot Li
                    float dot = glm::max(0.0f, glm::dot((worldNormal), Lm));
                    float attenuation = glm::min((1/(m_lights[i].function[0] + m_lights[i].function[1] + pow(m_lights[i].function[2], 2))), 1.0);

                    glm::vec4 diffuse;
                    //if primitive has texture, incorporate into diffuse
                    if(shape.material.textureMap.isUsed && settings.useTextureMapping){
                        QRgb textureColor = texture.pixel(st[0], st[1]);
                        glm::vec4 myVec = glm::vec4(qRed(textureColor), qGreen(textureColor), qBlue(textureColor), 255.0);
                        diffuse = shape.material.blend*myVec + (1-shape.material.blend)*(255*m_global.kd * shape.material.cDiffuse*dot);

                    }
                    else{
                        //regular diffuse
                        diffuse = (255/3)*m_global.kd * shape.material.cDiffuse*dot;
                    }

                    //this is the whole sigma summation part
                    //if directional, ignore attenuation
                    if(m_lights[i].type == LightType::LIGHT_DIRECTIONAL){
                        cumulative += (diffuse + specular);
                    }
                    else{
                        cumulative += attenuation*(diffuse + specular);
                    }
                }
                cumulative[0] = cumulative[0]/m_lights.size();
                cumulative[1] = cumulative[1]/m_lights.size();
                cumulative[2] = cumulative[2]/m_lights.size();
                cumulative[3] = cumulative[3]/m_lights.size();

                //only ambient contributes if in shadow
                //only apply shadows if shadows are checked
                if(inShadow && settings.useShadows){
                    //std::cout << "in shadow" << std::endl;
                    pixel->r = ambient[0];
                    pixel->g = ambient[1];
                    pixel->b = ambient[2];
                    pixel->a = 255;
                }
                else{
                    pixel->r = ambient[0] + cumulative[0];
                    pixel->g = ambient[1] + cumulative[1];
                    pixel->b = ambient[2] + cumulative[2];
                    pixel->a = 255;
                }
            }
        }
    }
}

//convert pixel coordinates (x,y) to Pfilm -- point on the normalized film plane, (px,py,pz)
glm::vec4 RayScene::pixToFilm(int x, int y, int width, int height){
    return glm::vec4((2.0*x)/width - 1.0, 1.0 - (2.0*y)/height, -1.0, 1.0);
}

//transform Pfilm from normalized film plane into untransformed world space point Pworld
//ie camera space to world space
glm::vec4 RayScene::filmToWorld(glm::vec4 Pfilm, Camera *camera){

    glm::mat4 viewMatrix = camera->getViewMatrix();
    glm::mat4 scaleMatrix = camera->getScaleMatrix();

    glm::mat4 invView = glm::inverse(viewMatrix);
    glm::mat4 invScale = glm::inverse(scaleMatrix);
    glm::mat4 filmToWorld = invView*invScale;

    return filmToWorld*Pfilm;

}

glm::vec4 RayScene::findD(glm::vec4 Pworld, glm::vec4 Peye){
    glm::vec4 d = glm::normalize(Pworld - Peye);

    return d;
}

//used to transform Peye and d to object space
glm::vec4 RayScene::worldToObject(glm::vec4 worldVec, glm::mat4 modelMatrix){
    return glm::inverse(modelMatrix)*worldVec;
}

bool RayScene::approxEqual(float a, float b){
    float epsilon = 0.0001;
    if(abs(a-b) <= epsilon){
        return true;
    }
    else{
        return false;
    }
}

