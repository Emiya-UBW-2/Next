#include "SaveData.hpp"
#include <filesystem>
#include <fstream>

bool IsFileExist(const std::string& name) {
	return std::filesystem::is_regular_file(name);
}
const SaveData* SingletonBase<SaveData, "SaveData">::m_Singleton = nullptr;

void SaveData::SetPath() {
	size_t buf;
	getenv_s(&buf, m_SavePath, sizeof(m_SavePath), "APPDATA");
	strcatDx(m_SavePath, "\\..\\LocalLow\\");
	//会社のパス作成
	strcatDx(m_SavePath, ProjectData::Instance()->GetCompanyPath());
	strcatDx(m_SavePath, "\\");
	CreateDirectory(m_SavePath,NULL);
	//ゲームのパス作成
	strcatDx(m_SavePath, ProjectData::Instance()->GetGameName());
	strcatDx(m_SavePath, "\\");
	CreateDirectory(m_SavePath, NULL);
}

void SaveData::Load() {
	char ConfigPath[260] = {};
	strcpyDx(ConfigPath, m_SavePath);
	strcatDx(ConfigPath, "config.ini");
	if (std::filesystem::is_regular_file(ConfigPath)) {
		//コンフィグ読み込み
		std::ifstream ConfigIni{};
		ConfigIni.open(ConfigPath);
		if (!ConfigIni.is_open()) {
			return;
		}
		char line[260]{};
		char right[260]{};
		while (!ConfigIni.eof()) {
			ConfigIni.getline(line, sizeof(line));
			int Point = strchrDx(line, '=') - line;
			if (Point < 0) { continue; }
			strpcpyDx(right, line, Point + 1);

			if (strncmpDx(line, ConfigStr[0], sizeof(ConfigStr[0])) == 0) {
				for (int loop = 0; loop < (sizeof(WindowSettingStr)/sizeof(WindowSettingStr[0])); ++loop) {
					if (strcmpDx(right, WindowSettingStr[loop]) == 0) {
						m_WindowSetting = static_cast<WindowSetting>(loop);
						break;
					}
				}
				strcpyDx(right, line);
			}
		}
		ConfigIni.close();
	}
	else {
		//コンフィグの新規作成
		m_WindowSetting = WindowSetting::Default;
	}
}

void SaveData::Save() {
	char ConfigPath[260] = {};
	strcpyDx(ConfigPath, m_SavePath);
	strcatDx(ConfigPath, "config.ini");

	std::ofstream ConfigIni;
	ConfigIni.open(ConfigPath);

	ConfigIni << ConfigStr[0] << "=" << GetWindowSettingStr() << std::endl;

	ConfigIni.close();
}
