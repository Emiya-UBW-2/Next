#pragma once

#include "DxLib.h"

#include "Singleton.hpp"

#include "Algorithm.hpp"

#include "FrameWork.hpp"
#include "InputControl.hpp"

#include "GraphControl.hpp"
#include "FontControl.hpp"
#include "SoundControl.hpp"
#include "FadeControl.hpp"

#include "Scene/SceneController.hpp"

#include "SaveData.hpp"

namespace ColorPalette {
	inline unsigned int Black = GetColor(0, 0, 0);
	inline unsigned int Gray085 = GetColor(32, 32, 32);
	inline unsigned int Gray025 = GetColor(192, 192, 192);
	inline unsigned int White = GetColor(255, 255, 255);

	inline unsigned int Red = GetColor(255, 0, 0);
	inline unsigned int Red075 = GetColor(192, 0, 0);
}

