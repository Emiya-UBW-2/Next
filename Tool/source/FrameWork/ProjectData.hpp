#pragma once

#include "Define.hpp"

static const char* ProjectSettingStr[] = {
	"CompanyName",
	"GameName",
};

class ProjectData : public SingletonBase<ProjectData, "ProjectData"> {
private:
	friend class SingletonBase<ProjectData, "ProjectData">;
private:
	ProjectData() {
		Load();
	}
	virtual ~ProjectData() {
	}
private:
	ProjectData(const ProjectData&) = delete;
	ProjectData& operator=(const ProjectData&) = delete;
	ProjectData(ProjectData&&) = delete;
	ProjectData& operator=(ProjectData&&) = delete;
private:
	char m_CompanyName[260] = {};
	char m_GameName[260] = {};
public:
	const char* GetCompanyPath() const { return m_CompanyName; }
	const char* GetGameName() const { return m_GameName; }
public:
	void Load();
};

