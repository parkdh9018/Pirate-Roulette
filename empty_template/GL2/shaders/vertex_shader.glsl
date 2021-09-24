/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/
#version 330

//uniform mat4 gl_ModelViewMatrix;
//uniform mat4 gl_ProjectionMatrix;
uniform mat4 mvp;
uniform mat4 obj_m;

in vec3 a_pos;
in vec3 a_nor;
in vec2 a_uv;


out vec4 v;
out vec4 N_from_vshader;
out vec2 uv;

void main()
{
	//v = gl_ModelViewMatrix * gl_ProjectionMatrix * vec4(a_pos, 1.0);
	//N = normalize(gl_ModelViewMatrix * gl_ProjectionMatrix * vec4(a_nor, 0.0f));
	v = mvp * obj_m * vec4(a_pos, 1.0);
	//N = normalize(mvp * vec4(a_nor, 0.0f));
	N_from_vshader = vec4(a_nor, 0.0f);

	gl_Position = v;
	uv = a_uv;
};