#ifndef SHIP_HPP
#define SHIP_HPP

#include <Jopnal/Jopnal.hpp>
#include "Consts.hpp"

enum class ShipType
{
    player,
    enemyLight,
    enemyHeavy
};

class Ship
{
    jop::Scene& m_scene;
    int m_health;

public:
    
    ShipType m_type;
    jop::WeakReference<jop::Object> m_obj;

    //////////////////////////////////////////

    int getHealth() const
    {
        return m_health;
    }

    //////////////////////////////////////////

    void setHeath(const int health)
    {
        m_health = health;
    }

    //////////////////////////////////////////

    glm::vec2 getPosition()
    {
        return glm::vec2(m_obj->getGlobalPosition().x, m_obj->getGlobalPosition().y);
    }

    //////////////////////////////////////////

    void setPosition(glm::vec2 pos)
    {
        m_obj->setPosition(pos.x, pos.y, 0.f);
    }

    //////////////////////////////////////////

    void moveShip(const int dir, const float speed)
    {
        m_obj->setPosition(m_obj->getGlobalPosition().x + dir * speed, m_obj->getGlobalPosition().y, 0.f);
    }

    //////////////////////////////////////////

    Ship(jop::Scene& scene, const ShipType type, const glm::vec2 position = glm::vec2(0.f, 0.f)) :
        m_scene(scene),
        m_type(type),
        m_obj(),
        m_health(type == ShipType::enemyHeavy ? 2 : 1)
    {
        switch (m_type)
        {
        case ShipType::player:
            m_obj = scene.createChild("player");
            m_obj->setPosition(position.x, position.y, 0.f);
            m_obj->createComponent<jop::Drawable>(scene.getRenderer()).setModel(*c_playerModel);
            break;

        case ShipType::enemyLight:
            m_obj = scene.createChild("light");
            m_obj->setPosition(position.x, position.y, 0.f);
            m_obj->createComponent<jop::Drawable>(scene.getRenderer()).setModel(*c_enemyLightModel);
            break;

        case ShipType::enemyHeavy:
            m_obj = scene.createChild("heavy");
            m_obj->setPosition(position.x, position.y, 0.f);
            m_obj->createComponent<jop::Drawable>(scene.getRenderer()).setModel(*c_enemyHeavyModel);
            break;
        }
    }
};

#endif