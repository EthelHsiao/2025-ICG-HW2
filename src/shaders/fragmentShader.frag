#version 330 core
out vec4 FragColor;

in vec2 TexCoord; 

uniform sampler2D ourTexture;
uniform vec3 breathingColor;
uniform float intensity;

void main()
{
    // TODO: Implement Breathing Light Effect
	//   1. Retrieve the color from the texture at texCoord.
	//   2. Set FragColor to be the texture color * (breathingColor * intensity)
	//   Note: Ensure FragColor is appropriately set for both breathing light and normal cases.
} 