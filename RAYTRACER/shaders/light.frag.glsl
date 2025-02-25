#version 330 core

in vec3 mynormal; 
in vec4 myvertex; 

uniform mat4 modelview;

out vec4 fragColor;

uniform vec3 color;

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// material parameters: ambient, diffuse, specular, shininess

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {

        float nDotL = dot(normal, direction);         
        vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0);  

        float nDotH = dot(normal, halfvec); 
        vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess); 

        vec4 retval = lambert + phong; 
        return retval;     
}

void main(void) {
    if (enablelighting) {
        // apply the transformation to myvertex
        vec4 transf_vertex = modelview * myvertex;

        // normalized direction vector from eye position 
        vec3 eye = vec3(0, 0, 0);
        vec4 eyeModelView = modelview * normalize(vec4(eye, 1.0));
        vec3 eyeDirection = normalize(-eyeModelView.xyz);

        // inverse transpose modelview and use it to transform normal vectors
        mat3 transposeMat = transpose(mat3(modelview));
        mat3 inverseMat = inverse(transposeMat);
        mat3 normMat = inverseMat;
        vec3 normal = normalize(normMat * mynormal);

        // initialize final color
        vec4 finalcolor = vec4(0, 0, 0, 0);
        
        // loop through each light
        for (int i = 0; i < numused; ++i) {
            // initialize vector to track light direction
            vec3 lightDirection = vec3(0, 0, 0);

            // magic number set up
            int point_lighting = 1;
            int directional_lighting = 0;

            // if directional light
            if (lightposn[i].w == directional_lighting) {
                lightDirection = normalize(lightposn[i].xyz);
            }
            // if point light
            else {
                int point_lighting = 1;
                vec3 pos = lightposn[i].xyz / point_lighting;
                lightDirection = normalize(pos - transf_vertex.xyz);
            }
            // normalize values 
            vec3 halfvec = normalize(eyeDirection + lightDirection);
            vec3 norm_direction = normalize(lightDirection);
            // use light equations (lambert and phong) to compute light
            vec4 col = ComputeLight(norm_direction, lightcolor[i], normal, halfvec, diffuse, specular, shininess);
            // apply the result
            finalcolor += col;
        }
        // apply emission and ambient
        finalcolor += (ambient + emission);
        // apply final result to fragment color
        fragColor = finalcolor;
    } else {
        fragColor = vec4(color, 1.0);
    }
}
