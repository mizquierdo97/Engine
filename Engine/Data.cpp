#include "Data.h"
#include <algorithm>

Data::Data()
{
}


Data::~Data()
{
}

void Data::SaveAsXML(std::string path)
{
	if (path.find(".xml") == std::string::npos) { //xml extension is not especified
		path += ".xml";
	}
	std::ofstream file(path);
	cereal::XMLOutputArchive archive(file);
	for (int i = 0; i < data_names.size(); i++) {
		if (data_names[i] == "New_Section") {
			std::replace(data_names[i + 1].begin(), data_names[i + 1].end(), ' ', '_');
			archive.setNextName(data_names[i + 1].c_str());
			archive.startNode();
			i++;
		}
		else if (data_names[i] == "Section_Close") {
			archive.finishNode();
		}
		else {
			archive(cereal::make_nvp(data_names[i], data_values[i]));
		}
	}
}

bool Data::LoadXML(std::string path)
{
	bool ret = false;
	bool outsideSection = true;
	int sectionsOpen = 0;
	data_names.clear();
	data_values.clear();
	std::ifstream file(path);
	if (file.is_open()) {
		cereal::XMLInputArchive archive(file);
		while (true) {
			std::string nodeName;
			std::string nodeValue;
			if (archive.getNodeName() != NULL) {
				nodeName = archive.getNodeName();
			}
			else {
				if (sectionsOpen == 1) {
					outsideSection = true;
				}
				if (archive.isLastNode()) {
					break;
				}
				else {
					data_names.push_back("Section_Close");
					data_values.push_back("");
					archive.finishNode();
					sectionsOpen--;
					continue;
				}
			}
			archive.startNode();
			archive.loadValue(nodeValue);
			data_names.push_back(nodeName);
			if (nodeValue != "") {
				data_values.push_back(nodeValue);
				if (outsideSection) {
					out_section_values.push_back(nodeValue);
					out_section_names.push_back(nodeName);
				}
			}
			else {
				data_values.push_back("Section");
				outsideSection = false;
				sectionsOpen++;
				continue;
			}
			archive.finishNode();
		}
		ret = true;
	}
	return ret;
}

void Data::SaveAsJSON(std::string path)
{
	if (path.find(".json") == std::string::npos) { //json extension is not especified
		//path += ".json";
	}
	std::ofstream file(path);
	cereal::JSONOutputArchive archive(file);
	for (int i = 0; i < data_names.size(); i++) {
		if (data_names[i] == "New_Section") {
			std::replace(data_names[i + 1].begin(), data_names[i + 1].end(), ' ', '_');
			archive.setNextName(data_names[i + 1].c_str());
			archive.startNode();
			i++;
		}
		else if (data_names[i] == "Section_Close") {
			archive.finishNode();
		}
		else {
			archive(cereal::make_nvp(data_names[i], data_values[i]));
		}
	}
}

bool Data::LoadJSON(std::string path)
{
	bool ret = false;
	bool outsideSection = true;
	int sectionsOpen = 0;
	data_names.clear();
	data_values.clear();
	std::ifstream file(path);
	if (file.is_open()) {
		cereal::JSONInputArchive archive(file);
		while (true) {
			std::string nodeName;
			std::string nodeValue;
			if (archive.isObject()) {
				if (archive.isEmptyObject()) {
					nodeName = archive.getNodeName();
					data_names.push_back(nodeName);
					data_values.push_back("Section");
					archive.startNode();
					data_names.push_back("Section_Close");
					data_values.push_back("");					
					archive.finishNode();
					continue;
				}
				else {
					if (archive.getNodeName() != NULL) {
						nodeName = archive.getNodeName();
						data_names.push_back(nodeName);
						data_values.push_back("Section");
						archive.startNode();
						outsideSection = false;
						sectionsOpen++;
					}
					else {
						data_names.push_back("Section_Close");
						data_values.push_back("");
						sectionsOpen--;
						if (sectionsOpen == 0) {
							outsideSection = true;
						}
						if (archive.finishNode())
						{
							break;
						}
					}
				}
			}
			else {
				if (archive.getNodeName() != NULL) {
					nodeName = archive.getNodeName();
					archive.loadValue(nodeValue);
					data_names.push_back(nodeName);
					data_values.push_back(nodeValue);
					if (outsideSection) {
						out_section_values.push_back(nodeValue);
						out_section_names.push_back(nodeName);
					}
				}
				else {
					data_names.push_back("Section_Close");
					sectionsOpen--;
					data_values.push_back("");
					if (sectionsOpen == 0) {
						outsideSection = true;
					}
					if (archive.finishNode())
					{
						break;
					}
				}
			}
		}
		ret = true;
	}
	return ret;
}

void Data::SaveAsBinary(std::string path)
{
	std::ofstream file(path);
	cereal::BinaryOutputArchive archive(file);
	archive(*this);
}

bool Data::LoadBinary(std::string path)
{
	bool ret = false;
	bool outsideSection = true;
	int sectionsOpen = 0;
	data_names.clear();
	data_values.clear();
	std::ifstream file(path);
	if (file.is_open()) {
		cereal::BinaryInputArchive archive(file);
		archive(*this);
		for (int i = 0; i < data_names.size(); i++) {
			if (data_names[i] == "New_Section") {
				data_names.erase(data_names.begin() + i);
				data_values.erase(data_values.begin() + i);
				outsideSection = false;
				sectionsOpen++;
			}
			else if (data_names[i] == "Section_Close") {
				sectionsOpen--;
				if (sectionsOpen == 0) {
					outsideSection = true;
				}
			}
			else {
				if (outsideSection) {
					out_section_names.push_back(data_names[i]);
					out_section_values.push_back(data_values[i]);
				}
			}
		}
		ret = true;
	}
	return ret;
}

void Data::ClearData()
{
	data_names.clear();
	data_values.clear();
}

void Data::CreateSection(std::string sectionName)
{
	data_names.push_back("New_Section");
	data_values.push_back("");
	data_names.push_back(sectionName);
	data_values.push_back("Section");
}

bool Data::EnterSection(std::string sectionName)
{
	bool ret = false;
	getting_from_section = true;

	std::vector<std::string>::iterator close_it = find(data_names.begin() + current_index+1, data_names.end(), "Section_Close");
	std::vector<std::string>::iterator it = find(data_names.begin() + current_index, data_names.end(), sectionName);
		
	
	int first_index = 0;
	if (last_index.size()) {
		first_index = last_index.back()+1;
	}
	int close_index = first_index;

	close_index = GetLastIndex(close_index);
		
	int index = it - data_names.begin();


	if (close_index < index || index < first_index)
		return false;

	if (it != data_names.end()) {
		int index = it - data_names.begin();
		if (data_values[index] == "Section") {
			last_index.push_back(index);
			last_index_name.push_back(sectionName);
			current_index = index;
			in_section_values.clear();
			in_section_names.clear();
			sections_open++;
			int internalSectionsOpen = 1; //Avoid getting data from other sections inside this one
			bool is_vector = false;
			for (int i = index + 1; internalSectionsOpen > 0; i++) {
				if (data_values[i] == "Section") {
					if (data_names[i + 1] != "value_X" && data_names[i + 2] != "value_Y") { //std::vectors are not new sections
						internalSectionsOpen++;
					}
					else {
						is_vector = true;
					}
				}
				if (data_names[i] == "Section_Close") {
					if (!is_vector) {
						internalSectionsOpen--;
					}
					else {
						is_vector = false;
					}
				}
				
					in_section_values.push_back(data_values[i]);
					in_section_names.push_back(data_names[i]);
				
			}
			//current_index++;
			ret = true;
		}
	}
	return ret;
}

void Data::LeaveSection() {
	in_section_values.clear();
	in_section_names.clear();
	sections_open--;
	if (!last_index.empty() && !last_index_name.empty()) {
		last_index.pop_back();
		last_index_name.pop_back();
	}
	if (sections_open == 0) {
		getting_from_section = false;
	}

	
	
}


int Data::GetLastIndex(int n)
{
	int num_sections = 0;
	while (num_sections >= 0) {

		if (!strcmp(data_values[n].c_str(), "Section")) {
			num_sections++;
		}

		if (!strcmp(data_names[n].c_str(), "Section_Close")) {
			num_sections--;
		}
		if (num_sections >= 0)
			n++;
	}
	return n;
}

void Data::CloseSection()
{
	data_names.push_back("Section_Close");
	data_values.push_back("");
}

bool Data::GetBool(std::string valueName)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		if (vec_values[index] == "1") return true;
	}

	return false;
}

int Data::GetInt(std::string valueName)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		return stoi(vec_values[index]);
	}
	return -1;
}

uint Data::GetUInt(std::string valueName)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		return stoul(vec_values[index]);
	}
	return 0;
}

double Data::GetDouble(std::string valueName)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		return stod(vec_values[index]);
	}
	return -1.0;
}

float Data::GetFloat(std::string valueName)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		return stof(vec_values[index]);
	}
	return -1.0f;
}

std::string Data::GetString(std::string valueName)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		return vec_values[index];
	}
	
	return "value not found";
}

ImVec2 Data::GetVector2(std::string valueName)
{
	ImVec2 ret;
	std::replace(valueName.begin(), valueName.end(), ' ', '_');

	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		ret.x = stof(vec_values[index + 1]);
		ret.y = stof(vec_values[index + 2]);
	}
	else {
		ret.x = -1.0f;
		ret.y = -1.0f;
	}
	
	return ret;
}

ImVec3 Data::GetVector3(std::string valueName)
{
	ImVec3 ret;
	std::replace(valueName.begin(), valueName.end(), ' ', '_');

	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		ret.x = stof(vec_values[index + 1]);
		ret.y = stof(vec_values[index + 2]);
		ret.z = stof(vec_values[index + 3]);
	}
	else {
		ret.x = -1.0f;
		ret.y = -1.0f;
		ret.z = -1.0f;
	}
	
	return ret;
}

ImVec4 Data::GetVector4(std::string valueName)
{
	ImVec4 ret;
	std::replace(valueName.begin(), valueName.end(), ' ', '_');

	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		ret.x = stof(vec_values[index + 1]);
		ret.y = stof(vec_values[index + 2]);
		ret.z = stof(vec_values[index + 3]);
		ret.w = stof(vec_values[index + 4]);
	}
	else {
		ret.x = -1.0f;
		ret.y = -1.0f;
		ret.z = -1.0f;
		ret.w = -1.0f;
	}
	
	return ret;
}

float2 Data::GetVector2f(std::string valueName)
{
	float2 ret;
	std::replace(valueName.begin(), valueName.end(), ' ', '_');

	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		ret.x = stof(vec_values[index + 1]);
		ret.y = stof(vec_values[index + 2]);
	}
	else {
		ret.x = -1.0f;
		ret.y = -1.0f;
	}
	
	return ret;
}

float3 Data::GetVector3f(std::string valueName)
{
	float3 ret;
	std::replace(valueName.begin(), valueName.end(), ' ', '_');

	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		ret.x = stof(vec_values[index + 1]);
		ret.y = stof(vec_values[index + 2]);
		ret.z = stof(vec_values[index + 3]);
	}
	else {
		ret.x = -1.0f;
		ret.y = -1.0f;
		ret.z = -1.0f;
	}
	
	return ret;
}

float4 Data::GetVector4f(std::string valueName)
{
	float4 ret;
	std::replace(valueName.begin(), valueName.end(), ' ', '_');

	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		ret.x = stof(vec_values[index + 1]);
		ret.y = stof(vec_values[index + 2]);
		ret.z = stof(vec_values[index + 3]);
		ret.w = stof(vec_values[index + 4]);
	}
	else {
		ret.x = -1.0f;
		ret.y = -1.0f;
		ret.z = -1.0f;
		ret.w = -1.0f;
	}

	return ret;
}

void Data::ChangeString(std::string valueName, std::string value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		vec_values[index] = value;
		data_values[index] = value;
	}

	
}

void Data::ChangeDouble(std::string valueName, double value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	std::vector<std::string> vec_names;
	std::vector<std::string> vec_values;

	if (getting_from_section) {
		vec_names = in_section_names;
		vec_values = in_section_values;
	}
	else {
		vec_names = out_section_names;
		vec_values = out_section_values;
	}

	std::vector<std::string>::iterator it = find(vec_names.begin(), vec_names.end(), valueName);
	if (it != vec_names.end()) {
		int index = it - vec_names.begin();
		vec_values[index] = std::to_string(value);
		data_values[index] = std::to_string(value);
	}
	
}


void Data::AddBool(std::string valueName, bool value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	data_values.push_back(std::to_string(value));
	data_names.push_back(valueName);
}

void Data::AddInt(std::string valueName, int value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	data_values.push_back(std::to_string(value));
	data_names.push_back(valueName);
}

void Data::AddUInt(std::string valueName, uint value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	data_values.push_back(std::to_string(value));
	data_names.push_back(valueName);
}

void Data::AddDouble(std::string valueName, double value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	data_values.push_back(std::to_string(value));
	data_names.push_back(valueName);
}

void Data::AddFloat(std::string valueName, float value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	data_values.push_back(std::to_string(value));
	data_names.push_back(valueName);
}

void Data::AddString(std::string valueName, std::string value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	data_values.push_back(value);
	data_names.push_back(valueName);
}

void Data::AddVector2(std::string valueName, ImVec2 value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	CreateSection(valueName);
	data_values.push_back(std::to_string(value.x));
	data_names.push_back("value_X");
	data_values.push_back(std::to_string(value.y));
	data_names.push_back("value_Y");
	CloseSection();
}

void Data::AddVector3(std::string valueName, ImVec3 value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	CreateSection(valueName);
	data_values.push_back(std::to_string(value.x));
	data_names.push_back("value_X");
	data_values.push_back(std::to_string(value.y));
	data_names.push_back("value_Y");
	data_values.push_back(std::to_string(value.z));
	data_names.push_back("value_Z");
	CloseSection();
}

void Data::AddVector4(std::string valueName, ImVec4 value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	CreateSection(valueName);
	data_values.push_back(std::to_string(value.x));
	data_names.push_back("value_X");
	data_values.push_back(std::to_string(value.y));
	data_names.push_back("value_Y");
	data_values.push_back(std::to_string(value.z));
	data_names.push_back("value_Z");
	data_values.push_back(std::to_string(value.w));
	data_names.push_back("value_W");
	CloseSection();
}

void Data::AddVector2(std::string valueName, float2 value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	CreateSection(valueName);
	data_values.push_back(std::to_string(value.x));
	data_names.push_back("value_X");
	data_values.push_back(std::to_string(value.y));
	data_names.push_back("value_Y");
	CloseSection();
}

void Data::AddVector3(std::string valueName, float3 value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	CreateSection(valueName);
	
	

	data_values.push_back(std::to_string(FloatPrecision(value.x,2)));
	data_names.push_back("value_X");
	data_values.push_back(std::to_string(FloatPrecision(value.y, 2)));
	data_names.push_back("value_Y");
	data_values.push_back(std::to_string(FloatPrecision(value.z, 2)));
	data_names.push_back("value_Z");
	CloseSection();
}

void Data::AddVector4(std::string valueName, float4 value)
{
	std::replace(valueName.begin(), valueName.end(), ' ', '_');
	CreateSection(valueName);
	data_values.push_back(std::to_string(value.x));
	data_names.push_back("value_X");
	data_values.push_back(std::to_string(value.y));
	data_names.push_back("value_Y");
	data_values.push_back(std::to_string(value.z));
	data_names.push_back("value_Z");
	data_values.push_back(std::to_string(value.w));
	data_names.push_back("value_W");
	CloseSection();
}