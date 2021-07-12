#include "main.hpp"
#include "hooks.hpp"
#include "PluginConfig.hpp"
#
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
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
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

DEFINE_TYPE(TestMod, ImageUpdater);

MAKE_HOOK_MATCH(ImageSongStart, &AudioTimeSyncController::StartSong, void, AudioTimeSyncController* self, float startTimeOffset) {
    getLogger().info("Song Started");
    
    GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(imagecoroutine())));

    ImageSongStart(self, startTimeOffset);
}

MAKE_HOOK_MATCH(ImageSongEnd, &AudioTimeSyncController::StopSong, void, AudioTimeSyncController* self) {
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

MAKE_HOOK_MATCH(ImageGameEnergyUIPanel_HandleGameEnergyDidChange, &GameEnergyUIPanel::HandleGameEnergyDidChange, void, GameEnergyUIPanel* self, float energy) {
    
    
    ImageGameEnergyUIPanel_HandleGameEnergyDidChange(self, energy);
    ImageSetColor(energy);
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


MAKE_HOOK_MATCH(NoteController_SendNoteWasCutEvent, &NoteController::SendNoteWasCutEvent, void, GlobalNamespace::NoteController* self, GlobalNamespace::NoteCutInfo& noteCutInfo) {
    
    if(noteCutInfo.get_allIsOK()) {
        if (getPluginConfig().AnimatedHeart.GetValue()) {
            GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(imageAnimation())));
        }
    }

    NoteController_SendNoteWasCutEvent(self, noteCutInfo);
}

MAKE_HOOK_MATCH(ImageCoreGameHUDController_Start, &CoreGameHUDController::Start, void, GlobalNamespace::CoreGameHUDController* self) {
	imageUI = nullptr;
    
    ImageCoreGameHUDController_Start(self);
}

void TestMod::ImageUpdater::Update() {
    auto uiImage = get_gameObject()->GetComponent<UnityEngine::UI::Image*>();
    if(imageinLevel) {
		if(!imageUI) {
			imageUI = get_gameObject();
			auto imagePanel = UnityEngine::GameObject::Find(il2cpp_utils::newcsstr("RightPanel"));
			if(imagePanel) {
				UnityEngine::Transform* imageUITransform = imageUI->get_transform();
				UnityEngine::Transform* imagePanelTransform = imagePanel->get_transform();
				if(imageUITransform && imagePanelTransform) {
					auto imagePanelPos = imagePanelTransform->get_position();
					imagePanelPos.y += 0.4f;
					imageUITransform->set_position(imagePanelPos);
				}
			}
		}
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
    INSTALL_HOOK(getLogger(), ImageSongStart);
    INSTALL_HOOK(getLogger(), ImageSongEnd);
    INSTALL_HOOK(getLogger(), ImageGameEnergyUIPanel_HandleGameEnergyDidChange);
    INSTALL_HOOK(getLogger(), ImageCoreGameHUDController_Start);
    INSTALL_HOOK(getLogger(), NoteController_SendNoteWasCutEvent);    
}