// utilities.h
#ifndef UTILITIES_H
#define UTILITIES_H

typedef struct {
        float3 x, y, z;
} float3x3;

__DEVICE__ float3x3 make_float3x3(float3 a, float3 b, float3 c);

__DEVICE__ inline float3 vdot(float3x3 m, float3 v);

__DEVICE__ inline float3x3 inv_f33(float3x3 m);

__DEVICE__ inline float3x3 simpleInsetMatrix(float red, float green, float blue, int inv);

__DEVICE__ inline float satmask(float3 rgb);

__DEVICE__ inline float3 NonLinearGamutMapping(float3 in, float p, float m, float t0);

__DEVICE__ inline float3 powerptoef3f(float3 a, float p, float m, float t0);

__DEVICE__ inline float powerptoe(float x, float p, float m, float t0);

__DEVICE__ inline float spowf(float a, float b);

__DEVICE__ inline float _sign(float x);

#define matrix_rec709_to_xyz                                                                                                               \
    make_float3x3(make_float3(0.412390917540f, 0.357584357262f, 0.180480793118f),                                                          \
                  make_float3(0.212639078498f, 0.715168714523f, 0.072192311287f),                                                          \
                  make_float3(0.019330825657f, 0.119194783270f, 0.950532138348f))

__DEVICE__ inline float3x3 simpleHueMatrix(float red, float green, float blue);

__DEVICE__ inline float lumMask(float3 rgb, float redw, float greenw, float bluew);

__DEVICE__ float3 inline modifiedDanieleCurve(float3 rgb, float mx,float my,float py,float g,float o, int inv);

__DEVICE__ inline float3 maxf3(float b, float3 a);

__DEVICE__ inline float3 EnergyCorrection(float3 pre, float3 post,float3x3 mt);

__DEVICE__ inline float3 avgweights(float3x3 mt);

__DEVICE__ inline float3x3 transpose_f33(float3x3 A);

// Function to clip values to the gamut
__DEVICE__ float3 simpleClip(float3 color, float3 minGamut, float3 maxGamut) {
    color.x = clamp(color.x, minGamut.x, maxGamut.x);
    color.y = clamp(color.y, minGamut.y, maxGamut.y);
    color.z = clamp(color.z, minGamut.z, maxGamut.z);
    return color;
}

// Function to convert RGB to XYZ
__DEVICE__ float3 rgbToXyz(float3 color) {
    float3 xyz;
    // Linearize the RGB values
    color.x = (color.x > 0.04045) ? pow((color.x + 0.055) / 1.055, 2.4) : (color.x / 12.92);
    color.y = (color.y > 0.04045) ? pow((color.y + 0.055) / 1.055, 2.4) : (color.y / 12.92);
    color.z = (color.z > 0.04045) ? pow((color.z + 0.055) / 1.055, 2.4) : (color.z / 12.92);

    // Apply the RGB to XYZ conversion matrix
    xyz.x = 0.4124564 * color.x + 0.3575761 * color.y + 0.1804375 * color.z;
    xyz.y = 0.2126729 * color.x + 0.7151522 * color.y + 0.0721750 * color.z;
    xyz.z = 0.0193339 * color.x + 0.1191920 * color.y + 0.9503041 * color.z;

    return xyz;
}

// Function to convert XYZ to Lab
__DEVICE__ float3 xyzToLab(float3 xyz) {
    float3 lab;
    xyz.x = xyz.x / 0.95047;
    xyz.z = xyz.z / 1.08883;

    xyz.x = (xyz.x > 0.008856) ? pow(xyz.x, 1.0 / 3.0) : (7.787 * xyz.x) + (16.0 / 116.0);
    xyz.y = (xyz.y > 0.008856) ? pow(xyz.y, 1.0 / 3.0) : (7.787 * xyz.y) + (16.0 / 116.0);
    xyz.z = (xyz.z > 0.008856) ? pow(xyz.z, 1.0 / 3.0) : (7.787 * xyz.z) + (16.0 / 116.0);

    lab.x = (116.0 * xyz.y) - 16.0;
    lab.y = 500.0 * (xyz.x - xyz.y);
    lab.z = 200.0 * (xyz.y - xyz.z);
    return lab;
}

// Function to convert Lab to XYZ
__DEVICE__ float3 labToXyz(float3 lab) {
    float y = (lab.x + 16.0) / 116.0;
    float x = lab.y / 500.0 + y;
    float z = y - lab.z / 200.0;

    float3 xyz;
    xyz.x = 0.95047 * ((x * x * x > 0.008856) ? (x * x * x) : ((x - 16.0 / 116.0) / 7.787));
    xyz.y = (y * y * y > 0.008856) ? (y * y * y) : ((y - 16.0 / 116.0) / 7.787);
    xyz.z = 1.08883 * ((z * z * z > 0.008856) ? (z * z * z) : ((z - 16.0 / 116.0) / 7.787));
    return xyz;
}

// Function to convert XYZ to RGB
__DEVICE__ float3 xyzToRgb(float3 xyz) {
    float3 rgb;
    rgb.x =  3.2404542 * xyz.x - 1.5371385 * xyz.y - 0.4985314 * xyz.z;
    rgb.y = -0.9692660 * xyz.x + 1.8760108 * xyz.y + 0.0415560 * xyz.z;
    rgb.z =  0.0556434 * xyz.x - 0.2040259 * xyz.y + 1.0572252 * xyz.z;
    return rgb;
}

// Reinhard tone mapping function
__DEVICE__ float reinhard(float value, float key, float peakLuminance) {
    float scaledValue = value * key;
    return (scaledValue * (1.0 + scaledValue / (peakLuminance * peakLuminance))) / (1.0 + scaledValue);
}
#endif