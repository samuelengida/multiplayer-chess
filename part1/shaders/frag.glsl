// ==================================================================
#version 330 core

// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// Take in our previous texture coordinates.
in vec3 FragPos;
in vec2 v_texCoord;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in mat3 TBN;


// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 
uniform sampler2D u_NormalMap;
uniform vec3 colors; 

void main()
{
	// Store the texture coordinates
	vec3 normal = texture(u_NormalMap, v_texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	vec3 color =  texture(u_DiffuseMap, v_texCoord).rgb;

	// ambinet
	vec3 ambient = 0.8f * color; // 0.1f to reduce brightness of back side.

	// diffuse light
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	float diffImpact = max(dot(lightDir, normal), 0.0);
	vec3 diffuseLight = diffImpact * color;

	// specular
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	// Learned from source mentioned in read me.
	vec3 reflectDir = normalize(lightDir + viewDir);
	
	float spec = pow(max(dot(normal, reflectDir), 0.0), 32.0);
	vec3 specular = color * spec * 0.5f;

	FragColor = vec4(ambient + diffuseLight + specular, 1.0);
	FragColor[0] = colors[0];
	FragColor[1] += colors[1];
	FragColor[2] += colors[2];
	
}
// ==================================================================
