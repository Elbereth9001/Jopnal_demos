#ifndef BRIDGESCENE_HPP
#define BRIDGESCENE_HPP
#include <Jopnal/Jopnal.hpp>

static const glm::vec3 startPos(-90.f, 60.f, 0.f);



class BRIDGESCENE : public jop::Scene
{
private:

    bool m_broken;
    jop::WeakReference<jop::Object> m_cam;
    jop::WeakReference<jop::Object> m_car;
    jop::WeakReference<jop::Object> m_ground;

    ///////////////////////////////////////////////////

    void createBridge()
    {
        //Create a ground to drive on
        {
            std::vector<glm::vec2> ground_vec =
            {
                glm::vec2(-100.f, 50.f),
                glm::vec2(-50.f, 50.f),
                glm::vec2(-50.f, 0.f),
                glm::vec2(50.f, 0.f),
                glm::vec2(50.f, 50.f),
                glm::vec2(100.f, 50.f)
            };

            jop::RigidBody2D::ConstructInfo2D groundInfo(jop::ResourceManager::getNamed<jop::TerrainShape2D>("ground2D", ground_vec));
            m_ground->createComponent<jop::RigidBody2D>(getWorld<2>(), groundInfo);
        }

        //Create two pieces to hold the bridge in place
        {
            //Static Sensor = unaffected by gravity and we can drive through it
            jop::RigidBody2D::ConstructInfo2D holdInfo(jop::ResourceManager::getNamed<jop::RectangleShape2D>("holders", 1.f, 1.f), jop::RigidBody2D::Type::StaticSensor);
            auto h1 = m_ground->createChild("holder1");
            h1->setPosition(-52.f, 51.f, 0.f);
            h1->createComponent<jop::RigidBody2D>(getWorld<2>(), holdInfo);
            auto h2 = m_ground->createChild("holder2");
            h2->setPosition(52.f, 51.f, 0.f);
            h2->createComponent<jop::RigidBody2D>(getWorld<2>(), holdInfo);
        }

        //Creating the actual bridge
        {
            //Creating a model to use in the bridge
            jop::Model plankModel = jop::Model(
                //Creating a model requires a Mesh and a Material
                //Mesh and its size
                jop::ResourceManager::getNamed<jop::RectangleMesh>("plankMesh", glm::vec2(2.f, 0.5f)),
                //Material and its mapping
                jop::ResourceManager::getEmpty<jop::Material>("plankMat", true).setMap(jop::Material::Map::Diffuse, jop::ResourceManager::get<jop::Texture2D>("plank.png")));

            jop::RigidBody2D::ConstructInfo2D plankInfo(jop::ResourceManager::getNamed<jop::RectangleShape2D>("plank", 2.f, 0.5f), jop::RigidBody2D::Type::Dynamic, 1.2f);
            auto b = m_ground->createChild("bridge");
            const unsigned int bridgePlanks = 33;

            //Create and place the bridge planks
            for (unsigned int i = 0; i < bridgePlanks; ++i)
            {
                auto plank = b->createChild("");
                plank->setPosition(-48.f + (3.f * (float)i), 50.f, 0.f);
                plank->createComponent<jop::RigidBody2D>(getWorld<2>(), plankInfo);
                plank->createComponent<jop::Drawable>(getRenderer()).setModel(plankModel);
            }

            //Attach one plank to the next
            for (unsigned int i = 0; i < bridgePlanks - 1; ++i)
            {
                auto* cLink = b->getChildren()[i].getComponent<jop::RigidBody2D>();
                auto* nLink = b->getChildren()[i + 1].getComponent<jop::RigidBody2D>();
                cLink->link<jop::RopeJoint2D>(*nLink, true,
                    glm::vec2(0.9f, 0.f),
                    glm::vec2(-0.9f, 0.f)
                    );
                cLink->getJoint<jop::RopeJoint2D>()->setDamping(cLink->getJoint<jop::RopeJoint2D>()->getDamping().first, 1.f);
            }

            //Attach the first and the last plank to the holding elements
            m_ground->findChild("holder1")->getComponent<jop::RigidBody2D>()->link<jop::RopeJoint2D>(*b->getChildren().begin()->getComponent<jop::RigidBody2D>(), true);
            m_ground->findChild("holder2")->getComponent<jop::RigidBody2D>()->link<jop::RopeJoint2D>(*b->getChildren().back().getComponent<jop::RigidBody2D>(), true);
        }
    }

    ///////////////////////////////////////////////////

    void createCar()
    {
        //Create a car

        //Chassis
        {
            jop::RigidBody2D::ConstructInfo2D chassisInfo(jop::ResourceManager::getNamed<jop::RectangleShape2D>("carchassis", 8.f, 2.f), jop::RigidBody2D::Type::Dynamic, 2.6f);
            m_car->setPosition(startPos.x, startPos.y, startPos.z);
            m_car->createComponent<jop::RigidBody2D>(getWorld<2>(), chassisInfo);
        }

        //Wheels
        {
            jop::Model wheelModel = jop::Model(
                jop::ResourceManager::getNamed<jop::CircleMesh>("wheelMesh", 1.2f, 30u),
                jop::ResourceManager::getEmpty<jop::Material>("wheelMat", true).setMap(jop::Material::Map::Diffuse, jop::ResourceManager::get<jop::Texture2D>("wheel.png")));


            jop::RigidBody2D::ConstructInfo2D wheelInfo(jop::ResourceManager::getNamed<jop::CircleShape2D>("wheels", 1.2f), jop::RigidBody2D::Type::Dynamic, 0.75f);
            wheelInfo.friction *= 5.f;

            for (unsigned int i = 0; i < 2; ++i)
            {
                float wheelX = (-3.f + (i * 6.f));
                float wheelY = -3.f;
                auto wheel = m_car->createChild("");
                wheel->setPosition(wheelX, wheelY, 0.f);
                wheel->createComponent<jop::Drawable>(getRenderer()).setModel(wheelModel);

                //Chassis
                m_car->getComponent<jop::RigidBody2D>()

                    //Attach a joint to it
                    ->link<jop::WheelJoint2D>(

                    //First argument in linking is the second rigidbody to attach the joint to.
                    //In this case it's the wheel, whose rigidbody we'll create just before attaching it to the chassis
                    wheel->createComponent<jop::RigidBody2D>(getWorld<2>(), wheelInfo),

                    //Rest of the arguments for the WheelJoint2D constructor
                    false, glm::vec2(0.f, 1.f), glm::vec2(wheelX, wheelY));
            }
        }
    }

    ///////////////////////////////////////////////////

    void breakBridge()
    {
        //Let's keep this simple and break the bridge only once
        if (m_broken)
            return;

        //Get the main bridge object
        auto& bridge = m_ground->findChild("bridge");

        //How many pieces does the bridge have?
        unsigned int max = bridge->getChildren().size() - 1;

        //Create randomizer
        jop::Randomizer r;

        //Randomize a childIndex
        unsigned int breakHere = r.range(0u, max);

        //Get a piece of the bridge where to break a joint
        bridge->getChildren()[breakHere]
            //Get the rigidbody of that piece
            .getComponent<jop::RigidBody2D>()
            //Break the joint
            ->breakJoint<jop::RopeJoint2D>();

        //No more breaking
        m_broken = true;
    }

    ///////////////////////////////////////////////////

public:

    BRIDGESCENE() :
        jop::Scene(typeid(BRIDGESCENE).name()),
        m_broken(false),
        m_cam(createChild("cam")),
        m_car(createChild("car")),
        m_ground(createChild("ground2D"))
    {

        getWorld<2>().setDebugMode(true);

        m_cam->setPosition(startPos.x, startPos.y, startPos.z + 60.f);
        m_cam->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);

        createBridge();
        createCar();
    }

    void postUpdate(const float deltaTime) override
    {
        //Keep the camera following the car
        m_cam->setPosition(
            m_car->getGlobalPosition().x,
            m_car->getGlobalPosition().y,
            m_cam->getGlobalPosition().z
            );

        {
            using k = jop::Keyboard;

            if (k::isKeyDown(k::Left) || k::isKeyDown(k::A))
                //Apply rotation to all the wheels of the car
                std::for_each(m_car->getChildren().begin(), m_car->getChildren().end(), [](jop::WeakReference<jop::Object> obj){obj->getComponent<jop::RigidBody2D>()->applyTorque(20.f); });

            if (k::isKeyDown(k::Right) || k::isKeyDown(k::D))
                //Apply the same rotation in opposite direction
                std::for_each(m_car->getChildren().begin(), m_car->getChildren().end(), [](jop::WeakReference<jop::Object> obj){obj->getComponent<jop::RigidBody2D>()->applyTorque(-20.f); });

            if (k::isKeyDown(k::Space))
                breakBridge();

        }

    }
};



#endif