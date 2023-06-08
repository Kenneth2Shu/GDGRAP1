#version 330 core

layout(location = 0) in vec3 aPos;

//uniform float x;
//uniform float y;

uniform mat4 transform;

//for ortho camera
uniform mat4 projection;

//for matrrix
uniform mat4 view;

//tex coord / UV is at 0
//access the UV and assign it to aTex
layout(location = 2) in vec2 aTex;

//outputs and passes the tex coord to the fragment shader
out vec2 texCoord;

void main(){

	//vec3 newPos = vec3(aPos.x + x, aPos.y + y, aPos.z);

	//gl_Position = vec4(aPos, 1.0);

	//multiply transformation matrix
	gl_Position = projection * view * transform * vec4(aPos, 1.0);

	//assign the UV
	texCoord = aTex;
}