#include "main.hpp"
#include "hooks.hpp"
#include "PluginConfig.hpp"
using namespace TestMod;

#include "TextUpdater.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Color.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "GlobalNamespace/PracticeSettings.hpp"
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
#include "UnityEngine/CanvasGroup.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "GlobalNamespace/GameEnergyUIPanel.hpp"
#include "GlobalNamespace/GameEnergyCounter.hpp"
#include "GlobalNamespace/CoreGameHUDController.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "UnityEngine/RectTransform.hpp"

bool inLevel = false;
UnityEngine::Color color;
UnityEngine::GameObject* panel;
UnityEngine::GameObject* energyPanel;
UnityEngine::GameObject* energyBar;
UnityEngine::GameObject* TextUI;

int health;
float barValue = 0;

custom_types::Helpers::Coroutine coroutine() {
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(1)));
    inLevel = true;
    health = 50;
    if (getPluginConfig().ColoredUI.GetValue()) {
        color.r = 0.715;
        color.g = 1;
        color.b = 0;
        color.a = 1;
    }
    else {
        color.r = 1;
        color.g = 1;
        color.b = 1;
        color.a = 1;
    }
    co_return;
}

DEFINE_TYPE(TestMod, TextUpdater);

MAKE_HOOK_MATCH(SongStart, &AudioTimeSyncController::StartSong, void, AudioTimeSyncController* self, float startTimeOffset) {
    getLogger().info("Song Started");
    
    GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(coroutine())));

    SongStart(self, startTimeOffset);
}

custom_types::Helpers::Coroutine disableSongProgress() {
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.2)));
    auto songProgressPanel = UnityEngine::GameObject::Find(il2cpp_utils::newcsstr("SongProgressCanvas"));
    if(songProgressPanel != nullptr) {
        songProgressPanel->get_transform()->set_position(UnityEngine::Vector3(energyBar->get_transform()->get_position().x, energyBar->get_transform()->get_position().y - 0.2, energyBar->get_transform()->get_position().z));
    }
    co_return;
}
custom_types::Helpers::Coroutine disableEnergyIcons() {
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.1)));
    auto EnergyIconEmpty = UnityEngine::GameObject::Find(il2cpp_utils::newcsstr("EnergyIconEmpty"));
    auto EnergyIconFull = UnityEngine::GameObject::Find(il2cpp_utils::newcsstr("EnergyIconFull"));
    auto BarBGICON = UnityEngine::GameObject::Find(il2cpp_utils::newcsstr("BarBG"));
    auto EnergyBar = UnityEngine::GameObject::Find(il2cpp_utils::newcsstr("EnergyBarWrapper"));
    if(EnergyBar != nullptr) {
        getLogger().info("WE GOT HERE");
        energyBar = EnergyBar;
        UnityEngine::RectTransform* rectTrans = energyBar->GetComponent<UnityEngine::RectTransform*>();
        
    }  
    if(EnergyIconEmpty != nullptr) {
        EnergyIconEmpty->SetActive(false);
    }
    if(EnergyIconFull != nullptr) {
        EnergyIconFull->SetActive(false);
    }
    if(BarBGICON != nullptr) {
        BarBGICON->SetActive(false);
    }
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(WaitForSeconds::New_ctor(0.2)));
    
    energyPanel->SetActive(false);
    
    
    co_return;
}

MAKE_HOOK_MATCH(CoreGameHUDController_Start, &CoreGameHUDController::Start, void, GlobalNamespace::CoreGameHUDController* self) {
    auto EnergyPanel = UnityEngine::GameObject::Find(il2cpp_utils::newcsstr("EnergyPanel"));

    if(EnergyPanel != nullptr) {
        energyPanel = EnergyPanel;
        if(energyPanel != nullptr) {
            GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(disableEnergyIcons())));
        }
    }
    
    TextUI = nullptr;

    CoreGameHUDController_Start(self);
}

MAKE_HOOK_MATCH(SongEnd, &AudioTimeSyncController::StopSong, void, AudioTimeSyncController* self) {
    getLogger().info("Song Ended");

    inLevel = false;

    SongEnd(self);
}

void SetColor(float energy) {
    health = energy * 100;
    if (getPluginConfig().ColoredUI.GetValue()) {
        if (energy < 0.3) {
            color.r = 1;
            color.g = 0;
            color.b = 0;
            color.a = 1;
        } else if (energy < 0.5) {
            color.r = 1;
            color.g = 0.8;
            color.b = 0;
            color.a = 1;
        } else if (energy > 0.5 && energy < 0.7) {
            color.r = 0.715;
            color.g = 1;
            color.b = 0;
            color.a = 1;
        } else if (energy > 0.7) {
            color.r = 0;
            color.g = 1;
            color.b = 0;
            color.a = 1;
        }
    }
}

MAKE_HOOK_MATCH(GameEnergyUIPanel_HandleGameEnergyDidChange, &GameEnergyUIPanel::HandleGameEnergyDidChange, void, GameEnergyUIPanel* self, float energy) {
    
    
    GameEnergyUIPanel_HandleGameEnergyDidChange(self, energy);
    SetColor(energy);
}


void TestMod::TextUpdater::Update() {
    if(inLevel) {
        if(!TextUI) {
            TextUI = get_gameObject();
            auto rightPanel = UnityEngine::GameObject::Find(il2cpp_utils::newcsstr("RightPanel"));
            if(rightPanel != nullptr) {
                panel = rightPanel;
                if(panel != nullptr) {
                    UnityEngine::Transform* transform = TextUI->get_transform();
                    if(transform != nullptr)
                        transform->set_position(UnityEngine::Vector3(panel->get_transform()->get_position().x + 2.65, panel->get_transform()->get_position().y - 0.4, panel->get_transform()->get_position().z));
                }
            GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(reinterpret_cast<custom_types::Helpers::enumeratorT*>(custom_types::Helpers::CoroutineHelper::New(disableSongProgress())));
            }
        }
        
        auto text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
        text->set_text(il2cpp_utils::newcsstr(std::to_string(health) + " HP"));
        text->set_fontSize(4);
    }
    else {
        auto text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
        text->set_fontSize(0);
    }
    auto text = get_gameObject()->GetComponent<TextMeshProUGUI*>();
    text->set_color(color);
}

void TestMod::InstallHooks() {
    INSTALL_HOOK(getLogger(), SongStart);
    INSTALL_HOOK(getLogger(), SongEnd);
    INSTALL_HOOK(getLogger(), GameEnergyUIPanel_HandleGameEnergyDidChange);
    INSTALL_HOOK(getLogger(), CoreGameHUDController_Start);
}