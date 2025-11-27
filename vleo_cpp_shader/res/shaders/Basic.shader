#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aColor;

flat out vec3 vColor;

uniform mat4 u_MVP;

void main()
{
    vColor = aColor;
    gl_Position = u_MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#shader fragment
#version 330 core
flat in vec3 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor.x, vColor.y, vColor.z, 1.0f); // Orange color
}

#shader compute
#version 430
        
layout(local_size_x = 16, local_size_y = 16) in;
        
// Input: framebuffer texture with triangle IDs (unsigned integer)
layout(r16ui, binding = 0) uniform uimage2D framebuffer_texture;
        
// Output: histogram buffer (one slot per triangle)
layout(std430, binding = 1) buffer HistogramBuffer 
{
    uint histogram[];
}
        
void main() 
{
    ivec2 coord = ivec2(gl_GlobalInvocationID.xy);
    ivec2 fb_size = imageSize(framebuffer_texture);
            
    // Check bounds
    if (coord.x >= fb_size.x || coord.y >= fb_size.y) return;
            
    // Read triangle ID from framebuffer (single channel R16UI - unsigned)
    uint triangle_id = imageLoad(framebuffer_texture, coord).r;
            
    // Skip background (triangle ID 0)
    if (triangle_id > 0 && triangle_id < histogram.length()) 
    {
        // Atomically increment histogram for this triangle
        atomicAdd(histogram[triangle_id], 1u);
    }
}