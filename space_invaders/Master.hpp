#ifndef MASTER_HPP
#define MASTER_HPP

#include <Jopnal/Jopnal.hpp>

#include "Bullet.hpp"
#include "Ship.hpp"


class Master : public jop::Scene
{
    jop::Scene& m_scene;
    jop::WeakReference<jop::Object> m_cam;
    jop::WeakReference<jop::Object> m_ui;
    Ship* m_player;
    std::vector<Ship*> m_ships;
    std::vector<Bullet*> m_bullets;
    float m_moveSpeed;
    float m_shootCD;
    float m_timer;


    ///////////////////////////////////////////

    void init(const unsigned int difficulty)
    {
        m_player = new Ship(m_scene, ShipType::player);

        float x(-10.f);
        float y(30.f);

        for (unsigned int i = 0u; i < difficulty + 20u; ++i)
        {
            if (x >= 10.f)
            {
                x = -10.f;
                y -= 3.f;
            }
            m_ships.emplace_back(new Ship(m_scene, i % 3 ? ShipType::enemyLight : ShipType::enemyHeavy, glm::vec2(x, y)));
            x += 2.f;
        }

        auto bg = m_scene.createChild("bg");
        bg->setPosition(0.f, 15.f, -0.1f);
        //bg->setScale(0.1f);
        bg->createComponent<jop::Drawable>(getRenderer()).setModel(*c_backgroundModel);

    }

    ///////////////////////////////////////////

    void gameOver()
    {
        jop::Engine::setState(jop::Engine::State::RenderOnly);
    }

    ///////////////////////////////////////////

    void nextLevel()
    {
        if (m_timer < 5.f + difficulty * 0.1f)
            score += 10u * ((unsigned int)floor(5.f - m_timer));
        difficulty += 10u;
        delete m_player;
        jop::Engine::createScene<Master>();
    }

    ///////////////////////////////////////////

    void updateShips()
    {
        if (m_ships.empty())
            nextLevel();

        for (auto itr : m_ships)
            itr->moveShip(m_moveDir, m_moveSpeed);

        for (auto itr : m_ships)
        {
            if (itr->getPosition().x > 20.f || itr->getPosition().x < -20.f)
            {
                for (auto itr2 : m_ships)
                    itr2->setPosition(glm::vec2(itr2->getPosition().x, itr2->getPosition().y - 3.f));
                m_moveDir = -m_moveDir;
                m_moveSpeed += 0.05f;
                break;
            }
        }

        for (auto itr : m_ships)
            if (itr->getPosition().y <= 0.f)
                gameOver();
    }

    ///////////////////////////////////////////

    void updateBullets()
    {
        for (auto& itr : m_bullets)
        {
            auto p = itr->m_obj->getGlobalPosition();
            itr->m_obj->setPosition(p.x, p.y + 1.f, p.z);
        }

        for (auto& itrb : m_bullets)
        {
            if (!itrb->m_hasHit)
            {
                for (unsigned int j = 0u; j < m_ships.size(); ++j)
                {
                    auto bs = m_ships[j]->m_obj->getComponent<jop::Drawable>()->getGlobalBounds();
                    auto ps = m_ships[j]->m_obj->getGlobalPosition();
                    auto pb = itrb->m_obj->getGlobalPosition();
                    if (
                        ps.x + bs.second.x >= pb.x &&
                        ps.x + bs.first.x <= pb.x &&
                        ps.y + bs.second.y >= pb.y &&
                        ps.y + bs.first.y <= pb.y
                        )
                    {
                        itrb->m_hasHit = true;
                        m_ships[j]->setHeath(m_ships[j]->getHealth() - 1);

                        if (m_ships[j]->getHealth() <= 0)
                        {
                            ++score;
                            if (m_ships[j]->m_type == ShipType::enemyHeavy)
                                ++score;

                            m_ships[j]->m_obj->removeSelf();
                            delete m_ships[j];
                            m_ships.erase(m_ships.begin() + j);
                        }
                        break;
                    }
                }
            }
        }

        auto b = m_bullets;

        for (unsigned int i = 0u; i < b.size(); ++i)
        {
            if (b[i]->m_hasHit)
                b[i]->m_obj->getComponent<jop::Drawable>()->setRenderGroup(5u);

            if (b[i]->m_obj->getGlobalPosition().y > 40.f)
            {
                removeNews(i);
            }
        }
    }

    ///////////////////////////////////////////

    void removeNews(const unsigned int index)
    {
        m_bullets[index]->m_obj->removeSelf();
        delete m_bullets[index];
        m_bullets.erase(m_bullets.begin() + index);
    }

    ///////////////////////////////////////////

public:

    int m_moveDir;

    ///////////////////////////////////////////

    void fire()
    {
        glm::vec3 p = m_player->m_obj->getGlobalPosition();

        m_bullets.emplace_back(new Bullet(m_scene, glm::vec3(p.x, p.y + 1.2f, p.z)));

    }

    ///////////////////////////////////////////


    Master() :
        jop::Scene("Master"),
        m_scene(*this),
        m_cam(createChild("cam")),
        m_ui(createChild("ui")),
        m_bullets(),
        m_player(),
        m_ships(),
        m_moveDir(-1),
        m_moveSpeed(0.1f),
        m_shootCD(shootCD),
        m_timer(0.f)
    {
        createChild("bullets")->reserveChildren(9u);
        m_cam->setPosition(0.f, 15.f, 0.f);
        m_cam->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Orthographic).setSize(45.f, 35.f);

        m_ui->setPosition(-20.f, -1.f, 1.f);
        m_ui->setScale(0.05f);
        m_ui->createComponent<jop::Text>(getRenderer()).setColor(jop::Color::Blue);

        init(difficulty);
    }

    ///////////////////////////////////////////

    void preUpdate(const float deltaTime) override
    {
        using k = jop::Keyboard;

        m_shootCD -= deltaTime;
        m_timer += deltaTime;

        if (k::isKeyDown(k::Left))
        {
            m_player->moveShip(-1, 0.5f);
        }
        else if (k::isKeyDown(k::Right))
        {
            m_player->moveShip(1, 0.5f);
        }
        if (k::isKeyDown(k::Space))
        {
            if (m_shootCD <= 0.f)
            {
                fire();
                m_shootCD = shootCD;
            }
        }

        auto p = m_player->m_obj->getGlobalPosition();

        if (p.x < -20.f)
            m_player->m_obj->setPosition(-20.f, p.y, p.z);
        if (p.x > 20.f)
            m_player->m_obj->setPosition(20.f, p.y, p.z);

        updateShips();
        updateBullets();

        m_ui->getComponent<jop::Text>()->setString(std::to_string(score));
    }

    ///////////////////////////////////////////

};

#include "EventHandler.hpp"

#endif