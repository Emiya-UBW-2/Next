#include "ProjectData.hpp"
#include <fstream>

const ProjectData* SingletonBase<ProjectData, "ProjectData">::m_Singleton = nullptr;

void ProjectData::Load() {
	std::ifstream SettingIni{};
	SettingIni.open("data/ProjectSetting.ini");
	if (!SettingIni.is_open()) {
		return;
	}
	char line[260]{};
	char right[260]{};
	while (!SettingIni.eof()) {
		SettingIni.getline(line, sizeof(line));
		int Point = static_cast<int>(strchrDx(line, '=') - line);
		if (Point < 0) { continue; }
		strpcpyDx(right, line, Point + 1);
		//
		if (strncmpDx(line, ProjectSettingStr[0], sizeof(ProjectSettingStr[0])) == 0) {
			strcpyDx(m_CompanyName, right);
		}
		if (strncmpDx(line, ProjectSettingStr[1], sizeof(ProjectSettingStr[1])) == 0) {
			strcpyDx(m_GameName, right);
		}
	}
	SettingIni.close();
}
