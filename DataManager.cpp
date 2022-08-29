#include "DataManager.h"

#include <utils/Conversion.h>

#include <filesystem> //c++ 17
#include <fstream>

#include "Logging.h"
namespace fs = std::filesystem;

//=============================================================//
//=================DataManager Member function=================//
//=============================================================//

// constructor
Example::DataManager::DataManager()
{
}

//default get all file paths under ./data
void Example::DataManager::ReadAllFilePaths(const std::string& path)
{
	std::string tempPath;
	tempPath = (path == "") ? "./testdata" : path;
	
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
	rawDataCurIndex.emplace(path, 0);
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

// increment each file's raw data index by 1 until it's the last
// if init is true, it means that it init first time rather than increment
void Example::DataManager::UpdateRawIndexOnce(bool init)
{
	uin updated_index;

	for (const auto& [path, index] : rawDataCurIndex) {

		if (init) {
			updated_index = 0;
		}
		else {
			updated_index =
				((index + 1) == rawDataSet[path]->size()) ?
				index : index + 1;
		}
		rawDataCurIndex[path] = updated_index;

		disDataSet[path]->parse_raw_data(rawDataSet[path]->at(updated_index));
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

// firstly get all file paths;
// then read in all raw data;
// then convert raw data into disdata;
void Example::DataManager::Init(const std::string& path)
{
	ReadAllFilePaths(path);
	ReadAllFilesRaw();
	GetAllDisData();
	UpdateRawIndexOnce(true);

	//test: disDataSet[filePaths[0]]->print_pos();

	return;
}

// return file name at index, "" if index out of range
std::string Example::DataManager::GetFile(int index)
{
	if (index >= filePaths.size()) {
		return "";
	}
	return filePaths[index];
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