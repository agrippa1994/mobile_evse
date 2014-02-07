#ifndef MAP_H
#define MAP_H

#include <Arduino.h>
#include "vector.h"

template<typename T, typename U> class Map
{
public:
  typedef T key_type;
  typedef U val_type;
  
private:
  size_t allocated_;
  
  key_type **keys_;
  val_type **vals_;
  
  val_type val_on_fail_;
  
public:
  Map(U val_on_fail = U()) 
  : allocated_(0), keys_(0), vals_(0), val_on_fail_(val_on_fail)
  {
  }
  
  // Copy-Konstruktor
  Map(const Map<T, U> & rhs) 
  : allocated_(0), keys_(0), vals_(0)
  {
    copyAllElementsFromMapToThis(rhs);
  }
  
  ~Map()
  {
    deleteAllElements();
  }
  
  
  Map<T, U> & operator = (Map<T, U> & rhs)
  {
    copyAllElementsFromMapToThis(rhs);
  }
  
  const size_t & size() const 
  { 
    return allocated_; 
  }

  Vector<T> allKeys() const
  {
    Vector<T> vec;
    for(int i=0; i < allocated_; i++)
      vec += (T)(* keys_[i]);
    return vec;  
  }
  
  U & operator[] (T key)
  {
    // Der Schlüssel existiert  
    for(int i = 0; i < allocated_; i++)
      if(key == * keys_[i])
        return (U &) * vals_[i];
        
    // Der Schlüssel existiert nicht    
    append(key, val_on_fail_);
    
    for(int i = 0; i < allocated_; i++)
     if(key == * keys_[i])
       return (U &) * vals_[i];
        
    return val_on_fail_;
  }
  
  bool isset(T key)
  {
    for(int i = 0; i < allocated_; i++)
      if(key == * keys_[i])
        return true;
    return false;
  }
  
  bool operator -= (T idx)
  {
    if(allocated_ <= 0 || !isset(idx))
      return false;
      
    size_t new_allocated = allocated_ - 1;
    T **new_keys= new T * [new_allocated];
    U **new_vals= new T * [new_allocated];
    
    delete keys_[idx];
    delete vals_[idx];
    
    keys_[idx] = 0;
    vals_[idx] = 0;
    
    for(int i=0, u=0; i < new_allocated; i++, u++)
    {
      if(vals_[i] == 0 && keys_[i] == 0)
        u++;
        
      new_keys[i] = keys_[u];
      new_vals[i] = vals_[u];  
    }
    
    delete [] keys_;
    delete [] vals_;
  
    
    keys_ = new_keys;
    vals_ = new_vals;
    
    allocated_ = new_allocated;  
  
    return true;
  }
private:
  void deleteAllElements()
  {
    if(allocated_ && keys_ && vals_)
    {
      for(int i = 0; i < allocated_; i++)
      {
        delete keys_[i]; 
        delete vals_[i];
        
        keys_[i] = 0;
        vals_[i] = 0; 
      }
      
      delete [] keys_;
      delete [] vals_;
      
      keys_ = 0;
      vals_ = 0;
      
      allocated_ = 0;
    }
  }
  
  void copyAllElementsFromMapToThis(const Map<T, U> & rhs)
  { 
    
    deleteAllElements();
    
    allocated_ = rhs.allocated_;
    
    val_on_fail_ = rhs.val_on_fail_;
    
    keys_ = new T * [allocated_];
    vals_ = new U * [allocated_];
    
    for(int i=0; i < allocated_; i++)
    {
      keys_[i] = new T(* rhs.keys_[i]);
      vals_[i] = new U(* rhs.vals_[i]);
    }
    
  }

  void append(T & t, U & u)
  {
    size_t new_allocated = allocated_ + 1;
    
    T **new_keys = new T * [new_allocated];
    U **new_vals = new U * [new_allocated];
    
    if(allocated_ > 0)
    {
      memcpy(new_keys, keys_, allocated_ * sizeof(T *));
      memcpy(new_vals, vals_, allocated_ * sizeof(U *));
    }
    
    new_keys[allocated_] = new T(t);
    new_vals[allocated_] = new U(u);
    
    if(allocated_ > 0)
    {
      delete [] keys_;
      delete [] vals_;
    }
    
    keys_ = new_keys;
    vals_ = new_vals;
    
    allocated_ = new_allocated;
  }
};

#endif
