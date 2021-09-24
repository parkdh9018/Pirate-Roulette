/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#version 330

uniform mat4 mvp;
uniform vec4 light_position;
uniform vec4 light_product_ambient;
uniform vec4 light_product_diffuse;
uniform vec4 light_product_specular;
uniform float mat_shininess;

uniform sampler2D wood;
uniform sampler2D dark_steel;
uniform sampler2D wood_floor;
uniform sampler2D water;
uniform float location;

in vec4 v;
in vec4 N_from_vshader;
in vec2 uv; // from vertex shader

out vec4 frag_color;

// https://www.khronos.org/registry/OpenGL/specs/gl/GLSLangSpec.1.50.pdf

float mip_map_level(in vec2 texture_coordinate) // in texel units
{
    vec2  dx_vtc        = dFdx(texture_coordinate);
    vec2  dy_vtc        = dFdy(texture_coordinate);
    float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
    float mml = 0.5 * log2(delta_max_sqr);
    return max( 0, mml ); // Thanks @Nims
}
void main()
{
	vec4 N = N_from_vshader;
	//N.x = sin(uv.x*100.0f);
	N = mvp * normalize(N);

	vec3 L = (normalize(light_position.xyz - v.xyz)).xyz;
	vec3 E = (normalize(-v.xyz)).xyz;	// we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = (normalize(-reflect(L.xyz, N.xyz))).xyz;

	//calculate Ambient Term:  
	vec4 Iamb = light_product_ambient;

	//calculate Diffuse Term:  
	vec4 Idiff = light_product_diffuse * max(dot(N.xyz, L.xyz), 0.0);

	// calculate Specular Term:
	vec4 Ispec = light_product_specular	* pow(max(dot(R, E), 0.0), 0.3*mat_shininess);

	// write Total Color:  
	
	
	frag_color = Iamb + Idiff + Ispec;
	
	if(location == 0.0f){
		if((uv.y <= 0.575 && uv.y >= 0.4) || (uv.y <= 0.993 && uv.y >= 0.85) || (uv.y <= 0.148 && uv.y >= 0.02))
			frag_color *= vec4(texture(dark_steel, uv.xy).rgb, 1.0);
		else
			frag_color *= 0.8*vec4(texture(wood, uv.xy).rgb, 1.0);
	}
	else if(location == 1.0f){
			frag_color *= vec4(texture(wood_floor, uv.xy).rgb, 1.0);
	}
	else if(location == 3.0f){
			frag_color = vec4(texture(water, uv.xy).rgb, 1.0);
	}
	else if(location == 4.0f){
			frag_color *= 3.0*vec4(texture(wood_floor, uv.xy).rgb, 1.0);
	}

	/*if (uv.x > 0.5)
	frag_color *= vec4(1.0, 0.0, 0.0, 1.0);
	else
	frag_color *= vec4(0.0, 0.0, 1.0, 1.0);*/
};
