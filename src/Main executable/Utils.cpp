#include <string>
#include <boost/algorithm/string/replace.hpp>

__declspec(dllexport) void SplitPath(char* fullpath, std::string& path, std::string& filename)
{
	std::string fullpathStr(fullpath);
	// TODO: this should happen outside of this function, fullpath should be already in the correct format
	boost::replace_all(fullpathStr, "\\", "/");
	size_t pos = fullpathStr.find_last_of("/");
	if (pos == std::string::npos)
	{
		// No directory separator found, use current directory
		path = ".";
		filename = fullpathStr;
	}
	else
	{
		// Directory separator found, split path and file pattern
		path = fullpathStr.substr(0, pos);
		filename = fullpathStr.substr(pos + 1);
	}
}
