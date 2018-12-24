#ifndef NARROW_PARAMS_H
#define NARROW_PARAMS_H

namespace narrow
{
  template<typename M>
  class Params
  {
  public:

    typedef typename M::real_type    T;
    typedef typename M::value_traits VT;

  protected:
    
    T      m_envelope;              ///< Procentage of scale of smallest object size to be used as collision envelope
    size_t m_chunk_bytes;

  public:
    
    T      const & get_envelope()      const { return this->m_envelope;           }
    size_t const & get_chunk_bytes()   const { return this->m_chunk_bytes;        }


  public:      
    
    void set_envelope(T const & value)              { this->m_envelope       = value;   }
    void set_chunk_bytes(size_t const & value)      { this->m_chunk_bytes    = value;   }

  public:
    
    Params()
    : m_envelope(VT::numeric_cast(0.01))
    , m_chunk_bytes(8000)
    {}
  };
  
} // namespace narrow

// NARROW_PARAMS_H
#endif 
