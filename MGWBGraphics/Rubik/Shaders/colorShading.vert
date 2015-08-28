#version 430

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

//out vec3 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main(){
	
	gl_Position = P * V * M * vec4(vertexPosition, 1.0);

	fragmentColor = vertexColor;
	//fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y); //flipping y coordinate, otherwise sprite would be upside down
	fragmentUV = vec2(vertexUV.x, vertexUV.y); //flipping y coordinate, otherwise sprite would be upside down
}