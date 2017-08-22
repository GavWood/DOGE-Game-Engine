////////////////////////////////////////////////////////////////////////////////
// RsColour.h

#pragma once
#include "BtTypes.h"
#include "MtVector4.h"

class RsColour;

// Class Declaration
class RsColour
{
public:

	// Constructors
	RsColour();
	RsColour( BtU32 colour );
	RsColour( BtFloat fRed, BtFloat fGreen, BtFloat fBlue );
	RsColour( BtFloat fRed, BtFloat fGreen, BtFloat fBlue, BtFloat fAlpha );
	RsColour( BtU8 red, BtU8 green, BtU8 blue, BtU8 alpha );
    RsColour( const MtVector4& colour );

	// Public structures
    static const RsColour PinkColour()
    {
        return RsColour( 1.0f, 0.0f, 1.0f, 1.0f );
    }
    
    /*
    RsColour RsColour::BLACK  = RsColour( 0.0f, 0.0f, 0.0f, 1.0f );
    RsColour RsColour::WHITE  = RsColour( 1.0f, 1.0f, 1.0f, 1.0f );
    RsColour RsColour::DARKGREY  = RsColour( 0.1f, 0.1f, 0.1f, 1.0f );
    RsColour RsColour::GREY   = RsColour( 0.5f, 0.5f, 0.5f, 1.0f );
    RsColour RsColour::RED    = RsColour( 1.0f, 0.0f, 0.0f, 1.0f );
    RsColour RsColour::GREEN  = RsColour( 0.0f, 1.0f, 0.0f, 1.0f );
    RsColour RsColour::BLUE   = RsColour( 0.0f, 0.0f, 1.0f, 1.0f );
    RsColour RsColour::YELLOW = RsColour( 1.0f, 1.0f, 0.0f, 1.0f );
    RsColour RsColour::PINK   = RsColour( 1.0f, 0.0f, 1.0f, 1.0f );
    RsColour RsColour::TRANSPARENT_BLACK = RsColour( 0.0f, 0.0f, 0.0f, 0.0f );
    */
    
	static const RsColour WhiteColour()
	{
		return RsColour( 1.0f, 1.0f, 1.0f, 1.0f );
	}
	static const RsColour BlackColour()
	{
		return RsColour( 0.0f, 0.0f, 0.0f, 1.0f );
	}
	static const RsColour YellowColour()
	{
		return RsColour( 1.0f, 1.0f, 0.0f, 1.0f );
	}
	static const RsColour RedColour()
	{
		return RsColour( 1.0f, 0.0f, 0.0f, 1.0f );
	}
	static const RsColour NoColour()
	{
		return RsColour( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	static const RsColour GreenColour()
	{
		return RsColour( 0.0f, 1.0f, 0.0f, 1.0f );
	}
	static const RsColour BlueColour()
	{
		return RsColour( 0.0f, 0.0f, 1.0f, 1.0f );
	}

	// Operators
  	bool operator == ( const RsColour& ) const;
	bool operator != ( const RsColour& ) const;
	RsColour& operator += ( const RsColour& );
	RsColour operator + ( const RsColour& other ) const;
	RsColour& operator *= ( BtFloat );

	// Accessors
    void                                        SetRed( BtFloat red ) { m_red = red; }
    void                                        SetGreen( BtFloat green ) { m_green = green; }
    void                                        SetBlue( BtFloat blue ) { m_blue = blue; }
    
	BtFloat 									Red() const;
	BtFloat 									Green() const;
	BtFloat 									Blue() const;
	BtFloat 									Alpha() const;
	void										Alpha( BtFloat alpha );

	BtU32										asABGR() const;
	BtU32										asARGB() const;
	BtU32										asRGBA() const;
	BtU32										asWord() const;
	
	BtBool										IsColour() const;

	static void									HSVToRGB(double H, double S, double V, double &R, double &G, double &B);

private:

	// Member variables
	BtFloat 									m_red;
	BtFloat 									m_green;
	BtFloat 									m_blue;
	BtFloat 									m_alpha;
};

////////////////////////////////////////////////////////////////////////////////
// operator += ( const RsColour& colour )

inline RsColour& RsColour::operator += ( const RsColour& colour )
{
	m_red+=colour.m_red;
	m_green+=colour.m_green;
	m_blue+=colour.m_blue;
	m_alpha+=colour.m_alpha;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
// operator *= ( const RsColour& colour )

inline RsColour& RsColour::operator *= ( BtFloat fScale )
{
	m_red*=fScale;
	m_green*=fScale;
	m_blue*=fScale;
	m_alpha*=fScale;
	return *this;
}

inline RsColour RsColour::operator + ( const RsColour& other ) const
{
	RsColour colour;

	colour.m_red   = m_red + other.m_red;
	colour.m_green = m_green + other.m_green;
	colour.m_blue  = m_blue + other.m_blue;
	colour.m_alpha = m_alpha + other.m_alpha;
	return colour;
}

////////////////////////////////////////////////////////////////////////////////
// IsColour

inline BtBool RsColour::IsColour() const
{
	if( ( m_red == 0 ) && ( m_green == 0 ) && ( m_blue == 0 ) && ( m_alpha == 0 ) )
	{
		return BtFalse;
	}
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// Red

inline BtFloat RsColour::Red() const
{
	return m_red;
}

////////////////////////////////////////////////////////////////////////////////
// Green

inline BtFloat RsColour::Green() const
{
	return m_green;
}

////////////////////////////////////////////////////////////////////////////////
// Blue

inline BtFloat RsColour::Blue() const
{
	return m_blue;
}

////////////////////////////////////////////////////////////////////////////////
// Alpha

inline BtFloat RsColour::Alpha() const
{
	return m_alpha;
}

////////////////////////////////////////////////////////////////////////////////
// Alpha

inline void RsColour::Alpha( BtFloat alpha )
{
	m_alpha = alpha;
}
