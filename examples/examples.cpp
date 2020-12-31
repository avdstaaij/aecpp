// This file contains example code for AEC++.

#include <iostream>
#include <iomanip>

#include "aecpp.h"

int main() {

	// ==== Basic usage ==== //

	std::cout << aec::bold << aec::red << "Hello world!\n" << aec::reset;

	std::cout
	<< "\n"
	<< aec::blue + aec::italic
	<< "This text is blue italic\n"
	<< aec::creset
	<< "This text is just italic\n"
	<< aec::green
	<< "This text is green italic\n"
	<< aec::reset
	<< "This text is normal\n";

	std::cout
	<< "\n"
	<< aec::bgred
	<< "Be careful with background colors and newlines\n"
	<< aec::reset
	<< aec::bgblue
	<< "Use aec::reset before the newline"
	<< aec::reset
	<< "\n";

	// ==== Style objects ==== //

	auto style = aec::red + aec::bold;

	std::cout
	<< "\n"
	<< style
	<< "Style objects can be used to store escape code combinations\n"
	<< aec::reset;

	std::cout
	<< "\n"
	<< aec::italic + aec::cyan
	<< "Create a style object by adding codes with '+'\n"
	<< aec::reset
	<< aec::Style(aec::magenta)
	<< "Or create them explicitly\n"
	<< aec::reset
	<< aec::italic + aec::Style(aec::yellow)
	<< "Or combine both methods\n"
	<< aec::reset;

	std::cout
	<< "\n"
	<< aec::italic + aec::bold + aec::blue + aec::bgred + aec::red + aec::bgcyan
	<< "The last value of each type is used, in this case bold, red and bgcyan"
	<< aec::reset
	<< "\n";

	std::cout
	<< "\n"
	<< aec::red   + aec::reset               << "normal" << aec::reset << "\n"
	<< aec::reset + aec::red                 << "red"    << aec::reset << "\n"
	<< aec::red   + aec::reset + aec::bgblue << "bgblue" << aec::reset << "\n"
	<< aec::bold  + aec::reset               << "normal" << aec::reset << "\n"
	<< aec::reset + aec::bold                << "bold"   << aec::reset << "\n"
	<< aec::bold  + aec::reset + aec::red    << "red"    << aec::reset << "\n";

	// ==== Modes ==== //

	std::cout << "\n";

	aec::setMode(aec::Mode::Never);
	std::cout << aec::red << "Mode: Never\n"  << aec::reset;

	aec::setMode(aec::Mode::Auto);
	std::cout << aec::red << "Mode: Auto\n"   << aec::reset;

	aec::setMode(aec::Mode::Semi);
	std::cout << aec::red << "Mode: Semi\n"   << aec::reset;

	aec::setMode(aec::Mode::Always);
	std::cout << aec::red << "Mode: Always\n" << aec::reset;

	aec::setMode(aec::Mode::Auto);

	// ==== All supported escape codes (except creset and bgreset) ==== //

	std::cout
	<< "\n\n"
	<< aec::black           << "black          " << aec::reset << " "
	<< aec::brightblack     << "brightblack    " << aec::reset << " "
	<< aec::bgblack         << "bgblack        " << aec::reset << " "
	<< aec::bgbrightblack   << "bgbrightblack  " << aec::reset << " "
	<< "\n"
	<< aec::red             << "red            " << aec::reset << " "
	<< aec::brightred       << "brightred      " << aec::reset << " "
	<< aec::bgred           << "bgred          " << aec::reset << " "
	<< aec::bgbrightred     << "bgbrightred    " << aec::reset << " "
	<< "\n"
	<< aec::green           << "green          " << aec::reset << " "
	<< aec::brightgreen     << "brightgreen    " << aec::reset << " "
	<< aec::bggreen         << "bggreen        " << aec::reset << " "
	<< aec::bgbrightgreen   << "bgbrightgreen  " << aec::reset << " "
	<< "\n"
	<< aec::yellow          << "yellow         " << aec::reset << " "
	<< aec::brightyellow    << "brightyellow   " << aec::reset << " "
	<< aec::bgyellow        << "bgyellow       " << aec::reset << " "
	<< aec::bgbrightyellow  << "bgbrightyellow " << aec::reset << " "
	<< "\n"
	<< aec::blue            << "blue           " << aec::reset << " "
	<< aec::brightblue      << "brightblue     " << aec::reset << " "
	<< aec::bgblue          << "bgblue         " << aec::reset << " "
	<< aec::bgbrightblue    << "bgbrightblue   " << aec::reset << " "
	<< "\n"
	<< aec::magenta         << "magenta        " << aec::reset << " "
	<< aec::brightmagenta   << "brightmagenta  " << aec::reset << " "
	<< aec::bgmagenta       << "bgmagenta      " << aec::reset << " "
	<< aec::bgbrightmagenta << "bgbrightmagenta" << aec::reset << " "
	<< "\n"
	<< aec::cyan            << "cyan           " << aec::reset << " "
	<< aec::brightcyan      << "brightcyan     " << aec::reset << " "
	<< aec::bgcyan          << "bgcyan         " << aec::reset << " "
	<< aec::bgbrightcyan    << "bgbrightcyan   " << aec::reset << " "
	<< "\n"
	<< aec::gray            << "gray           " << aec::reset << " "
	<< aec::brightgray      << "brightgray     " << aec::reset << " "
	<< aec::bggray          << "bggray         " << aec::reset << " "
	<< aec::bgbrightgray    << "bgbrightgray   " << aec::reset << " "
	<< "\n\n"
	<< aec::bold            << "bold"            << aec::reset << " "
	<< aec::dim             << "dim"             << aec::reset << " "
	<< aec::italic          << "italic"          << aec::reset << " "
	<< aec::underline       << "underline"       << aec::reset << " "
	<< aec::blink           << "blink"           << aec::reset << " "
	<< aec::rapidblink      << "rapidblink"      << aec::reset << " "
	<< aec::reversevideo    << "reversevideo"    << aec::reset << " "
	<< aec::conceal         << "conceal"         << aec::reset << " "
	<< aec::crossout        << "crossout"        << aec::reset << " "
	<< "\n";

	return 0;
}
