#include "GreenDemon.h"

#include "Library/LiveActor/LiveActorUtil.h"

#include "Scene/StageScene.h"
#include "al/Library/LiveActor/ActorInitInfo.h"
#include "al/Library/LiveActor/ActorClippingFunction.h"
#include "al/Library/LiveActor/ActorInitInfo.h"
#include "al/Library/LiveActor/ActorClippingFunction.h"
#include "al/Library/LiveActor/ActorMovementFunction.h"
#include "al/Library/LiveActor/ActorPoseKeeper.h"
#include "al/Library/LiveActor/ActorFlagFunction.h"
#include "al/Library/LiveActor/LiveActorFlag.h"

#include "al/Library/Nerve/NerveUtil.h"

#include "al/Library/Demo/DemoFunction.h"
#include "al/Library/Math/MathLengthUtil.h"

#include "RedStar/Player/PlayerActorHakoniwa.h"
#include "RedStar/Scene/StageScene.h"
#include "RedStar/Player/PlayerAnimator.h"
#include "RedStar/Player/PlayerDamageKeeper.h"

#include "al/include/Library/Nerve/NerveUtil.h"
#include "math/seadVectorFwd.h"

#include "global/generic.h"
#include "global/demon.h"

namespace ca {

    NERVE_IMPL(GreenDemon, WaitForScene);
    NERVE_IMPL(GreenDemon, Wait);
    NERVE_IMPL(GreenDemon, WaitPlayerMove);
    NERVE_IMPL(GreenDemon, WaitInDemo);
    NERVE_IMPL(GreenDemon, Chase);
    NERVE_IMPL(GreenDemon, Collected);

    NERVE_MAKE(GreenDemon, WaitForScene);
    NERVE_MAKE(GreenDemon, Wait);
    NERVE_MAKE(GreenDemon, WaitPlayerMove);
    NERVE_MAKE(GreenDemon, WaitInDemo);
    NERVE_MAKE(GreenDemon, Chase);
    NERVE_MAKE(GreenDemon, Collected);
}

namespace ca {

    GreenDemon::GreenDemon(const char* name) : al::LiveActor(name) { }

    void GreenDemon::init(const al::ActorInitInfo& info) {

        al::initActorWithArchiveName(this, info, "GreenDemon", 0);

        al::initNerve(this, &WaitForScene, 0);

        makeActorAlive();

        al::invalidateClipping(this);
    }

    bool GreenDemon::receiveMsg(const al::SensorMsg *message, al::HitSensor *source, al::HitSensor *target) {

        return false;
    }

    void GreenDemon::attackSensor(al::HitSensor *target, al::HitSensor *source) { return; }

    void GreenDemon::control(void) {

        if (global::stageScene() != nullptr) {
            if (al::isActiveDemo(global::stageScene()) && !al::isNerve(this, &WaitInDemo)) {
                al::setNerve(this, &WaitInDemo);
            }
        }

        // returning debug values
        //demon::isInWater() = al::isInWaterArea(this);

        if (global::stageScene() != nullptr) {
            global::demon::distanceToPlayer() = al::calcDistance(this, global::playerActorHakoniwa());
        }
    }

    void GreenDemon::exeWaitForScene() {
        LOG_NERVE();
        
        if (al::isFirstStep(this)) {
            al::setTrans(this, sead::Vector3f::zero);
        }
        if (global::stageScene() != nullptr) {
            al::setNerve(this, &WaitPlayerMove);
        }
    }
    void GreenDemon::exeWait() {
        LOG_NERVE();

        if (al::isFirstStep(this)) {
            al::onCollide(this);
        }

        // TODO
        // add addition check for mario being in a cutscene(jumping out of a pipe)
        if (al::isGreaterEqualStep(this, 120)) {
            al::setNerve(this, &Chase);
        }
    }
    void GreenDemon::exeWaitPlayerMove() {
        LOG_NERVE();
        
        PlayerActorHakoniwa* player = global::playerActorHakoniwa();

        if (al::isFirstStep(this)) {
            mPlayerStartPos = al::getTrans(player);
            al::offCollide(this);
            al::setTrans(this, sead::Vector3f(mPlayerStartPos.x, mPlayerStartPos.y + 500.f, mPlayerStartPos.z));
        }
        else if ((al::getTrans(player) - mPlayerStartPos).length() > 10.f) { // SeadUtils::calcDistance(mPlayerStartPos, al::getTrans(player))
            al::setNerve(this, &Wait);
        }
    }
    void GreenDemon::exeWaitInDemo() {
        LOG_NERVE();

        if (al::isFirstStep(this)) {
            al::setVelocityZero(this);
        }

        if (!al::isActiveDemo(global::stageScene())) {
            al::setNerve(this, &WaitForScene);
        }
    }
    void GreenDemon::exeChase() {
        LOG_NERVE();

        PlayerActorHakoniwa* player = global::playerActorHakoniwa();

        sead::Vector3f playerPos = al::getTrans(player);

        sead::Vector3f greenDemonPos = al::getTrans(this);

        sead::Vector3f marioHeightCenter = sead::Vector3f(playerPos.x, playerPos.y + 77.5, playerPos.z);

        // calculates the directionalVector
        sead::Vector3f directionalVector = marioHeightCenter - greenDemonPos;
        al::normalize(&directionalVector);

        // calculates the speed
        float speed = 15.f;
        float distanceToMarioWithOffset = al::calcDistance(this, player) - 500.f;
        if (distanceToMarioWithOffset >= 0.f) {
            speed += sqrt(distanceToMarioWithOffset) * 0.1;
        }
        else {
            speed -= sqrt(sqrt(pow(distanceToMarioWithOffset, 2))) * 0.1;
        }

        if (al::isInWater(this)) {
            speed *= 0.2;
        }

        if (al::calcDistance(this, player) > global::demon::clippingDistance()) {
            if (!this->mFlags->isCollideOff) {
                al::offCollide(this);
            }
        }
        else {
            if (this->mFlags->isCollideOff) {
                al::onCollide(this);
            }
        }

        if (!global::demon::isCheat()) {
            // faces and moves towards mario with the calculated direction and speed
            al::setVelocityToDirection(this, directionalVector, speed);
            al::setFront(this, directionalVector);

            // al::faceToDirectionSupportUp(this, directionalVector);
            // mPoseKeeper->updatePoseRotate(sead::Vector3f(0.f, rotationVector.y, 0.f));

            // if mario got da mush, then u die lolololololololol
            if (al::calcDistance(this, marioHeightCenter) < 100.f && !global::demon::isCheat()) {
                al::setNerve(this, &Collected);
            }
        }
        else {
            al::setVelocityZero(this);
        }
    }
    void GreenDemon::exeCollected() {
        LOG_NERVE();

        PlayerActorHakoniwa* player = global::playerActorHakoniwa();

        if (al::isFirstStep(this)) {
            al::setVelocity(this, sead::Vector3f::zero);
            player->startDemoPuppetable();
            player->mPlayerAnimator->startAnimDead();
            player->mPlayerDamageKeeper->dead();
            global::demon::deathCount()++;

            mShrinkSpeed = -0.05f;

            //player->kill();
            //GameDataFunction::killPlayer(*getStageScene()->mHolder);
        }

        if ((al::getScaleX(this) - mShrinkSpeed) < 0.f) {
            al::setScaleAll(this, 0.f);

            GreenDemon::kill();
        }
        else {
            al::setScaleAll(this, al::getScaleX(this) - mShrinkSpeed);

            mShrinkSpeed += 0.01f;
        }
    }
    LOG_NERVE_Kill(GreenDemon)
}