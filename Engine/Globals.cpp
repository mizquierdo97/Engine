#pragma once
#include "Globals.h"

#include "Application.h"
void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);
	if (App) {
		App->gui->console.AddLog(tmp_string);
	}
}

std::string GetExtension(std::string path) 
{
	std::transform(path.begin(), path.end(), path.begin(), ::tolower);
	std::string str_copy = path;
	int length = 0;
	while (path.back() != '.')
	{
		path.pop_back();
		length++;
	}

	std::string file_extension = str_copy.substr(str_copy.length() - length, length);

	return file_extension;
}

double FloatPrecision(double value, double precision)
{
	return (floor((value * pow(10, precision) + 0.5)) / pow(10, precision));
}

std::string GetFileName(std::string path)
{
	std::transform(path.begin(), path.end(), path.begin(), ::tolower);
	std::string str_path = path;
	std::string str_temp;
	int length = 0;
	int extension_length = 0;
	while (str_path.back() != '.')
	{
		str_path.pop_back();
		length++;
		extension_length++;
	}
	str_path.pop_back();
	length++;
	extension_length++;

	while (str_path.back() != '\\' && str_path.back() != '/')
	{
		str_path.pop_back();
		length++;
		if (length >= path.length())
			break;
	}
	
	str_temp = path.substr(path.length() - length, length- extension_length);

	return str_temp;
	
}

std::string GetFileNameExtension(std::string path)
{
	std::transform(path.begin(), path.end(), path.begin(), ::tolower);
	std::string str_path = path;
	std::string str_temp;
	int length = 0;
	int extension_length = 0;
	
	while (str_path.back() != '\\' && str_path.back() != '/')
	{
		str_path.pop_back();
		length++;
		if (length >= path.length())
			break;
	}

	str_temp = path.substr(path.length() - length, length - extension_length);

	return str_temp;

	
}

bool ExistsFile(const std::string & name)
{
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

void NormalizePath(char * full_path)
{
	int len = strlen(full_path);
	for (int i = 0; i < len; ++i)
	{
		if (full_path[i] == '\\')
			full_path[i] = '/';
		else
			full_path[i] = tolower(full_path[i]);
	}
}