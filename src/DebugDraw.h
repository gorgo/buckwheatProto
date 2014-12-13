
#ifndef DebugDraw_h
#define DebugDraw_h

#include "Box2D/Box2D.h"
#include "gameplay.h"

class DebugDraw : public b2Draw
{

public:


    virtual void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);

    virtual void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);

    virtual void DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color);

    virtual void DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color);

    virtual void DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count);

    virtual void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color);

    virtual void DrawTransform(const b2Transform &xf);

    void draw(b2World * world, gameplay::Node *node);

protected:

    struct Part {
        std::vector<short> indices;
        gameplay::Mesh::PrimitiveType type;
    };


    std::vector<float> _vertices;
    std::vector<Part> _parts;
    gameplay::Material *_material;

    void pushVertex(float x, float y, float r, float g, float b, float a);
};


#endif // DebugDraw_h