#include "al/Library/Controller/InputFunction.h"
#include "al/Library/Memory/MemorySystem.h"
#include "al/Library/Sequence/Sequence.h"
#include "al/Library/Nerve/Nerve.h"
#include "al/Library/Nerve/NerveKeeper.h"
#include "al/Library/LiveActor/ActorInitInfo.h"
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

static void drawDbgGui()
{
    if (ImGui::Begin("BaseWindow")) {

        ImGui::GetIO().FontGlobalScale = .3;

        ImGui::Text("Hello World!");
    }

    ImGui::End();

    // draw mouse cursor
    ImVec2 mousePos = ImGui::GetMousePos();
    ImGui::RenderMouseCursor(mousePos, 1, ImGui::GetMouseCursor(), ImU32(0xFFFFFFFF), ImU32(0xFF000000), ImU32(0x00000000));
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
