#ifndef CARTERRAIN_HPP
#define CARTERRAIN_HPP

#include <Jopnal/Jopnal.hpp>

class CARTERRAINSCENE : public jop::Scene
{

    jop::WeakReference<jop::Object> m_cam;
    jop::WeakReference<jop::Object> m_car;
    jop::WeakReference<jop::Object> m_ground;

    /////////////////////////////////////////////////////////

    void createGround(const unsigned int length, const unsigned int width, const unsigned int entropy)
    {
        auto r = [&entropy]()
        {
            static jop::Randomizer r;
            return r.range<float>(0.f, ((float)entropy * 0.4f));
        };

        const float c_width = (float)width / (float)entropy;
        const float c_length = ((float)length / ((float)entropy * 2.f));

        float lWidth(0.f);
        float uWidth(c_width);
        float lLength(0.f);
        float uLength(c_length);

        std::vector<glm::vec3> ground;
        ground.reserve((c_width + c_length) * 3);

        //Need to remember Y-value, could use iterators and find_if
        std::vector<float> oldYs;
        std::vector<float> newYs;

        for (unsigned int i = 0u; i < entropy + 1u; ++i)
        {
            oldYs.emplace_back(r());
            newYs.emplace_back(r());
        }
        {
            while (uLength <= length)
            {
                for (unsigned int i = 0u; i < entropy; ++i)
                {
                    ground.emplace_back(
                        lWidth,
                        oldYs[i],
                        lLength
                        );

                    ground.emplace_back(
                        uWidth,
                        oldYs[i + 1u],
                        lLength
                        );

                    ground.emplace_back(
                        lWidth,
                        newYs[i],
                        uLength
                        );

                    ground.emplace_back(
                        lWidth,
                        newYs[i],
                        uLength
                        );

                    ground.emplace_back(
                        uWidth,
                        oldYs[i + 1u],
                        lLength
                        );

                    ground.emplace_back(
                        uWidth,
                        newYs[i + 1u],
                        uLength
                        );

                    lWidth = uWidth;
                    uWidth += c_width;
                }

                for (unsigned int i = 0u; i < newYs.size(); ++i)
                {
                    oldYs[i] = newYs[i];
                    newYs[i] = r();
                }

                lLength = uLength;
                uLength += c_length;
                lWidth = 0u;
                uWidth = c_width;
            }
        }
        jop::RigidBody::ConstructInfo groundInfo(jop::ResourceManager::getNamed<jop::TerrainShape>("ground", ground));
        m_ground->createComponent<jop::RigidBody>(getWorld<3>(), groundInfo);

        std::vector<std::pair<glm::vec3, glm::vec4>> meshPoints;
        std::vector<unsigned int> meshIndices;
        jop::uint32 base(0u);
        float colorHere(0.f);
        float colorNext(0.f);
        const glm::vec4 color1(1.f, 0.2f, 0.f, 1.f);
        const glm::vec4 color2(0.f, 0.5f, 0.f, 1.f);
        const glm::vec4 color3(1.2f, 0.5f, 0.2f, 1.f);

        meshPoints.reserve(ground.size() * 4u);
        meshPoints.reserve(ground.size() * 6u);

        for (unsigned int i = 0u; i < ground.size() - 3u;)
        {
            base = i * 4u;

            for (unsigned int j = 0u; j < 4u; ++j)
            {
                meshIndices.push_back(base + 2u);
                meshIndices.push_back(base + 1u);
                meshIndices.push_back(base + 0u);

                meshPoints.emplace_back(ground[i], color1);
                meshPoints.emplace_back(ground[i + 1u], color2);
                meshPoints.emplace_back(ground[i + 2u], color3);
            }

            i += 3u;
        }

        auto& mesh = jop::ResourceManager::getEmpty<jop::Mesh>("groundmesh");
        mesh.load(meshPoints.data(), meshPoints.size() * sizeof(std::pair<glm::vec3, glm::vec4>), jop::Mesh::Position | jop::Mesh::Color, meshIndices.data(), sizeof(unsigned int), meshIndices.size());
        auto& mat = jop::ResourceManager::getEmpty<jop::Material>("groundmat", false);

        m_ground->createComponent<jop::Drawable>(getRenderer()).setModel(jop::Model(mesh, mat));
    }

    /////////////////////////////////////////////////////////

    void createCar(const unsigned int length, const unsigned int wheels)
    {

        jop::RigidBody::ConstructInfo chassisInfo(jop::ResourceManager::getNamed<jop::BoxShape>("chassis", glm::vec3(4.f, 1.f, 6.f)), jop::RigidBody::Type::Dynamic, 3.7f);

        jop::Model chassisModel(jop::ResourceManager::getNamed<jop::BoxMesh>("chassismesh", glm::vec3(4.f, 1.f, 6.f)),
            jop::ResourceManager::getEmpty<jop::Material>("chassismat", true).setMap(jop::Material::Map::Diffuse, jop::ResourceManager::get<jop::Texture2D>("chassis.png")));

        jop::RigidBody::ConstructInfo wheelInfo(jop::ResourceManager::getNamed<jop::SphereShape>("wheels", 0.5f), jop::RigidBody::Type::Dynamic, 0.5f);

        jop::Model wheelModel(jop::ResourceManager::getNamed<jop::SphereMesh>("wheelmesh", 0.5f, 15),
            jop::ResourceManager::getEmpty<jop::Material>("wheelmat", true).setMap(jop::Material::Map::Diffuse, jop::ResourceManager::get<jop::Texture2D>("wheel.png")));

        wheelInfo.friction *= 8.f;

        m_car->setPosition(5.f, 10.f, 5.f);
        m_car->createComponent<jop::RigidBody>(getWorld<3>(), chassisInfo);
        m_car->createComponent<jop::Drawable>(getRenderer()).setModel(chassisModel);


        for (int i = 0; i < 4; ++i)
        {
            glm::vec3 oldPos = m_car->getGlobalPosition();
            glm::vec3 newPos;

            switch (i)
            {
            case 0:
                newPos = glm::vec3(-1.5f, -2.f, -2.f);
                break;
            case 1:
                newPos = glm::vec3(-1.5f, -2.f, 2.f);
                break;
            case 2:
                newPos = glm::vec3(1.5f, -2.f, -2.f);
                break;
            case 3:
                newPos = glm::vec3(1.5f, -2.f, 2.f);
                break;
            }

            auto wheel = m_car->createChild("");
            wheel->setPosition(newPos);
            wheel->createComponent<jop::RigidBody>(getWorld<3>(), wheelInfo);
            wheel->createComponent<jop::Drawable>(getRenderer()).setModel(wheelModel);

            m_car->getComponent<jop::RigidBody>()->link<jop::WheelJoint>(*wheel->getComponent<jop::RigidBody>(), true);
        }


    }


public:

    CARTERRAINSCENE() :
        jop::Scene("CARTERRAINSCENE"),
        m_cam(createChild("cam")),
        m_car(createChild("car")),
        m_ground(createChild("ground"))
    {

        unsigned int groundLength(160u);
        unsigned int groundWidth(32u);
        unsigned int entropy(4u);

        unsigned int carLength(6u);
        unsigned int wheels(4u);

        createGround(groundLength, groundWidth, entropy);
        createCar(carLength, wheels);

        //   glm::quat q(0.006f, 0.020f, 0.963f, -0.267f);
        //   auto a = std::atan2(2 * (q.w*q.x + q.y*q.z), 1 - 2 * (sqrt(q.x) + sqrt(q.y)));
        //   auto b = asin(2 * (q.w*q.y - q.z*q.x));
        //   auto c = std::atan2(2 * (q.w*q.z + q.x*q.y), 1 - 2 * (sqrt(q.y) + sqrt(q.z)));
        //   m_cam->rotate(a, b, c);


        m_cam->rotate(2.6f, 0.f, -3.1f);
        //m_cam->rotate(glm::quat(0.006f, 0.020f, 0.963f, -0.267f));

        m_cam->setPosition(10.f, 10.f, -10.f);

        m_cam->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);


        //        getWorld<3>().setDebugMode(true);

    }

    void preUpdate(const float deltaTime) override
    {
        glm::vec3 pg = m_car->getGlobalPosition();
        glm::vec3 pl = m_car->getLocalPosition();
        glm::quat rg = m_car->getGlobalRotation();
        glm::quat rl = m_car->getLocalRotation();

        m_cam->setPosition(pl.x, pl.y + 4.f, pl.z - 9.f);
        m_cam->lookAt(pg);


        using k = jop::Keyboard;
        if (k::isKeyDown(k::Up))
            std::for_each(m_car->getChildren().begin(), m_car->getChildren().end(),
            [](jop::WeakReference<jop::Object> obj){obj->getComponent<jop::RigidBody>()->applyTorque(glm::vec3(2.f, 0.f, 0.f)); });
        if (k::isKeyDown(k::Down))
            std::for_each(m_car->getChildren().begin(), m_car->getChildren().end(),
            [](jop::WeakReference<jop::Object> obj){obj->getComponent<jop::RigidBody>()->applyTorque(glm::vec3(-2.f, 0.f, 0.f)); });
        if (k::isKeyDown(k::Left))
            for (unsigned int i = 0u; i < m_car->getChildren().size(); ++i)
                if (i % 2)
                {
                    auto* j = m_car->getChildren()[i].getComponent<jop::RigidBody>()->getJoint<jop::WheelJoint>();
                    j->setAngle(j->getAngle() - deltaTime);
                }
        if (k::isKeyDown(k::Right))
            for (unsigned int i = 0u; i < m_car->getChildren().size(); ++i)
                if (i % 2)
                {
                    auto* j = m_car->getChildren()[i].getComponent<jop::RigidBody>()->getJoint<jop::WheelJoint>();
                    j->setAngle(j->getAngle() + deltaTime);
                }
    }

};
#endif