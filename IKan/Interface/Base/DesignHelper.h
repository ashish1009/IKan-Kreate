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
} // namespace IKan
