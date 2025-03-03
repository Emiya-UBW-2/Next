#pragma once

#include "DxLib.h"

#include "Singleton.hpp"

#include "Algorithm.hpp"

#include "FrameWork.hpp"
#include "InputControl.hpp"

#include "GraphControl.hpp"
#include "FontControl.hpp"
#include "FadeControl.hpp"

#include "Scene/SceneController.hpp"

#include "ProjectData.hpp"
#include "SaveData.hpp"

namespace ColorPalette {
	inline unsigned int Black = GetColor(0, 0, 0);
	inline unsigned int Gray085 = GetColor(32, 32, 32);
	inline unsigned int Gray050 = GetColor(128, 128, 128);
	inline unsigned int Gray025 = GetColor(192, 192, 192);
	inline unsigned int Gray010 = GetColor(230, 230, 230);
	inline unsigned int White = GetColor(255, 255, 255);

	inline unsigned int Red = GetColor(255, 0, 0);
	inline unsigned int Red075 = GetColor(192, 0, 0);

	inline unsigned int Green = GetColor(0, 255, 0);
}

