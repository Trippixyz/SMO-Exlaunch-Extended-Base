#include "al/Library/Controller/InputFunction.h"
#include "al/Library/Memory/MemorySystem.h"
#include "al/Library/Sequence/Sequence.h"
#include "al/Library/Nerve/Nerve.h"
#include "al/Library/Nerve/NerveKeeper.h"
#include "al/Library/LiveActor/ActorInitInfo.h"
#include "ca/GreenDemon.h"
#include "diag/assert.hpp"
#include "heap/seadExpHeap.h"
#include "heap/seadHeapMgr.h"
#include "helpers/InputHelper.h"
#include "hook/trampoline.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "lib.hpp"
#include "nn/fs.h"
#include "program/imgui_nvn.h"
#include <sead/filedevice/seadFileDeviceMgr.h>
#include "RedStar/Scene/StageScene.h"

#include "global/generic.h"
#include "global/demon.h"

static void drawDbgGui()
{
    if (ImGui::Begin("GreenDemon")) {

        ImGui::GetIO().FontGlobalScale = 1;//.3;

        ImGui::Text("Time in StageScene: %fs", ((float)global::stageSceneTimer()) / 60.f);

        float x, y;
        InputHelper::getMouseCoords(&x, &y);

        //ImGui::Text("Mouse: X: %f | Y: %f", x, y);
        ImGui::Text("Distance To Player: %f", global::demon::distanceToPlayer());
        ImGui::SliderFloat("Clipping Distance", &global::demon::clippingDistance(), 5000.f, 50000.f);

        if (ImGui::Button(global::demon::isCheat() ? "Cheating" : "Not Cheating")) {

            global::demon::isCheat() = !global::demon::isCheat();
        }

        ImGui::Text("Death Counter: %i", global::demon::deathCount());

        if (global::demon::liveActor() != nullptr) {
            if (global::demon::liveActor()->mCurrentNerveName != nullptr) {
                ImGui::Text("Nerve: %s", global::demon::liveActor()->mCurrentNerveName);
            }
            else {
                ImGui::Text("Nerve: None");
            }
        }
    }

    ImGui::End();

    ImVec2 mousePos = ImGui::GetMousePos();

    //ImGui::GetForegroundDrawList()->AddCircleFilled(mousePos, 50.f, ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 0.5f)));

    ImGui::RenderMouseCursor(mousePos, 1, ImGui::GetMouseCursor(), ImU32(0xFFFFFFFF), ImU32(0xFF000000), ImU32(0x00000000));

    //ImGui::GetForegroundDrawList()->AddLine(ImVec2(mousePos.x, 0), ImVec2(mousePos.x, ImGui::GetIO().DisplaySize.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1)), 2.f);
    //ImGui::GetForegroundDrawList()->AddLine(ImVec2(0, mousePos.y), ImVec2(ImGui::GetIO().DisplaySize.x, mousePos.y), ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1)), 2.f);
}

HOOK_DEFINE_TRAMPOLINE(StageSceneControlHook) {
    static void Callback(StageScene* scene) {

        global::stageScene() = scene;
        global::stageSceneTimer()++;

        Orig(scene);
    }
};

HOOK_DEFINE_TRAMPOLINE(InitActorWithArchiveNameHook) {
    static void Callback(al::LiveActor* actor, al::ActorInitInfo const &initInfo, sead::SafeStringBase<char> const &string, char const *anotherString) {

        global::stageScene() = nullptr;
        global::stageSceneTimer() = 0;

        global::demon::liveActor() = new ca::GreenDemon("GreenDemon");
        al::initCreateActorNoPlacementInfo(global::demon::liveActor(), initInfo);

        Orig(actor, initInfo, string, anotherString);
    }
};

extern "C" void exl_main(void* x0, void* x1)
{
    exl::hook::Initialize();

    StageSceneControlHook::InstallAtSymbol("_ZN10StageScene7controlEv");
    InitActorWithArchiveNameHook::InstallAtOffset(0x0041b384);

    nvnImGui::InstallHooks();
    nvnImGui::addDrawFunc(drawDbgGui);
}

extern "C" NORETURN void exl_exception_entry()
{
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}
