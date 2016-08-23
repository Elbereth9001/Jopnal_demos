#include <Jopnal/Jopnal.hpp>
#include "Master.hpp"

void getResources()
{
    using namespace jop;
    using r = jop::ResourceManager;
    using b = jop::RigidBody2D;

    static const b::ConstructInfo2D bulletInfo(r::getNamed<ConeShape2D>("bullet", 0.2f, 0.6f), b::Type::Kinematic);
    c_bulletInfo = &bulletInfo;

    static const b::ConstructInfo2D playerInfo(r::getNamed<ConeShape2D>("player", 0.65f, 1.5f), b::Type::Kinematic);
    c_playerInfo = &playerInfo;

    static const b::ConstructInfo2D enemyLightInfo(r::getNamed<ConeShape2D>("enemyLight", 0.5f, 1.2f), b::Type::Kinematic);
    c_enemyLightInfo = &enemyLightInfo;

    static const b::ConstructInfo2D enemyHeavyInfo(r::getNamed<ConeShape2D>("enemyHeavy", 0.8f, 1.8f), b::Type::Kinematic);
    c_enemyHeavyInfo = &enemyHeavyInfo;

    static const Model bulletmodel(
        r::getNamed<RectangleMesh>("bulletMesh", glm::vec2(0.35f, 0.75f)),
        r::getEmpty<Material>("bulletMat", true).setMap(Material::Map::Diffuse, r::get<Texture2D>("Invaders/bullet.png")));
    c_bulletModel = &bulletmodel;

    static const Model playermodel(
        r::getNamed<RectangleMesh>("playerMesh", glm::vec2(0.65f, 1.5f)),
        r::getEmpty<Material>("playerMat", true).setMap(Material::Map::Diffuse, r::get<Texture2D>("Invaders/player.png")));
    c_playerModel = &playermodel;

    static const Model enemyLightModel(
        r::getNamed<RectangleMesh>("enemyLightMesh", glm::vec2(0.5f, 1.2f)),
        r::getEmpty<Material>("enemyLightMat", true).setMap(Material::Map::Diffuse, r::get<Texture2D>("Invaders/enemyLight.png")));
    c_enemyLightModel = &enemyLightModel;

    static const Model enemyHeavyModel(
        r::getNamed<RectangleMesh>("enemyHeavyMesh", glm::vec2(0.8, 1.8f)),
        r::getEmpty<Material>("enemyHeavyMat", true).setMap(Material::Map::Diffuse, r::get<Texture2D>("Invaders/enemyHeavy.png")));
    c_enemyHeavyModel = &enemyHeavyModel;

    static const Model background(
        r::getNamed<RectangleMesh>("bgMesh", glm::vec2(40.f, 30.f)),
        r::getEmpty<Material>("bgMat", true).setMap(Material::Map::Diffuse, r::get<Texture2D>("Invaders/bg.png")));
    c_backgroundModel = &background;

}



int main(int argc, char* argv[])
{
    JOP_ENGINE_INIT("MyProject", argc, argv);

    getResources();

    jop::Engine::createScene<Master>();

    return JOP_MAIN_LOOP;
}