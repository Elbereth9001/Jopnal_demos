#ifndef CONSTS_HPP
#define CONSTS_HPP

#include <Jopnal/Jopnal.hpp>

//Consts

////////////

static const float shootCD(0.1f);

const jop::Model* c_bulletModel;
const jop::Model* c_playerModel;
const jop::Model* c_enemyLightModel;
const jop::Model* c_enemyHeavyModel;
const jop::Model* c_backgroundModel;

const jop::AnimationAtlas* c_playerAnim;
const jop::AnimationAtlas* c_enemyLightAnim;
const jop::AnimationAtlas* c_enemyHeavyAnim;

const jop::RigidBody2D::ConstructInfo2D* c_bulletInfo;
const jop::RigidBody2D::ConstructInfo2D* c_playerInfo;
const jop::RigidBody2D::ConstructInfo2D* c_enemyLightInfo;
const jop::RigidBody2D::ConstructInfo2D* c_enemyHeavyInfo;

/////////////

//Saved stats

/////////////

unsigned int score(0u);
unsigned int difficulty(0u);

#endif