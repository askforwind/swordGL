/*=================================================================
#
# Copyright 2016-2020
#
# Author: luoyi 844262725@qq.com
#
# Last modified: 2016-02-24 14:31
#
# Filename: LogManager.cc
#
# Description:
#
==================================================================*/

#include "LogManager.h"
#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/helpers/loglog.h>

namespace WIND {

LogString LogManager::default_console_format_ =
    WIND_LOG_TEXT("%D{%H:%M:%S} [%p: %m] [NDC:%x] [in %F:%L %M]\n");

LogString LogManager::default_file_format_ =
    WIND_LOG_TEXT("%D{%H:%M:%S} [%p: %m] [NDC:%x] [in %F:%L %M]\n");

WindShareAppendePtr LogManager::default_file_appender_(
    new log4cplus::FileAppender(WIND_LOG_TEXT("Wind.log")));

WindShareAppendePtr LogManager::default_console_appender_(
    new log4cplus::ConsoleAppender(false, true));

WindLogger LogManager::default_file_logger_ =
    LogManager::get_instance(WIND_LOG_TEXT("default_file_logger_"));

WindLogger LogManager::default_console_logger_ =
    LogManager::get_instance(WIND_LOG_TEXT("default_console_logger_"));

void LogManager::initialize() {
    log4cplus::initialize();
    log4cplus::helpers::LogLog::getLogLog()->setInternalDebugging(true);
    initDefaultFileLogger();
    initDefaultConsoleLogger();
}

void LogManager::initDefaultFileLogger() {
    default_file_appender_->setName(WIND_LOG_TEXT("default_file_appender"));
    set_format(default_file_appender_, default_file_format_);
    default_file_logger_.addAppender(default_file_appender_);
    default_file_logger_.setLogLevel(LogLevel::Trace);
}

void LogManager::initDefaultConsoleLogger() {

    default_console_appender_->setName(WIND_LOG_TEXT("default_console_appender"));
    set_format(default_console_appender_, default_console_format_);
    default_console_logger_.addAppender(default_console_appender_);
    default_console_logger_.setLogLevel(LogLevel::Trace);
}

WindLogger LogManager::get_instance(const LogString& name) {
    return log4cplus::Logger::getInstance(name);
}

WindLogger& LogManager::get_default_console_logger() {
    return default_console_logger_;
}

WindLogger& LogManager::get_default_file_logger() {
    return default_file_logger_;
}

LogString LogManager::set_default_file_logger_format(const LogString& format) {
    set_format(default_file_appender_, format);
    LogString old = default_file_format_;
    default_file_format_ = format;
    return old;
}

LogString LogManager::set_default_console_logger_format(const LogString& format) {
    set_format(default_console_appender_, format);
    LogString old = default_console_format_;
    default_console_format_ = format;
    return old;
}

void LogManager::set_format(WindShareAppendePtr appender, const LogString& format) {
    appender->setLayout(std::auto_ptr<log4cplus::Layout>(
                            new log4cplus::PatternLayout(format)));
}

}// namespace WIND














