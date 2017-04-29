// RsColour.cpp

// Library includes 
#include "BtTypes.h"
#include "BtMemory.h"
#include "RsColour.h"
#include "ApConfig.h"

// Constructor
RsColour::RsColour()
{
	m_red   = 0.0f;
	m_green = 0.0f;
	m_blue  = 0.0f;
	m_alpha = 0.0f;
}

RsColour::RsColour( BtU32 colour )
{
	BtU8 alpha  = ((BtU32)(colour >> 24) & 0xFF);
	BtU8 blue   = ((BtU32)(colour >> 16) & 0xFF);
	BtU8 green  = ((BtU32)(colour >> 8) & 0xFF);
	BtU8 red    = ((BtU32)(colour >> 0) & 0xFF);

	m_red   = (BtFloat)red / 255.0f;
	m_green = (BtFloat)green / 255.0f;
	m_blue  = (BtFloat)blue / 255.0f;
	m_alpha = (BtFloat)alpha / 255.0f;
}

RsColour::RsColour(BtFloat fRed, BtFloat fGreen, BtFloat fBlue)
{
	m_red   = fRed;
	m_green = fGreen;
	m_blue  = fBlue;
	m_alpha = 1.0f;
}

RsColour::RsColour(BtFloat fRed, BtFloat fGreen, BtFloat fBlue, BtFloat fAlpha)
{
	m_red   = fRed;
	m_green = fGreen;
	m_blue  = fBlue;
	m_alpha = fAlpha;
}

RsColour::RsColour(BtU8 red, BtU8 green, BtU8 blue, BtU8 alpha)
{
	m_red   = (BtFloat)red / 255.0f;
	m_green = (BtFloat)green / 255.0f;
	m_blue  = (BtFloat)blue / 255.0f;;
	m_alpha = (BtFloat)alpha / 255.0f;
}

////////////////////////////////////////////////////////////////////////////////
// operator != ( const RsColour& colour )

bool RsColour::operator != ( const RsColour& colour ) const
{
	if( ( m_red != colour.m_red ) ||
		( m_green != colour.m_green ) ||
		( m_blue != colour.m_blue ) ||
		( m_alpha != colour.m_alpha ) )
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// operator == ( const RsColour& colour )

bool RsColour::operator == ( const RsColour& colour ) const
{
	if( ( m_red ==colour.m_red ) &&
		( m_green == colour.m_green ) &&
		( m_blue == colour.m_blue ) &&
		( m_alpha == colour.m_alpha ) )
	{
		return true;
	}
	return false;
}

BtU32 RsColour::asARGB() const
{
	BtU32 colour;

	BtU8 nRed   = (BtU8) (m_red   * 255.0f);
	BtU8 nGreen = (BtU8) (m_green * 255.0f);
	BtU8 nBlue  = (BtU8) (m_blue  * 255.0f);
	BtU8 nAlpha = (BtU8) (m_alpha * 255.0f);

	colour = (BtU32)((((nAlpha)&0xff)<<24)|(((nRed)&0xff)<<16)|(((nGreen)&0xff)<<8)|((nBlue)&0xff));
	return colour;
}

BtU32 RsColour::asABGR() const
{
	BtU32 colour;

	BtU8 nRed   = (BtU8) (m_red   * 255.0f);
	BtU8 nGreen = (BtU8) (m_green * 255.0f);
	BtU8 nBlue  = (BtU8) (m_blue  * 255.0f);
	BtU8 nAlpha = (BtU8) (m_alpha * 255.0f);

	colour = (BtU32)((((nAlpha)&0xff)<<24)|(((nBlue)&0xff)<<16)|(((nGreen)&0xff)<<8)|((nRed)&0xff));
	return colour;
}

BtU32 RsColour::asRGBA() const
{
	BtU32 colour;

	BtU8 nRed   = (BtU8) (m_red   * 255.0f);
	BtU8 nGreen = (BtU8) (m_green * 255.0f);
	BtU8 nBlue  = (BtU8) (m_blue  * 255.0f);
	BtU8 nAlpha = (BtU8) (m_alpha * 255.0f);

	colour = (BtU32)((((nRed)&0xff)<<24)|(((nGreen)&0xff)<<16)|(((nBlue)&0xff)<<8)|((nAlpha)&0xff));
	return colour;
}

////////////////////////////////////////////////////////////////////////////////
// HSVToRGB

void RsColour::HSVToRGB(double H, double S, double V, double &R, double &G, double &B)
{
	double step = 1.0 / 6.0;
	double vh = H / step;

	int i = (int)floor(vh);

	double f = vh - i;
	double p = V * (1.0 - S);
	double q = V * (1.0 - (S * f));
	double t = V * (1.0 - (S * (1.0 - f)));

	if (i == 0)
	{
		R = V; G = t; B = p;
	}
	else if (i == 1)
	{
		R = q; G = V; B = p;
	}
	else if (i == 2)
	{
		R = p; G = V; B = t;
	}
	else if (i == 3)
	{
		R = p; G = q; B = V;
	}
	else if (i == 4)
	{
		R = t; G = p; B = V;
	}
	else if (i == 5)
	{
		R = V; G = p; B = q;
	}
}