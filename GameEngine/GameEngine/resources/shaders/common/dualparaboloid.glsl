vec3 DualParaboloidTexCoords(vec3 _lightToFrag)
{
    float depth = 1.0 + abs(_lightToFrag.z);
    vec3 coords;
    coords.x = (_lightToFrag.x / depth) * 0.5 + 0.5;
    coords.y = (_lightToFrag.y / depth) * 0.5 + 0.5;

    coords.z = ceil((sign(_lightToFrag.z) + 1.0) * 0.5); // -1 = 0, 1 = 1, 0 = 1

    return coords;
}