//
//  EditorConsoleSink.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "spdlog/sinks/base_sink.h"

#include "Editor/Console/EditorConsoleMessage.hpp"

namespace IKan
{
  class EditorConsoleSink : public spdlog::sinks::base_sink<std::mutex>
  {
  public:
    /// This is the Editor console sink constructor
    /// - Parameter bufferCapacity: buffer capacity
    explicit EditorConsoleSink(uint32_t bufferCapacity);
    /// This is the default Editor console sink destructor
    virtual ~EditorConsoleSink() = default;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(EditorConsoleSink);
    
  protected:
    /// Override sink it for spd log
    /// - Parameter msg: message
    void sink_it_(const spdlog::details::log_msg& msg) override;
    /// Override flusg it for spd log
    void flush_() override;
    
  private:
    /// This function return the message category
    /// - Parameter level: log level
    static ConsoleMessage::Category GetMessageCategory(spdlog::level::level_enum level);
    
  private:
    uint32_t m_messageBufferCapacity;
    std::vector<ConsoleMessage> m_messageBuffer;
    uint32_t m_messageCount = 0;
  };
} // namespace IKan
