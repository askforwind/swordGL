/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-02-24 12:19
#
# Filename: Log.h
#
# Description:
#
==================================================================*/
#ifndef WIND7_SRC_LOG_H_
#define WIND7_SRC_LOG_H_
#include <log4cplus/loggingmacros.h>
#include <log4cplus/appender.h>
#include <log4cplus/logger.h>

#define WIND_LOG_TRACE(_logger , _log_event)      \
    LOG4CPLUS_TRACE(_logger , _log_event)

#define WIND_LOG_DEBUG(_logger , _log_event)      \
    LOG4CPLUS_DEBUG(_logger , _log_event)

#define WIND_LOG_INFO(_logger , _log_event)       \
    LOG4CPLUS_INFO(_logger , _log_event)

#define WIND_LOG_WARN(_logger , _log_event)       \
    LOG4CPLUS_WARN(_logger , _log_event)

#define WIND_LOG_ERROR(_logger , _log_event)      \
    LOG4CPLUS_ERROR(_logger , _log_event)

#define WIND_LOG_FATAL(_logger , _log_event)      \
    LOG4CPLUS_FATAL(_logger ,  _log_event)

namespace WIND {
using WindLogger = log4cplus::Logger;
using WindShareAppendePtr = log4cplus::SharedAppenderPtr;

enum LogLevel {
    Trace = log4cplus::TRACE_LOG_LEVEL,
    Debug = log4cplus::DEBUG_LOG_LEVEL,
    Info = log4cplus::INFO_LOG_LEVEL,
    Warn = log4cplus::WARN_LOG_LEVEL,
    Error = log4cplus::ERROR_LOG_LEVEL,
    Fatal = log4cplus::FATAL_LOG_LEVEL
};

}// namespace WIND

#endif   // WIND7_SRC_LOG_H_




















