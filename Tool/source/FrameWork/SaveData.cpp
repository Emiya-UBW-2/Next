#include "SaveData.hpp"
#include <filesystem>
#include <fstream>

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
	//コンフィグ
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
			int Point = static_cast<int>(strchrDx(line, '=') - line);
			if (Point < 0) { continue; }
			strpcpyDx(right, line, Point + 1);

			if (strncmpDx(line, ConfigStr[0], sizeof(ConfigStr[0])) == 0) {
				for (int loop = 0; loop < (sizeof(WindowSettingStr)/sizeof(WindowSettingStr[0])); ++loop) {
					if (strcmpDx(right, WindowSettingStr[loop]) == 0) {
						m_WindowSetting = static_cast<WindowSetting>(loop);
						break;
					}
				}
			}
			if (strncmpDx(line, ConfigStr[1], sizeof(ConfigStr[1])) == 0) {
				m_BGMVol = static_cast<int>(Mathf::Clamp(static_cast<float>(std::atoi(right)), 0.f, 100.f));
			}
			if (strncmpDx(line, ConfigStr[2], sizeof(ConfigStr[2])) == 0) {
				m_SEVol = static_cast<int>(Mathf::Clamp(static_cast<float>(std::atoi(right)), 0.f, 100.f));
			}
			if (strncmpDx(line, ConfigStr[3], sizeof(ConfigStr[3])) == 0) {
				m_IsMuteBGMVol = (strcmpDx(right, "TRUE") == 0);
			}
			if (strncmpDx(line, ConfigStr[4], sizeof(ConfigStr[4])) == 0) {
				m_IsMuteSEVol = (strcmpDx(right, "TRUE") == 0);
			}
		}
		ConfigIni.close();
	}
	else {
		//コンフィグの新規作成
	}
	//タスクデータ
	strcpyDx(ConfigPath, m_SavePath);
	strcatDx(ConfigPath, "config.ini");
	if (std::filesystem::is_regular_file(ConfigPath)) {
		//タスクデータ読み込み
		std::ifstream ConfigIni{};
		ConfigIni.open(ConfigPath);
		if (!ConfigIni.is_open()) {
			return;
		}
		char line[260]{};
		char right[260]{};
		while (!ConfigIni.eof()) {
			ConfigIni.getline(line, sizeof(line));
			int Point = static_cast<int>(strchrDx(line, '=') - line);
			if (Point < 0) { continue; }
			strpcpyDx(right, line, Point + 1);

			if (strncmpDx(line, ConfigStr[1], sizeof(ConfigStr[1])) == 0) {
				m_BGMVol = static_cast<int>(Mathf::Clamp(static_cast<float>(std::atoi(right)), 0.f, 100.f));
			}
		}
		ConfigIni.close();
	}
	else {
		//タスクデータの新規作成
	}
}

void SaveData::Save() {
	char ConfigPath[260] = {};
	strcpyDx(ConfigPath, m_SavePath);
	strcatDx(ConfigPath, "config.ini");

	std::ofstream ConfigIni;
	ConfigIni.open(ConfigPath);

	ConfigIni << ConfigStr[0] << "=" << GetWindowSettingStr() << std::endl;
	ConfigIni << ConfigStr[1] << "=" << std::to_string(m_BGMVol) << std::endl;
	ConfigIni << ConfigStr[2] << "=" << std::to_string(m_SEVol) << std::endl;
	ConfigIni << ConfigStr[3] << "=" << (m_IsMuteBGMVol ? "TRUE" : "FALSE") << std::endl;
	ConfigIni << ConfigStr[4] << "=" << (m_IsMuteSEVol ? "TRUE" : "FALSE") << std::endl;

	ConfigIni.close();
}
