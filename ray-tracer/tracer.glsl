#version 460 core

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;
layout(rgba32f, binding = 1) uniform image3D voxgrid;

uniform vec3 camera_position;
uniform vec3 camera_direction;
uniform vec3 camera_up;
uniform vec3 camera_right;

vec4 rcast(vec3 ray_origin, vec3 ray_direction)
{
    ivec3 pos = ivec3(floor(ray_origin)); 
    ivec3 step = ivec3(sign(ray_direction));
    
    vec3 tDelta = abs(vec3(1.0) / ray_direction); 
    vec3 tMax = vec3(0); 

    tMax.x = (step.x > 0 ? (pos.x + 1 - ray_origin.x) : (ray_origin.x - pos.x)) * tDelta.x;
    tMax.y = (step.y > 0 ? (pos.y + 1 - ray_origin.y) : (ray_origin.y - pos.y)) * tDelta.y;
    tMax.z = (step.z > 0 ? (pos.z + 1 - ray_origin.z) : (ray_origin.z - pos.z)) * tDelta.z;

    vec4 color = vec4(0);
    ivec3 voxel_grid_size = imageSize(voxgrid);

    while (true) {
        if (pos.x >= 0 && pos.x < voxel_grid_size.x && pos.y >= 0 && pos.y < voxel_grid_size.y && pos.z >= 0 && pos.z < voxel_grid_size.z) {
            int index = pos.x + voxel_grid_size.y * (pos.y + voxel_grid_size.z * pos.z);
            vec4 voxel_color = vec4(imageLoad(voxgrid, pos));

            if (voxel_color.a > 0.0) {
                color.rgb = mix(color.rgb, voxel_color.rgb, voxel_color.a);
                color.a = max(color.a, voxel_color.a);
            }

            if (color.a >= 1.0) {
                break;
            }
        }
       
        if (tMax.x < tMax.y) {
            if (tMax.x < tMax.z) {
                pos.x += step.x;
                tMax.x += tDelta.x;

                if (step.x > 0 && pos.x >= voxel_grid_size.x) {
                    return color;
                } else if (step.x < 0 && pos.x < 0) {
                    return color;
                }
            } else {
                pos.z += step.z;
                tMax.z += tDelta.z;

                if (step.z > 0 && pos.z >= voxel_grid_size.z) {
                    return color;
                } else if (step.z < 0 && pos.z < 0) {
                    return color;
                }
            }
        } else {
            if (tMax.y < tMax.z) {
                pos.y += step.y;
                tMax.y += tDelta.y;

                if (step.y > 0 && pos.y >= voxel_grid_size.y) {
                    return color;
                } else if (step.y < 0 && pos.y < 0) {
                    return color;
                }
            } else {
                pos.z += step.z;
                tMax.z += tDelta.z;

                if (step.z > 0 && pos.z >= voxel_grid_size.z) {
                    return color;
                } else if (step.z < 0 && pos.z < 0) {
                    return color;
                }
            }
        }
    }
    
    return color;
}

void main()
{
    ivec2 pixel_coordinates = ivec2(gl_GlobalInvocationID.xy);
    ivec2 screen_size = imageSize(screen);
    
    vec2 coordinates = (2.0 * vec2(pixel_coordinates) / vec2(screen_size) - 1.0);
    
    float aspect_ratio = float(screen_size.x) / float(screen_size.y);
    float fov = 60.0;
    float tan_half_fov = tan(radians(fov) / 2.0);
    
    vec3 ray_origin = camera_position;
    vec3 ray_direction = normalize(
        (coordinates.x * camera_right * aspect_ratio * tan_half_fov +
        coordinates.y * camera_up * tan_half_fov +
        camera_direction)
    );

    imageStore(screen, pixel_coordinates, rcast(ray_origin, ray_direction));
}
