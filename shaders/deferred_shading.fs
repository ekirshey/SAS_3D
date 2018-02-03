#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
    float Radius;
};
const int NR_LIGHTS = 2;
uniform DirLight dirlight;
uniform PointLight lights[NR_LIGHTS];
uniform vec3 viewPos;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewdir, vec3 texdiff, float texspec);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragpos, vec3 viewdir, vec3 texdiff, float texspec);

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // then calculate lighting as usual
    vec3 viewDir  = normalize(viewPos - FragPos);
	vec3 lighting = CalcDirLight(dirlight, Normal, viewDir, Diffuse, Specular);
    for(int i = 0; i < NR_LIGHTS; ++i) {
		lighting += CalcPointLight(lights[i], Normal, FragPos, viewDir, Diffuse, Specular);
    }    
    FragColor = vec4(lighting, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewdir, vec3 texdiff, float texspec)
{
    vec3 lightdir = normalize(-light.direction);
	vec3 halfwaydir = normalize(lightdir + viewdir);
    // diffuse shading
    float diff = max(dot(normal, lightdir), 0.0);
    // specular shading
    vec3 reflectdir = reflect(-lightdir, normal);
    float spec = pow(max(dot(normal, halfwaydir), 0.0), 32);
    // combine results
    vec3 ambient = light.ambient * texdiff;
    vec3 diffuse = light.diffuse * diff * texdiff;
    vec3 specular = light.specular * spec * texspec;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragpos, vec3 viewdir, vec3 texdiff, float texspec)
{
	vec3 specular;
	vec3 diffuse;
    // calculate distance between light source and current fragment
    float distance = length(light.Position - fragpos);
    if(distance < light.Radius)
    {
        // diffuse
        vec3 lightDir = normalize(light.Position - fragpos);
        diffuse = max(dot(normal, lightDir), 0.0) * texdiff * light.Color;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewdir);  
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        specular = light.Color * spec * texspec;
        // attenuation
        float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
    }
    return (diffuse + specular);
}