#pragma once

#include "main.hpp"

#include <string>
#include <unordered_map>

//#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(PluginConfig,

CONFIG_VALUE(ColoredUI, bool, "Colored Health Counter", true);
CONFIG_VALUE(AnimatedHeart, bool, "Animated Heart", true);

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(ColoredUI);
        CONFIG_INIT_VALUE(AnimatedHeart);
    )
)