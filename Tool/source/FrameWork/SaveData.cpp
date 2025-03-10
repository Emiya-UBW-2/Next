#include "SaveData.hpp"
#include <filesystem>
#include <fstream>
#include "../Scene/TaskTab.hpp"

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
			if (strncmpDx(line, ConfigStr[0], strlenDx(ConfigStr[0])) == 0) {
				for (int loop = 0; loop < (sizeof(WindowSettingStr)/sizeof(WindowSettingStr[0])); ++loop) {
					if (strcmpDx(right, WindowSettingStr[loop]) == 0) {
						m_WindowSetting = static_cast<WindowSetting>(loop);
						break;
					}
				}
			}
			if (strncmpDx(line, ConfigStr[1], strlenDx(ConfigStr[1])) == 0) {
				m_BGMVol = static_cast<int>(Mathf::Clamp(static_cast<float>(std::atoi(right)), 0.f, 100.f));
			}
			if (strncmpDx(line, ConfigStr[2], strlenDx(ConfigStr[2])) == 0) {
				m_SEVol = static_cast<int>(Mathf::Clamp(static_cast<float>(std::atoi(right)), 0.f, 100.f));
			}
			if (strncmpDx(line, ConfigStr[3], strlenDx(ConfigStr[3])) == 0) {
				m_IsMuteBGMVol = (strcmpDx(right, "TRUE") == 0);
			}
			if (strncmpDx(line, ConfigStr[4], strlenDx(ConfigStr[4])) == 0) {
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
	strcatDx(ConfigPath, "task.txt");
	if (std::filesystem::is_regular_file(ConfigPath)) {
		m_TabData.clear();
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
			if (strncmpDx(line, TaskStr[0], strlenDx(TaskStr[0])) == 0) {
				m_TabData.emplace_back();
				m_TabData.back().m_TabName = right;
				m_TabData.back().m_Type = 0;
			}
			if (strncmpDx(line, TaskStr[1], strlenDx(TaskStr[1])) == 0) {
				for (int loop = 0; loop < (sizeof(TaskTypeStr) / sizeof(TaskTypeStr[0])); ++loop) {
					if (strcmpDx(right, TaskTypeStr[loop]) == 0) {
						m_TabData.back().m_TaskType = static_cast<TaskType>(loop);
						break;
					}
				}
			}
			if (strncmpDx(line, TaskStr[2], strlenDx(TaskStr[2])) == 0) { m_TabData.back().m_Type += 60 * 60 * (std::atoi(right) % 24); }
			if (strncmpDx(line, TaskStr[3], strlenDx(TaskStr[3])) == 0) { m_TabData.back().m_Type += 60 * (std::atoi(right) % 60); }
			if (strncmpDx(line, TaskStr[4], strlenDx(TaskStr[4])) == 0) { m_TabData.back().m_Type += (std::atoi(right) % 60); }
			if (strncmpDx(line, TaskStr[5], strlenDx(TaskStr[5])) == 0) { m_TabData.back().m_CheckFilePath = right; }
			if (strncmpDx(line, TaskStr[6], strlenDx(TaskStr[6])) == 0) { m_TabData.back().m_CheckKey = right; }
			if (strncmpDx(line, TaskStr[7], strlenDx(TaskStr[7])) == 0) {
				for (int loop = 0; loop < (sizeof(TaskMoveStr) / sizeof(TaskMoveStr[0])); ++loop) {
					if (strcmpDx(right, TaskMoveStr[loop]) == 0) {
						m_TabData.back().m_TaskMove = static_cast<TaskMove>(loop);
						break;
					}
				}
			}
			if (strncmpDx(line, TaskStr[8], strlenDx(TaskStr[8])) == 0) {
				for (int loop = 0; loop < (sizeof(TaskNextStr) / sizeof(TaskNextStr[0])); ++loop) {
					if (strcmpDx(right, TaskNextStr[loop]) == 0) {
						m_TabData.back().m_TaskNext = static_cast<TaskNext>(loop);
						break;
					}
				}
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
	{
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
	{
		strcpyDx(ConfigPath, m_SavePath);
		strcatDx(ConfigPath, "task.txt");

		std::ofstream ConfigIni;
		ConfigIni.open(ConfigPath);
		for (auto& d : m_TabData) {
			ConfigIni << TaskStr[0] << "=" << d.m_TabName << std::endl;
			ConfigIni << TaskStr[1] << "=" << TaskTypeStr[static_cast<int>(d.m_TaskType)] << std::endl;
			ConfigIni << TaskStr[2] << "=" << std::to_string(d.GetHour()) << std::endl;
			ConfigIni << TaskStr[3] << "=" << std::to_string(d.GetMinute()) << std::endl;
			ConfigIni << TaskStr[4] << "=" << std::to_string(d.GetSecond()) << std::endl;
			ConfigIni << TaskStr[5] << "=" << d.m_CheckFilePath << std::endl;
			ConfigIni << TaskStr[6] << "=" << d.m_CheckKey << std::endl;
			ConfigIni << TaskStr[7] << "=" << TaskMoveStr[static_cast<int>(d.m_TaskMove)] << std::endl;
			ConfigIni << TaskStr[8] << "=" << TaskNextStr[static_cast<int>(d.m_TaskNext)] << std::endl;
		}

		ConfigIni.close();
	}
}
