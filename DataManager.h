#pragma once

#include "Connection.h"
#include "DISData.h"

#include <string>
#include <vector>
#include <map>

typedef std::vector<std::vector<std::string>> My2DStr;


namespace Example
{
	class DataManager {
	public:
		DataManager();

		void GetAllFilePaths(const std::string& path);

		void ReadFileRaw(const std::string& path);
		void ReadAllFilesRaw();

		void GetDisData(const std::string& path, int id);
		void GetAllDisData();

		void SendFileDisData(Example::UDPConnection* conn, const std::string& path);
		void SendAllFiles(Example::UDPConnection* conn);

		void Init();
		void Init(const std::string& path);

		

	private:
		// a list of cahced file paths 
		// i.e. "./data/I1.txt"
		std::vector<std::string> filePaths;

		// a map of the file type belongs to DATA_TYPE
		// i.e. "./data/I1.txt" : MISSILE
		std::map<std::string, DATA_TYPE> fileTypes;

		// a map of (each file w/path : file content)
		// i.e. "./data/I1.txt" : 0,0,0", "0,0,0", "0"]
		std::map<
			std::string, 
			std::shared_ptr<My2DStr>
		> rawDataSet;

		// a map of (each file w/path : DISData)
		std::map<
			std::string,
			std::shared_ptr<Example::DISData>
		> disDataSet;
		
		// TODO: use some database i.e. Redis/SQL to handle
		// Data in a more manageable way

	public:
		void PrintFile(const std::string& path);

		void PrintFilePaths();
		void PrintPathsTypes();
		void PrintALL();
	};
}