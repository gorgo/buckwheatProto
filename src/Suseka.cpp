#include "Suseka.h"
#include "Box2D/Box2D.h"
#include "DebugDraw.h"

DebugDraw debugDraw;

// Declare our game instance
Suseka game;

class MyEndListener : public AnimationClip::Listener{

public:
    virtual void animationEvent(AnimationClip *clip, EventType type);
};


void MyEndListener::animationEvent(AnimationClip *clip, AnimationClip::Listener::EventType type) {
    Logger::log(Logger::LEVEL_INFO, "yohoho!");

    clip->getAnimation()->getClip("down")->play();
}

MyEndListener listener;

Suseka::Suseka()
    : _scene(NULL), _wireframe(false)
{
}

void prepareAlpha (Node* node){
    node->getModel()->getMaterial(0)->getStateBlock()->setBlend(true);
    node->getModel()->getMaterial(0)->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    node->getModel()->getMaterial(0)->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
    node->getModel()->getMaterial(0)->getStateBlock()->setDepthTest(false);
    node->getModel()->getMaterial(0)->getStateBlock()->setCullFace(false);
    node->setTag("alphaBlend", "1");
};

void prepareAlphaRecursive (Node* node) {
    prepareAlpha(node);
    Node* child = node->getFirstChild();
    while (child) {
        if (child->getModel())
            prepareAlphaRecursive(child);
        child = child->getNextSibling();
    }
};

void Suseka::initialize()
{
    // Load game scene from file
    _scene = Scene::load("res/flyingCat/cat.gpb");
    // Get the box model and initialize its material parameter values and bindings

    Node* boxNode = _scene->findNode("torso");
    Model* boxModel = boxNode->getModel();
    Material* boxMaterial = boxModel->getMaterial(0);

    prepareAlphaRecursive(boxNode);

    if (boxMaterial) Logger::log(Logger::LEVEL_INFO, "material got");
    Logger::log(Logger::LEVEL_INFO, "\ntransform actual %f, %f, %f\n", boxNode->getTranslationX(), boxNode->getTranslationY(), boxNode->getTranslationZ());

    // Set the aspect ratio for the scene's camera to match the current resolution
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());
    _scene->getActiveCamera()->getNode()->rotate(Vector3::unitY(), MATH_DEG_TO_RAD(90));
    Animation *anim = boxNode->getAnimation("torso");
    anim->createClips("res/nlaTests/clips.animation#animations");
    anim->getClip("fly")->play();
    anim->getClip("tailRoll")->play();

    _cat = Node::create("cat");
    _scene->addNode(_cat);
    boxNode->addRef();
    _scene->removeNode(boxNode);
    _cat->addChild(boxNode);
    boxNode->release();

    //_form = Form::create("res/forms/oneButton.form");
    //static_cast<Button*>(_form->getControl("testButton"))->addListener(this, Control::Listener::CLICK);
    _b2World = new b2World(b2Vec2(0.0f, -9.20f));

    b2PolygonShape shape;
    shape.SetAsBox(3.10f, 3.10f);
    b2BodyDef bdef;
    bdef.type = b2BodyType::b2_dynamicBody;
    bdef.userData = _cat;

    b2Body *body = _b2World->CreateBody(&bdef);
    body->CreateFixture(&shape, 1.0f);

    body = _b2World->CreateBody(&bdef);
    b2CircleShape circle;
    circle.m_radius = 3.0f;
    body->CreateFixture(&circle, 10.0f);

    bdef.type = b2BodyType::b2_staticBody;
    bdef.position = b2Vec2(0.0f, -3.0f);
    body = _b2World->CreateBody(&bdef);
    body->CreateFixture(&shape, 0.0f);


    setVsync(true);


    _debugDrawNode = _scene->addNode("debugDraw");
    _b2World->SetDebugDraw(&debugDraw);
}


void Suseka::controlEvent(Control *control, EventType evt) {
    Logger::log(Logger::LEVEL_INFO, "click");
}

void Suseka::finalize()
{
    SAFE_RELEASE(_scene);
    SAFE_RELEASE(_form);
    delete _b2World;
}

void Suseka::update(float elapsedTime)
{
    // Rotate model
    //_scene->findNode("box")->rotateY(MATH_DEG_TO_RAD((float)elapsedTime / 1000.0f * 180.0f));
    //_scene->getActiveCamera()->getNode()->rotateY(MATH_DEG_TO_RAD(1));
    //_form->update(elapsedTime);
    _b2World->Step(elapsedTime * 0.001f, 10, 10);
    debugDraw.draw(_b2World, _debugDrawNode);

    for (b2Body* b = _b2World->GetBodyList(); b; b = b->GetNext())
    {
        static_cast<Node*>(b->GetUserData())->setTranslation(b->GetPosition().x, 0.0, b->GetPosition().y);
    }

}

void Suseka::render(float elapsedTime)
{
    // Clear the color and depth buffers
    const float brightness = 0.1f;
    clear(CLEAR_COLOR_DEPTH, Vector4(brightness, brightness, brightness, brightness), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &Suseka::drawScene);
    //_form->draw();
}

bool Suseka::drawScene(Node* node)
{
    // If the node visited contains a model, draw it
    Model* model = node->getModel(); 
    if (model)
    {
        model->draw(_wireframe);
    }
    return true;
}

void Suseka::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
}



void Suseka::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS: {
        _wireframe = !_wireframe;
        Ray ray;
        _scene->getActiveCamera()->pickRay(Rectangle(0,0,getWidth(), getHeight()), x, y, &ray);
        /*PhysicsController::HitResult hitResult;
        PhysicsController::HitFilter hitFilter;
        getPhysicsController()->rayTest(ray, 1000, &hitResult, &hitFilter);*/

        float ratio = -ray.getOrigin().y / ray.getDirection().y;
        Vector3 xzPlaneProj;
        xzPlaneProj.y = 0;
        xzPlaneProj.x = ray.getOrigin().x + ray.getDirection().x * ratio;
        xzPlaneProj.z = ray.getOrigin().z + ray.getDirection().z * ratio;

        float from[3] = {_cat->getTranslationX(), 0.0f, _cat->getTranslationZ()};
        float to[3] = {xzPlaneProj.x, 0.0f, xzPlaneProj.z};

        Animation *anim = _cat->createAnimationFromTo("move", Transform::ANIMATE_TRANSLATE, from, to, Curve::InterpolationType::SINE_IN_OUT , 1000);
        anim->getClip()->play();

        break;
    };
    case Touch::TOUCH_RELEASE:
        break;
    case Touch::TOUCH_MOVE:
        break;
    };
}

void Suseka::setupDebugDraw() {

}
