def intersect_cylinder( p, d ):
    '''
    Given a ray as a point `p` and direction `d`, returns ( t, n )
    such that `p + t*d` is the closest point on the cylinder
    intersected by the ray and n is the normal at that point.
    
    NOTE: This pseudocode does not consider the transformation into
          and out of object-space.
    '''
    
    ## The list of all intersections.
    ts = []
    
    ## z = 0 plane: F( x,y,z ) = -z
    t = -p.z / d.z
    q = p + t * d
    if q.x^2 + q.y^2 < 1:
        ts.append( ( t, ( 0, 0, -1 ) )
    
    
    ## z = 1 plane: F( x,y,z ) = z - 1
    t = ( 1 - p.z ) / d.z
    q = p + t * d
    if q.x^2 + q.y^2 < 1:
        ts.append( ( t, ( 0, 0, 1 ) )
    
    
    ## sides of the cylinder: F( x,y,z ) = x^2 + y^2 - 1
    ## F( p + t*d ) = ( p.x + t*d.x )^2 + ( p.y + t*d.y )^2 - 1
    ## <=> t^2 * a + t * b + c
    a = p.x something something p.y something something d.x something something d.y
    b = p.x something something p.y something something d.x something something d.y
    c = p.x something something p.y something something d.x something something d.y something something
    
    ## quadratic formula: t = ( -b +- sqrt( b^2 - 4ac ) )/( 2*a )
    radical = b^2 - 4*a*c
    if radical >= 0:
        t = ( -b + sqrt( radical ) )/( 2*a )
        q = p + t * d
        if 0 < q.z < 1: ts.append( ( t, ( 2*q.x, 2*q.y, 0 ) ) )
        
        t = ( -b - sqrt( radical ) )/( 2*a )
        q = p + t * d
        if 0 < q.z < 1: ts.append( ( t, ( 2*q.x, 2*q.y, 0 ) ) )    
    
    ts.sort()
    for t, normal in ts:
        if t >= 0: return t, normal
