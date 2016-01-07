#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

#include "gl_core_3_3.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;

string loadTextfile(string path);

bool Contains(string input, vector<string> &keywords);

bool CompileShader(bool isVertexShader, const char* raw, int rawSize, const char* defines, int definesSize);

int main(int argc, char *argv[])
{
	sf::err().rdbuf(NULL);

	sf::ContextSettings settings(0, 0, 0, 3, 3);
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
	vector<string> vtxKeywords = { "vs", "vertex" };
	vector<string> frgKeywords = { "fs", "fragment", "pixel", "ps" };

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
		CompileShader(true, shaderSource.c_str(), shaderSource.size(), vtxDefines.c_str(), vtxDefines.size());
		CompileShader(false, shaderSource.c_str(), shaderSource.size(), frgDefines.c_str(), frgDefines.size());
	}
	else 
	{
		CompileShader(isVertexShader, shaderSource.c_str(), shaderSource.size(), 0, 0);
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
	for (const auto &key : keywords)
	{
		size_t found = input.find(key);
		if (found != string::npos)
			return true;
	}
	return false;
}

bool CompileShader(bool isVertexShader, const char* raw, int rawSize, const char* defines, int definesSize)
{
	cout << (isVertexShader ? "Vertex Shader " : "Fragment Shader ");

	GLuint shader = glCreateShader(isVertexShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

	if (definesSize > 0)
	{
		const char *sources[2] = { defines, raw };
		const int counts[2] = { definesSize, rawSize };
		glShaderSource(shader, 2, sources, counts);
	}
	else
	{
		const int counts[1] = { rawSize };
		glShaderSource(shader, 1, &raw, counts);
	}
	
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