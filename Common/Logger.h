//////////////////////////////////////////////////////////////////////
//
// Logger.h: interface for the Logger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_LOGGER_H__E49478AA_691B_4FE7_B42C_48BCAC83A282__INCLUDED_)
#define _LOGGER_H__E49478AA_691B_4FE7_B42C_48BCAC83A282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>
#include <windows.h>

// Note: Need to include the path to "log4cplus" in the "VC++ Directories->Include Directories" first
#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h"
#include "log4cplus/configurator.h"
#include "log4cplus/initializer.h"

// Note: Need to include the path ("../Common/log4cplus") in the "Linker -> General -> Additional Library Directories"
#ifdef _DEBUG
	#ifdef _UNICODE
		#pragma comment(lib, "log4cplusUD.lib")
	#else
		#pragma comment(lib, "log4cplusD.lib")
	#endif	// _UNICODE
#else
	#ifdef _UNICODE
		#pragma comment(lib, "log4cplusU.lib")
	#else
	#pragma comment(lib, "log4cplus.lib")
	#endif	// _UNICODE
#endif // _DEBUG

namespace Logger
{
	static BOOL bInitialized = FALSE;

	static void InitLogger(LPCTSTR pcszConfigFile)
	{
		if (_taccess(pcszConfigFile, 0) == -1)
		{
			TCHAR szBuf[1000];
			_stprintf_s(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), _T("Could not find the config file '%s'"), pcszConfigFile);
			throw szBuf;
		}

		log4cplus::Initializer initializer;
		log4cplus::PropertyConfigurator::doConfigure(pcszConfigFile);

		bInitialized = TRUE;
	}

	static void InitLogger()
	{
		InitLogger(_T("log4cplus.ini"));
	}

	static log4cplus::Logger GetLogger(LPCTSTR pcszName)
	{
		if (!bInitialized)
		{
			InitLogger();
		}

		return log4cplus::Logger::getInstance(pcszName);
	}
}
#endif // !defined(_LOGGER_H__E49478AA_691B_4FE7_B42C_48BCAC83A282__INCLUDED_)
