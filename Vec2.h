//A 2d-vector class
#ifndef __AVec2__
#define __AVec2__

#include <cmath>

template<typename Val> class AVec2 {
	Val mx, my;
public:
	AVec2(Val xx = Val(), Val yy = Val())
		: mx(xx), my(yy)
	{ }

	/*     AVec2(const AVec2& v)
	*         : mx(v.mx), my(v.my)
	*     { }
	*
	*     AVec2& operator=(AVec2 v)
	*     {
	*         mx = v.mx;
	*         my = v.my;
	*         return *this;
	*     }
	*/

	void setPolar(Val radians, Val distance)
	{
		mx = distance*cos(radians);
		my = distance*sin(radians);
	}

	Val x() const { return mx; }
	Val y() const { return my; }

	Val radians() const
	{
		if (mx == my == 0.0)
			return 0;
		return atan2(my, mx);
	}


	Val magnitudeSquared() const
	{
		return mx*mx + my*my;
	}

	Val magnitude() const
	{
		return sqrt(magnitudeSquared());
	}

	AVec2& normalize()
	{
		Val m = magnitude();
		if (m != 0.0)
		{
			mx /= m;
			my /= m;
		}
		return *this;
	}

	AVec2& operator+=(const AVec2& rhs)
	{
		mx += rhs.mx;
		my += rhs.my;
		return *this;
	}

	AVec2& operator-=(const AVec2& rhs)
	{
		mx -= rhs.mx;
		my -= rhs.my;
		return *this;
	}

	AVec2& operator*=(const AVec2& rhs)
	{
		mx *= rhs;
		my *= rhs;
		return *this;
	}

	AVec2& operator/=(const AVec2& rhs)
	{
		mx /= rhs;
		my /= rhs;
		return *this;
	}

	//Vector2DAddition / Subtraction

	AVec2 operator+ (const AVec2 &rhs) const
	{
		return AVec2(mx + rhs.mx, my + rhs.my);
	}

	AVec2 operator- (const AVec2 &rhs) const
	{
		return AVec2(mx - rhs.mx, my - rhs.my);
	}

	AVec2 operator* (Val rhs) const
	{
		return AVec2(mx * rhs, my * rhs);
	}

	AVec2 operator/ (Val rhs) const
	{
		return AVec2(mx / rhs, my / rhs);
	}

	//Vector2DDot product
	Val dotproduct(const AVec2 &rhs)
	{
		return mx*rhs.mx + my*rhs.my;
	}

	//Vector2DSign operators

	AVec2 operator- () const
	{
		return AVec2(-mx, -my);
	}

	AVec2 operator+ () const
	{
		return AVec2(+mx, +my);
	}

	//Vector2DComparison

	bool operator== (const AVec2 &rhs) const
	{
		return (mx == rhs.mx) && (my == rhs.my);
	}

	bool operator!= (const AVec2 &rhs) const
	{
		return !(*this == rhs);
	}
};

//Vector2DScalar product
template<typename Val>
inline AVec2<Val> operator*(Val lhs, AVec2<Val> rhs)
{
	return rhs * lhs;
}

//Vector2DAbsolute value / magnitude

template<typename Val>
inline Val abs(const AVec2<Val> &v)
{
	return v.magnitude();
}


//A couple of handy typedefs

typedef AVec2<float> AVec2f;
typedef AVec2<double> AVec2d;

typedef AVec2<int> AVec2i;

#endif