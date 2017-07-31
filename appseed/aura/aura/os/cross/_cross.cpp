#pragma once



#ifndef WINDOWSEX

#include "windows/_cross_windows.cpp"

#endif


#if !defined(LINUX) && !defined(__APPLE__) && !defined(SOLARIS) && !defined(ANDROID)

#include "ansios/_cross_ansios.cpp"

#endif


#ifndef METROWIN

#include "metrowin/_cross_metrowin.cpp"

#endif



