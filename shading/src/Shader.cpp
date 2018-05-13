#include "Shader.h"
#include "Model.h"
#include "Light.h"
#include <iostream>
#include <fstream>

Shader::Shader(std::string shaderName) {
	//set name
	_shaderName = shaderName;
	//build it
	_build();
}

Shader::~Shader() {

}

inline bool fileExists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

void Shader::_build() {
    unsigned int vertexSH;
    unsigned int fragmentSH;
    unsigned int geomSH;

    std::string fileNameVert = SHADERS_LOCATION + _shaderName;
    fileNameVert.append(".vert");
    std::string fileNameFrag = SHADERS_LOCATION + _shaderName;
    fileNameFrag.append(".frag");
    std::string fileNameGeom = SHADERS_LOCATION + _shaderName;
    fileNameGeom.append(".geom");

    //Compile each shader
    if (fileExists(fileNameVert)) {
        vertexSH = _compile((char*)fileNameVert.c_str(), GL_VERTEX_SHADER);
    }
    else {
        vertexSH = -1;
    }
    if (fileExists(fileNameFrag)) {
        fragmentSH = _compile((char*)fileNameFrag.c_str(), GL_FRAGMENT_SHADER);
    }
    else {
        fragmentSH = -1;
    }
    if (fileExists(fileNameGeom)) {
        geomSH = _compile((char*)fileNameGeom.c_str(), GL_GEOMETRY_SHADER);
    }
    else {
        geomSH = -1;
    }

    //Link shaders
    _link(vertexSH, fragmentSH, geomSH);
}

void Shader::_link(unsigned int vertexSH, unsigned int fragmentSH, unsigned int geomSH) {
    _shaderContext = glCreateProgram();

    if (vertexSH != -1) {
        glAttachShader(_shaderContext, vertexSH);
    }
    if (fragmentSH != -1) {
        glAttachShader(_shaderContext, fragmentSH);
    }
    if (geomSH != -1) {
        glAttachShader(_shaderContext, geomSH);
    }

    glLinkProgram(_shaderContext);

    GLint      successfully_linked = 0;
    glGetProgramiv(_shaderContext, GL_LINK_STATUS, &successfully_linked);

    // Exit if the program couldn't be linked correctly
    if (!successfully_linked) {
        GLint errorLoglength;
        GLint actualErrorLogLength;
        //Attempt to get the length of our error log.
        glGetProgramiv(_shaderContext, GL_INFO_LOG_LENGTH, &errorLoglength);

        std::cout << errorLoglength << std::endl;

        //Create a buffer to read compilation error message
        char* errorLogText = (char*)malloc(sizeof(char) * errorLoglength);

        //Used to get the final length of the log.
        glGetProgramInfoLog(_shaderContext, errorLoglength, &actualErrorLogLength, errorLogText);

        std::cout << actualErrorLogLength << std::endl;

        // Display errors.
        std::cout << errorLogText << std::endl;

        // Free the buffer malloced earlier
        free(errorLogText);

        std::cout << "Program was not linked correctly!" << std::endl;
    }
}

// Loading shader function
unsigned int Shader::_compile(char* filename, unsigned int type)
{
    FILE *pfile;
    unsigned int handle;
    const GLchar* files[1];

    // shader Compilation variable
    GLint result;				// Compilation code result
    GLint errorLoglength;
    char* errorLogText;
    GLsizei actualErrorLogLength;

    char buffer[400000];
    memset(buffer, 0, 400000);

    errno_t err = fopen_s(&pfile, filename, "rb");
    if (err != 0) {
        printf("Sorry, can't open file: '%s'.\n", filename);
        return 0;
    }

    fread(buffer, sizeof(char), 400000, pfile);

    fclose(pfile);

    handle = glCreateShader(type);
    if (!handle) {
        //We have failed creating the vertex shader object.
        printf("Failed creating vertex shader object from file: %s.", filename);
        return 0;
    }

    files[0] = (const GLchar*)buffer;
    glShaderSource(
        handle, //The handle to our shader
        1, //The number of files.
        files, //An array of const char * data, which represents the source code of theshaders
        nullptr);

    glCompileShader(handle);

    //Compilation checking.
    glGetShaderiv(handle, GL_COMPILE_STATUS, &result);

    // If an error was detected.
    if (!result) {
        //We failed to compile.
        printf("Shader '%s' failed compilation.\n", filename);

        //Attempt to get the length of our error log.
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &errorLoglength);

        //Create a buffer to read compilation error message
        errorLogText = (char*)malloc(sizeof(char) * errorLoglength);

        //Used to get the final length of the log.
        glGetShaderInfoLog(handle, errorLoglength, &actualErrorLogLength, errorLogText);

        // Display errors.
        printf("%s\n", errorLogText);

        // Free the buffer malloced earlier
        free(errorLogText);
    }

    return handle;
}

GLint Shader::getShaderContext() {
    return _shaderContext;
}