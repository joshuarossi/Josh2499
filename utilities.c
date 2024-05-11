#include "utilities.h"

__DEVICE__ float3x3 make_float3x3(float3 a, float3 b, float3 c) {
    float3x3 d;
    d.x = a, d.y = b, d.z = c;
    return d;
}

__DEVICE__ inline float3 vdot(float3x3 m, float3 v)
{
  return make_float3(
      m.x.x * v.x + m.x.y * v.y + m.x.z * v.z,
      m.y.x * v.x + m.y.y * v.y + m.y.z * v.z,
      m.z.x * v.x + m.z.y * v.y + m.z.z * v.z);
}

__DEVICE__ inline float3x3 inv_f33(float3x3 m) {
    float d = m.x.x * (m.y.y * m.z.z - m.z.y * m.y.z) - m.x.y * (m.y.x * m.z.z - m.y.z * m.z.x) + m.x.z * (m.y.x * m.z.y - m.y.y * m.z.x);
    float id = 1.0f / d;
    float3x3 c = make_float3x3(make_float3(1.0f, 0.0f, 0.0f), make_float3(0.0f, 1.0f, 0.0f), make_float3(0.0f, 0.0f, 1.0f));
    c.x.x = id * (m.y.y * m.z.z - m.z.y * m.y.z);
    c.x.y = id * (m.x.z * m.z.y - m.x.y * m.z.z);
    c.x.z = id * (m.x.y * m.y.z - m.x.z * m.y.y);
    c.y.x = id * (m.y.z * m.z.x - m.y.x * m.z.z);
    c.y.y = id * (m.x.x * m.z.z - m.x.z * m.z.x);
    c.y.z = id * (m.y.x * m.x.z - m.x.x * m.y.z);
    c.z.x = id * (m.y.x * m.z.y - m.z.x * m.y.y);
    c.z.y = id * (m.z.x * m.x.y - m.x.x * m.z.y);
    c.z.z = id * (m.x.x * m.y.y - m.y.x * m.x.y);
    return c;
}

// Calculate a matrix that separately inset each primary
__DEVICE__ inline float3x3 simpleInsetMatrix(float red, float green, float blue, int inv)
{
  float3 white = make_float3(1, 1, 1);
  float gain_coef = 1 + red + green + blue;
  float3 redcoeff = make_float3(1 + red, green, blue) / gain_coef;
  float3 greencoeff = make_float3(red, 1 + green, blue) / gain_coef;
  float3 bluecoeff = make_float3(red, green, 1 + blue) / gain_coef;
  float3x3 M = make_float3x3(redcoeff, greencoeff, bluecoeff);
  M = inv ? inv_f33(M) : M;
  return M;
}

__DEVICE__ inline float satmask(float3 rgb)
{
  float PI_LOCAL = _atan2f(1, 1) * 4;
  rgb = vdot(simpleInsetMatrix(0.1, 0.7, 0.2, 0), rgb);
  float avg = (rgb.x + rgb.y + rgb.z) / 3;
  float3 hma = make_float3(
      _atan2f(0.5f * (2.0f * rgb.x - rgb.y - rgb.z), (rgb.y - rgb.z) * _sqrtf(3.0f / 4.0f)),
      avg == 0 ? 0 : _sqrtf((rgb.x - avg) * (rgb.x - avg) + (rgb.y - avg) * (rgb.y - avg) + (rgb.z - avg) * (rgb.z - avg)) / (2 * avg),
      avg);
  hma.x = (hma.x < 0 ? hma.x + 2 * PI_LOCAL : hma.x) / (2 * PI_LOCAL);
  return hma.y;
}

__DEVICE__ inline float3 NonLinearGamutMapping(float3 in, float p, float m, float t0) {

    float mx = max3(in.x, in.y, in.z);

    float3 rats;
    rats.x = mx == 0.0f ? 0.0f: in.x/mx;
    rats.y = mx == 0.0f ? 0.0f: in.y/mx;
    rats.z = mx == 0.0f ? 0.0f: in.z/mx;
    rats = powerptoef3f(rats, p, m, t0);

    float3 out = rats * mx;

    return out;
}

__DEVICE__ inline float3 powerptoef3f(float3 a, float p, float m, float t0) {
    return make_float3(powerptoe(a.x, p, m, t0), powerptoe(a.y, p, m, t0), powerptoe(a.z, p, m, t0));
}

// https://www.desmos.com/calculator/jrff9lrztn
__DEVICE__ inline float powerptoe(float x, float p, float m, float t0) {
    float y = x > t0 ? x : (x - t0) * spowf(spowf((t0 - x) / (t0 - m), 1.0f / p) + 1.0f, -p) + t0;
    return y;
}

__DEVICE__ inline float spowf(float a, float b) {
    // Compute "safe" power of float a, reflected over the origin

    a = _sign(a) * _powf(_fabs(a), b);
    return a;
}

__DEVICE__ inline float _sign(float x) {
    // Return the sign of float x
    if (x > 0.0f)
        return 1.0f;
    if (x < 0.0f)
        return -1.0f;
    return 0.0f;
}

__DEVICE__ inline float3 NonLinearGamutMapping(float3 in, float p, float m, float t0) {

    float mx = max3(in.x, in.y, in.z);

    float3 rats;
    rats.x = mx == 0.0f ? 0.0f: in.x/mx;
    rats.y = mx == 0.0f ? 0.0f: in.y/mx;
    rats.z = mx == 0.0f ? 0.0f: in.z/mx;
    rats = powerptoef3f(rats, p, m, t0);

    float3 out = rats * mx;

    return out;
}
