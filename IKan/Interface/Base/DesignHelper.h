//
//  DesignHelper.h
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

namespace IKan
{
  /// This is the helper class for any class implementation bridge
  template<typename T> class Pimpl
  {
  public:
    /// Default constructor of Pimpl Bridge
    Pimpl()
    : m_instance(CreateScope<T>()) {}
    /// Default destructor of Pimpl Bridge
    ~Pimpl() {}
    
    /// Argumented constructor of Pimpl Bridge
    /// - Parameter args: runtime arguments
    template <typename ...Args> Pimpl(Args&& ...args)
    : m_instance(CreateScope<T>(std::forward<Args>(args)...)) {}
    
    /// This function returns the pointer of pimpl instance
    T* operator->() { return m_instance.get(); }
    /// This function returns the const pointer of pimpl instance
    const T* operator->() const { return m_instance.get(); }
    /// This function returns the reference of pimpl instance
    T& operator*() { return *m_instance; }
    
  private:
    Scope<T> m_instance;
  };
  
  /// This class is the Base iterator for any vector containing class. Provides basic APIs regarding vector data
  template <typename T> class VectorIterator
  {
  public:
    virtual ~VectorIterator() = default;
    // Forward Iterators ------------------------------------------------------------------------------------------
    std::vector<T>::iterator begin() { return m_data.begin(); }
    std::vector<T>::iterator end() { return m_data.end(); }
    std::vector<T>::const_iterator begin() const { return m_data.begin(); }
    std::vector<T>::const_iterator end() const { return m_data.end(); }
    
    // Reverse Iterators ------------------------------------------------------------------------------------------
    std::vector<T>::const_reverse_iterator rbegin() const { return m_data.rbegin(); }
    std::vector<T>::const_reverse_iterator rend() const { return m_data.rend(); }
    std::vector<T>::reverse_iterator rbegin() { return m_data.rbegin(); }
    std::vector<T>::reverse_iterator rend() { return m_data.rend(); }
    
  protected:
    VectorIterator(std::vector<T>& data) : m_data(data) {}
    std::vector<T>& m_data;
  };

} // namespace IKan
