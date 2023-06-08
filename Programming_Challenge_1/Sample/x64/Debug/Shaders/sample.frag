#version 330 core

out vec4 FragColor; //Color pixel

//variable for texture to be passed
uniform sampler2D tex0;

//in to receive coord from vertex
in vec2 texCoord;

void main(){
	//manually coloring	 r    g     b     a
	//FragColor = vec4(0.0f, 0.7f, 0.0f, 1.0f);

	//assign texture color using the func from vertex to the pixel
	FragColor = texture(tex0, texCoord);
	//converts UV to XYZ
}