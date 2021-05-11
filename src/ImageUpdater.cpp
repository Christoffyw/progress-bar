#include "main.hpp"
#include "hooks.hpp"
#include "PluginConfig.hpp"
using namespace TestMod;

#include "ImageUpdater.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "GlobalNamespace/PracticeSettings.hpp"
#include "GlobalNamespace/NoteCutInfo.hpp"
using namespace GlobalNamespace;
#include <iostream>
#include <string>
using namespace std;
using namespace UnityEngine;
using namespace TMPro;

#include "System/Collections/IEnumerator.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "GlobalNamespace/GameEnergyUIPanel.hpp"
#include "GlobalNamespace/GameEnergyCounter.hpp"
#include "GlobalNamespace/CoreGameHUDController.hpp"

bool imageinLevel = false;
UnityEngine::Color imageColor;
UnityEngine::GameObject* imagePanel;
UnityEngine::GameObject* imageUI;

custom_types::Helpers::Coroutine imagecoroutine() {
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(1)));
    imageinLevel = true;
    if (getPluginConfig().ColoredUI.GetValue()) {
        imageColor.r = 0.715;
        imageColor.g = 1;
        imageColor.b = 0;
        imageColor.a = 1;
    }
    else {
        imageColor.r = 1;
        imageColor.g = 1;
        imageColor.b = 1;
        imageColor.a = 1;
    }
    co_return;
}

DEFINE_TYPE(TestMod::ImageUpdater);

MAKE_HOOK_OFFSETLESS(ImageSongStart, void,
        Il2CppObject* self,
        Il2CppString* gameMode,
        Il2CppObject* difficultyBeatmap,
        Il2CppObject* previewBeatmapLevel,
        Il2CppObject* overrideEnvironmentSettings,
        Il2CppObject* overrideColorScheme,
        Il2CppObject* gameplayModifiers,
        Il2CppObject* playerSpecificSettings,
        PracticeSettings* practiceSettings,
        Il2CppString* backButtonText,
        bool useTestNoteCutSoundEffects) {
    getLogger().info("Song Started");
    
    GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(imagecoroutine())));

    ImageSongStart(self, gameMode, difficultyBeatmap, previewBeatmapLevel, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects);
}

MAKE_HOOK_OFFSETLESS(ImageSongEnd, void, Il2CppObject* self) {
    getLogger().info("Song Ended");

    imageinLevel = false;

    ImageSongEnd(self);
}

int imagehealth;

void ImageSetColor(float energy) {
    imagehealth = energy * 100;
    if (getPluginConfig().ColoredUI.GetValue()) {
        if (energy < 0.3) {
            imageColor.r = 1;
            imageColor.g = 0;
            imageColor.b = 0;
            imageColor.a = 1;
        } else if (energy < 0.5) {
            imageColor.r = 1;
            imageColor.g = 0.8;
            imageColor.b = 0;
            imageColor.a = 1;
        } else if (energy > 0.5 && energy < 0.7) {
            imageColor.r = 0.715;
            imageColor.g = 1;
            imageColor.b = 0;
            imageColor.a = 1;
        } else if (energy > 0.7) {
            imageColor.r = 0;
            imageColor.g = 1;
            imageColor.b = 0;
            imageColor.a = 1;
        }
    }
}

MAKE_HOOK_OFFSETLESS(ImageGameEnergyUIPanel_HandleGameEnergyDidChange, void, GameEnergyUIPanel* self, float energy) {
    
    
    ImageGameEnergyUIPanel_HandleGameEnergyDidChange(self, energy);
    getLogger().info(std::to_string(energy));
    ImageSetColor(energy);
}

MAKE_HOOK_OFFSETLESS(ImageCoreGameHUDController_Start, void, GlobalNamespace::CoreGameHUDController* self) {
    auto imagerightPanel = UnityEngine::GameObject::Find(il2cpp_utils::newcsstr("RightPanel"));
    if(imagerightPanel != nullptr) {
        imagePanel = imagerightPanel;
        if(imagePanel != nullptr) {
            UnityEngine::Transform* imageTransform = imageUI->get_transform();
            imageTransform->set_position(UnityEngine::Vector3(imagePanel->get_transform()->get_position().x, imagePanel->get_transform()->get_position().y + 0.4, imagePanel->get_transform()->get_position().z));
        }
    }

    ImageCoreGameHUDController_Start(self);
}

custom_types::Helpers::Coroutine imageAnimation() {
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.05,1.05,1.05));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.1,1.1,1.1));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.15,1.15,1.15));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.2,1.2,1.2));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.25,1.25,1.25));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.3,1.3,1.3));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.25,1.25,1.25));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.2,1.2,1.2));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.15,1.15,1.15));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.1,1.1,1.1));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1.05,1.05,1.05));;
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.01)));
    imageUI->get_transform()->set_localScale(UnityEngine::Vector3(1,1,1));
    co_return;
}


MAKE_HOOK_OFFSETLESS(NoteController_SendNoteWasCutEvent, void, NoteController* self, NoteCutInfo* noteCutInfo) {
    
    if(noteCutInfo->get_allIsOK()) {
        if (getPluginConfig().AnimatedHeart.GetValue()) {
            GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(imageAnimation())));
        }
    }

    NoteController_SendNoteWasCutEvent(self, noteCutInfo);
}


void TestMod::ImageUpdater::Update() {
    auto uiImage = get_gameObject()->GetComponent<UnityEngine::UI::Image*>();
    if(imageinLevel) {
        imageUI = get_gameObject();
        imageColor.a = 1;
    }
    else {
        imageColor.a = 0;
    }

    uiImage->get_material();
    auto material = uiImage->get_material();
    material->set_color(imageColor);
}

void TestMod::InstallImageHooks() {
    INSTALL_HOOK_OFFSETLESS(getLogger(), ImageSongStart, il2cpp_utils::FindMethodUnsafe("", "StandardLevelScenesTransitionSetupDataSO", "Init", 10));
    INSTALL_HOOK_OFFSETLESS(getLogger(), ImageSongEnd, il2cpp_utils::FindMethodUnsafe("", "StandardLevelGameplayManager", "OnDestroy", 0));
    INSTALL_HOOK_OFFSETLESS(getLogger(), ImageGameEnergyUIPanel_HandleGameEnergyDidChange, il2cpp_utils::FindMethodUnsafe("", "GameEnergyUIPanel", "HandleGameEnergyDidChange", 1));
    INSTALL_HOOK_OFFSETLESS(getLogger(), ImageCoreGameHUDController_Start, il2cpp_utils::FindMethodUnsafe("", "CoreGameHUDController", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(getLogger(), NoteController_SendNoteWasCutEvent, il2cpp_utils::FindMethodUnsafe("", "NoteController", "SendNoteWasCutEvent", 1));    
}