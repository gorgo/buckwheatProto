#include "DebugDraw.h"
using namespace gameplay;

const float SOLID_ALPHA = 0.2f;
const float LINE_ALPHA = 0.9f;
const int CIRCLE_RESOLUTION = 16;
const float TRANSFORM_SIZE = 1.0f;

void DebugDraw::pushVertex(float x, float y, float r, float g, float b, float a) {
    _vertices.push_back(x);
    _vertices.push_back(0.0f);
    _vertices.push_back(y);
    _vertices.push_back(r);
    _vertices.push_back(g);
    _vertices.push_back(b);
    _vertices.push_back(a);

}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color){
//    Logger::log(Logger::LEVEL_INFO, "draw polygon!\n");

    short start = (short)(_vertices.size() / 7);

    Part part;
    part.indices.clear();
    part.type = Mesh::LINES;

    for (int32 i = 0; i < vertexCount; ++i) {
        pushVertex(vertices[i].x, vertices[i].y, color.r, color.g, color.b, LINE_ALPHA);
        part.indices.push_back(start + (short)i);
        part.indices.push_back(start + (short)(i == (vertexCount - 1) ? 0 : i + 1)); // connect to next or to first
    }

    _parts.push_back(part);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
//    Logger::log(Logger::LEVEL_INFO, "draw solid polygon!\n");
    short start = (short)(_vertices.size() / 7);

    Part partLine, partSolid;
    partLine.type = Mesh::LINES;
    partSolid.type = Mesh::TRIANGLES;

    for (int32 i = 0; i < vertexCount; ++i) {
        pushVertex(vertices[i].x, vertices[i].y, color.r, color.g, color.b, SOLID_ALPHA);
        if (i > 1) {
            partSolid.indices.push_back(start+(short)(i - 1));
            partSolid.indices.push_back(start+(short)i);
            partSolid.indices.push_back(start);
        }
    }

    start += (short) vertexCount;

    for (int32 i = 0; i < vertexCount; ++i) {
        pushVertex(vertices[i].x, vertices[i].y, color.r, color.g, color.b, LINE_ALPHA);
        partLine.indices.push_back(start + (short)i);
        partLine.indices.push_back(start + (short)(i == (vertexCount - 1) ? 0 : i + 1)); // connect to next or to first
    }

    _parts.push_back(partSolid);
    _parts.push_back(partLine);
}

void DebugDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) {
    Logger::log(Logger::LEVEL_INFO, "draw circle!\n");
    short start = (short)(_vertices.size() / 7);

    const int vertexCount = 8;
    float da = 2.0f * MATH_PI / vertexCount;

    Part part;
    part.type = Mesh::LINES;

    for (int i = 0; i < vertexCount; ++i) {
        pushVertex(center.x + cosf(da*i)*radius, center.x + sinf(da*i)*radius, color.r, color.g, color.b, LINE_ALPHA);
        part.indices.push_back(start + (short)i);
        part.indices.push_back(start + (short)(i == (vertexCount - 1) ? 0 : i + 1)); // connect to next or to first
    }

    _parts.push_back(part);
}

void DebugDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) {
    Logger::log(Logger::LEVEL_INFO, "draw solid circle!\n");
    short start = (short)(_vertices.size() / 7);

    const int vertexCount = CIRCLE_RESOLUTION;
    float da = 2.0f * MATH_PI / vertexCount;

    Part partLine, partSolid;
    partLine.type = Mesh::LINES;
    partSolid.type = Mesh::TRIANGLES;

    for (int i = 0; i < vertexCount; ++i) {
        pushVertex(center.x + cosf(da*i)*radius, center.y + sinf(da*i)*radius, color.r, color.g, color.b, SOLID_ALPHA);

        if (i > 1) {
            partSolid.indices.push_back(start+(short)(i - 1));
            partSolid.indices.push_back(start+(short)i);
            partSolid.indices.push_back(start);
        }
    }
    start += vertexCount;

    for (int i = 0; i < vertexCount; ++i) {
        pushVertex(center.x + cosf(da*i)*radius, center.y + sinf(da*i)*radius, color.r, color.g, color.b, LINE_ALPHA);

        partLine.indices.push_back(start + (short)i);
        partLine.indices.push_back(start + (short)(i == (vertexCount - 1) ? 0 : i + 1)); // connect to next or to first
    }
    //axis
    start += vertexCount;
    pushVertex(center.x, center.y, color.r, color.g, color.b, LINE_ALPHA);
    pushVertex(center.x + axis.x*radius, center.y + axis.y * radius, color.r, color.g, color.b, LINE_ALPHA);

    partLine.indices.push_back(start);
    partLine.indices.push_back(start + (short)1);

    start += vertexCount;
    _parts.push_back(partSolid);
    _parts.push_back(partLine);
}

void DebugDraw::DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) {
    Logger::log(Logger::LEVEL_INFO, "draw paricles!\n");
}

void DebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
    Logger::log(Logger::LEVEL_INFO, "draw segment!\n");
}

void DebugDraw::DrawTransform(const b2Transform &xf) {
    Logger::log(Logger::LEVEL_INFO, "draw transform!\n");
    Part partLine;
    partLine.type = Mesh::LINES;

    //axis
    const float len = TRANSFORM_SIZE;
    short start = (short)(_vertices.size() / 7);
    pushVertex(xf.p.x, xf.p.y, 0.8f, 0.2f, 0.2f, LINE_ALPHA);
    pushVertex(xf.p.x + xf.q.GetXAxis().x * len, xf.p.y + xf.q.GetXAxis().y * len, 0.2f, 0.2f, 0.8f, LINE_ALPHA);
    pushVertex(xf.p.x + xf.q.GetYAxis().x * len, xf.p.y + xf.q.GetYAxis().y * len, 0.2f, 0.8f, 0.2f, LINE_ALPHA);

    partLine.indices.push_back(start);
    partLine.indices.push_back(start + (short)1);
    partLine.indices.push_back(start);
    partLine.indices.push_back(start + (short)2);

    _parts.push_back(partLine);
}

//    float a = 5.0f * 0.5f;
//    float c[3] = {0.0f, 1.0f, 0.0f};
//    float al = 0.8f;
//
//    float vertices[] =
//            {
//                    -a, 0.0f,  -a,    c[0], c[1], c[2], al,
//                    -a, 0.0f,  a,    c[0], c[1], c[2], al,
//                    a,  0.0f,  a,    c[0], c[1], c[2], al,
//                    a,  0.0f,  -a,    c[0], c[1], c[2], al
//            };
//    short indices[] =
//            {
//                    0, 1, 2, 0, 2, 3
//            };
//    unsigned int vertexCount = 4;
//    unsigned int indexCount = 6;
//    VertexFormat::Element elements[] =
//            {
//                    VertexFormat::Element(VertexFormat::POSITION, 3),
//                    VertexFormat::Element(VertexFormat::COLOR, 4)
//            };
//    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), vertexCount, true);
//    mesh->setVertexData(vertices, 0, vertexCount);
//    MeshPart* meshPart = mesh->addPart(Mesh::TRIANGLES, Mesh::INDEX16, indexCount, true);
//    meshPart->setIndexData(indices, 0, indexCount);
//    //------------
//
//    Node *dDrawNode = _scene->addNode("debugDraw");
//    Mesh *dDrawMesh = mesh;
//    Model *dDrawModel = Model::create(dDrawMesh);
//    dDrawNode->setModel(dDrawModel);
//
//    // Create the material for the cube model and assign it to the first mesh part.
//    Material* material = dDrawModel->setMaterial("res/shaders/draw.vert", "res/shaders/draw.frag");
//
//    // These parameters are normally set in a .material file but this example sets them programmatically.
//    // Bind the uniform "u_worldViewProjectionMatrix" to use the WORLD_VIEW_PROJECTION_MATRIX from the scene's active camera and the node that the model belongs to.
//    material->setParameterAutoBinding("u_projectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
//    material->setParameterAutoBinding("u_inverseTransposeWorldViewMatrix", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX");
//    // Set the ambient color of the material.
//
//    //dDrawNode->rotateY(MATH_PIOVER4);
//    material->getStateBlock()->setCullFace(false);
//    material->getStateBlock()->setDepthTest(true);
//    material->getStateBlock()->setDepthWrite(true);
//    material->getStateBlock()->setBlend(true);
//    material->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
//    material->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
//    //dDrawNode->rotate(0.1f, 0.2f, 0.3f, 0.4f);


void DebugDraw::draw(b2World *world, Node *node) {
    _vertices.clear();
    _parts.clear();
    SetFlags(e_shapeBit | e_centerOfMassBit | e_jointBit);
    world->DrawDebugData();

    VertexFormat::Element elements[] =
            {
                    VertexFormat::Element(VertexFormat::POSITION, 3),
                    VertexFormat::Element(VertexFormat::COLOR, 4)
            };
    float v[_vertices.size()];
    for (int i = 0; i < _vertices.size(); ++i) v[i] = _vertices[i];
    Mesh* mesh = Mesh::createMesh(VertexFormat(elements, 2), (int)_vertices.size(), false);
    mesh->setVertexData(_vertices.data(), 0, (int)_vertices.size());

//    for (float i : _vertices) Logger::log(Logger::LEVEL_INFO, "%f ", i);
//    Logger::log(Logger::LEVEL_INFO, "\n");
    for (auto part : _parts) {
        MeshPart* meshPart = mesh->addPart(part.type, Mesh::INDEX16, (int)part.indices.size(), false);
        meshPart->setIndexData(part.indices.data(), 0, (int)part.indices.size());

//        Logger::log(Logger::LEVEL_INFO, "part ");
//        for (short i : part.indices)     Logger::log(Logger::LEVEL_INFO, "%d ", i);
//        Logger::log(Logger::LEVEL_INFO, "\n");

    }

    Model *model = Model::create(mesh);
    node->setModel(model);
    model->release();

    if (!_material) {
        _material = Material::create("res/shaders/draw.vert", "res/shaders/draw.frag");
        _material->setParameterAutoBinding("u_projectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");

        _material->getStateBlock()->setCullFace(false);
        _material->getStateBlock()->setDepthTest(false);
        _material->getStateBlock()->setDepthWrite(false);
        _material->getStateBlock()->setBlend(true);
        _material->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
        _material->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

    }
    model->setMaterial(_material);
}
