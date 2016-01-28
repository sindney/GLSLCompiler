#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

#include "ogl.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;

string loadTextfile(string path);

bool Contains(string input, vector<string> &keywords);

bool CompileShader(bool isVertexShader, const std::string &rawData, const std::string &defineStr);

int main(int argc, char *argv[])
{
	sf::err().rdbuf(NULL);

	sf::ContextSettings settings(0, 0, 0, 4, 3);
	sf::Context context(settings, 0, 0);
	
	if (ogl_LoadFunctions() < 1)
	{
		cout << "Failed to load gl functions" << endl;
		return -1;
	}

	if (argc < 2)
	{
		cout << "Pass shader's absolute path." << endl;
		return -1;
	}

	string filePath = argv[1];
	vector<string> vtxKeywords = { "vs", "vert", "vertex" };
	vector<string> frgKeywords = { "fs", "frag", "fragment", "pixel", "ps" };

	std::transform(filePath.begin(), filePath.end(), filePath.begin(), ::tolower);

	bool isVertexShader = Contains(filePath, vtxKeywords);
	bool isFragmentShader = Contains(filePath, frgKeywords);

	string shaderSource = loadTextfile(filePath);
	string vtxDefines = "#define VERTEX_SHADER\n";
	string frgDefines = "#define FRAGMENT_SHADER\n";

	if (shaderSource.size() == 0)
	{
		cout << "File empty or not found.";
		return -1;
	}

	if (!isVertexShader && !isFragmentShader)
	{
		CompileShader(true, shaderSource, vtxDefines);
		CompileShader(false, shaderSource, frgDefines);
	}
	else 
	{
		CompileShader(isVertexShader, shaderSource, "");
	}

    return 0;
}

string loadTextfile(string path)
{
	string result, line;
	ifstream textfile(path.c_str(), ios::in);

	while(textfile.good())
	{
		getline(textfile, line);
		result.append(line + "\n");
	}

	textfile.close();
	return result;
}

bool Contains(string input, vector<string> &keywords)
{
	for (unsigned int i = 0; i < keywords.size(); i++)
	{
		string keyword = keywords[i];
		size_t found = input.find(keyword);
		if (found != string::npos)
			return true;
	}
	return false;
}

void GetVersionInfo(const std::string &source, std::string &versionStr, std::string &mainStr)
{
	auto index = source.find("#version");
	if (index != std::string::npos)
	{
		auto eol = source.find('\n', index);
		if (eol != std::string::npos)
		{
			versionStr = source.substr(index, eol - index + 1);
		}
	}

	if (versionStr.size() == 0)
	{
		versionStr = "#version 330";
		mainStr = source;
	}
	else
	{
		auto start = index + versionStr.size();
		auto size = source.size() - start;
		mainStr = source.substr(0, index) + source.substr(start, size);
	}
}

bool CompileShader(bool isVertexShader, const std::string &rawData, const std::string &defineStr)
{
	cout << (isVertexShader ? "Vertex Shader " : "Fragment Shader ");

	GLuint shader = glCreateShader(isVertexShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

	if(shader == 0)
	{
		cout << "Failed to create shader context." << endl;
		return false;
	}

	std::string versionStr, mainStr;
	GetVersionInfo(rawData, versionStr, mainStr);
	const char *sources[3] = { versionStr.c_str(), defineStr.c_str(), mainStr.c_str() };
	const int counts[3] = { (int)versionStr.size(), (int)defineStr.size(), (int)mainStr.size() };
	glShaderSource(shader, 3, sources, counts);
	glCompileShader(shader);

	GLint isCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char * logCstr = new char[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, logCstr);

		cout << "Complie Failed!" << endl << logCstr << endl;

		glDeleteShader(shader);
		return false;
	}

	cout << "Compile Success!" << endl;
	glDeleteShader(shader);
	return true;
}