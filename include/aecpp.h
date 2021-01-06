/*

AEC++ - Yet another small C++11 library for colors and effects in terminal
output by means of ansi escape codes, made by Arthur van der Staaij

This file is released under the terms of the MIT license.
Read LICENSE.txt for more information.

To download the latest version, report issues or request new features, visit
https://github.com/avdstaaij/aecpp

*/

#pragma once

#include <iostream>
#include <atomic>
#include <cstring>
#include <cstdint>
#include <type_traits>
#include <algorithm>

#if defined(__unix__)  ||\
    defined(__unix)    ||\
    defined(__linux__) ||\
    defined(__APPLE__) ||\
    defined(__MACH__)
#define AECPP_OS_SUPPORTED
#endif

#ifdef AECPP_OS_SUPPORTED
#include <unistd.h>
#endif

namespace aec {

//============================================================================//
//------------------------------- Declarations -------------------------------//
//============================================================================//

enum Reset : int8_t {
	reset           = 0
};

enum Effect : int8_t {
	bold            = 1,
	dim             = 2,
	italic          = 3,
	underline       = 4,
	blink           = 5,
	rapidblink      = 6,
	reversevideo    = 7,
	conceal         = 8,
	crossout        = 9,
};

enum Color : int8_t {
	black           = 30,
	red             = 31,
	green           = 32,
	yellow          = 33,
	blue            = 34,
	magenta         = 35,
	cyan            = 36,
	gray            = 37,
	creset          = 39,

	brightblack     = 90,
	brightred       = 91,
	brightgreen     = 92,
	brightyellow    = 93,
	brightblue      = 94,
	brightmagenta   = 95,
	brightcyan      = 96,
	brightgray      = 97,
};

enum BGColor : int8_t {
	bgblack         = 40,
	bgred           = 41,
	bggreen         = 42,
	bgyellow        = 43,
	bgblue          = 44,
	bgmagenta       = 45,
	bgcyan          = 46,
	bggray          = 47,
	bgcreset        = 49,

	bgbrightblack   = 100,
	bgbrightred     = 101,
	bgbrightgreen   = 102,
	bgbrightyellow  = 103,
	bgbrightblue    = 104,
	bgbrightmagenta = 105,
	bgbrightcyan    = 106,
	bgbrightgray    = 107
};

enum class Mode {
	Always,
	Auto,
	Semi,
	Never
};

//============================================================================//
//-------------------------------- Internals ---------------------------------//
//============================================================================//

namespace aecImplementation {

std::atomic_bool aecSupportedStatic(false);
std::atomic_bool coutIsTTY(false);
std::atomic_bool cerrIsTTY(false);

std::atomic<Mode> mode(Mode::Auto);

struct InitializationDummy {
	InitializationDummy() noexcept {
#ifdef AECPP_OS_SUPPORTED
		const char *terms[] = {
			"ansi",
			"color",
			"console",
			"cygwin",
			"gnome",
			"konsole",
			"kterm",
			"linux",
			"msys",
			"putty",
			"rxvt",
			"screen",
			"vt100",
			"xterm"
		};
		auto envTerm = std::getenv("TERM");
		aecSupportedStatic = std::any_of(std::begin(terms), std::end(terms),
			[&](const char* term){
				return std::strstr(envTerm, term) != nullptr;
			}
		);
		if (aecSupportedStatic) {
			coutIsTTY = isatty(fileno(stdout));
			cerrIsTTY = isatty(fileno(stderr));
		}
#endif
	}
} initializationDummy;

inline bool isTTY(const std::ostream& os) noexcept {
	if (os.rdbuf() == std::cout.rdbuf()) {
		return coutIsTTY;
	}
	if (os.rdbuf() == std::cerr.rdbuf()) {
		return cerrIsTTY;
	}
	return false;
}

inline bool aecEnabled(const std::ostream& os) noexcept {
#ifdef AECPP_OS_SUPPORTED
	switch (mode) {
	case Mode::Auto:
		return aecSupportedStatic && isTTY(os);
	case Mode::Always:
		return true;
	case Mode::Never:
		return false;
	case Mode::Semi:
		return aecSupportedStatic;
	}
#endif
	return false;
}

template<typename T>
struct isCode {
	static bool constexpr value = (
		std::is_same<T, Reset>::value   ||
		std::is_same<T, Effect>::value  ||
		std::is_same<T, Color>::value   ||
		std::is_same<T, BGColor>::value
	);
};

} // namespace aecImplementation

//============================================================================//
//------------------------------ Mode changing -------------------------------//
//============================================================================//

inline void setMode(Mode mode) {
	aecImplementation::mode = mode;
}

inline Mode getMode() {
	return aecImplementation::mode;
}

//============================================================================//
//------------------------------- Style class --------------------------------//
//============================================================================//

class Style {
public:
	// Using these constructors explicitly is not recommended.
	// Instead, rely on the automatic conversion rules of c++.
	Style()                                   {}
	Style(Reset)           : reset(true)      {}
	Style(Effect effect)   : effect(effect)   {}
	Style(Color color)     : color(color)     {}
	Style(BGColor bgcolor) : bgcolor(bgcolor) {}

	Style& operator=(Reset reset)     {return *this = Style(reset);}
	Style& operator=(Effect effect)   {return *this = Style(effect);}
	Style& operator=(Color color)     {return *this = Style(color);}
	Style& operator=(BGColor bgcolor) {return *this = Style(bgcolor);}

	Style& operator+=(const Style& r);

	void setReset() {
		reset   = true;
		effect  = static_cast<Effect>(-1);
		color   = static_cast<Color>(-1);
		bgcolor = static_cast<BGColor>(-1);
	}
	void setEffect(Effect _effect)    {effect = _effect;}
	void unsetEffect()                {effect = static_cast<Effect>(-1);}
	void setColor(Color _color)       {color = _color;}
	void unsetColor()                 {color = static_cast<Color>(-1);}
	void setBGColor(BGColor _bgcolor) {bgcolor = _bgcolor;}
	void unsetBGColor()               {bgcolor = static_cast<BGColor>(-1);}

private:
	bool reset = false;
	Effect  effect  = static_cast<Effect>(-1);
	Color   color   = static_cast<Color>(-1);
	BGColor bgcolor = static_cast<BGColor>(-1);
	friend std::ostream& operator<<(std::ostream&, const Style&);
};

inline Style& Style::operator+=(const Style& r) {
	if (r.reset) {*this = r;}
	if (r.effect  != static_cast<Effect>(-1))  {effect = r.effect;}
	if (r.color   != static_cast<Color>(-1))   {color = r.color;}
	if (r.bgcolor != static_cast<BGColor>(-1)) {bgcolor = r.bgcolor;}
	return *this;
}

Style operator+(Style l, const Style& r) {
	return l += r;
}

template<typename Code1, typename Code2>
inline typename std::enable_if<
	aecImplementation::isCode<Code1>::value &&
	aecImplementation::isCode<Code2>::value,
	Style
>::type
operator+(Code1 l, Code2 r) {
	return Style(l) + r;
}

//============================================================================//
//------------------------------ Stream output -------------------------------//
//============================================================================//

template<typename Code>
inline typename std::enable_if<
	aecImplementation::isCode<Code>::value,
	std::ostream&
>::type
operator<<(std::ostream& os, Code code) {
	if (aecImplementation::aecEnabled(os)) {
		std::ostream osUnformatted(os.rdbuf());
		osUnformatted << "\e[" << static_cast<unsigned short>(code) << "m";
	}
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const Style& style) {
	if (!aecImplementation::aecEnabled(os)) {
		return os;
	}
	std::ostream osUnformatted(os.rdbuf());
	if (style.reset) {osUnformatted << reset;}
	bool wroteEsc = false;
	if (style.effect  != static_cast<Effect>(-1))  {
		osUnformatted << "\e[" << static_cast<unsigned short>(style.effect);
		wroteEsc = true;
	}
	if (style.color   != static_cast<Color>(-1))   {
		osUnformatted << (wroteEsc ? ";" : "\e[")
		              << static_cast<unsigned short>(style.color);
		wroteEsc = true;
	}
	if (style.bgcolor != static_cast<BGColor>(-1)) {
		osUnformatted << (wroteEsc ? ";" : "\e[")
		              << static_cast<unsigned short>(style.bgcolor);
		wroteEsc = true;
	}
	if (wroteEsc) {osUnformatted << "m";}
	return os;
}

} // namespace aec
