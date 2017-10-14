////////////////////////////////////////////////////////////////////////////////
// ExSchedule

#pragma once
#include "BtTypes.h"
#include "BtArray.h"
#include "BtSingleton.h"
#include "BaArchive.h"
#include "FsFindFile.h"
#include <vector>

class ExScheduleItem
{
public:

	BtChar								m_szFilename[MaxFilePath];
	BtU32								m_nResourceID;
	BtU64								m_tLastExport;
};
