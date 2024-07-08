//
//  DesignHelper.h
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

namespace IKan
{
  /// This class is the vector iterator for any vector containing class. Provides basic APIs regarding vector data.
  template <typename T> class VectorIterator
  {
  public:
    // Fundamentals --------------------------------------------------------------------------------------------------
    /// This function clears the vector data
    void Clear()
    {
      m_data.clear();
    }
    
    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns the size of vector
    size_t Size() const
    {
      return m_data.size();
    }
    /// This function returns the capacity of vector
    size_t Capacity() const
    {
      return m_data.capacity();
    }
    /// This function returns if vector is empty
    bool Empty() const
    {
      return m_data.empty();
    }
    
    // Forward Iterators ---------------------------------------------------------------------------------------------
    std::vector<T>::iterator begin() { return m_data.begin(); }
    std::vector<T>::iterator end() { return m_data.end(); }
    std::vector<T>::const_iterator begin() const { return m_data.begin(); }
    std::vector<T>::const_iterator end() const { return m_data.end(); }
    
    // Reverse Iterators ---------------------------------------------------------------------------------------------
    std::vector<T>::const_reverse_iterator rbegin() const { return m_data.rbegin(); }
    std::vector<T>::const_reverse_iterator rend() const { return m_data.rend(); }
    std::vector<T>::reverse_iterator rbegin() { return m_data.rbegin(); }
    std::vector<T>::reverse_iterator rend() { return m_data.rend(); }
    
  protected:
    /// This constructor creates the vector Itertator instance
    /// - Parameter data: vector data reference
    VectorIterator(std::vector<T>& data)
    : m_data(data)
    {
      
    }
    
    // Vector Data
    std::vector<T>& m_data;
  };
} // namespace IKan

