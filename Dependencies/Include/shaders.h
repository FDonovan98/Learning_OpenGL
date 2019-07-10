#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
    public:
        unsigned int ID;

        Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
            string vertexCode;
            string fragmentCode;
            ifstream vSFile;
            ifstream fSFile;

            vSFile.exceptions (ifstream::failbit | ifstream::badbit);
            fSFile.exceptions (ifstream::failbit | ifstream::badbit);

            //load files contents into strings
            try {
                vSFile.open(vertexPath);
                fSFile.open(fragmentPath);
                stringstream vSStream, fSStream;

                vSStream << vSFile.rdbuf();
                fSStream << fSFile.rdbuf();

                vSFile.close();
                fSFile.close();

                vertexCode = vSStream.str();
                fragmentCode = fSStream.str();
            }
            catch (ifstream::failure e) {
                cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            }
            const char* vSCode = vertexCode.c_str();
            const char* fSCode = fragmentCode.c_str();

            //compile vertex shader
            unsigned int vShader, fShader;
            int success;
            char infoLog[512];

            vShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vShader, 1, &vSCode, NULL);
            glCompileShader(vShader);

            //check vShader compiled corectly
            glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vShader, 512, NULL, infoLog);
                cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
            }

            //compile fragment shader
            fShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fShader, 1, &fSCode, NULL);
            glCompileShader(fShader);

            //check fShader compiled correctly
            glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fShader, 512, NULL, infoLog);
                cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
            }

            //setup sProgram
            ID = glCreateProgram();
            glAttachShader(ID, vShader);
            glAttachShader(ID, fShader);
            glLinkProgram(ID);

            //check sProgram linked correctly
            glGetShaderiv(ID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                cout << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n" << infoLog << "\n";
            }

            //cleanup
            glDeleteShader(vShader);
            glDeleteShader(fShader);
        }

        void use() {
            glUseProgram(ID);
        }

        void setBool(const string &name, bool value) {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        };

        void setInt(const string &name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        };

        void setFloat(const string &name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        };

        void setMat4(const string &name, glm::mat4 &mat) {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
};

#endif