float CalculateAttenuation(float _radius, float _distance)
{
	return pow(clamp(1.0 - _distance / _radius, 0.0, 1.0), 2.0);
}