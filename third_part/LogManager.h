/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-02-24 12:19
#
# Filename: LogManager.h
#
# Description:
#
==================================================================*/
#ifndef WIND7_SRC_LOGMANAGER_H_
#define WIND7_SRC_LOGMANAGER_H_
#include "Config.h"
#include "Logstring.h"
#include "Log.h"
namespace WIND {


class WIND_EXPORT LogManager {
  public:
    static void initialize();
    static WindLogger get_instance(const LogString& name);

    static WindLogger& get_default_console_logger();
    static WindLogger& get_default_file_logger();
    static void set_format(WindShareAppendePtr appender, const LogString& format);

    static LogString set_default_console_logger_format(const LogString& format);
    static LogString set_default_file_logger_format(const LogString& format);
  private:
    static void initDefaultConsoleLogger();
    static void initDefaultFileLogger();

    static WindLogger default_console_logger_;
    static WindLogger default_file_logger_;
    static LogString default_console_format_;
    static LogString default_file_format_;
    static WindShareAppendePtr default_file_appender_;
    static WindShareAppendePtr default_console_appender_;
};
}  // namespace WIND



#endif   // WIND7_SRC_LOGMANAGER_H_











