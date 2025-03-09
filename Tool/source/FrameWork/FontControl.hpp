#pragma once

#include "Define.hpp"

#include <string>
#include <vector>
#include <functional>
#include <memory>

class FontHandle : public ResourceHandle {
	ResourceHandle::ResourceHandle;
public:
	void Create(const TCHAR* FontName, int Size, int Thick, int FontType = -1, int CharSet = -1, int EdgeSize = -1, bool Italic = false, int Handle = -1) {
		this->SetHandle(CreateFontToHandle(FontName, Size, Thick, FontType, CharSet, EdgeSize, Italic, Handle));
	}
	void ReleaseFont() {
		DeleteFontToHandle(this->GetHandle());
		this->SetInvalid();
	}
};

class FontPool : public SingletonBase<FontPool, "FontPool"> {
private:
	friend class SingletonBase<FontPool, "FontPool">;
private:
	FontPool() {}
	virtual ~FontPool() {
		Dispose();
	}
private:
	FontPool(const FontPool&) = delete;
	FontPool& operator=(const FontPool&) = delete;
	FontPool(FontPool&&) = delete;
	FontPool& operator=(FontPool&&) = delete;
private:
	struct FontOnce {
		FontHandle m_FontHandle{};

		std::string m_FontName{};
		int m_Size = 0;
		int m_Thick = 0;
		int m_FontType = -1;
		int m_CharSet = -1;
		int m_EdgeSize = -1;
		bool m_Italic = false;
		int m_Handle = -1;
	public:
		bool IsSameFont(const TCHAR* FontName, int Size, int Thick, int FontType = -1, int CharSet = -1, int EdgeSize = -1, bool Italic = false, int Handle = -1) const {
			return (m_FontName == FontName) &&
			(m_Size == Size) &&
			(m_Thick == Thick) &&
			(m_FontType == FontType) &&
			(m_CharSet == CharSet) &&
			(m_EdgeSize == EdgeSize) &&
			(m_Italic == Italic) &&
			(m_Handle == Handle);
		}
	};
private:
	std::vector<FontOnce> m_Once;
private:
	void Dispose() {
		for (auto& o : m_Once) {
			o.m_FontHandle.ReleaseFont();
		}
		m_Once.clear();
	}
public:
	void Add(const TCHAR* FontName, int Size, int Thick, int FontType = -1, int CharSet = -1, int EdgeSize = -1, bool Italic = false, int Handle = -1) {
		if (Get(FontName, Size, Thick, FontType, CharSet, EdgeSize, Italic, Handle)) {
			return;
		}
		m_Once.emplace_back();
		m_Once.back().m_FontHandle.Create(FontName, Size, Thick, FontType, CharSet, EdgeSize, Italic, Handle);
		m_Once.back().m_FontName = FontName;
		m_Once.back().m_Size = Size;
		m_Once.back().m_Thick = Thick;
		m_Once.back().m_FontType = FontType;
		m_Once.back().m_CharSet = CharSet;
		m_Once.back().m_EdgeSize = EdgeSize;
		m_Once.back().m_Italic = Italic;
		m_Once.back().m_Handle = Handle;
	}
	FontHandle* Get(const TCHAR* FontName, int Size, int Thick, int FontType = -1, int CharSet = -1, int EdgeSize = -1, bool Italic = false, int Handle = -1) {
		for (auto& o : m_Once) {
			if (o.IsSameFont(FontName, Size, Thick, FontType, CharSet, EdgeSize, Italic, Handle)) {
				return &o.m_FontHandle;
			}
		}
		return nullptr;
	}
};

// --------------------------------------------------------------------------------------------------
// キーガイド
// --------------------------------------------------------------------------------------------------
enum class ControlType :int {
	XBox,
	PS4,
	PC,
	Max,
};
static const char* ControlTypeStr[] = {
	"XBox",
	"PS4",
	"PC",
};

static const size_t KeyNum = 109 + 3;
static const int KeyID[KeyNum] = {
	KEY_INPUT_BACK,
	KEY_INPUT_TAB,
	KEY_INPUT_RETURN,

	KEY_INPUT_LSHIFT,
	KEY_INPUT_RSHIFT,
	KEY_INPUT_LCONTROL,
	KEY_INPUT_RCONTROL,
	KEY_INPUT_ESCAPE,
	KEY_INPUT_SPACE,
	KEY_INPUT_PGUP,
	KEY_INPUT_PGDN,
	KEY_INPUT_END,
	KEY_INPUT_HOME,
	KEY_INPUT_LEFT,
	KEY_INPUT_UP,
	KEY_INPUT_RIGHT,
	KEY_INPUT_DOWN,
	KEY_INPUT_INSERT,
	KEY_INPUT_DELETE,

	KEY_INPUT_MINUS,
	KEY_INPUT_YEN,
	KEY_INPUT_PREVTRACK,
	KEY_INPUT_PERIOD,
	KEY_INPUT_SLASH,
	KEY_INPUT_LALT,
	KEY_INPUT_RALT,
	KEY_INPUT_SCROLL,
	KEY_INPUT_SEMICOLON,
	KEY_INPUT_COLON,
	KEY_INPUT_LBRACKET,
	KEY_INPUT_RBRACKET,
	KEY_INPUT_AT,
	KEY_INPUT_BACKSLASH,
	KEY_INPUT_COMMA,
	KEY_INPUT_KANJI,
	KEY_INPUT_CONVERT,
	KEY_INPUT_NOCONVERT,
	KEY_INPUT_KANA,
	KEY_INPUT_APPS,
	KEY_INPUT_CAPSLOCK,
	KEY_INPUT_SYSRQ,
	KEY_INPUT_PAUSE,
	KEY_INPUT_LWIN,
	KEY_INPUT_RWIN,

	KEY_INPUT_NUMLOCK,
	KEY_INPUT_NUMPAD0,
	KEY_INPUT_NUMPAD1,
	KEY_INPUT_NUMPAD2,
	KEY_INPUT_NUMPAD3,
	KEY_INPUT_NUMPAD4,
	KEY_INPUT_NUMPAD5,
	KEY_INPUT_NUMPAD6,
	KEY_INPUT_NUMPAD7,
	KEY_INPUT_NUMPAD8,
	KEY_INPUT_NUMPAD9,
	KEY_INPUT_MULTIPLY,
	KEY_INPUT_ADD,
	KEY_INPUT_SUBTRACT,
	KEY_INPUT_DECIMAL,
	KEY_INPUT_DIVIDE,
	KEY_INPUT_NUMPADENTER,

	KEY_INPUT_F1,
	KEY_INPUT_F2,
	KEY_INPUT_F3,
	KEY_INPUT_F4,
	KEY_INPUT_F5,
	KEY_INPUT_F6,
	KEY_INPUT_F7,
	KEY_INPUT_F8,
	KEY_INPUT_F9,
	KEY_INPUT_F10,
	KEY_INPUT_F11,
	KEY_INPUT_F12,

	KEY_INPUT_A,
	KEY_INPUT_B,
	KEY_INPUT_C,
	KEY_INPUT_D,
	KEY_INPUT_E,
	KEY_INPUT_F,
	KEY_INPUT_G,
	KEY_INPUT_H,
	KEY_INPUT_I,
	KEY_INPUT_J,
	KEY_INPUT_K,
	KEY_INPUT_L,
	KEY_INPUT_M,
	KEY_INPUT_N,
	KEY_INPUT_O,
	KEY_INPUT_P,
	KEY_INPUT_Q,
	KEY_INPUT_R,
	KEY_INPUT_S,
	KEY_INPUT_T,
	KEY_INPUT_U,
	KEY_INPUT_V,
	KEY_INPUT_W,
	KEY_INPUT_X,
	KEY_INPUT_Y,
	KEY_INPUT_Z,

	KEY_INPUT_0 ,
	KEY_INPUT_1,
	KEY_INPUT_2,
	KEY_INPUT_3,
	KEY_INPUT_4,
	KEY_INPUT_5,
	KEY_INPUT_6,
	KEY_INPUT_7,
	KEY_INPUT_8,
	KEY_INPUT_9,

	MOUSE_INPUT_LEFT | 0xF00,
	MOUSE_INPUT_RIGHT | 0xF00,
	MOUSE_INPUT_MIDDLE | 0xF00,
};
static const char* KeyStr[KeyNum] = {
	"BACK",
	"TAB",
	"RETURN",

	"LSHIFT",
	"RSHIFT",
	"LCONTROL",
	"RCONTROL",
	"ESCAPE",
	"SPACE",
	"PGUP",
	"PGDN",
	"END",
	"HOME",
	"LEFT",
	"UP",
	"RIGHT",
	"DOWN",
	"INSERT",
	"DELETE",

	"MINUS",
	"YEN",
	"PREVTRACK",
	"PERIOD",
	"SLASH",
	"LALT",
	"RALT",
	"SCROLL",
	"SEMICOLON",
	"COLON",
	"LBRACKET",
	"RBRACKET",
	"AT",
	"BACKSLASH",
	"COMMA",
	"KANJI",
	"CONVERT",
	"NOCONVERT",
	"KANA",
	"APPS",
	"CAPSLOCK",
	"SYSRQ",
	"PAUSE",
	"LWIN",
	"RWIN",

	"NUMLOCK",
	"NUMPAD0",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"MULTIPLY",
	"ADD",
	"SUBTRACT",
	"DECIMAL",
	"DIVIDE",
	"NUMPADENTER",

	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",

	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",

	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",

	"LMOUSE",
	"RMOUSE",
	"WMOUSE",
};

static const size_t DS4Num = 12 - 1 + 2 + 4 + 1;
static const int DS4ID[DS4Num] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	9,
	10,
	11,
	0xF001,
	0xF002,
	0xF010,
	0xF020,
	0xF040,
	0xF080,
	8,
};
static const char* DS4Str[DS4Num] = {
	"SQUARE",
	"NG",
	"OK",
	"TRIANGLE",
	"L1",
	"R1",
	"L2",
	"R2",
	"OPTION",
	"L3",
	"R3",
	"LSTICK",
	"RSTICK",
	"LEFT",
	"UP",
	"RIGHT",
	"DOWN",
	"ESCAPE",
};

static const size_t XBoxNum = 12 - 1 + 2 + 4 + 1;
static const int XBoxID[XBoxNum] = {
	14,
	12,
	13,
	15,

	8,
	9,

	0xF100,
	0xF200,

	4,

	6,
	7,

	0xF001,
	0xF002,
	2,
	0,
	3,
	1,

	5,
};
static const char* XBoxStr[XBoxNum] = {
	"SQUARE",
	"NG",
	"OK",
	"TRIANGLE",

	"L1",
	"R1",

	"L2",
	"R2",

	"OPTION",

	"L3",
	"R3",

	"LSTICK",
	"RSTICK",
	"LEFT",
	"UP",
	"RIGHT",
	"DOWN",

	"ESCAPE",
};

class KeyGuide : public SingletonBase<KeyGuide, "KeyGuide"> {
private:
	friend class SingletonBase<KeyGuide, "KeyGuide">;
private:
	// コンストラクタ
	KeyGuide(void) noexcept;
	// コピーしてはいけないので通常のコンストラクタ以外をすべてdelete
	KeyGuide(const KeyGuide&) = delete;
	KeyGuide(KeyGuide&& o) = delete;
	KeyGuide& operator=(const KeyGuide&) = delete;
	KeyGuide& operator=(KeyGuide&& o) = delete;
	// デストラクタはシングルトンなので呼ばれません
private:
	class KeyGuideGraph {
		int xsize{ 0 }, ysize{ 0 };
		GraphHandle GuideImg;
	public:
		KeyGuideGraph(void) noexcept {}
		KeyGuideGraph(const KeyGuideGraph&) = delete;
		KeyGuideGraph(KeyGuideGraph&& o) = delete;
		KeyGuideGraph& operator=(const KeyGuideGraph&) = delete;
		KeyGuideGraph& operator=(KeyGuideGraph&& o) = delete;

		~KeyGuideGraph(void) noexcept {}
	public:
		void AddGuide(int x, int y, int xs, int ys, const GraphHandle& baseImage) noexcept {
			GuideImg.SetHandle(DxLib::DerivationGraph(x, y, xs, ys, baseImage.GetHandle()));
			xsize = xs;
			ysize = ys;
		}
		void Dispose(void) noexcept { GuideImg.ReleaseGraph(); }
		int GetDrawSize(void) const noexcept;
		void Draw(int x, int y) const noexcept;
	};
private:
	bool													m_IsFlipGuide{ true };				// ガイドの更新フラグ
	GraphHandle												m_GuideBaseImage;					// 分割前の画像
	std::vector<std::shared_ptr<KeyGuideGraph>>				m_DerivationGuideImage;				// 分割後の画像
public:
	static const auto GetOffsettoID(int Offset, ControlType controlType) noexcept {
		switch (controlType) {
		case ControlType::XBox:
			for (size_t i = 0; i < static_cast<size_t>(XBoxNum); ++i) {
				if (static_cast<int>(i + KeyNum + XBoxNum) == Offset) {
					return XBoxID[i];
				}
			}
			break;
		case ControlType::PS4:
			for (size_t i = 0; i < static_cast<size_t>(DS4Num); ++i) {
				if (static_cast<int>(i + KeyNum) == Offset) {
					return DS4ID[i];
				}
			}
			break;
		case ControlType::PC:
			for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
				if (i == Offset) {
					return KeyID[i];
				}
			}
			break;
		default:
			break;
		}
		return -1;
	}
	static const auto GetStrtoID(const char* Str, ControlType controlType) noexcept {
		switch (controlType) {
		case ControlType::XBox:
			for (size_t i = 0; i < static_cast<size_t>(XBoxNum); ++i) {
				if (strcmpDx(XBoxStr[i], Str) == 0) {
					return XBoxID[i];
				}
			}
			break;
		case ControlType::PS4:
			for (size_t i = 0; i < static_cast<size_t>(DS4Num); ++i) {
				if (strcmpDx(DS4Str[i], Str) == 0) {
					return DS4ID[i];
				}
			}
			break;
		case ControlType::PC:
			for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
				if (strcmpDx(KeyStr[i], Str) == 0) {
					return KeyID[i];
				}
			}
			break;
		default:
			break;
		}
		return -1;
	}
	static const auto GetIDtoOffset(int ID, ControlType controlType) noexcept {
		switch (controlType) {
		case ControlType::XBox:
			for (size_t i = 0; i < static_cast<size_t>(XBoxNum); ++i) {
				if (XBoxID[i] == ID) {
					return static_cast<int>(i + KeyNum + XBoxNum);
				}
			}
			break;
		case ControlType::PS4:
			for (size_t i = 0; i < static_cast<size_t>(DS4Num); ++i) {
				if (DS4ID[i] == ID) {
					return static_cast<int>(i + KeyNum);
				}
			}
			break;
		case ControlType::PC:
			for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
				if (KeyID[i] == ID) {
					return static_cast<int>(i);
				}
			}
			break;
		default:
			break;
		}
		return -1;
	}
	static const auto GetIDtoStr(int ID, ControlType controlType) noexcept {
		switch (controlType) {
		case ControlType::XBox:
			for (size_t i = 0; i < static_cast<size_t>(XBoxNum); ++i) {
				if (XBoxID[i] == ID) {
					return XBoxStr[i];
				}
			}
			break;
		case ControlType::PS4:
			for (size_t i = 0; i < static_cast<size_t>(DS4Num); ++i) {
				if (DS4ID[i] == ID) {
					return DS4Str[i];
				}
			}
			break;
		case ControlType::PC:
			for (size_t i = 0; i < static_cast<size_t>(KeyNum); ++i) {
				if (KeyID[i] == ID) {
					return KeyStr[i];
				}
			}
			break;
		default:
			break;
		}
		return "NONE";
	};
	static bool GetButtonPress(ControlType controlType, int ID) noexcept {
		switch (controlType) {
		case ControlType::XBox:
		{
			XINPUT_STATE input;
			GetJoypadXInputState(DX_INPUT_PAD1, &input);
			// ボタン
			if (ID >= 0xF100) {
				switch (ID) {
				case 0xF100:
					return input.LeftTrigger > 0;
				case 0xF200:
					return input.RightTrigger > 0;
				default:
					break;
				}
			}
			else if (0 <= ID && ID < 16) {
				return (input.Buttons[ID] != 0);
			}
		}
		break;
		case ControlType::PS4:
		{
			DINPUT_JOYSTATE input;
			GetJoypadDirectInputState(DX_INPUT_PAD1, &input);
			// ボタン
			if (ID >= 0xF010) {
				// 十字キー
				float deg = static_cast<float>(input.POV[0]) / 100.f;
				bool w_key = false;
				bool s_key = false;
				bool a_key = false;
				bool d_key = false;
				if (input.POV[0] != 0xffffffff) {
					w_key = (310.f <= deg || deg <= 50.f);
					s_key = (130.f <= deg && deg <= 230.f);
					a_key = (220.f <= deg && deg <= 320.f);
					d_key = (40.f <= deg && deg <= 140.f);
				}
				switch (ID) {
				case 0xF010:
					return a_key;
				case 0xF020:
					return w_key;
				case 0xF040:
					return d_key;
				case 0xF080:
					return s_key;
				default:
					break;
				}
			}
			else if (0 <= ID && ID < 32) {
				return (input.Buttons[ID] != 0);
			}
		}
		break;
		case ControlType::PC:
			// ボタン
			if (0 <= ID) {
				if ((ID & 0xF00) != 0) {
					if (GetWindowActiveFlag()) {
						return ((GetMouseInput() & (ID & 0xFF)) != 0);
					}
					else {
						return 0;
					}
				}
				else {
					if (GetWindowActiveFlag()) {
						return (CheckHitKey(ID) != 0);
					}
					else {
						return 0;
					}
				}
			}
			break;
		default:
			break;
		}
		return false;
	}
public:
	//キー単体の描画
	void DrawButton(int x, int y, int graphOffset) const noexcept { m_DerivationGuideImage.at(graphOffset)->Draw(x, y); }
	int GetDrawSize(int graphOffset) const noexcept { return m_DerivationGuideImage.at(graphOffset)->GetDrawSize(); }
};
