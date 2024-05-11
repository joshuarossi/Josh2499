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
                  make_float3(0.019330825657f, 0.119194783270f, 0.950532138348f));

__DEVICE__ inline float3 NonLinearGamutMapping(float3 in, float p, float m, float t0);

#endif