#include "COMMON.h"

const char* DataTypes[] =
{
	"MISSILE",        //攻击导弹
	"INTERCEPTOR",    //拦截导弹
	"RADAR_M",        //导弹雷达
	"RADAR_T",        //目标雷达
	"TARGET"          //目标
};

// using regex to match file name to get DATA_TYPE
DATA_TYPE GETTYPE(const std::string& str)
{

	std::vector<std::string> vs
	{
		// ASCII x5C match backslash x2E match dot
		"^.*(\\x5C)I[0-9]+(\\x2E)txt$",
		"^.*(\\x5C)M[0-9]+(\\x2E)txt$",
		"^.*(\\x5C)R_M[0-9]+(\\x2E)txt$",
		"^.*(\\x5C)R_T[0-9]+(\\x2E)txt$",
		"^.*(\\x5C)T[0-9]+(\\x2E)txt$"
	};

	for (int i = 0; i < vs.size(); ++i) {
		if (std::regex_match(str, std::regex(vs[i]))) {
			//LOG_NOTICE(str + " " + vs[i] + STRTYPE(static_cast<DATA_TYPE>(i)));
			return static_cast<DATA_TYPE>(i);
		}
	}

	return OTHER;
}


// get string form of DATA_TYPE
std::string STRTYPE(enum DATA_TYPE type)
{
	return (type == OTHER) ? "OTHER" : DataTypes[type];
}

// python-string.split(str, splitter)-like method
// https://stackoverflow.com/questions/68396962/how-to-split-strings-in-c-like-in-python
std::vector<std::string> splitString(const std::string& str, char splitter)
{
	std::vector<std::string> result;
	std::string current = "";
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == splitter) {
			if (current != "") {
				result.push_back(current);
				current = "";
			}
			continue;
		}
		current += str[i];
	}
	if (current.size() != 0)
		result.push_back(current);
	return result;
}


// remove the pattern substr of given string str
// i.e. str = "IM:xxx" after remove "^(.*?):" -> str = "xxx"
// REMEMBER pattern here is regex; if pattern doesn't match then return str
std::string removeSubstr(const std::string& str, const std::string& pattern)
{
	std::string res = std::regex_replace(str, std::regex(pattern), "");
	return (res.empty()) ? str : res;
}

// convert string vector to float vector
std::vector<float> strV2floatV(const std::vector<std::string>& str_v)
{
	std::vector<float> float_v;

	std::for_each(str_v.begin(),
		str_v.end(),
		[&float_v](const std::string& element) {float_v.push_back(std::stof(element));  }
	);

	return float_v;
}

