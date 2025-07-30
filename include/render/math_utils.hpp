#pragma once

#include <cmath>

namespace render {

class Mat4 {
public:
    float data[16];
    
    Mat4() {
        identity();
    }
    
    void identity() {
        for (int i = 0; i < 16; ++i) {
            data[i] = 0.0f;
        }
        data[0] = data[5] = data[10] = data[15] = 1.0f;
    }
    
    static Mat4 perspective(float fov_y, float aspect, float near_plane, float far_plane) {
        Mat4 result;
        result.data[0] = result.data[5] = result.data[10] = result.data[15] = 0.0f;
        
        float tan_half_fov = std::tan(fov_y * 0.5f * M_PI / 180.0f);
        
        result.data[0] = 1.0f / (aspect * tan_half_fov);
        result.data[5] = 1.0f / tan_half_fov;
        result.data[10] = -(far_plane + near_plane) / (far_plane - near_plane);
        result.data[11] = -1.0f;
        result.data[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);
        
        return result;
    }
    
    static Mat4 look_at(float eye_x, float eye_y, float eye_z,
                       float center_x, float center_y, float center_z,
                       float up_x, float up_y, float up_z) {
        Mat4 result;
        
        // Forward vector
        float fx = center_x - eye_x;
        float fy = center_y - eye_y;
        float fz = center_z - eye_z;
        
        // Normalize forward
        float f_len = std::sqrt(fx*fx + fy*fy + fz*fz);
        fx /= f_len; fy /= f_len; fz /= f_len;
        
        // Right vector (forward x up)
        float rx = fy * up_z - fz * up_y;
        float ry = fz * up_x - fx * up_z;
        float rz = fx * up_y - fy * up_x;
        
        // Normalize right
        float r_len = std::sqrt(rx*rx + ry*ry + rz*rz);
        rx /= r_len; ry /= r_len; rz /= r_len;
        
        // Up vector (right x forward)
        float ux = ry * fz - rz * fy;
        float uy = rz * fx - rx * fz;
        float uz = rx * fy - ry * fx;
        
        result.data[0] = rx;  result.data[4] = ux;  result.data[8]  = -fx; result.data[12] = -(rx * eye_x + ux * eye_y - fx * eye_z);
        result.data[1] = ry;  result.data[5] = uy;  result.data[9]  = -fy; result.data[13] = -(ry * eye_x + uy * eye_y - fy * eye_z);
        result.data[2] = rz;  result.data[6] = uz;  result.data[10] = -fz; result.data[14] = -(rz * eye_x + uz * eye_y - fz * eye_z);
        result.data[3] = 0;   result.data[7] = 0;   result.data[11] = 0;   result.data[15] = 1;
        
        return result;
    }
    
    const float* ptr() const { return data; }
};

inline float normalize_vec3(float& x, float& y, float& z) {
    float len = std::sqrt(x*x + y*y + z*z);
    if (len > 0) {
        x /= len; y /= len; z /= len;
    }
    return len;
}

inline void cross_product(float ax, float ay, float az, float bx, float by, float bz, float& cx, float& cy, float& cz) {
    cx = ay * bz - az * by;
    cy = az * bx - ax * bz;
    cz = ax * by - ay * bx;
}

} // namespace render