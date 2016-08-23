#ifndef CONVEYORBELT_HPP
#define CONVEYORBELT_HPP

#include <Jopnal/Jopnal.hpp>

class CONVEYORSCENE : public jop::Scene
{
    //Param 1: space for parts
    //Param 2: distance between centers
    //Return 1: Amount of parts that can be fitted
    //Return 2: Space that can be added to each dbc
    std::pair<unsigned int, float> computeMainBeltArea(const float _sfp, const float dbc)
    {
        //Amount of parts
        unsigned int a(0u);

        //Space for parts
        float sfp(_sfp);

        //How many parts do we need?
        while (sfp > dbc)
        {
            sfp -= dbc;
            ++a;
        }

        return std::make_pair(a, sfp / a);
    }

    void createWheels(const glm::vec3 center, const float force, const float length, const float height)
    {
        //Wheel radius
        const float wr(height / 9.f);

        static const jop::Model wheelModel = jop::Model(
            jop::ResourceManager::getNamed<jop::CircleMesh>("wheelMesh", wr, 30u),
            jop::ResourceManager::getEmpty<jop::Material>("wheelMat", true).setMap(jop::Material::Map::Diffuse, jop::ResourceManager::get<jop::Texture2D>("wheel.png")));

        static const jop::RigidBody2D::ConstructInfo2D wheelInfo(jop::ResourceManager::getNamed<jop::CircleShape2D>("wheel", wr), jop::RigidBody2D::Type::Kinematic);


        //Current wheel center
        float cwcX(center.x - ((length * 0.5f) + wr));
        float cwcY(center.y);

        jop::WeakReference<jop::Object> wheels = m_system->createChild("wheels");

        //First wheel
        auto w1 = wheels->createChild("");
        w1->setPosition(cwcX, cwcY, center.z);
        w1->createComponent<jop::RigidBody2D>(getWorld<2>(), wheelInfo);

        cwcX += 2.f * wr;
        cwcY += 3.f * wr;

        auto mainBelt = computeMainBeltArea(length - (4.f * wr), (3.f * wr));
        wheels->reserveChildren(mainBelt.first);

        //Wheels upper, 2. -> n-1
        unsigned int i(0u);
        while (i < mainBelt.first)
        {
            //Wheel n upper
            auto wnu = wheels->createChild("");
            wnu->setPosition(cwcX, cwcY, center.z);
            wnu->createComponent<jop::RigidBody2D>(getWorld<2>(), wheelInfo);

            cwcX += (3.f * wr) + mainBelt.second;
            ++i;
        }

        //Last wheel
        {
            cwcX -= wr + mainBelt.second;
            cwcY = center.y;
            auto w2 = wheels->createChild("");
            w2->setPosition(cwcX, cwcY, center.z);
            w2->createComponent<jop::RigidBody2D>(getWorld<2>(), wheelInfo);
            cwcX -= 2.f * wr;
            cwcY -= 3.f * wr;
        }

        //Wheels lower, n-1 -> 2.
        while (i < mainBelt.first * 2u)
        {
            //Wheel n lower
            auto wnl = wheels->createChild("");
            wnl->setPosition(cwcX, cwcY, center.z);
            wnl->createComponent<jop::RigidBody2D>(getWorld<2>(), wheelInfo);
            cwcX -= (3.f * wr) + mainBelt.second;
            ++i;
        }

    }

    void createBelt(const glm::vec3 center, const float force, const float length, const float height)
    {

        //belt piece length & height
        const float pl(height / 6.f);
        const float ph(height / 12.f);

        //Piece apart
        const float pa(height / 18.f);

        //Creating models to use in the conveyor belt
        static const jop::Model pieceModel = jop::Model(
            jop::ResourceManager::getNamed<jop::RectangleMesh>("plankMesh", glm::vec2(pl, ph)),
            jop::ResourceManager::getEmpty<jop::Material>("plankMat", true).setMap(jop::Material::Map::Diffuse, jop::ResourceManager::get<jop::Texture2D>("plank.png")));

        static const jop::RigidBody2D::ConstructInfo2D pieceInfo(jop::ResourceManager::getNamed<jop::RectangleShape2D>("beltPiece", pl, ph), jop::RigidBody2D::Type::Dynamic, 0.005f);

        auto belt = m_system->createChild("belt");

        auto mainBelt = computeMainBeltArea(length, pl + pa);
        belt->reserveChildren(mainBelt.first);

        //Current piece center
        float cpcX(center.x - ((length * 0.5f) + (pl * 0.5f)));
        float cpcY(center.y + (height * 0.5f) + ph);

        //Belt upper, 2. -> n-1
        unsigned int i(0u);
        while (i < mainBelt.first)
        {
            //Belt n upper
            auto wnu = belt->createChild("");
            wnu->setPosition(cpcX, cpcY, center.z);
            wnu->createComponent<jop::RigidBody2D>(getWorld<2>(), pieceInfo);
            cpcX += pl + pa + mainBelt.second;
            ++i;
        }

        cpcY -= (cpcY - center.y) * 2.f;

        //Belt lower, n-1 -> 2.
        while (i < mainBelt.first * 2u)
        {
            //Belt n lower
            auto wnl = belt->createChild("");
            wnl->setPosition(cpcX, cpcY, center.z);
            wnl->createComponent<jop::RigidBody2D>(getWorld<2>(), pieceInfo);
            cpcX -= pl + pa + mainBelt.second;
            ++i;
        }

    }

    void linkBelt()
    {
        auto& itr = m_system->findChild("belt")->getChildren();

        for (unsigned int i = 0u; i < itr.size() - 1; ++i)
        {
            itr[i].getComponent<jop::RigidBody2D>()->link<jop::RopeJoint2D>(*itr[i + 1].getComponent<jop::RigidBody2D>(), true, glm::vec2(0.9f, 0.f), glm::vec2(-0.9f, 0.f));
        }
        itr.back().getComponent<jop::RigidBody2D>()->link<jop::RopeJoint2D>(*itr.begin()->getComponent<jop::RigidBody2D>(), true, glm::vec2(0.9f, 0.f), glm::vec2(-0.9f, 0.f));
    }

    void createConveyorBelt(const glm::vec3 center, const float force, const float length, const float height)
    {
        JOP_ASSERT((length / height > 1.f) && (length / height < 20.f), "Bad belt dimensions!");

        createWheels(center, force, length, height);
        createBelt(center, force, length, height);
        linkBelt();

        std::vector<glm::vec2> sdf({ glm::vec2(-1.f, 6.f), glm::vec2(11.f, 6.f) });

        jop::RigidBody2D::ConstructInfo2D asdf(jop::ResourceManager::getNamed<jop::TerrainShape2D>("gr", sdf));
      //  m_system->createChild("gr")->createComponent<jop::RigidBody2D>(getWorld<2>(), asdf);

    }

    jop::WeakReference<jop::Object> m_cam;
    jop::WeakReference<jop::Object> m_system;

public:
    CONVEYORSCENE() :
        jop::Scene("CONVEYORBELT"),
        m_cam(createChild("cam")),
        m_system(createChild("system"))
    {
        getWorld<2>().setDebugMode(true);

        glm::vec3 beltCenter(5.f, 10.f, 0.f);
        float beltForce(20.f);
        float beltLength(12.f);
        float beltHeight(6.f);

        m_cam->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);
        m_cam->setPosition(beltCenter.x, beltCenter.y, beltCenter.z + beltLength);

        createConveyorBelt(beltCenter, beltForce, beltLength, beltHeight);



    }

};

#endif