#version 330 core
out vec4 FragColor;

in vec2 TexCoord; 
in vec3 FragPos;  // 新增：世界座標

uniform sampler2D ourTexture;
uniform vec3 breathingColor;
uniform float intensity;
uniform float time;  // 新增：時間變數
uniform bool useCaustics;  // 新增：是否啟用 caustics

void main()
{
    // 步驟1: 從貼圖取得顏色
    vec4 texColor = texture(ourTexture, TexCoord);
    
    // 步驟2: 套用呼吸燈效果
    // 當 breathingColor = (1,1,1) 且 intensity = 1 時，輸出原始顏色
    // 當 breathingColor = (1,1,0) (黃色) 時，會有黃色調的效果
    FragColor = texColor * vec4(breathingColor * intensity, 1.0);
}
