#ifndef VECTOR_H
#define VECTOR_H

template<typename T> class vector
{
  size_t allocated_;
  size_t offset_;
  T **m_data;
  
public:
  vector() : allocated_(0), offset_(0), m_data(0)
  {
    
  }
  
  ~vector()
  {
    
  }
  
  vector(vector<T> & rhs)
  {
    
  }
  
  vector<T> & operator = (vector<T> & rhs)
  {
  }
  
  vector<T> & operator += (T t)
  {
  }
  
};

#endif
