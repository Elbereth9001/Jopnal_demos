#ifndef CONSTS_HPP
#define CONSTS_HPP

#include <Jopnal/Jopnal.hpp>

//Consts

static const float shootCD(0.1f);

const jop::Model* c_bulletModel;
const jop::Model* c_playerModel;
const jop::Model* c_enemyLightModel;
const jop::Model* c_enemyHeavyModel;
const jop::Model* c_backgroundModel;

/////////////

//Saved stats

unsigned int score(0u);
unsigned int difficulty(0u);

#endif