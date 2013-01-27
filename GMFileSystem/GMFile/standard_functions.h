#pragma once

#include <string>


std::string MakeRichPath(std::string inputString);

enum GMAttributes
{
	GMreadonly = 0x1,
	GMhidden = 0x2,
	GMsystem = 0x4,
	GMvolumeid = 0x8,
	GMdirectory = 0x10,
	GMarchiv = 0x20,

	GMall = 0x3F
};