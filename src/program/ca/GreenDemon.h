#pragma once

#include "al/Library/LiveActor/LiveActor.h"
#include "al/include/Library/LiveActor/LiveActor.h"

#include "al/include/Library/Nerve/NerveSetupUtil.h"

#include "CustomActorUtil.h"

namespace ca {

    class GreenDemon : public al::LiveActor {

    public:
        GreenDemon(const char* name);
        void init(const al::ActorInitInfo&) override;
        bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target) override;
        void attackSensor(al::HitSensor* source, al::HitSensor* target) override;
        void control(void) override;
        void kill() override;

        NERVE_FUNC(WaitForScene);
        NERVE_FUNC(Wait);
        NERVE_FUNC(WaitPlayerMove);
        NERVE_FUNC(WaitInDemo);
        NERVE_FUNC(Chase);
        NERVE_FUNC(Collected);

    private:
        LOG_NERVE_Impl();

        float mShrinkSpeed;
        sead::Vector3f mPlayerStartPos;
    };
}