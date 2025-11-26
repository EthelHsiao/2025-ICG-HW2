#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float squeezeFactor;

void main()
{
    // 步驟1: 複製原始位置
    vec3 position = aPos;
    
    // 步驟2: 保存原始 x 和 z 值，避免互相影響
    float originalX = aPos.x;
    float originalZ = aPos.z;
    
    // 步驟3: 根據 squeezeFactor 實作擠壓效果
    // 公式來自作業規格：
    // x += z * sin(squeezeFactor) / 2;
    // z += x * sin(squeezeFactor) / 2;
    position.x = originalX + originalZ * sin(squeezeFactor) / 2.0;
    position.z = originalZ + originalX * sin(squeezeFactor) / 2.0;
    
    // 步驟4: 計算最終的頂點位置（MVP 變換）
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    // 步驟5: 傳遞貼圖座標到 fragment shader
    TexCoord = aTexCoord;
}
