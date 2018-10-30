#pragma once

class Colour
{
public:
	constexpr Colour()
		:
		value()
	{}
	constexpr Colour( unsigned int val )
		:
		value( val )
	{}
	constexpr Colour( int r, int g, int b )
		:
		Colour( ( r << 16u ) | ( g << 8u ) | b )
	{}
	constexpr Colour( int r, int g, int b, int a )
		:
		Colour( ( a << 24u ) | ( r << 16u ) | ( g << 8u ) | b )
	{}
	constexpr Colour( const Colour& src )
		:
		value( src.value )
	{}
	Colour& operator=( const Colour& src )
	{
		value = src.value;
		return *this;
	}
	bool operator==( const Colour& rhs ) const
	{
		return ( value << 8u ) == ( rhs.value << 8u );
	}
	bool operator!=( const Colour& rhs ) const
	{
		return !( *this == rhs );
	}

	constexpr unsigned char GetA() const
	{
		return ( value >> 24u );
	}
	void SetA( unsigned char a )
	{
		value = ( value & 0x00FFFFFF ) | ( a << 24u );
	}

	constexpr unsigned char GetR() const
	{
		return ( value >> 16u ) & 0xFFu;
	}
	void SetR( unsigned char r )
	{
		value = ( value & 0xFF00FFFF ) | ( r << 16u );
	}

	constexpr unsigned char GetG() const
	{
		return ( value >> 8u ) & 0xFFu;
	}
	void SetG( unsigned char g )
	{
		value = ( value & 0xFFFF00FF ) | ( g << 8u );
	}

	constexpr unsigned char GetB() const
	{
		return ( value ) & 0xFFu;
	}
	void SetB( unsigned char b )
	{
		value = ( value & 0xFFFFFF00 ) | b;
	}

	unsigned int GetValue() const
	{
		return value;
	}
	void SetValue( unsigned int newval )
	{
		value = newval;
	}
private:
	unsigned int value;
};