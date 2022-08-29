#pragma once

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <regex>

typedef unsigned char uch;
typedef unsigned short ush;
typedef unsigned int uin;
typedef std::vector<std::vector<std::string>> My2DStr;

const unsigned int PORT = 3000;
const std::string IP("127.0.0.1");

enum DATA_TYPE {
	OTHER = -1,
	MISSILE,        //攻击导弹
	INTERCEPTOR,    //拦截导弹
	RADAR_M,        //导弹雷达
	RADAR_T,        //目标雷达
	TARGET          //目标
};

const char* DataTypes[];

namespace Example 
{
	enum DeadReckoningModel
	{
		STATIC = 1,
		DRM_FPW,
		DRM_RPW,
		DRM_RVW,
		DRM_FVW,
		DRM_FPB,
		DRM_RPB,
		DRM_RVB,
		DRM_FVB,
	};
}

DATA_TYPE GETTYPE(const std::string& str);
std::string STRTYPE(enum DATA_TYPE type);

std::vector<std::string> splitString(const std::string& str, char splitter);

std::string removeSubstr(const std::string& str, const std::string& pattern);

std::vector<float> strV2floatV(const std::vector<std::string>& str_v);


/*convert anytype to string
template <typename T>
std::string to_string(const T& value) {
	std::stringstream sstr;
	//sstr << value;
	return sstr.str();
}

*/
//template <typename T>
//std::string to_string( const T& value);