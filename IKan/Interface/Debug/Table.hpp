//
//  Table.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan
{
  /// This structre create a table with entries and dump them as log
  class Table
  {
  public:
    /// This structure stores the table common data for table
    struct Data
    {
      uint32_t numColumns = 0;
      std::string title = "Table";
      std::vector<std::string> columnHeaders;
    };
    
    struct Column
    {
      uint32_t maxWidth = 0;
      uint32_t totalEntries = 0;
      std::vector<std::string> entries;
      
      /// This function adds the entry in column of table
      /// - Parameter entry: entries in column
      void AddEntry(const std::string& entry);
    };
    
    /// This constructor creates the Table instance
    /// - Parameters:
    ///   - tableData: Title of table
    Table(const Data& tableData);
    
    /// This function Prints the Table in IK LOG
    /// - Parameters:
    ///   - level: log level
    ///   - moduleName: Module name of logger
    void Dump(Log::Level level = Log::Level::Info, const std::string& moduleName = "");
    /// This function adds the element in table row. Format them based on spaces
    /// - Parameter rowElements: row elements
    void AddRow(const std::vector<std::string>& rowElements);
    /// This function creates Rows in vector with format from table Entries string and returns
    std::vector<std::string> GetRows() const;
    
  private:
    Data m_data;
    std::vector<Column> m_tableEntries;
  };
} // namespace IKan
