#include "ShaderProgram.h"
CShaderProgram::~CShaderProgram()
{
	if (!m_IsInitialized)
		return;
	Stop();
	glDetachShader(m_ProgramID, m_VertexShaderID);
	glDetachShader(m_ProgramID, m_FragmentShaderID);
	glDeleteShader(m_VertexShaderID);
	glDeleteShader(m_FragmentShaderID);

	if (m_GeometryShader)
	{
		glDetachShader(m_ProgramID, m_GeometryShaderID);
		glDeleteShader(m_GeometryShaderID);
	}
	glDeleteProgram(m_ProgramID);
}
void CShaderProgram::InitShaderProgram(char* vertex_shader_file, char* fragment_shader_file)
{
	m_VertexShaderID = LoadShader(vertex_shader_file, GL_VERTEX_SHADER);
	m_FragmentShaderID = LoadShader(fragment_shader_file, GL_FRAGMENT_SHADER);
	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, m_VertexShaderID);
	glAttachShader(m_ProgramID, m_FragmentShaderID);
	BindAttributes();
	glLinkProgram(m_ProgramID);
	glValidateProgram(m_ProgramID);
	m_IsInitialized = true;
}
void CShaderProgram::InitShaderProgram(char * vertex_shader_file, char * fragment_shader_file, char * geometry_shader_file)
{
	m_VertexShaderID = LoadShader(vertex_shader_file, GL_VERTEX_SHADER);
	m_GeometryShaderID = LoadShader(geometry_shader_file, GL_GEOMETRY_SHADER);
	m_FragmentShaderID = LoadShader(fragment_shader_file, GL_FRAGMENT_SHADER);
	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, m_VertexShaderID);
	glAttachShader(m_ProgramID, m_GeometryShaderID);
	glAttachShader(m_ProgramID, m_FragmentShaderID);
	BindAttributes();
	glLinkProgram(m_ProgramID);
	glValidateProgram(m_ProgramID);
	m_IsInitialized = true;
	m_GeometryShader = true;
}
void CShaderProgram::InitShaderProgram(char * vertex_shader_file, char * fragment_shader_file, char * tesselation_shader_file, char * tesselation_evaluation_shader_file)
{	
	m_VertexShaderID = LoadShader(vertex_shader_file, GL_VERTEX_SHADER);
	m_FragmentShaderID = LoadShader(fragment_shader_file, GL_FRAGMENT_SHADER);
	m_TesselationControlShaderID = LoadShader(tesselation_shader_file, GL_TESS_CONTROL_SHADER);
	m_TesselationEvaluationShaderID = LoadShader(tesselation_evaluation_shader_file, GL_TESS_EVALUATION_SHADER);
	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, m_VertexShaderID);
	glAttachShader(m_ProgramID, m_FragmentShaderID);
	glAttachShader(m_ProgramID, m_TesselationControlShaderID);
	glAttachShader(m_ProgramID, m_TesselationEvaluationShaderID);
	BindAttributes();
	glLinkProgram(m_ProgramID);
	glValidateProgram(m_ProgramID);
	m_IsInitialized = true;
}
void CShaderProgram::LoadFile(const char* fn, std::string& str)
{
    std::ifstream in(fn);
        if(!in.is_open())
        {
            std::cout << "The file " << fn << " cannot be opened\n";
            return;
        }
        char tmp[300];
        while(!in.eof())
        {
            in.getline(tmp,300);
            str+=tmp;
            str+='\n';
        }
        in.close();
	/*std::ifstream in;
	in.exceptions(ifstream::failbit | std::ifstream::badbit);
	try {
		in.open(fn);
		char tmp[300];
		while (!in.eof())
		{
			in.getline(tmp, 300);
			str += tmp;
			str += '\n';
			cout << tmp <<endl ;
		}
		in.close();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "The file " << fn << " exceptions in reading: \n" << e.what() << endl;
	}*/
}

unsigned int CShaderProgram::LoadShader(char *filename, unsigned int mode)
{
	std::string source;
	LoadFile(filename, source);

	unsigned int id;
	id = glCreateShader(mode);

	const char* csource = source.c_str();

	glShaderSource(id, 1, &csource, NULL);
	glCompileShader(id);
	char error[1000];
	int length = 0;
	glGetShaderInfoLog(id, 1000, &length, error);
	GLint compiled = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cout << "\n\n[Error] ERRORS in Shader! \nFile name:\t" << filename << "\nCompile status: \n\n" << error << std::endl;
	}
	if (id == GL_FALSE)
	{
		system("pause");
		exit(0);
	}
	return id;
}
void CShaderProgram::Start() const
{
	if(m_IsInitialized)
	glUseProgram(m_ProgramID);
}
void CShaderProgram::Stop() const 
{
	glUseProgram(0);
}
int CShaderProgram::GetUniformLocation(const string& uniformName) const
{
	return glGetUniformLocation(m_ProgramID, uniformName.c_str());
}
void CShaderProgram::BindAttribute(int attribute, const string& variableName) const
{
	glBindAttribLocation(m_ProgramID, attribute, variableName.c_str());
}
void CShaderProgram::LoadValue(unsigned int loacation, const glm::mat4& value) const 
{
	glUniformMatrix4fv(loacation, 1, GL_FALSE, glm::value_ptr(value));
}
void CShaderProgram::LoadValue(unsigned int loacation, const glm::mat3& value) const 
{
	glUniformMatrix3fv(loacation, 1, GL_FALSE, glm::value_ptr(value));
}

void CShaderProgram::LoadValue(unsigned int loacation, const float& value) const 
{
	glUniform1f(loacation, value);
}

void CShaderProgram::LoadValue(unsigned int loacation, const int& value) const 
{
	glUniform1i(loacation, value);
}

void CShaderProgram::LoadValue(unsigned int loacation, const glm::vec2& value) const
{
	glUniform2fv(loacation, 1, glm::value_ptr(value));
}

void CShaderProgram::LoadValue(unsigned int loacation, const glm::vec3& value) const 
{
	glUniform3fv(loacation, 1, glm::value_ptr(value));
}

void CShaderProgram::LoadValue(unsigned int loacation, const glm::vec4& value) const 
{
	glUniform4fv(loacation, 1, glm::value_ptr(value));
}
