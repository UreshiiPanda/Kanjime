void
SetPointLight( int ilight, float x, float y, float z,  float r, float g, float b, float a)
{
	glLightfv( ilight, GL_POSITION,  Array3( x, y, z, 1. ) );
	glLightf(  ilight, GL_SPOT_CUTOFF, 180.f );
	glLightfv( ilight, GL_AMBIENT,   MulArray3( 0.1f,  1.f, 1.f, 1.f, 1.f ) );
	glLightfv( ilight, GL_DIFFUSE,   MulArray3( 0.6f, r, g, b, a ) );
	glLightfv( ilight, GL_SPECULAR,  MulArray3( 0.4f, 1.f, 1.f, 1.f, 1.f ) );
	glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
	glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
	glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
	glEnable( ilight );
}

void
SetSpotLight( int ilight, float x, float y, float z,  float xdir, float ydir, float zdir, float r, float g, float b )
{
	glLightfv( ilight, GL_POSITION,  Array3( x, y, z, 1. ) );
	glLightfv( ilight, GL_SPOT_DIRECTION,  Array3(xdir,ydir,zdir, 1.) );
	glLightf(  ilight, GL_SPOT_EXPONENT, 1.f );
	glLightf(  ilight, GL_SPOT_CUTOFF, 45.f );
	glLightfv( ilight, GL_AMBIENT,   Array3( 0., 0., 0., 1. ) );
	glLightfv( ilight, GL_DIFFUSE,   Array3( r, g, b, 1. ) );
	glLightfv( ilight, GL_SPECULAR,  Array3( r, g, b, 1. ) );
	glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
	glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
	glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
	glEnable( ilight );
}

