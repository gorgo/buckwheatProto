#ifndef TEMPLATEGAME_H_
#define TEMPLATEGAME_H_

#include "gameplay.h"
#include "Box2D/Box2D.h"
using namespace gameplay;

/**
 * Main game class.
 */
class Suseka: public Game, public Control::Listener
{
friend class AnimationController;
public:

    /**
     * Constructor.
     */
    Suseka();

    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:

    /**
     * Draws the scene each frame.
     */
    bool drawScene(Node* node);

    Scene* _scene;
    bool _wireframe;
    Scene *_scene2;
    Node *_cat;
    Form *_form;
public:
    virtual void controlEvent(Control *control, EventType evt);

    b2World *_b2World;

    void setupDebugDraw();

    Node *_debugDrawNode;
};

#endif
