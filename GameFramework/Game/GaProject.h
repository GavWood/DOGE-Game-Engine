////////////////////////////////////////////////////////////////////////////////
// GaProject.h

// Include guard
#pragma once
#include "BtTypes.h"

// Class definition
class GaProject
{
public:

	// Public functions
	virtual void					Init() = 0;
	virtual void					Reset() = 0;
	virtual void					Create() = 0;
	virtual void					Update() = 0;
	virtual void					Render() = 0;
	virtual void					Destroy() = 0;

	// Accessors
	void							SetClosing();
	BtBool							IsClosed();
	BtBool							IsClosing();

protected:

	BtBool							m_isClosing = BtFalse;
	BtBool							m_isClosed = BtFalse;
};

inline void GaProject::SetClosing()
{
	m_isClosing = BtTrue;
}

inline BtBool GaProject::IsClosed()
{
	return m_isClosed;
}

inline BtBool GaProject::IsClosing()
{
	return m_isClosing;
}
