#include "rc/version.h"

#include <TermAPI.hpp>
#include <ParamsAPI2.hpp>
#include <envpath.hpp>

#include <ctime>
#include <chrono>
#include <format>

static const std::string DEFAULT_DATE_FORMAT{ "[{0:%d}/{0:%m}/{0:%y}]" };
static const std::string DEFAULT_TIME_FORMAT{ "%H:%M:%S" };
static const std::string DEFAULT_SEPERATOR{ " " };

static struct {
	bool quiet{ false };
	std::string dateFormat{ DEFAULT_DATE_FORMAT };
	std::string timeFormat{ DEFAULT_TIME_FORMAT };
	std::string seperator{ DEFAULT_SEPERATOR };
} Global;

struct Help {
	const std::string& programName;

	constexpr Help(const std::string& programName) : programName{ programName } {}

	friend std::ostream& operator<<(std::ostream& os, const Help& h)
	{
		return os
			<< "dt  v" << dt_VERSION_EXTENDED << '\n'
			<< "  Commandline Date & Time Utility." << '\n'
			<< '\n'
			<< "USAGE:\n"
			<< "  " << h.programName << " [OPTIONS]" << '\n'
			<< '\n'
			<< "  The formatting of the date & time can be set independently; however the date always preceeds the time." << '\n'
			<< '\n'
			<< "OPTIONS:\n"
			<< "  -h, --help               Shows this help display, then exits." << '\n'
			<< "  -v, --version            Shows the current version number, then exits." << '\n'
			<< "  -q, --quiet              Prevents non-essential console output & formatting." << '\n'
			<< "  -d, --date <FORMAT>      Sets the date formatting style to use.  Set to a blank string \"\" to hide." << '\n'
			<< "                            Defaults to \"" << DEFAULT_DATE_FORMAT << "\".  A list of values can be found here:" << '\n'
			<< "                            https://en.cppreference.com/w/cpp/chrono/year_month_day/formatter" << '\n'
			<< "  -t, --time <FORMAT>      Sets the time formatting style to use.  Set to a blank string \"\" to hide." << '\n'
			<< "                            Defaults to \"" << DEFAULT_TIME_FORMAT << "\".  A list of values can be found here:" << '\n'
			<< "                            https://en.cppreference.com/w/cpp/io/manip/put_time" << '\n'
			<< "  -s, --sep <SEPERATOR>    Sets the seperator to use when showing both date & time.  Default: \"" << DEFAULT_SEPERATOR << "\"" << '\n'
			;
	}
};

int main(const int argc, char** argv)
{
	try {
		std::cout << term::EnableANSI;

		opt::ParamsAPI2 args{ argc, argv, 'd', "date", 't', "time", 's', "sep" };
		const auto& [programPath, programName] {env::PATH().resolve_split(argv[0])};

		Global.quiet = args.check_any<opt::Flag, opt::Option>('q', "quiet");

		if (args.check_any<opt::Flag, opt::Option>('h', "help")) {
			std::cout << Help(programName.generic_string()) << std::endl;
			return 0;
		}
		else if (args.check_any<opt::Flag, opt::Option>('v', "version")) {
			if (!Global.quiet)
				std::cout << "dt  v";
			std::cout << dt_VERSION_EXTENDED << std::endl;
			return 0;
		}

		Global.dateFormat = args.typegetv_any<opt::Flag, opt::Option>('d', "date").value_or(Global.dateFormat);
		Global.timeFormat = args.typegetv_any<opt::Flag, opt::Option>('t', "time").value_or(Global.timeFormat);
		Global.seperator = args.typegetv_any<opt::Flag, opt::Option>('s', "sep").value_or(Global.seperator);

		bool showDate{ !Global.dateFormat.empty() }, showTime{ !Global.timeFormat.empty() };


		using CLK = std::chrono::system_clock;
		using namespace std::chrono;


		const time_point now{ CLK::now() };

		if (showDate) { // date:
			std::cout << std::vformat(std::string_view(Global.dateFormat), std::make_format_args(floor<days>(now)));

			if (showTime) // if we're also displaying the time, print the seperator.
				std::cout << Global.seperator;
		}
		if (showTime) { // time:
			const std::time_t currentTime{ CLK::to_time_t(now) };
			std::cout << std::put_time(std::localtime(&currentTime), Global.timeFormat.c_str());
		}
		std::cout << std::endl;

		return 0;
	} catch (const std::exception& ex) {
		std::cerr << term::get_fatal() << ex.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << term::get_fatal() << "An undefined exception occurred!" << std::endl;
		return 1;
	}
}
