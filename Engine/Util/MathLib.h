#pragma once

#include <DirectXMath.h>
#include <intrin.h>

namespace Math
{
	constexpr float PI = DirectX::XM_PI;

	constexpr inline float DegToRad( const float deg )
	{
		return deg * (PI / 180.0f);
	}
	constexpr inline float RadToDeg( const float rad )
	{
		return rad * (180.0f / PI);
	}

	__m128 Abs( __m128 m );
	__m128 Sin( __m128 m_x );
	float Sin( float x );
	float Cos( float x );
	void SinCos( float x, float* s, float* c );

	void AngleVectors( const DirectX::XMFLOAT3& angles, DirectX::XMFLOAT3* forward = nullptr, DirectX::XMFLOAT3* right = nullptr, DirectX::XMFLOAT3* up = nullptr );
}

class Vec3 : public DirectX::XMFLOAT3
{
public:
	Vec3() = default;
	Vec3( const DirectX::XMFLOAT3& src )
	{
		x = src.x;
		y = src.y;
		z = src.z;
	}

	Vec3 operator=( const DirectX::XMFLOAT3& src )
	{
		x = src.x;
		y = src.y;
		z = src.z;

		return *this;
	}

	Vec3& operator*( float scalar )
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	Vec3& operator-()
	{
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}
};