#version 330 core

in float fieldValue;
out vec4 FragColor;

uniform float uMinValue;
uniform float uMaxValue;

vec3 valueToColor(float value) {
    // Normalize value to [0, 1]
    float normalized = (value - uMinValue) / (uMaxValue - uMinValue);
    normalized = clamp(normalized, 0.0, 1.0);
    
    // Blue to red color map
    vec3 color;
    if (normalized < 0.5) {
        // Blue to green
        float t = normalized * 2.0;
        color = mix(vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0), t);
    } else {
        // Green to red
        float t = (normalized - 0.5) * 2.0;
        color = mix(vec3(0.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), t);
    }
    
    return color;
}

void main() {
    vec3 color = valueToColor(fieldValue);
    FragColor = vec4(color, 1.0);
}