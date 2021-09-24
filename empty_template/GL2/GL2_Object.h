/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <GL/glew.h> // include glew first
#include "../Geometry/TriangularSurface/StaticTriangularSurface.h"
#include <glm\glm.hpp>
#include "GL2_ShaderProgram.h"
#include "GL2_Light.h"
#include "GL2_Material.h"
#include <glm/gtx/component_wise.hpp>
#include "ParticleSystem.h"

class GL2_Object
{
public:
	Array1D<GLuint> vbo_;
	int num_elements_ = 0;

	glm::mat4 m = glm::mat4(1.0f);
	glm::vec4 light_position_;
	glm::vec4 light_product_ambient_;
	glm::vec4 light_product_diffuse_;
	glm::vec4 light_product_specular_;

	glm::vec3 v = glm::vec3(0.0f, 0.0f, -1.0f);
	
	float t_time = 0.0f;
	float r_time = 0.0f;
	float speed = 10.0f;
	float mat_shininess_ = 20.0;

	ParticleSystem ps;
	GL2_Material mat_;

	GL2_Object()
	{

	}
	~GL2_Object()
	{
		glDeleteBuffersARB(vbo_.num_elements_, vbo_.values_);
	}

	void initPhongSurface(const StaticTriangularSurface& surface)
	{
		vbo_.init(3); // vertex, normal, connectivity
		glGenBuffers(vbo_.num_elements_, vbo_.values_);

		updatePhongSurface(surface);
	}

	void initPhongSurfaceWithTexture(const StaticTriangularSurface& surface)
	{
		vbo_.init(4); // vertex, normal, texture coordinates, connectivity
		glGenBuffers(vbo_.num_elements_, vbo_.values_);

		updatePhongSurfaceWithTexture(surface);
	}

	void updatePhongSurface(const StaticTriangularSurface& surface)
	{
		num_elements_ = surface.triangles_.num_elements_ * 3;

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glBufferData(GL_ARRAY_BUFFER, surface.vertex_positions_.num_elements_ * sizeof(float) * 3,
			surface.vertex_positions_.values_, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glBufferData(GL_ARRAY_BUFFER, surface.vertex_normals_.num_elements_ * sizeof(float) * 3,
			surface.vertex_normals_.values_, GL_STATIC_DRAW);

		Array1D<Vector3D<unsigned int> > tri_ix;
		surface.getUnsignedIntTriangles(tri_ix);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_ix.num_elements_ * sizeof(unsigned int) * 3,
			surface.triangles_.values_, GL_STATIC_DRAW);
	}

		void updatePhongSurfaceWithTexture(const StaticTriangularSurface& surface)
	{
		updatePhongSurface(surface);

		// texture coordinates
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[3]);
		glBufferData(GL_ARRAY_BUFFER, surface.vertex_uv_.num_elements_ * sizeof(float) * 2,
			surface.vertex_uv_.values_, GL_STATIC_DRAW);
	}

	void scale(glm::vec3 v)
	{
		m = glm::scale(m, v);
	}
	void translate(glm::vec3 v)
	{
		m = glm::translate(m, v);
	}
	void rotate(float angle, glm::vec3 v)
	{
		m = glm::rotate(m, glm::radians(angle), v);
	}
	void view(glm::mat4 v)
	{
		m = v * m;
	}
	void drawWithShader(const GL2_ShaderProgram& program)
	{
		//glUseProgram(shader_programme);	// activate your shader!

		//TODO: use one 'lightproduct' uniform for amb, dif, and spe.
		program.sendUniform(m, "obj_m");
		program.sendUniform(light_position_, "light_position");
		program.sendUniform(light_product_ambient_, "light_product_ambient");
		program.sendUniform(light_product_diffuse_, "light_product_diffuse");
		program.sendUniform(light_product_specular_, "light_product_specular");
		program.sendUniform(mat_shininess_, "mat_shininess");
		
		// draw here
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer
		(
			0,                  // attribute 0
			3,                  // size (r, g, b)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer
		(
			1,                  // attribute 0
			3,                  // size (r, g, b)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[2]);
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, num_elements_, GL_UNSIGNED_INT, 0);
	}
	void drawWithShaderWithTexture(const GL2_ShaderProgram& program)
	{
		//glUseProgram(shader_programme);	// activate your shader!

		//TODO: use one 'lightproduct' uniform for amb, dif, and spe.
		program.sendUniform(m, "obj_m");
		program.sendUniform(light_position_, "light_position");
		program.sendUniform(light_product_ambient_, "light_product_ambient");
		program.sendUniform(light_product_diffuse_, "light_product_diffuse");
		program.sendUniform(light_product_specular_, "light_product_specular");
		program.sendUniform(mat_shininess_, "mat_shininess");

		// draw here
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer
		(
			0,                  // attribute 0
			3,                  // size (r, g, b)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer
		(
			1,                  // attribute 0
			3,                  // size (r, g, b)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// texture coordinates
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[3]);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer
		(
			2,                  // attribute 2
			2,                  // size (u, v)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[2]);
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, num_elements_, GL_UNSIGNED_INT, 0);
	}

	void drawPhongSurface()
	{
		glUseProgram(0);

		glEnable(GL_LIGHTING);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[2]);
		glPolygonMode(GL_FRONT, GL_LINE);
		glDrawElements(GL_TRIANGLES, num_elements_, GL_UNSIGNED_INT, 0);	

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	void updateTranslate(StaticTriangularSurface &surface, const float dt)
	{
		//if (time == 0.0f)
		//	ps.initiate(angle);

		t_time += dt;



		if (t_time < 0.5)
			surface.translate(dt*v);

		
	}
	void updateRotate(const float dt, int &flag)
	{
		r_time += dt;

		if (r_time < 1.8f)
			rotate(flag*dt*speed, glm::vec3(0.0f, 1.0f, 0.0f));
		else {
			r_time = 0;
			flag = 0;
		}
	}

	void applyLighting(const GL2_Light& light)
	{

		light_product_ambient_ = light.ambient_ * mat_.ambient_ + mat_.emission_;
		light_product_diffuse_ = light.diffuse_ * mat_.diffuse_;
		light_product_specular_ = light.specular_ * mat_.specular_;

		light_position_ = light.position_;
		mat_shininess_ = mat_.shininess_;
	}
};
