//
//  LoggerAPIs.h
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#pragma once

#ifdef IK_ENABLE_LOG

// Core Logs API ------------------------------------------------------------------------------------------------------
#define IK_LOG_TRACE(tag, ...)   ::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Trace, tag, __VA_ARGS__);
#define IK_LOG_DEBUG(tag, ...)   ::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Debug, tag, __VA_ARGS__);
#define IK_LOG_INFO(tag, ...)    ::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Info, tag, __VA_ARGS__);
#define IK_LOG_WARN(tag, ...)    ::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Warning, tag, __VA_ARGS__);
#define IK_LOG_ERROR(tag, ...)   ::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Error, tag, __VA_ARGS__);
#define IK_LOG_CRITICAL(tag, ...)::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Critical, tag, __VA_ARGS__);

#else

#define IK_LOG_TRACE(...)
#define IK_LOG_DEBUG(...)
#define IK_LOG_INFO(...)
#define IK_LOG_WARN(...)
#define IK_LOG_ERROR(...)
#define IK_LOG_CRITICAL(...)

#endif
