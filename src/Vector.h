#ifndef Vector_h
#define Vector_h

#define VECTORLENGTH_EPSILON 0.000001f

#include <math.h>

class Vector
{
public:
	float x,y,z;

	Vector() {};

	inline Vector(float nx, float ny, float nz)
	{
		x=nx;
		y=ny;
		z=nz;
	}

	~Vector() {};

	inline void Normalize()
	{
		float invlength=1.0f/(float)sqrt(x*x+y*y+z*z);
		x*=invlength;
		y*=invlength;
		z*=invlength;
	}

	inline void NormalizeSafe()
	{
		float invlength=(float)sqrt(x*x+y*y+z*z);
		if(invlength<VECTORLENGTH_EPSILON)
		{
			x=1;
			y=0;
			z=0;
			return;
		}
		invlength=1.0f/invlength;
		x*=invlength;
		y*=invlength;
		z*=invlength;
	}

	inline float Length() const
	{
		return (float)sqrt(x*x+y*y+z*z);
	}

	inline Vector operator+(const Vector &a) const
	{
		return Vector(x+a.x, y+a.y, z+a.z);
	}

	inline Vector operator-(const Vector &a) const
	{
		return Vector(x-a.x, y-a.y, z-a.z);
	}

	inline float operator*(const Vector &a) const // Dot Product
	{
		return x*a.x+y*a.y+z*a.z;
	}

	inline float operator/(const Vector &a) const // Distance Between
	{
		return (float)sqrt((x-a.x)*(x-a.x)+(y-a.y)*(y-a.y)+(z-a.z)*(z-a.z));
	}

	inline Vector operator*(const float &a) const
	{
		return Vector(x*a, y*a, z*a);
	}

	inline Vector operator%(const Vector &a) const // Cross Product
	{
		return Vector(y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x);
	}

	inline void operator+=(const Vector &a)
	{
		x+=a.x;
		y+=a.y;
		z+=a.z;
	}

	inline void operator-=(const Vector &a)
	{
		x-=a.x;
		y-=a.y;
		z-=a.z;
	}

	inline Vector operator-() const	// Oposite Vector
	{
		return Vector(-x, -y, -z);
	}

	inline void operator*=(const float &a)
	{
		x*=a;
		y*=a;
		z*=a;
	}

	inline bool operator==(const Vector &a) const
	{
		return (x==a.x && y==a.y && z==a.z);
	}

	inline bool operator!=(const Vector &a) const
	{
		return !(x==a.x && y==a.y && z==a.z);
	}
};
#endif
