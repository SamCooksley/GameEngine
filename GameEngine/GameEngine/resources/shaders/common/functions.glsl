float InverseMix(float _a, float _b, float _value)
{
  return clamp((_value - _a) / (_b - _a), 0.0, 1.0);
}