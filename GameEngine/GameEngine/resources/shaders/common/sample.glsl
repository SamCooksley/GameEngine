vec3 CubeCoords(vec3 _p)
{
    vec3 coord;

    _p = normalize(_p);
    
    vec3 a = abs(_p);

    if (a.x >= a.y && a.x >= a.z)
    {
        _p.y = -_p.y;

        if (_p.x >= 0.0)
        {
            _p.z = -_p.z;

            coord.z = 0.0;
        }
        else
        {
            coord.z = 1.0;
        }

        _p /= a.x;
        coord.xy = _p.zy;

        //coord.z = 0.0 + step(_p.x, 0.0); 
    }
    else if (a.y >= a.x && a.y >= a.z)
    {
        if (_p.y >= 0.0)
        {
           coord.z = 2.0;
        }
        else
        {
            _p.z = -_p.z;

            coord.z = 3.0;
        }

        _p /= a.y;
        coord.xy = _p.xz;

        //coord.z = 2.0 + step(_p.y, 0.0); 
    }
    else
    {
        _p.y = -_p.y;
        
        if (_p.z >= 0.0)
        {
            coord.z = 4.0;
        }
        else
        {
            _p.x = -_p.x;

            coord.z = 5.0;
        }

        _p /= a.z;
        coord.xy = _p.xy;

        //coord.z = 4.0 + step(_p.z, 0.0); 
    }

    coord.xy = coord.xy * 0.5 + 0.5;

    return coord;
}