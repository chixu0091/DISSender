#include "DataManager.h"

#include <utils/Conversion.h>

#include <iostream>
#include <filesystem> //c++ 17
#include <regex>
#include <fstream>

#include "Logging.h"
namespace fs = std::filesystem;

const char* DataTypes[] =
{
	"MISSILE",        //攻击导弹
	"INTERCEPTOR",    //拦截导弹
	"RADAR_M",        //导弹雷达
	"RADAR_T",        //目标雷达
	"TARGET"          //目标
};

//=============================================================//
//=======================Helper function=======================//
//=============================================================//


// get string form of DATA_TYPE
std::string STRTYPE(enum DATA_TYPE type)
{
	return (type == OTHER) ? "OTHER" : DataTypes[type];
}

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

// remove the pattern substr of given string str
// i.e. str = "IM:xxx" after remove "^(.*?):" -> str = "xxx"
// REMEMBER pattern here is regex; if pattern doesn't match then return str
std::string removeSubstr(const std::string& str, const std::string& pattern)
{
	std::string res = std::regex_replace(str, std::regex(pattern), "");
	return (res.empty()) ? str : res;
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




//=============================================================//
//=================DataManager Member function=================//
//=============================================================//

// constructor
Example::DataManager::DataManager()
{
}


//default get all file paths under ./data
void Example::DataManager::GetAllFilePaths(const std::string& path)
{
	std::string tempPath;
	tempPath = (path == "") ? "./data" : path;
	
	if (!fs::exists(tempPath)) {
		LOG_ERROR("File path NOT vaild: " + tempPath);
		return;
	}

	for (const auto& entry : fs::directory_iterator(tempPath)) {
		std::string siglefielpath = entry.path().string();
		filePaths.push_back(siglefielpath);
		fileTypes.emplace(siglefielpath, GETTYPE(siglefielpath));
	}
		
	return;
}

// Read file line by line and save 
std::shared_ptr<My2DStr> ReadFileHelper(const std::string& path)
{
	std::shared_ptr<My2DStr> ptr = std::make_shared<My2DStr>();
	//std::cout << path;
	std::ifstream infile(path);
	
	if (infile.is_open()) {
		std::string str;
		while (std::getline(infile, str))
		{
			ptr->push_back(splitString(
				// regex ^(.*?): match prefix before (include) first instance of ":" 
				// ^ start of str
				// (.*?) lazy match any char
				// : match first occurrence of ":"
				removeSubstr(str, "^(.*?):"),
				' ' // split string with space
			));
		}
	}
	else {
		LOG_ERROR("File open FAIL:" + path);
		return nullptr;
	}
	
	LOG_NOTICE("Finish read file: " + path);
	return ptr;
}

// wrapper
void Example::DataManager::ReadFileRaw(const std::string& path)
{
	rawDataSet.emplace(path, ReadFileHelper(path));
	return;
}

// construct rawData map
void Example::DataManager::ReadAllFilesRaw()
{
	LOG_NOTICE("Start reading all files...");
	for (const auto& path : filePaths) {
		ReadFileRaw(path);
	}
	LOG_NOTICE("End reading all files...");
	return;
}

// Init DISData with entity id 
void Example::DataManager::GetDisData(const std::string& path, int id)
{
	disDataSet.emplace(path,
		std::make_shared<Example::DISData>
		(0, 0, id, fileTypes[path],
			Example::STATIC) //hardcode drp to all static
	); 
	return;
}

//construct all DISData for all paths
void Example::DataManager::GetAllDisData()
{
	if (!filePaths.size()) {
		LOG_WARNING("Try get all disData with NO files...End");
		return;
	}

	for (int i = 0; i < filePaths.size(); ++i) {
		GetDisData(filePaths[i], i);
	}

	return;
}

void Example::DataManager::SendFileDisData(Example::UDPConnection* conn, const std::string& path)
{
	
	DIS::DataStream buffer(DIS::BIG);
	buffer.clear();
	if (disDataSet.find(path) == disDataSet.end()) {
		LOG_ERROR("File path found FAIL when SendFileDisData: " + path);
		return;
	}

	if (disDataSet[path] == nullptr) {
		LOG_ERROR("File DISData is NULL: " + path);
		return;
	}

	disDataSet[path]->marshal(buffer);
	conn->Send(&buffer[0], buffer.size());
	LOG_NOTICE("Send " 
		+ path 
		+ " " + std::to_string(disDataSet[path]->get_entity_id()));
	buffer.clear();
	
	return;
}


void Example::DataManager::SendAllFiles(Example::UDPConnection* conn)
{
	for (const std::string& path : filePaths) {
		SendFileDisData(conn, path);
	}
	return;
}

void Example::DataManager::Init()
{
	Init("");
	return;
}

void Example::DataManager::Init(const std::string& path)
{
	GetAllFilePaths(path);
	ReadAllFilesRaw();
	GetAllDisData();
	return;
}

//=============================================================//
//=======================print functions=======================//
//=============================================================//

void Example::DataManager::PrintFile(const std::string& path)
{
	if (rawDataSet.find(path) == rawDataSet.end()) {
		//not found
		LOG_ERROR("File path found FAIL or File doesn't get read or it's null: " + path);
		return;
	}
	auto lines = rawDataSet[path];
	for (const auto& line : *lines) {
		for (const auto& word : line) {
			std::cout << word << "**";
		}
		std::cout << std::endl;
	}
}


void Example::DataManager::PrintFilePaths() 
{
	for (const auto& path : filePaths) {
		std::cout << path << std::endl;
	}
	return;
}

void Example::DataManager::PrintPathsTypes()
{
	for (const auto& [path, type] : fileTypes) {
		std::cout << path << ": " << type << " " << STRTYPE(type) << std::endl;
	}
}

void Example::DataManager::PrintALL()
{
	for (const auto& [path, type] : fileTypes) {
		std::cout << "*************************************\n";
		std::cout << path << ": " << type << " " << STRTYPE(type) << std::endl;
		PrintFile(path);
		std::cout << "*************************************\n";

	}
}