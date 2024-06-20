// 2500.h
#ifndef _2500_H
#define _2500_H

typedef struct
{
  float3 x, y, z;
} float3x3;

__DEVICE__ float3x3 make_float3x3(float3 a, float3 b, float3 c)
{
  float3x3 d;
  d.x = a, d.y = b, d.z = c;
  return d;
}

#define matrix_davinciwg_to_xyz                                                  \
  make_float3x3(make_float3(0.700622320175f, 0.148774802685f, 0.101058728993f),  \
                make_float3(0.274118483067f, 0.873631775379f, -0.147750422359f), \
                make_float3(-0.098962903023f, -0.137895315886f, 1.325916051865f))
// #define matrix_xyz_to_davinciwg                                    \
//   make_float3x3(make_float3(1.4628067f, -0.1840623f, -0.2743606f), \
//                 make_float3(-0.5217936f, 1.4472381f, 0.0677227f),  \
//                 make_float3(0.0349342f, 0.0986234f, 0.7551330f))
#define matrix_xyz_to_davinciwg                                       \
  make_float3x3(make_float3(1.51667204f, -0.28147805f, -0.14696363f), \
                make_float3(-0.46491710f, 1.25142378f, 0.17488461f),  \
                make_float3(0.06484905f, 0.10913934f, 0.76141462f))

#define matrix_xyz_to_rec709                                                    \
  make_float3x3(make_float3(3.2409699419f, -1.53738317757f, -0.498610760293f),  \
                make_float3(-0.969243636281f, 1.87596750151f, 0.041555057407f), \
                make_float3(0.055630079697f, -0.203976958889f, 1.05697151424f))
#define matrix_rec709_to_xyz                                                    \
  make_float3x3(make_float3(0.412390917540f, 0.357584357262f, 0.180480793118f), \
                make_float3(0.212639078498f, 0.715168714523f, 0.072192311287f), \
                make_float3(0.019330825657f, 0.119194783270f, 0.950532138348f))
#define matrix_p3d65_to_xyz                                                     \
  make_float3x3(make_float3(0.486571133137f, 0.265667706728f, 0.198217317462f), \
                make_float3(0.228974640369f, 0.691738605499f, 0.079286918044f), \
                make_float3(-0.000000000000f, 0.045113388449f, 1.043944478035f))
#define matrix_p3dci_to_xyz                                         \
  make_float3x3(make_float3(0.44516982f, 0.27713441f, 0.27713441f), \
                make_float3(0.20949168f, 0.72159525f, 0.06891307f), \
                make_float3(-0.000000000000f, 0.04706056f, 0.90735539f))
#define matrix_AcerSwiftX_to_xyz                                    \
  make_float3x3(make_float3(0.42538169f, 0.34126547f, 0.18380874f), \
                make_float3(0.2195273f, 0.724392f, 0.05608072f),    \
                make_float3(0.01657684f, 0.10659271f, 0.9658882f))
#define matrix_xyz_to_rec2020                                                    \
  make_float3x3(make_float3(1.71665118797f, -0.355670783776f, -0.253366281374f), \
                make_float3(-0.666684351832f, 1.61648123664f, 0.015768545814f),  \
                make_float3(0.017639857445f, -0.042770613258f, 0.942103121235f))
// APO/ACES 2065-1 to DWG using cat02 for chroamtic adaptation
#define matrix_AP0_to_DWG                                             \
  make_float3x3(make_float3(1.32832312f, -0.21352364f, -0.11479948f), \
                make_float3(-0.01316075f, 0.91286542f, 0.10029534f),  \
                make_float3(0.09831923f, 0.07961846f, 0.82206231f))

__DEVICE__ inline float3x3 inv_f33(float3x3 m)
{
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

__DEVICE__ inline float oetf_davinci_intermediate(float x, int inv)
{
  /* DaVinci Intermediate Log
      https://documents.blackmagicdesign.com/InformationNotes/DaVinci_Resolve_17_Wide_Gamut_Intermediate.pdf
  */

  const float A = 0.0075f;
  const float B = 7.0f;
  const float C = 0.07329248f;
  const float M = 10.44426855f;
  const float LIN_CUT = 0.00262409f;
  const float LOG_CUT = 0.02740668f;

  if (inv == 1)
  {
    return x <= LOG_CUT ? x / M : _exp2f(x / C - B) - A;
  }
  else
  {
    return x <= LIN_CUT ? x * M : (_log2f(x + A) + B) * C;
  }
}

__DEVICE__ inline float3 vdot(float3x3 m, float3 v)
{
  return make_float3(
      m.x.x * v.x + m.x.y * v.y + m.x.z * v.z,
      m.y.x * v.x + m.y.y * v.y + m.y.z * v.z,
      m.z.x * v.x + m.z.y * v.y + m.z.z * v.z);
}

__DEVICE__ inline float max3(float x, float y, float z)
{
  return _fmaxf(x, _fmaxf(y, z));
}

__DEVICE__ inline float _sign(float x)
{
  // Return the sign of float x
  if (x > 0.0f)
    return 1.0f;
  if (x < 0.0f)
    return -1.0f;
  return 0.0f;
}

__DEVICE__ inline float spowf(float a, float b)
{
  // Compute "safe" power of float a, reflected over the origin

  a = _sign(a) * _powf(_fabs(a), b);
  return a;
}

// https://www.desmos.com/calculator/jrff9lrztn
__DEVICE__ inline float powerptoe(float x, float p, float m, float t0)
{
  float y = x > t0 ? x : (x - t0) * spowf(spowf((t0 - x) / (t0 - m), 1.0f / p) + 1.0f, -p) + t0;
  return y;
}

__DEVICE__ inline float3 powerptoef3f(float3 a, float p, float m, float t0)
{
  return make_float3(powerptoe(a.x, p, m, t0), powerptoe(a.y, p, m, t0), powerptoe(a.z, p, m, t0));
}

__DEVICE__ inline float3x3 transpose_f33(float3x3 A)
{

  float3x3 B = A;
  A.x = make_float3(B.x.x, B.y.x, B.z.x);
  A.y = make_float3(B.x.y, B.y.y, B.z.y);
  A.z = make_float3(B.x.z, B.y.z, B.z.z);

  return A;
}

__DEVICE__ inline float3x3 simpleHueMatrix(float red, float green, float blue)
{

  float3 redcoeff = make_float3(1, green > 0 ? green : 0, blue < 0 ? -blue : 0);
  float3 greencoeff = make_float3(red > 0 ? red : 0, 1, blue > 0 ? blue : 0);
  float3 bluecoeff = make_float3(red < 0 ? -red : 0, green < 0 ? -green : 0, 1);

  float3 white;
  white.x = 1 + redcoeff.y + redcoeff.z;
  white.y = greencoeff.x + 1 + greencoeff.z;
  white.z = bluecoeff.x + bluecoeff.y + 1;

  redcoeff = redcoeff / white.x;
  greencoeff = greencoeff / white.y;
  bluecoeff = bluecoeff / white.z;

  float3x3 M = make_float3x3(redcoeff, greencoeff, bluecoeff);

  return M;
}

__DEVICE__ inline float desat(float3 rgb, float redw, float greenw, float bluew, float L0, float L1, float a1, float a)
{

  float3 weights = make_float3(redw, greenw, bluew);
  weights *= rgb;
  float x = weights.x + weights.y + weights.z; // take the norm
  float b = -_logf((a - L1) / (L0 - L1)) / a1;
  float y = L0 * _expf(-b * x) + L1 * (1 - _expf(-b * x));
  return y;
}

__DEVICE__ inline float lumMask(float3 rgb, float redw, float greenw, float bluew)
{

  float3 weights = make_float3(redw, greenw, bluew);
  weights *= rgb;
  float lum = weights.x + weights.y + weights.z; // take the norm

  lum = _powf((lum / (lum + 1.16820931127f)), 1.1f);
  lum = lum * lum / (lum + 0.01f);
  lum = _powf(lum, 2.5) * (1 - lum) + lum * lum;

  return lum;
}

__DEVICE__ inline float3 avgweights(float3x3 mt)
{

  float3 weights = vdot(mt, make_float3(1.0f, 1.0f, 1.0f));

  mt.x /= weights.x;
  mt.y /= weights.y;
  mt.z /= weights.z;

  mt = transpose_f33(mt);

  weights = make_float3(mt.x.x + mt.x.y + mt.x.z, mt.y.x + mt.y.y + mt.y.z, mt.z.x + mt.z.y + mt.z.z);
  weights /= 3.0f;

  return weights;
}

__DEVICE__ inline float3 EnergyCorrection(float3 pre, float3 post, float3x3 mt)
{

  float PI_LOCAL = _atan2f(1, 1) * 4;

  float3 weights = avgweights(mt);

  float preavg = weights.x * pre.x + weights.y * pre.y + weights.z * pre.z;
  float postavg = weights.x * post.x + weights.y * post.y + weights.z * post.z;

  float gain = 4 * _atan2f(preavg, postavg) / PI_LOCAL;
  gain = powerptoe(gain, 0.16f, 0.0f, 1.0f);
  gain = _tanf(gain * PI_LOCAL / 4);

  float3 out = post * gain;

  return out;
}

__DEVICE__ inline float3 NonLinearGamutMapping(float3 in, float p, float m, float t0)
{

  float mx = max3(in.x, in.y, in.z);

  float3 rats;
  rats.x = mx == 0.0f ? 0.0f : in.x / mx;
  rats.y = mx == 0.0f ? 0.0f : in.y / mx;
  rats.z = mx == 0.0f ? 0.0f : in.z / mx;
  rats = powerptoef3f(rats, p, m, t0);

  float3 out = rats * mx;

  return out;
}

__DEVICE__ inline float3 rec709_2_moment(float3 rgb)
{

  float PI_LOCAL = _atan2f(1, 1) * 4;

  rgb = vdot(matrix_rec709_to_xyz, rgb);

  float3 white = vdot(matrix_rec709_to_xyz, make_float3(1, 1, 1));

  rgb = rgb / white;

  float avg = (rgb.x + rgb.y + rgb.z) / 3;

  float3 hma = make_float3(
      _atan2f(0.5f * (2.0f * rgb.x - rgb.y - rgb.z), (rgb.y - rgb.z) * _sqrtf(3.0f / 4.0f)),
      avg == 0 ? 0 : _sqrtf((rgb.x - avg) * (rgb.x - avg) + (rgb.y - avg) * (rgb.y - avg) + (rgb.z - avg) * (rgb.z - avg)) / (2 * avg),
      avg);

  hma.x = (hma.x < 0 ? hma.x + 2 * PI_LOCAL : hma.x) / (2 * PI_LOCAL);

  return hma;
}

// Calculate a matrix that separately inset each primary
__DEVICE__ inline float3x3 simpleInsetMatrix(float red, float green, float blue, int inv)
{

  // float3 white = make_float3(1.0f, 1.0f, 1.0f);

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

__DEVICE__ float3 inline modifiedDanieleCurve(float3 rgb, float mx, float my, float py, float g, float o, int inv)
{

  float mi = 0.5 * (my + _sqrtf(my * my + 4 * my * o));
  float pi = 0.5 * (py + _sqrtf(py * py + 4 * py * o));
  float s = ((1 - _powf(mi / pi, 1 / g)) / _powf(mi / pi, 1 / g)) * mx;

  if (inv == 1)
  {
    rgb.x = 0.5 * (rgb.x + _sqrtf(rgb.x * rgb.x + rgb.x * 4 * o));
    rgb.y = 0.5 * (rgb.y + _sqrtf(rgb.y * rgb.y + rgb.y * 4 * o));
    rgb.z = 0.5 * (rgb.z + _sqrtf(rgb.z * rgb.z + rgb.z * 4 * o));

    g = 1 / g;

    rgb.x = s * _powf(rgb.x / pi, g) / (1 - _powf(rgb.x / pi, g));
    rgb.y = s * _powf(rgb.y / pi, g) / (1 - _powf(rgb.y / pi, g));
    rgb.z = s * _powf(rgb.z / pi, g) / (1 - _powf(rgb.z / pi, g));

    return rgb;
  }
  else
  {
    rgb.x = pi * _powf(rgb.x / (rgb.x + s), g);
    rgb.y = pi * _powf(rgb.y / (rgb.y + s), g);
    rgb.z = pi * _powf(rgb.z / (rgb.z + s), g);

    rgb.x = rgb.x * rgb.x / (rgb.x + o);
    rgb.y = rgb.y * rgb.y / (rgb.y + o);
    rgb.z = rgb.z * rgb.z / (rgb.z + o);
    return rgb;
  }

  return rgb;
}

__DEVICE__ inline float3 maxf3(float b, float3 a)
{
  // For each component of float3 a, return max of component and float b
  return make_float3(_fmaxf(a.x, b), _fmaxf(a.y, b), _fmaxf(a.z, b));
}

__DEVICE__ float3 minf3(float b, float3 a)
{
  // For each component of float3 a, return max of component and float b
  return make_float3(_fminf(a.x, b), _fminf(a.y, b), _fminf(a.z, b));
}

__DEVICE__ inline float3 pow3(float b, float3 a, int inv)
{

  b = inv ? b : 1 / b;

  return make_float3(_powf(a.x, b), _powf(a.y, b), _powf(a.z, b));
}

__DEVICE__ inline float3 eotf_pq(float3 rgb, int inverse)
{
  /* Apply the ST-2084 PQ Forward or Inverse EOTF
      ITU-R Rec BT.2100-2 https://www.itu.int/rec/R-REC-BT.2100
      ITU-R Rep BT.2390-9 https://www.itu.int/pub/R-REP-BT.2390
      Note: in the spec there is a normalization for peak display luminance.
      For this function we assume the input is already normalized such that 1.0 = 10,000 nits
  */

  // const float Lp = 1.0f;
  const float m1 = 2610.0f / 16384.0f;
  const float m2 = 2523.0f / 32.0f;
  const float c1 = 107.0f / 128.0f;
  const float c2 = 2413.0f / 128.0f;
  const float c3 = 2392.0f / 128.0f;

  if (inverse == 1)
  {
    // rgb /= Lp;
    rgb = pow3(m1, rgb, 1);
    rgb = pow3(m2, (c1 + c2 * rgb) / (1.0f + c3 * rgb), 1);
  }
  else
  {
    rgb = pow3(1.0f / m2, rgb, 1);
    rgb = pow3(1.0f / m1, (rgb - c1) / (c2 - c3 * rgb), 1);
    // rgb *= Lp;
  }
  return rgb;
}

__DEVICE__ inline float3 eotf_hlg(float3 rgb, int inverse)
{
  // Aply the HLG Forward or Inverse EOTF. Implements the full ambient surround illumination model
  // ITU-R Rec BT.2100-2 https://www.itu.int/rec/R-REC-BT.2100
  // ITU-R Rep BT.2390-8: https://www.itu.int/pub/R-REP-BT.2390
  // Perceptual Quantiser (PQ) to Hybrid Log-Gamma (HLG) Transcoding: https://www.bbc.co.uk/rd/sites/50335ff370b5c262af000004/assets/592eea8006d63e5e5200f90d/BBC_HDRTV_PQ_HLG_Transcode_v2.pdf

  rgb = maxf3(1e-10f, rgb);
  const float HLG_Lw = 1000.0f;
  // const float HLG_Lb = 0.0f;
  const float HLG_Ls = 5.0f;
  const float h_a = 0.17883277f;
  const float h_b = 1.0f - 4.0f * 0.17883277f;
  const float h_c = 0.5f - h_a * _logf(4.0f * h_a);
  const float h_g = 1.2f * spowf(1.111f, _log2f(HLG_Lw / 1000.0f)) * spowf(0.98f, _log2f(_fmaxf(1e-6f, HLG_Ls) / 5.0f));
  if (inverse == 1)
  {
    float Yd = 0.2627f * rgb.x + 0.6780f * rgb.y + 0.0593f * rgb.z;
    // HLG Inverse OOTF
    rgb = rgb * spowf(Yd, (1.0f - h_g) / h_g);
    // HLG OETF
    rgb.x = rgb.x <= 1.0f / 12.0f ? _sqrtf(3.0f * rgb.x) : h_a * _logf(12.0f * rgb.x - h_b) + h_c;
    rgb.y = rgb.y <= 1.0f / 12.0f ? _sqrtf(3.0f * rgb.y) : h_a * _logf(12.0f * rgb.y - h_b) + h_c;
    rgb.z = rgb.z <= 1.0f / 12.0f ? _sqrtf(3.0f * rgb.z) : h_a * _logf(12.0f * rgb.z - h_b) + h_c;
  }
  else
  {
    // HLG Inverse OETF
    rgb.x = rgb.x <= 0.5f ? rgb.x * rgb.x / 3.0f : (_expf((rgb.x - h_c) / h_a) + h_b) / 12.0f;
    rgb.y = rgb.y <= 0.5f ? rgb.y * rgb.y / 3.0f : (_expf((rgb.y - h_c) / h_a) + h_b) / 12.0f;
    rgb.z = rgb.z <= 0.5f ? rgb.z * rgb.z / 3.0f : (_expf((rgb.z - h_c) / h_a) + h_b) / 12.0f;
    // HLG OOTF
    float Ys = 0.2627f * rgb.x + 0.6780f * rgb.y + 0.0593f * rgb.z;
    rgb = rgb * spowf(Ys, h_g - 1.0f);
  }
  return rgb;
}

__DEVICE__ float inline unroll_highlight(float x, float peak, float input_middle_grey)
{

  x = x < 0 ? ((input_middle_grey - 1.0f) * peak * x) / ((1 - peak) * x - peak + input_middle_grey) : ((input_middle_grey - 1.0f) * peak * x) / ((peak - 1.0f) * x - peak + input_middle_grey);

  return x;
}

__DEVICE__ float3 inline unroll_highlight3(float3 rgb, float peak, float input_middle_grey, float output_middle_grey)
{
  rgb.x = unroll_highlight(rgb.x, peak * input_middle_grey / output_middle_grey, input_middle_grey) * output_middle_grey / input_middle_grey;
  rgb.y = unroll_highlight(rgb.y, peak * input_middle_grey / output_middle_grey, input_middle_grey) * output_middle_grey / input_middle_grey;
  rgb.z = unroll_highlight(rgb.z, peak * input_middle_grey / output_middle_grey, input_middle_grey) * output_middle_grey / input_middle_grey;

  return rgb;
}
#endif