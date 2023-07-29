//
//  Table.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Table.hpp"

namespace IKan
{
  
#define TABLE_TRACE(...)    IK_CORE_TRACE(moduleName.c_str(), __VA_ARGS__);
#define TABLE_DEBUG(...)    IK_CORE_DEBUG(moduleName.c_str(), __VA_ARGS__);
#define TABLE_INFO(...)     IK_CORE_INFO(moduleName.c_str(), __VA_ARGS__);
#define TABLE_WARN(...)     IK_CORE_WARN(moduleName.c_str(), __VA_ARGS__);
#define TABLE_ERROR(...)    IK_CORE_ERROR(moduleName.c_str(), __VA_ARGS__);
#define TABLE_CRITICAL(...) IK_CORE_CRITICAL(moduleName.c_str(), __VA_ARGS__);
  
#define TABLE_LOG(x) LogTable(level, moduleName, x);
  
  ///  This function logs the table content based on the log level with module name
  /// - Parameters:
  ///   - level: Log Level
  ///   - moduleName: Module name of Table
  ///   - data: data to be logged
  static void LogTable(Log::Level level, const std::string& moduleName, const std::string& data)
  {
    switch (level)
    {
      case Log::Level::Trace:    TABLE_TRACE("{0}", data); break;
      case Log::Level::Debug:    TABLE_DEBUG("{0}", data); break;
      case Log::Level::Info:     TABLE_INFO("{0}", data); break;
      case Log::Level::Warning:  TABLE_WARN("{0}", data); break;
      case Log::Level::Error:    TABLE_ERROR("{0}", data); break;
      case Log::Level::Critical: TABLE_CRITICAL("{0}", data); break;
      default:
        break;
    }
  }
  
  void Table::Column::AddEntry(const std::string &entry)
  {
    entries.push_back(entry);
    maxWidth = std::max(maxWidth, (uint32_t)entry.size());
    totalEntries++;
  }
  
  Table::Table(const Data& tableData)
  : m_data(tableData)
  {
    // Reserve data for colums
    m_tableEntries.resize(m_data.numColumns);
    
    // Add the headers as first row if header data is provided
    if (m_data.columnHeaders.size() > 0)
    {
      IK_CORE_ASSERT(m_data.numColumns == m_data.columnHeaders.size());
      AddRow(m_data.columnHeaders);
    }
  }
  
  void Table::AddRow(const std::vector<std::string>& rowElements)
  {
    for(uint32_t elementIdx = 0; elementIdx < (uint32_t)rowElements.size(); elementIdx++)
    {
      m_tableEntries[elementIdx].AddEntry(rowElements[elementIdx]);
    }
  }
  
  std::vector<std::string> Table::GetRows() const
  {
    std::vector<std::string> resultRows;
    // For each column etries process each column
    for (int colIdx = 0; colIdx < m_data.numColumns; colIdx++)
    {
      auto& col = m_tableEntries.at(colIdx);
      resultRows.resize(col.totalEntries);
      
      // For each entry in column data process each element
      for (int rowIdx = 0; rowIdx < col.totalEntries; rowIdx++)
      {
        std::string& val = resultRows[rowIdx];
        const std::string& entry = col.entries.at(rowIdx);
        
        // Format the data with correct spaces
        val += "    ";
        val += entry;
        val += std::string(size_t(col.maxWidth - (uint32_t)entry.size()), ' ');
      }
    }
    return resultRows;
  }
  
  void Table::Dump(Log::Level level, const std::string& moduleName)
  {
    // Print Header
    TABLE_LOG(m_data.title);
    
    // Print Rows ...
    std::vector<std::string> rows = GetRows();
    for (int rowIdx = 0; rowIdx < rows.size(); rowIdx++)
    {
      if (rowIdx == 0)
      {
        TABLE_LOG(std::string(size_t(rows[rowIdx].size()), '-').c_str());
      }
      TABLE_LOG(rows.at(rowIdx).c_str());
      
      if (rowIdx == 0 and m_data.columnHeaders.size() > 0)
      {
        TABLE_LOG(std::string(size_t(rows[rowIdx].size()), '-').c_str());
      }
    }
  }
} // namespace IKan
