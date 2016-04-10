/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-02-24 15:26
#
# Filename: Logstring.h
#
# Description:
#
==================================================================*/

#ifndef WIND7_SRC_LOGSTRING_H_
#define WIND7_SRC_LOGSTRING_H_

#include<log4cplus/tstring.h>

namespace WIND {

using LogString = log4cplus::tstring;

#define WIND_LOG_TEXT(STR) LOG4CPLUS_TEXT(STR)

} // namespace WIND

#endif // WIND7_SRC_LOGSTRING_H_

