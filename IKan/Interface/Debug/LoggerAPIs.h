//
//  LoggerAPIs.h
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#pragma once

#ifdef IK_ENABLE_LOG

// Console Logs API ---------------------------------------------------------------------------------------------------
#define IK_CONSOLE_TRACE(tag, ...)    ::IKan::Logger::PrintMessage(::IKan::LogType::Editor, ::IKan::LogLevel::Trace, tag, __VA_ARGS__)
#define IK_CONSOLE_DEBUG(tag, ...)    ::IKan::Logger::PrintMessage(::IKan::LogType::Editor, ::IKan::LogLevel::Debug, tag, __VA_ARGS__)
#define IK_CONSOLE_INFO(tag, ...)     ::IKan::Logger::PrintMessage(::IKan::LogType::Editor, ::IKan::LogLevel::Info, tag, __VA_ARGS__)
#define IK_CONSOLE_WARN(tag, ...)     ::IKan::Logger::PrintMessage(::IKan::LogType::Editor, ::IKan::LogLevel::Warning, tag, __VA_ARGS__)
#define IK_CONSOLE_ERROR(tag, ...)    ::IKan::Logger::PrintMessage(::IKan::LogType::Editor, ::IKan::LogLevel::Error, tag, __VA_ARGS__)
#define IK_CONSOLE_CRITICAL(tag, ...) ::IKan::Logger::PrintMessage(::IKan::LogType::Editor, ::IKan::LogLevel::Critical, tag, __VA_ARGS__)

// Core Logs API ------------------------------------------------------------------------------------------------------
#define IK_LOG_TRACE(tag, ...)   ::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Trace, tag, __VA_ARGS__);
#define IK_LOG_DEBUG(tag, ...)   ::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Debug, tag, __VA_ARGS__);
#define IK_LOG_INFO(tag, ...)    ::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Info, tag, __VA_ARGS__);
#define IK_LOG_WARN(tag, ...)    ::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Warning, tag, __VA_ARGS__);
#define IK_LOG_ERROR(tag, ...)   ::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Error, tag, __VA_ARGS__);
#define IK_LOG_CRITICAL(tag, ...)::IKan::Logger::PrintMessage(::IKan::LogType::Core, ::IKan::LogLevel::Critical, tag, __VA_ARGS__);

// Profiler Log API ---------------------------------------------------------------------------------------------------
#define IK_PROFILE_TRACE(...)    ::IKan::Logger::PrintMessageWithoutTag(::IKan::LogType::Profiler, ::IKan::LogLevel::Trace, __VA_ARGS__)
#define IK_PROFILE_DEBUG(...)    ::IKan::Logger::PrintMessageWithoutTag(::IKan::LogType::Profiler, ::IKan::LogLevel::Debug, __VA_ARGS__)
#define IK_PROFILE_INFO(...)     ::IKan::Logger::PrintMessageWithoutTag(::IKan::LogType::Profiler, ::IKan::LogLevel::Info, __VA_ARGS__)
#define IK_PROFILE_WARN(...)     ::IKan::Logger::PrintMessageWithoutTag(::IKan::LogType::Profiler, ::IKan::LogLevel::Warning, __VA_ARGS__)
#define IK_PROFILE_ERROR(...)    ::IKan::Logger::PrintMessageWithoutTag(::IKan::LogType::Profiler, ::IKan::LogLevel::Error, __VA_ARGS__)
#define IK_PROFILE_CRITICAL(...) ::IKan::Logger::PrintMessageWithoutTag(::IKan::LogType::Profiler, ::IKan::LogLevel::Critical, __VA_ARGS__)

#else

#define IK_CONSOLE_TRACE(tag, ...)
#define IK_CONSOLE_DEBUG(tag, ...)
#define IK_CONSOLE_INFO(tag, ...)
#define IK_CONSOLE_WARN(tag, ...)
#define IK_CONSOLE_ERROR(tag, ...)
#define IK_CONSOLE_CRITICAL(tag, ...)

#define IK_LOG_TRACE(...)
#define IK_LOG_DEBUG(...)
#define IK_LOG_INFO(...)
#define IK_LOG_WARN(...)
#define IK_LOG_ERROR(...)
#define IK_LOG_CRITICAL(...)

#define IK_PROFILE_TRACE(tag, ...)
#define IK_PROFILE_DEBUG(tag, ...)
#define IK_PROFILE_INFO(tag, ...)
#define IK_PROFILE_WARN(tag, ...)
#define IK_PROFILE_ERROR(tag, ...)
#define IK_PROFILE_CRITICAL(tag, ...)

#endif
