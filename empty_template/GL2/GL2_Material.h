/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <glm/glm.hpp>

class GL2_Material
{
public:
	glm::vec4 ambient_ = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	glm::vec4 diffuse_    = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
	glm::vec4 specular_ = glm::vec4(0.2, 0.2, 0.2, 1.0);
	glm::vec4 emission_  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float shininess_ = 10.0;

	GL2_Material()
	{
	}

	~GL2_Material()
	{
	}

	void setCyan()
	{
		ambient_ = glm::vec4(0.0f, 0.05f, 0.05f, 1.0f);
		diffuse_ = glm::vec4(0.4f, 0.5f, 0.5f, 1.0f);
		specular_ = glm::vec4(0.04f, 0.7f, 0.7f, 1.0f);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 10.0f;
	}
	void setGold()
	{
		// http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html
		ambient_ = glm::vec4(0.24725f, 0.2245f, 0.00645f, 1.0f);
		diffuse_ = glm::vec4(0.8f, 0.4f, 0.001f, 1.0f);
		specular_ = glm::vec4(0.797357f, 0.523991f, 0.00208006f, 1.0f);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 83.2;
	}
	void setRuby()
	{
		ambient_ = glm::vec4(0.1745f, 0.01175f, 0.01175f, 0.55f);
		diffuse_ = glm::vec4(0.61424f, 0.04136f, 0.04136f, 0.55f);
		specular_ = glm::vec4(0.727811f, 0.626959f, 0.626959f, 0.55f);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 2.8f;

	}
	void setBrass()
	{
		ambient_ = glm::vec4(0.329412f, 0.223529f, 0.027451f, 1.0f);
		diffuse_ = glm::vec4(0.780392f, 0.568627f, 0.113725f, 1.0f);
		specular_ = glm::vec4(0.992157f, 0.941176f, 0.807843f, 1.0f);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 27.8974f;
	}
	void setPerl()
	{
		ambient_ = glm::vec4(0.25f, 0.20725f, 0.20725f, 0.922f);
		diffuse_ = glm::vec4(1.0f, 0.829f, 0.829f, 0.922f);
		specular_ = glm::vec4(0.296648f, 0.296648f, 0.296648f, 0.922f);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 11.264f;
	}
	void setEmerald()
	{
		ambient_ = glm::vec4(0.0215f, 0.1745f, 0.0215f, 0.55f);
		diffuse_ = glm::vec4(0.07568f, 0.61424f, 0.07568f, 0.55f);
		specular_ = glm::vec4(0.633f, 0.727811f, 0.633f, 0.55f);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 76.8f;
	}
	void setCopper()
	{
		ambient_ = glm::vec4 (0.19125f, 0.0735f, 0.0225f, 1.0f);
		diffuse_ = glm::vec4(0.7038f, 0.27048f, 0.0828f, 1.0f);
		specular_ = glm::vec4(0.256777f, 0.137622f, 0.086014f, 1.0f);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 12.8f;
	}
	void setYellowpastic()
	{
		ambient_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		diffuse_ = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		specular_ = glm::vec4(0.60f, 0.60f, 0.50f, 1.0f);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 32.0f;
	}
	void setGreenpastic()
	{
		ambient_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		diffuse_ = glm::vec4(0.1f, 0.35f, 0.1f, 1.0f);
		specular_ = glm::vec4(0.45f, 0.55f, 0.45f, 1.0f);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 32.0f;
	}
};
