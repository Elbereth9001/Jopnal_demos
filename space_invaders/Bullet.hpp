#ifndef MAGNUM_HPP
#define MAGNUM_HPP

#include <Jopnal/Jopnal.hpp>

#include "Consts.hpp"

class Bullet
{
    jop::Scene& m_scene;

public:

    bool m_hasHit;
    jop::WeakReference<jop::Object> m_obj;

    Bullet(jop::Scene& scene, glm::vec3& position) :
        m_scene(scene),
        m_hasHit(false),
        m_obj(m_scene.findChild("bullets")->createChild(""))
    {
        m_obj->setPosition(position);
        m_obj->createComponent<jop::Drawable>(m_scene.getRenderer()).setModel(*c_bulletModel);
    }

};

#endif