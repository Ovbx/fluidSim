  #version 330 core
  layout (location = 0) in vec3 aPos;
  layout (location = 1) in vec3 aColor;
  layout (location = 2) in vec4 instanceData;
  layout (location = 3) in float colorT;

  out vec3 ourColor;

  uniform mat4 view;
  uniform mat4 projection;

  void main () {
    vec3 blue = vec3(0, 0, 1);
    vec3 green = vec3(0, 1, 0);
    vec3 yellow = vec3(1, 1, 0);
    vec3 red = vec3(1, 0, 0);

    vec3 color;
    if (colorT < 0.33) {
      color = mix(blue, green, colorT / 0.33);
    }
    else if (colorT < 0.66) {
      color = mix(green, yellow, (colorT - 0.33) / 0.33);
    }
    else {
      color = mix(yellow, red, (colorT - 0.66) / 0.34);
    }
    ourColor = color;
    float angle = instanceData.z;
    float scale = instanceData.w;
    float cosOfInstanceData = cos(angle);
    float sinOfInstanceData = sin(angle);
    mat2 rotation = mat2(cosOfInstanceData, sinOfInstanceData, -sinOfInstanceData, cosOfInstanceData);
    vec2 rotatedPosition = (rotation * aPos.xy) * scale + instanceData.xy;
    gl_Position = projection * view * vec4(rotatedPosition, aPos.z, 1.0);
}

