#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <intrin.h>

namespace Bat
{
	template <typename T>
	class _Vec2
	{
	public:
		_Vec2 operator-( const _Vec2& rhs ) const
		{
			return { x - rhs.x, y - rhs.y };
		}
		_Vec2 operator+( const _Vec2& rhs ) const
		{
			return { x + rhs.x, y + rhs.y };
		}

		_Vec2& operator-=( const _Vec2& rhs )
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		_Vec2& operator+=( const _Vec2& rhs )
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
	public:
		T x;
		T y;
	};

	using Vei2 = _Vec2<int>;
	using Veu2 = _Vec2<unsigned>;

	template <typename T>
	class _Vec3
	{
	public:
		_Vec3 operator-( const _Vec3& rhs ) const
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z };
		}
		_Vec3 operator+( const _Vec3& rhs ) const
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z };
		}

		_Vec3& operator-=( const _Vec3& rhs )
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		_Vec3& operator+=( const _Vec3& rhs )
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
	public:
		T x;
		T y;
		T z;
	};

	using Vei3 = _Vec3<int>;
	using Veu3 = _Vec3<unsigned>;

	template <typename T>
	class _Vec4
	{
	public:
		_Vec4( T x, T y, T z, T w )
			:
			x( x ),
			y( y ),
			z( z ),
			w( w )
		{}

		_Vec4 operator-( const _Vec4& rhs ) const
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
		}
		_Vec4 operator+( const _Vec4& rhs ) const
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}

		_Vec4& operator-=( const _Vec4& rhs )
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}
		_Vec4& operator+=( const _Vec4& rhs )
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}
	public:
		T x;
		T y;
		T z;
		T w;
	};

	using Vei4 = _Vec4<int>;
	using Veu4 = _Vec4<unsigned>;

	// Vec2/Vec3 aren't templated so that they can be used with the XMMath functions

	class Vec2 : public DirectX::XMFLOAT2
	{
	public:
		Vec2() = default;
		Vec2( const float x, const float y )
			:
			DirectX::XMFLOAT2( x, y )
		{}
		Vec2( const DirectX::XMFLOAT2& src )
		{
			x = src.x;
			y = src.y;
		}
		Vec2( const DirectX::XMVECTOR v )
		{
			DirectX::XMStoreFloat2( this, v );
		}

		Vec2& operator=( const DirectX::XMFLOAT2& src )
		{
			x = src.x;
			y = src.y;

			return *this;
		}
		Vec2 operator*( float scalar ) const
		{
			Vec2 res;
			res.x = x * scalar;
			res.y = y * scalar;

			return res;
		}
		Vec2 operator-() const
		{
			Vec2 res;
			res.x = -x;
			res.y = -y;

			return res;
		}
		Vec2 operator+( const Vec2& rhs ) const
		{
			Vec2 res;
			res.x = x + rhs.x;
			res.y = y + rhs.y;

			return res;
		}
		Vec2 operator-( const Vec2& rhs ) const
		{
			Vec2 res;
			res.x = x - rhs.x;
			res.y = y - rhs.y;

			return res;
		}

		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat2( this );
		}
	};

	class Vec3 : public DirectX::XMFLOAT3
	{
	public:
		Vec3() = default;
		Vec3( const float x, const float y, const float z )
			:
			DirectX::XMFLOAT3( x, y, z )
		{}
		Vec3( const DirectX::XMFLOAT3& src )
		{
			x = src.x;
			y = src.y;
			z = src.z;
		}
		Vec3( const DirectX::XMVECTOR v )
		{
			DirectX::XMStoreFloat3( this, v );
		}

		Vec3& operator=( const DirectX::XMFLOAT3& src )
		{
			x = src.x;
			y = src.y;
			z = src.z;

			return *this;
		}
		Vec3 operator*( float scalar ) const
		{
			Vec3 res;
			res.x = x * scalar;
			res.y = y * scalar;
			res.z = z * scalar;

			return res;
		}
		Vec3 operator-() const
		{
			Vec3 res;
			res.x = -x;
			res.y = -y;
			res.z = -z;

			return res;
		}
		Vec3 operator+( const Vec3& rhs ) const
		{
			Vec3 res;
			res.x = x + rhs.x;
			res.y = y + rhs.y;
			res.z = z + rhs.z;

			return res;
		}
		Vec3 operator-( const Vec3& rhs ) const
		{
			Vec3 res;
			res.x = x - rhs.x;
			res.y = y - rhs.y;
			res.z = z - rhs.z;

			return res;
		}
		Vec3& operator+=( const Vec3& rhs )
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;

			return *this;
		}
		Vec3& operator-=( const Vec3& rhs )
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;

			return *this;
		}

		float LengthSq() const
		{
			return x*x + y*y + z*z;
		}
		
		float Length() const
		{
			return sqrt( LengthSq() );
		}

		Vec3& Normalize()
		{
			float length = Length();
			x /= length;
			y /= length;
			z /= length;

			return *this;
		}

		Vec3 Normalized() const
		{
			float length = Length();

			return { x / length, y / length, z / length };
		}

		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat3( this );
		}
	};

	class Vec4 : public DirectX::XMFLOAT4
	{
	public:
		Vec4() = default;
		Vec4( const float x, const float y, const float z, const float w )
			:
			DirectX::XMFLOAT4( x, y, z, w )
		{}
		Vec4( const Vec3& v3, const float vw )
		{
			x = v3.x;
			y = v3.y;
			z = v3.z;
			w = vw;
		}

		Vec4( const DirectX::XMFLOAT4& src )
		{
			x = src.x;
			y = src.y;
			z = src.z;
			w = src.w;
		}
		Vec4( const DirectX::XMVECTOR v )
		{
			DirectX::XMStoreFloat4( this, v );
		}

		Vec4& operator=( const DirectX::XMFLOAT4& src )
		{
			x = src.x;
			y = src.y;
			z = src.z;
			w = src.w;

			return *this;
		}
		Vec4& operator=( const DirectX::XMFLOAT3& src )
		{
			x = src.x;
			y = src.y;
			z = src.z;
			w = 1.0f;

			return *this;
		}
		Vec4 operator*( float scalar ) const
		{
			Vec4 res;
			res.x = x * scalar;
			res.y = y * scalar;
			res.z = z * scalar;
			res.w = w * scalar;

			return res;
		}
		Vec4 operator/( float scalar ) const
		{
			Vec4 res;
			res.x = x / scalar;
			res.y = y / scalar;
			res.z = z / scalar;
			res.w = w / scalar;

			return res;
		}
		Vec4 operator-() const
		{
			Vec4 res;
			res.x = -x;
			res.y = -y;
			res.z = -z;
			res.w = -w;

			return res;
		}

		Vec4& operator*=( float scalar )
		{
			x = x * scalar;
			y = y * scalar;
			z = z * scalar;
			w = w * scalar;

			return *this;
		}
		Vec4& operator/=( float scalar )
		{
			x = x / scalar;
			y = y / scalar;
			z = z / scalar;
			w = w / scalar;

			return *this;
		}

		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat4( this );
		}
	};

	struct Plane
	{
		Vec3 n; // Plane normal
		float d; // Distance from origin
	};

	// Axis-aligned bounding box
	class AABB
	{
	public:
		AABB() = default;
		AABB( const Vec3* points, size_t num_points );

		Vec3 GetCenter() const { return (mins + maxs) / 2; }
		// Re-aligned transformed mins/maxs
		// This just gets the AABB of the rotated AABB of the mesh
		// Will have lots of empty space, but better than recalculating AABB for rotated mesh vertices
		AABB Transform( DirectX::XMMATRIX transform ) const;
		void GetPoints( Vec3 out[8] ) const;
	public:
		Vec3 mins;
		Vec3 maxs;
	};

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

		Vec3 QuaternionToEuler( const Vec4& quat );
		Vec4 EulerToQuaternion( const Vec3& euler );

		__m128 Abs( __m128 m );
		__m128 Sin( __m128 m_x );
		float Sin( float x );
		float Cos( float x );
		void SinCos( float x, float* s, float* c );

		void AngleVectors( const Vec3& angles, Vec3* forward = nullptr, Vec3* right = nullptr, Vec3* up = nullptr );

		// Returns a random int in the range [min, max]
		int GetRandomInt( int min, int max );
		// Returns a random float in the range [min, max)
		float GetRandomFloat( float min, float max );

		bool CloseEnough( float a, float b, float epsilon = 0.001f );
	}
}