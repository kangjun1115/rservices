#ifndef HID_DEVICE_H_
#define HID_DEVICE_H_

#include <mutex>
#include <condition_variable>
#include <linux/input.h>
#include "rservice.h"
#include "abstract_device.h"
#include "nrt_thread.h"

class HIDDevice : public AbstractDevice {
public:
	static const constexpr char* kHIDName = "hid_name";
	static const constexpr char* kHIDDirectory = "/dev/input";
	static const constexpr char* kEventInterfaceName = "event";
	static const constexpr char* kKeyUpdated = "key_updated";

	HIDDevice(const string& name);
	~HIDDevice();

	virtual error_condition ReConfig(const CommonKeyPairs& config);
	virtual error_condition DeConfig();

	virtual error_condition Set(const string& key, const unsigned int value);
	virtual error_condition Set(const string& key, const vector<uint8_t>& value);
	virtual error_condition Get(const string& key, unsigned int& result);
	virtual error_condition Get(const string& key, vector<uint8_t>& result);

	string FindKeyName(int key_code);
	string FindAbsName(int abs_code);

	virtual void ReceiveWork();

private:
	int hid_fd_;
	NRTThread hid_receive_thread_;
	mutex hid_receive_mutex_;
	map<string, int> hid_map_;
	atomic_bool hid_updated_;
	mutex hid_update_mutex_;
	condition_variable hid_update_cond_;

	map<int, string> events = {
			{EV_SYN, "Sync"},					{EV_KEY, "Key"},
			{EV_REL, "Relative"},				{EV_ABS, "Absolute"},
			{EV_MSC, "Misc"},					{EV_LED, "LED"},
			{EV_SND, "Sound"},					{EV_REP, "Repeat"},
			{EV_FF,	 "ForceFeedback"},			{EV_PWR, "Power"},
			{EV_FF_STATUS, "ForceFeedbackStatus"}
	};

	map<int, string> keys = {
			{KEY_RESERVED, "Reserved"},			{KEY_ESC, "Esc"},
			{KEY_1, "1"},						{KEY_2, "2"},
			{KEY_3, "3"},						{KEY_4, "4"},
			{KEY_5, "5"},						{KEY_6, "6"},
			{KEY_7, "7"},						{KEY_8, "8"},
			{KEY_9, "9"},						{KEY_0, "0"},
			{KEY_MINUS, "Minus"},				{KEY_EQUAL, "Equal"},
			{KEY_BACKSPACE, "Backspace"},		{KEY_TAB, "Tab"},
			{KEY_Q, "Q"},						{KEY_W, "W"},
			{KEY_E, "E"},						{KEY_R, "R"},
			{KEY_T, "T"},						{KEY_Y, "Y"},
			{KEY_U, "U"},						{KEY_I, "I"},
			{KEY_O, "O"},						{KEY_P, "P"},
			{KEY_LEFTBRACE, "LeftBrace"},		{KEY_RIGHTBRACE, "RightBrace"},
			{KEY_ENTER, "Enter"},				{KEY_LEFTCTRL, "LeftControl"},
			{KEY_A, "A"},						{KEY_S, "S"},
			{KEY_D, "D"},						{KEY_F, "F"},
			{KEY_G, "G"},						{KEY_H, "H"},
			{KEY_J, "J"},						{KEY_K, "K"},
			{KEY_L, "L"},						{KEY_SEMICOLON, "Semicolon"},
			{KEY_APOSTROPHE, "Apostrophe"},		{KEY_GRAVE, "Grave"},
			{KEY_LEFTSHIFT, "LeftShift"},		{KEY_BACKSLASH, "BackSlash"},
			{KEY_Z, "Z"},						{KEY_X, "X"},
			{KEY_C, "C"},						{KEY_V, "V"},
			{KEY_B, "B"},						{KEY_N, "N"},
			{KEY_M, "M"},						{KEY_COMMA, "Comma"},
			{KEY_DOT, "Dot"},					{KEY_SLASH, "Slash"},
			{KEY_RIGHTSHIFT, "RightShift"},		{KEY_KPASTERISK, "KPAsterisk"},
			{KEY_LEFTALT, "LeftAlt"},			{KEY_SPACE, "Space"},
			{KEY_CAPSLOCK, "CapsLock"},			{KEY_F1, "F1"},
			{KEY_F2, "F2"},						{KEY_F3, "F3"},
			{KEY_F4, "F4"},						{KEY_F5, "F5"},
			{KEY_F6, "F6"},						{KEY_F7, "F7"},
			{KEY_F8, "F8"},						{KEY_F9, "F9"},
			{KEY_F10, "F10"},					{KEY_NUMLOCK, "NumLock"},
			{KEY_SCROLLLOCK, "ScrollLock"},		{KEY_KP7, "KP7"},
			{KEY_KP8, "KP8"},					{KEY_KP9, "KP9"},
			{KEY_KPMINUS, "KPMinus"},			{KEY_KP4, "KP4"},
			{KEY_KP5, "KP5"},					{KEY_KP6, "KP6"},
			{KEY_KPPLUS, "KPPlus"},				{KEY_KP1, "KP1"},
			{KEY_KP2, "KP2"},					{KEY_KP3, "KP3"},
			{KEY_KP0, "KP0"},					{KEY_KPDOT, "KPDot"},
			{KEY_ZENKAKUHANKAKU, "Zenkaku/Hankaku"},{KEY_102ND, "102nd"},
			{KEY_F11, "F11"},					{KEY_F12, "F12"},
			{KEY_RO, "RO"},						{KEY_KATAKANA, "Katakana"},
			{KEY_HIRAGANA, "HIRAGANA"},			{KEY_HENKAN, "Henkan"},
			{KEY_KATAKANAHIRAGANA, "Katakana/Hiragana"},{KEY_MUHENKAN, "Muhenkan"},
			{KEY_KPJPCOMMA, "KPJpComma"},		{KEY_KPENTER, "KPEnter"},
			{KEY_RIGHTCTRL, "RightCtrl"},		{KEY_KPSLASH, "KPSlash"},
			{KEY_SYSRQ, "SysRq"},				{KEY_RIGHTALT, "RightAlt"},
			{KEY_LINEFEED, "LineFeed"},			{KEY_HOME, "Home"},
			{KEY_UP, "Up"},						{KEY_PAGEUP, "PageUp"},
			{KEY_LEFT, "Left"},					{KEY_RIGHT, "Right"},
			{KEY_END, "End"},					{KEY_DOWN, "Down"},
			{KEY_PAGEDOWN, "PageDown"},			{KEY_INSERT, "Insert"},
			{KEY_DELETE, "Delete"},				{KEY_MACRO, "Macro"},
			{KEY_MUTE, "Mute"},					{KEY_VOLUMEDOWN, "VolumeDown"},
			{KEY_VOLUMEUP, "VolumeUp"},			{KEY_POWER, "Power"},
			{KEY_KPEQUAL, "KPEqual"},			{KEY_KPPLUSMINUS, "KPPlusMinus"},
			{KEY_PAUSE, "Pause"},				{KEY_KPCOMMA, "KPComma"},
			{KEY_HANGUEL, "Hanguel"},			{KEY_HANJA, "Hanja"},
			{KEY_YEN, "Yen"},					{KEY_LEFTMETA, "LeftMeta"},
			{KEY_RIGHTMETA, "RightMeta"},		{KEY_COMPOSE, "Compose"},
			{KEY_STOP, "Stop"},					{KEY_AGAIN, "Again"},
			{KEY_PROPS, "Props"},				{KEY_UNDO, "Undo"},
			{KEY_FRONT, "Front"},				{KEY_COPY, "Copy"},
			{KEY_OPEN, "Open"},					{KEY_PASTE, "Paste"},
			{KEY_FIND, "Find"},					{KEY_CUT, "Cut"},
			{KEY_HELP, "Help"},					{KEY_MENU, "Menu"},
			{KEY_CALC, "Calc"},					{KEY_SETUP, "Setup"},
			{KEY_SLEEP, "Sleep"},				{KEY_WAKEUP, "WakeUp"},
			{KEY_FILE, "File"},					{KEY_SENDFILE, "SendFile"},
			{KEY_DELETEFILE, "DeleteFile"},		{KEY_XFER, "X-fer"},
			{KEY_PROG1, "Prog1"},				{KEY_PROG2, "Prog2"},
			{KEY_WWW, "WWW"},					{KEY_MSDOS, "MSDOS"},
			{KEY_COFFEE, "Coffee"},				{KEY_DIRECTION, "Direction"},
			{KEY_CYCLEWINDOWS, "CycleWindows"},	{KEY_MAIL, "Mail"},
			{KEY_BOOKMARKS, "Bookmarks"},		{KEY_COMPUTER, "Computer"},
			{KEY_BACK, "Back"},					{KEY_FORWARD, "Forward"},
			{KEY_CLOSECD, "CloseCD"},			{KEY_EJECTCD, "EjectCD"},
			{KEY_EJECTCLOSECD, "EjectCloseCD"},	{KEY_NEXTSONG, "NextSong"},
			{KEY_PLAYPAUSE, "PlayPause"},		{KEY_PREVIOUSSONG, "PreviousSong"},
			{KEY_STOPCD, "StopCD"},				{KEY_RECORD, "Record"},
			{KEY_REWIND, "Rewind"},				{KEY_PHONE, "Phone"},
			{KEY_ISO, "ISOKey"},				{KEY_CONFIG, "Config"},
			{KEY_HOMEPAGE, "HomePage"},			{KEY_REFRESH, "Refresh"},
			{KEY_EXIT, "Exit"},					{KEY_MOVE, "Move"},
			{KEY_EDIT, "Edit"},					{KEY_SCROLLUP, "ScrollUp"},
			{KEY_SCROLLDOWN, "ScrollDown"},		{KEY_KPLEFTPAREN, "KPLeftParenthesis"},
			{KEY_KPRIGHTPAREN, "KPRightParenthesis"}, {KEY_F13, "F13"},
			{KEY_F14, "F14"},					{KEY_F15, "F15"},
			{KEY_F16, "F16"},					{KEY_F17, "F17"},
			{KEY_F18, "F18"},					{KEY_F19, "F19"},
			{KEY_F20, "F20"},					{KEY_F21, "F21"},
			{KEY_F22, "F22"},					{KEY_F23, "F23"},
			{KEY_F24, "F24"},					{KEY_PLAYCD, "PlayCD"},
			{KEY_PAUSECD, "PauseCD"},			{KEY_PROG3, "Prog3"},
			{KEY_PROG4, "Prog4"},				{KEY_SUSPEND, "Suspend"},
			{KEY_CLOSE, "Close"},				{KEY_PLAY, "Play"},
			{KEY_FASTFORWARD, "Fast Forward"},	{KEY_BASSBOOST, "Bass Boost"},
			{KEY_PRINT, "Print"},				{KEY_HP, "HP"},
			{KEY_CAMERA, "Camera"},				{KEY_SOUND, "Sound"},
			{KEY_QUESTION, "Question"},			{KEY_EMAIL, "Email"},
			{KEY_CHAT, "Chat"},					{KEY_SEARCH, "Search"},
			{KEY_CONNECT, "Connect"},			{KEY_FINANCE, "Finance"},
			{KEY_SPORT, "Sport"},				{KEY_SHOP, "Shop"},
			{KEY_ALTERASE, "Alternate Erase"},	{KEY_CANCEL, "Cancel"},
			{KEY_BRIGHTNESSDOWN, "Brightness down"},{KEY_BRIGHTNESSUP, "Brightness up"},
			{KEY_MEDIA, "Media"},				{KEY_UNKNOWN, "Unknown"},
			{BTN_0, "Btn0"},					{BTN_1, "Btn1"},
			{BTN_2, "Btn2"},					{BTN_3, "Btn3"},
			{BTN_4, "Btn4"},					{BTN_5, "Btn5"},
			{BTN_6, "Btn6"},					{BTN_7, "Btn7"},
			{BTN_8, "Btn8"},					{BTN_9, "Btn9"},
			{BTN_LEFT, "LeftBtn"},				{BTN_RIGHT, "RightBtn"},
			{BTN_MIDDLE, "MiddleBtn"},			{BTN_SIDE, "SideBtn"},
			{BTN_EXTRA, "ExtraBtn"},			{BTN_FORWARD, "ForwardBtn"},
			{BTN_BACK, "BackBtn"},				{BTN_TASK, "TaskBtn"},
			{BTN_TRIGGER, "Trigger"},			{BTN_THUMB, "ThumbBtn"},
			{BTN_THUMB2, "ThumbBtn2"},			{BTN_TOP, "TopBtn"},
			{BTN_TOP2, "TopBtn2"},				{BTN_PINKIE, "PinkieBtn"},
			{BTN_BASE, "BaseBtn"},				{BTN_BASE2, "BaseBtn2"},
			{BTN_BASE3, "BaseBtn3"},			{BTN_BASE4, "BaseBtn4"},
			{BTN_BASE5, "BaseBtn5"},			{BTN_BASE6, "BaseBtn6"},
			{BTN_DEAD, "BtnDead"},				{BTN_A, "BtnA"},
			{BTN_B, "BtnB"},					{BTN_C, "BtnC"},
			{BTN_X, "BtnX"},					{BTN_Y, "BtnY"},
			{BTN_Z, "BtnZ"},					{BTN_TL, "BtnTL"},
			{BTN_TR, "BtnTR"},					{BTN_TL2, "BtnTL2"},
			{BTN_TR2, "BtnTR2"},				{BTN_SELECT, "BtnSelect"},
			{BTN_START, "BtnStart"},			{BTN_MODE, "BtnMode"},
			{BTN_THUMBL, "BtnThumbL"},			{BTN_THUMBR, "BtnThumbR"},
			{BTN_TOOL_PEN, "ToolPen"},			{BTN_TOOL_RUBBER, "ToolRubber"},
			{BTN_TOOL_BRUSH, "ToolBrush"},		{BTN_TOOL_PENCIL, "ToolPencil"},
			{BTN_TOOL_AIRBRUSH, "ToolAirbrush"},{BTN_TOOL_FINGER, "ToolFinger"},
			{BTN_TOOL_MOUSE, "ToolMouse"},		{BTN_TOOL_LENS, "ToolLens"},
			{BTN_TOUCH, "Touch"},				{BTN_STYLUS, "Stylus"},
			{BTN_STYLUS2, "Stylus2"},			{BTN_TOOL_DOUBLETAP, "Tool Doubletap"},
			{BTN_TOOL_TRIPLETAP, "Tool Tripletap"}, {BTN_GEAR_DOWN, "WheelBtn"},
			{BTN_GEAR_UP, "Gear up"},			{KEY_OK, "Ok"},
			{KEY_SELECT, "Select"},				{KEY_GOTO, "Goto"},
			{KEY_CLEAR, "Clear"},				{KEY_POWER2, "Power2"},
			{KEY_OPTION, "Option"},				{KEY_INFO, "Info"},
			{KEY_TIME, "Time"},					{KEY_VENDOR, "Vendor"},
			{KEY_ARCHIVE, "Archive"},			{KEY_PROGRAM, "Program"},
			{KEY_CHANNEL, "Channel"},			{KEY_FAVORITES, "Favorites"},
			{KEY_EPG, "EPG"},					{KEY_PVR, "PVR"},
			{KEY_MHP, "MHP"},					{KEY_LANGUAGE, "Language"},
			{KEY_TITLE, "Title"},				{KEY_SUBTITLE, "Subtitle"},
			{KEY_ANGLE, "Angle"},				{KEY_ZOOM, "Zoom"},
			{KEY_MODE, "Mode"},					{KEY_KEYBOARD, "Keyboard"},
			{KEY_SCREEN, "Screen"},				{KEY_PC, "PC"},
			{KEY_TV, "TV"},						{KEY_TV2, "TV2"},
			{KEY_VCR, "VCR"},					{KEY_VCR2, "VCR2"},
			{KEY_SAT, "Sat"},					{KEY_SAT2, "Sat2"},
			{KEY_CD, "CD"},						{KEY_TAPE, "Tape"},
			{KEY_RADIO, "Radio"},				{KEY_TUNER, "Tuner"},
			{KEY_PLAYER, "Player"},				{KEY_TEXT, "Text"},
			{KEY_DVD, "DVD"},					{KEY_AUX, "Aux"},
			{KEY_MP3, "MP3"},					{KEY_AUDIO, "Audio"},
			{KEY_VIDEO, "Video"},				{KEY_DIRECTORY, "Directory"},
			{KEY_LIST, "List"},					{KEY_MEMO, "Memo"},
			{KEY_CALENDAR, "Calendar"},			{KEY_RED, "Red"},
			{KEY_GREEN, "Green"},				{KEY_YELLOW, "Yellow"},
			{KEY_BLUE, "Blue"},					{KEY_CHANNELUP, "ChannelUp"},
			{KEY_CHANNELDOWN, "ChannelDown"},	{KEY_FIRST, "First"},
			{KEY_LAST, "Last"},					{KEY_AB, "AB"},
			{KEY_NEXT, "Next"},					{KEY_RESTART, "Restart"},
			{KEY_SLOW, "Slow"},					{KEY_SHUFFLE, "Shuffle"},
			{KEY_BREAK, "Break"},				{KEY_PREVIOUS, "Previous"},
			{KEY_DIGITS, "Digits"},				{KEY_TEEN, "TEEN"},
			{KEY_TWEN, "TWEN"},					{KEY_DEL_EOL, "Delete EOL"},
			{KEY_DEL_EOS, "Delete EOS"},		{KEY_INS_LINE, "Insert line"},
			{KEY_DEL_LINE, "Delete line"},
	};

	map<int, string> relatives = {
			{REL_X, "X"},						{REL_Y, "Y"},
			{REL_Z, "Z"},						{REL_HWHEEL, "HWheel"},
			{REL_DIAL, "Dial"},					{REL_WHEEL, "Wheel"},
			{REL_MISC, "Misc"},
	};

	map<int, string> absolutes = {
			{ABS_X, "X"},						{ABS_Y, "Y"},
			{ABS_Z, "Z"},						{ABS_RX, "Rx"},
			{ABS_RY, "Ry"},						{ABS_RZ, "Rz"},
			{ABS_THROTTLE, "Throttle"},			{ABS_RUDDER, "Rudder"},
			{ABS_WHEEL, "Wheel"},				{ABS_GAS, "Gas"},
			{ABS_BRAKE, "Brake"},				{ABS_HAT0X, "Hat0X"},
			{ABS_HAT0Y, "Hat0Y"},				{ABS_HAT1X, "Hat1X"},
			{ABS_HAT1Y, "Hat1Y"},				{ABS_HAT2X, "Hat2X"},
			{ABS_HAT2Y, "Hat2Y"},				{ABS_HAT3X, "Hat3X"},
			{ABS_HAT3Y, "Hat 3Y"},				{ABS_PRESSURE, "Pressure"},
			{ABS_DISTANCE, "Distance"},			{ABS_TILT_X, "XTilt"},
			{ABS_TILT_Y, "YTilt"},				{ABS_TOOL_WIDTH, "Tool Width"},
			{ABS_VOLUME, "Volume"},				{ABS_MISC, "Misc"},
	};

	map<int, string> misc = {
			{MSC_SERIAL, "Serial"},				{MSC_PULSELED, "Pulseled"},
			{MSC_GESTURE, "Gesture"},			{MSC_RAW, "RawData"},
			{MSC_SCAN, "ScanCode"},
	};

	map<int, string> leds = {
			{LED_NUML, "NumLock"},				{LED_CAPSL, "CapsLock"},
			{LED_SCROLLL, "ScrollLock"},		{LED_COMPOSE, "Compose"},
			{LED_KANA, "Kana"},					{LED_SLEEP, "Sleep"},
			{LED_SUSPEND, "Suspend"},			{LED_MUTE, "Mute"},
			{LED_MISC, "Misc"},
	};

	map<int, string> repeats = {
			{REP_DELAY, "Delay"}, 				{REP_PERIOD, "Period"},
	};

	map<int, string> sounds = {
			{SND_CLICK, "Click"},				{SND_BELL, "Bell"},
			{SND_TONE, "Tone"},
	};

	map<int, string> syns = {
			{SYN_REPORT, "Report"},				{SYN_CONFIG, "Config"},
#ifdef SYN_MT_REPORT
			{SYN_MT_REPORT, "MT Report"},
#endif
	};

	map<int, map<int, string>&> names = {
			{EV_SYN, events},					{EV_KEY, keys},
			{EV_REL, relatives},				{EV_ABS, absolutes},
			{EV_MSC, misc},						{EV_LED, leds},
			{EV_SND, sounds},					{EV_REP, repeats},
	};
};

#endif //HID_DEVICE_H_
