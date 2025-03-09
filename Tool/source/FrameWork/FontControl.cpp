#include "FontControl.hpp"
#include <fstream>

const FontPool* SingletonBase<FontPool, "FontPool">::m_Singleton = nullptr;

const KeyGuide* SingletonBase<KeyGuide, "KeyGuide">::m_Singleton = nullptr;

int KeyGuide::KeyGuideGraph::GetDrawSize(void) const noexcept { return (xsize * 24 / ysize) + 3; }
void KeyGuide::KeyGuideGraph::Draw(int x, int y) const noexcept {
	DrawExtendGraph(x, y, x + (xsize * 24 / ysize), y + 24, GuideImg.GetHandle(), FALSE);
}
// 
KeyGuide::KeyGuide(void) noexcept {
	m_GuideBaseImage.LoadGraph("data/key/OutputFont.png");
	{
		int count = 0;
		std::ifstream FileStream{};
		FileStream.open("data/key/OutputFont.psf");
		char line[260]{};
		char right[260]{};
		while (!FileStream.eof()) {
			FileStream.getline(line, sizeof(line));
			int Point = static_cast<int>(strchrDx(line, '=') - line);
			if (Point < 0) { continue; }
			strpcpyDx(right, line, Point + 1);
			std::string RIGHT = right;
			std::vector<int> Args;
			while (true) {
				auto div = RIGHT.find(",");
				if (div != std::string::npos) {
					Args.emplace_back(std::stoi(RIGHT.substr(0, div)));
					RIGHT = RIGHT.substr(div + 1);
				}
				else {
					Args.emplace_back(std::stoi(RIGHT));
					break;
				}
			}
			//“¾‚½î•ñ‚ğ‚à‚Æ‚É•ªŠ„‚µ‚½‰æ‘œ‚ğ“¾‚é
			m_DerivationGuideImage.emplace_back(std::make_shared<KeyGuideGraph>());
			m_DerivationGuideImage.back()->AddGuide(Args.at(0), Args.at(1), Args.at(2), Args.at(3), m_GuideBaseImage);
			++count;
		}
	}
}
