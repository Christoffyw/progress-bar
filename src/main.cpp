#include "main.hpp"
#include "PluginConfig.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/CanvasScaler.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/CanvasRenderer.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Color.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/CurvedCanvasSettings.hpp"
#include "HMUI/Touchable.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/Monobehaviour.hpp"
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"

#include <iostream>
#include <string>
using namespace std;

#include "TextUpdater.hpp"
#include "ImageUpdater.hpp"
using namespace TestMod;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration configuration(modInfo);
    return configuration;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load();
    getPluginConfig().Init(info);
    getConfig().Reload();
    getConfig().Write();

    getLogger().info("Completed setup!");
}

UnityEngine::Color hideColor;

UnityEngine::Canvas* canvas;
UnityEngine::UI::VerticalLayoutGroup* layout;
UnityEngine::UI::Image* postedImage;
TMPro::TextMeshProUGUI* postedText;

#include "GlobalNamespace/MainMenuViewController.hpp"
MAKE_HOOK_MATCH(MainMenuViewController_DidActivate, &GlobalNamespace::MainMenuViewController::DidActivate, void, GlobalNamespace::MainMenuViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

  if (firstActivation) {
    auto canvas_object = UnityEngine::GameObject::New_ctor(il2cpp_utils::newcsstr("Canvas"));
    canvas = canvas_object->AddComponent<UnityEngine::Canvas*>();
    auto canvas_scaler = canvas_object->AddComponent<UnityEngine::UI::CanvasScaler*>();
    auto canvas_renderer = canvas_object->AddComponent<UnityEngine::CanvasRenderer*>();

    canvas_object->AddComponent<HMUI::CurvedCanvasSettings*>();
    auto curved_canvas = canvas_object->GetComponent<HMUI::CurvedCanvasSettings*>();
    curved_canvas->SetRadius(0);
    canvas_object->get_transform()->set_localScale(UnityEngine::Vector3(0.1, 0.1, 0.1));

    UnityEngine::Object::DontDestroyOnLoad(canvas_object);

    canvas->set_renderMode(UnityEngine::RenderMode::WorldSpace);

    std::string base64Junk = R"(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)";

    layout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(canvas_object->get_transform());
    auto hp_text = QuestUI::BeatSaberUI::CreateText(canvas_object->get_transform(), "HP", UnityEngine::Vector2(0,0));
    auto hp_image = QuestUI::BeatSaberUI::CreateImage(canvas_object->get_transform(), QuestUI::BeatSaberUI::Base64ToSprite(base64Junk, 300, 278), UnityEngine::Vector2(0,2), UnityEngine::Vector2(5,5));
    hp_text->get_transform()->set_position(UnityEngine::Vector3(0, 0, 0));
    hp_image->get_transform()->set_position(UnityEngine::Vector3(0, 0, 0));
    hp_image->get_gameObject()->AddComponent<TestMod::ImageUpdater*>();
    hp_text->get_gameObject()->AddComponent<TestMod::TextUpdater*>();
    hp_text->set_fontSize(0);
    auto text = hp_text->get_gameObject()->GetComponent<TMPro::TextMeshProUGUI*>();
    auto image = hp_image->get_gameObject()->GetComponent<UnityEngine::UI::Image*>();
    text->set_text(il2cpp_utils::newcsstr("0 HP"));
    postedImage = image;
    postedText = text;
  }
  canvas->get_gameObject()->SetActive(true);
}

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    getLogger().info("DidActivate: %p, %d, %d, %d", self, firstActivation, addedToHierarchy, screenSystemEnabling);

    if(firstActivation) {
        self->get_gameObject()->AddComponent<HMUI::Touchable*>();
        UnityEngine::GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());



        auto* textGrid = container;
//        textGrid->set_spacing(1);

        QuestUI::BeatSaberUI::CreateText(textGrid->get_transform(), "Progress Bar Mod settings.");

//        buttonsGrid->set_spacing(1);

        auto* boolGrid = container;

        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(boolGrid->get_transform(), getPluginConfig().ColoredUI)->get_gameObject(),"Makes the mod's UI ingame change color based on your health.");
        QuestUI::BeatSaberUI::AddHoverHint(AddConfigValueToggle(boolGrid->get_transform(), getPluginConfig().AnimatedHeart)->get_gameObject(),"Makes the heart image animated ingame.");
    }
}

extern "C" void load() {
  il2cpp_functions::Init();
  QuestUI::Init();

  custom_types::Register::AutoRegister();

  TestMod::InstallHooks();
  TestMod::InstallImageHooks();
  QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);

  INSTALL_HOOK(getLogger(), MainMenuViewController_DidActivate);
}