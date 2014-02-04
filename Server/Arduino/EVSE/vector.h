#ifndef VECTOR_H
#define VECTOR_H

/* 
  Aufbau des Vektors
  
  Dynamisches Array, welches man beschreiben usw. kann.
  Unterstützt auch das Kopieren.
  
  
  Aufbau des Arrays im Speicher:
  data_ ist ein Pointer zu einem dynamischen reservierten Speicher, der sogenannte Heap.
  
  data_
  0x0 ->  Pointer zum ersten Element
  0x4 ->  Pointer zum zweiten Element
  0x8 ->  Pointer zum dritten Element
  0xC ->  Pointer zum vierten Element
  usw.
  
  allocated_ hält die Information, wieviele Einträge es im dynamischen Speicher gibt (Anzahl der Elemente).
  Im oberen Fall wäre es 4
  
*/

#include <Arduino.h>


template<typename T> class vector
{
  size_t allocated_;
  T **data_;
public:
  // Standard Konstruktor
  vector() 
  : allocated_(0), data_(0)
  {
    
  }
  
  // Copy-Konstruktor
  vector( const vector<T> & rhs) 
  : allocated_(0), data_(0)
  {
    copyAllElementsFromVectorToThis(rhs);
  }
  
  ~vector()
  {
    deleteAllElements();
  }
  
  
  vector<T> & operator = (vector<T> & rhs)
  {
    copyAllElementsFromVectorToThis(rhs);
  }
  
  vector<T> & operator += (T & t)
  {
    append(t);
    
    return * this;
  }
  
  vector<T> & operator += (T t)
  {
    append(t);
    
    return * this;
  }
  
  void appendByRef(T & t)
  {
    append(t); 
  }
  
  const size_t & size() const 
  { 
    return allocated_; 
  }
  
  T & operator[] (size_t idx) 
  { 
    return (T &) * data_[idx]; 
  }
  
  void remove(size_t idx)
  {
    if(allocated_ <= 0 || idx >= allocated_)
      return;
        
        
    size_t new_allocated = allocated_ - 1;
    T **new_data = new T * [new_allocated];
    
    delete data_[idx];
    data_[idx] = 0;

    for(int i=0, u; i < new_allocated; i++, u++)
    {
      if(data_[i] == 0)
        u++;
        
      new_data[i] = data_[u];  
    }
    
    delete [] data_;
  
    
    data_ = new_data;
    allocated_ = new_allocated;  
      
  }
private:
  void deleteAllElements()
  {
    if(allocated_ && data_)
    {
      for(int i = 0; i < allocated_; i++){
        delete data_[i]; 
        data_[i] = 0; 
      }
      
      delete [] data_;
      data_ = 0;
      allocated_ = 0;
    }
  }
  
  void copyAllElementsFromVectorToThis(const vector<T> & rhs)
  {
    deleteAllElements();
    
    allocated_ = rhs.allocated_;
    data_ = new T * [allocated_];
    
    for(int i=0; i < allocated_; i++)
    {
      data_[i] = new T;
      memcpy(data_[i], rhs.data_[i], sizeof(T));
    }
  }

  void append(T & t)
  {
    size_t new_allocated = allocated_ + 1;
    T **new_data = new T * [new_allocated];
    
    if(allocated_ > 0)
    {
      memcpy(new_data, data_, allocated_ * sizeof(T *));
    }
    
    new_data[allocated_] = new T(t);

    if(allocated_ > 0)
    {
      delete [] data_;
    }
    
    data_ = new_data;
    allocated_ = new_allocated;
  }
};

#endif
