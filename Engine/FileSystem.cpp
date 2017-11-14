#include "FileSystem.h"
#include "Application.h"
#include "ModuleAssimp.h"
#include <iostream>
#include <filesystem>

namespace fs = std::experimental::filesystem;
void FileSystem::InitFileSystem()
{
	if (CreateDirectory(ASSETS_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		
	}
	if (CreateDirectory(LIBRARY_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{

	}
	if (CreateDirectory(MESHES_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{

	}
	if (CreateDirectory(MATERIALS_PATH, NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{

	}


	std::string path = ASSETS_PATH;
	for (auto & p : fs::directory_iterator(path)) {	

		//GET STRING
		const wchar_t* temp = p.path().c_str();
		std::wstring ws(temp);
		std::string str(ws.begin(), ws.end());
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		//

		//GET EXTENSION
		std::string file_extension = GetExtension(str);		
		//
		if (!strcmp(file_extension.c_str(), "meta"))
			continue;

		//SWITCH
		if (!strcmp(file_extension.c_str(), "fbx")) {
			CreateMeta(str);
		//	App->resources->ImportFile(str.c_str());
		}
		if (!strcmp(file_extension.c_str(), "png") || !strcmp(file_extension.c_str(), "jpg") || !strcmp(file_extension.c_str(), "tga")) {
			CreateMeta(str);
		//	App->resources->ImportFile(str.c_str());
		}

	}
	for (auto & p : fs::directory_iterator(path)) {
		//GET STRING
		const wchar_t* temp = p.path().c_str();
		std::wstring ws(temp);
		std::string str(ws.begin(), ws.end());
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		char* temp_str = (char*)str.c_str();
		NormalizePath(temp_str);
		str = temp_str;

		//

		//GET EXTENSION
		std::string file_extension = GetExtension(str);
		//
		if (!strcmp(file_extension.c_str(), "meta")) {
			ImportFile(str);

		}
	}

}

std::string FileSystem::CreateMeta(std::string path)
{
	std::string name = ASSETS_PATH + GetFileNameExtension(path);
	std::string meta_path = name + ".meta";
	if (ExistsFile(meta_path))
		return meta_path;

	
	
	LPCTSTR nom = path.c_str();
	WIN32_FILE_ATTRIBUTE_DATA    fileInfo;
	GET_FILEEX_INFO_LEVELS FileInfosLevel = GetFileExInfoStandard;
	GetFileAttributesEx(nom, FileInfosLevel, &fileInfo);
	double date_time = fileInfo.ftLastWriteTime.dwLowDateTime;


	Data meta_data;
	meta_data.AddString("Original Path", path);
	meta_data.AddString("Imported Path", "");
	meta_data.AddDouble("Time" ,date_time);
	meta_data.SaveAsJSON(meta_path);

	return meta_path;
}

void FileSystem::ImportFile(std::string meta_path, bool force )
{
	Data meta_data;
	if (meta_data.LoadJSON(meta_path)) {
		std::string original_file = meta_data.GetString("Original Path");
		std::string imported_file = meta_data.GetString("Imported Path");

		if (!ExistsFile(original_file)) {
			if (ExistsFile(imported_file)) {
				remove(imported_file.c_str());
				remove(meta_path.c_str());
				return;
			}

		}
		else {
			UUID obj_uuid = App->resources->ImportFile(original_file.c_str(), force);
			if (!ExistsFile(imported_file) || imported_file == "" || force) {
				Data n_meta_data;
				Resource* res = App->resources->Get(obj_uuid);	

				LPCTSTR nom = original_file.c_str();
				WIN32_FILE_ATTRIBUTE_DATA    fileInfo;
				GET_FILEEX_INFO_LEVELS FileInfosLevel = GetFileExInfoStandard;
				GetFileAttributesEx(nom, FileInfosLevel, &fileInfo);
				double date_time = fileInfo.ftLastWriteTime.dwLowDateTime;


				n_meta_data.AddString("Original Path", res->file);
				n_meta_data.AddString("Imported Path", res->exported_file);
				n_meta_data.AddDouble("Time", date_time);
				
				n_meta_data.SaveAsJSON(meta_path);
			}
		}

	}

}

void FileSystem::CheckFilesUpdates()
{
	std::string path = ASSETS_PATH;
	for (auto & p : fs::directory_iterator(path)) {

		const wchar_t* temp = p.path().c_str();
		std::wstring ws(temp);
		std::string str(ws.begin(), ws.end());
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		//
		double last_time = 0;
		bool save = false;
		double date_time = 0;
		//GET EXTENSION
		
		std::string original_file;
		std::string imported_file;
		std::string file_extension = GetExtension(str);
		//
				if (!strcmp(file_extension.c_str(), "meta")) {
			Data meta_data;
			if (meta_data.LoadJSON(str)) {
				last_time = meta_data.GetDouble("Time");
				original_file = meta_data.GetString("Original Path");
				imported_file = meta_data.GetString("Imported Path");

				LPCTSTR nom = original_file.c_str();
				WIN32_FILE_ATTRIBUTE_DATA    fileInfo;
				GET_FILEEX_INFO_LEVELS FileInfosLevel = GetFileExInfoStandard;
				GetFileAttributesEx(nom, FileInfosLevel, &fileInfo);
				date_time = fileInfo.ftLastWriteTime.dwLowDateTime;

				if (date_time != last_time) {						
					save = true;
				}
					
			}	

		}
				if (save) {
					Data new_meta_data;
					new_meta_data.AddString("Original Path",original_file);
					new_meta_data.AddString("Imported Path", imported_file);
					new_meta_data.AddDouble("Time", date_time);
					new_meta_data.SaveAsJSON(str);
					ImportFile(str,true);

				}

	}
}

