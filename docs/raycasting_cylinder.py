def intersect_cylinder( p, d ):
    
    ## z = 0 plane: F( x,y,z ) = z
    t_z0 = -p.z / d.z
    c = p + t_z0 * d
    if c.x^2 + c.y^2 < 1:
        ts.append( ( t_z0, ( 0, 0, -1 ) )
    
    
    ## z = 1 plane: F( x,y,z ) = z - 1
    t_z1 = ( 1 - p.z ) / d.z
    c = p + t_z1 * d
    if c.x^2 + c.y^2 < 1:
        ts.append( ( t_z1, ( 0, 0, 1 ) )
    
    
    ## sides of the cylinder: F( x,y,z ) = x^2 + y^2 - 1
    ## F( p + t*d ) = ( p.x + t*d.x )^2 + ( p.y + t*d.y )^2 - 1
    ## <=> t^2 * a + t * b + c
    a = p.x something something p.y something something d.x something something d.y
    b = p.x something something p.y something something d.x something something d.y
    c = p.x something something p.y something something d.x something something d.y something something
    
    ## quadratic formula: t = ( -b +- sqrt( b^2 - 4ac ) )/( 2*a )
    radical = b^2 - 4*a*c
    if radical >= 0:
        t1 = ( -b + sqrt( radical ) )/( 2*a )
        if 0 < p.z + t1 * d.z < 1: ts.append( ( t1, ( 2*(p.x + t1*d.x), 2*(p.y + t1*d.y), 0 ) )
        t2 = ( -b - sqrt( radical ) )/( 2*a )
        if 0 < p.z + t2 * d.z < 1: ts.append( t2, ... )
    
    
    ts.sort()
    
    for t, normal in ts:
        if t >= 0: return t, normal
