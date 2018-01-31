#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outercutoff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define MAX_POINT_LIGHTS 2

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform int numPointLights;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewdir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragpos, vec3 viewdir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragpos, vec3 viewdir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewdir = normalize(viewPos - FragPos);
    
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewdir);

    // phase 2: point lights
    for(int i = 0; i < numPointLights; i++)
       result += CalcPointLight(pointLights[i], norm, FragPos, viewdir);  
		
    // phase 3: spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewdir);    
    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewdir)
{
    vec3 lightdir = normalize(-light.direction);
	vec3 halfwaydir = normalize(lightdir + viewdir);
    // diffuse shading
    float diff = max(dot(normal, lightdir), 0.0);
    // specular shading
    vec3 reflectdir = reflect(-lightdir, normal);
    float spec = pow(max(dot(normal, halfwaydir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragpos, vec3 viewdir)
{
    vec3 lightdir = normalize(light.position - fragpos);
	vec3 halfwaydir = normalize(lightdir + viewdir);
    // diffuse shading
    float diff = max(dot(normal, lightdir), 0.0);
    // specular shading
    vec3 reflectdir = reflect(-lightdir, normal);
    float spec = pow(max(dot(normal, halfwaydir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragpos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragpos, vec3 viewdir)
{
    vec3 lightdir = normalize(light.position - fragpos);
	vec3 halfwaydir = normalize(lightdir + viewdir);
    // diffuse shading
    float diff = max(dot(normal, lightdir), 0.0);
    // specular shading
    vec3 reflectdir = reflect(-lightdir, normal);
    float spec = pow(max(dot(normal, halfwaydir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragpos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightdir, normalize(-light.direction)); 
    float epsilon = light.cutoff - light.outercutoff;
    float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

