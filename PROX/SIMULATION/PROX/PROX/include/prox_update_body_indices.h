#ifndef PROX_UPDATE_BODY_INDICES_H
#define PROX_UPDATE_BODY_INDICES_H

namespace prox
{
  
  namespace detail
  {
    /**
     * Body indices are used to access the proper blocks of assembled matrices. This
     * function makes sure that body indices are incremental and contigous such that
     * bodies are mapped ``nicely'' into blocked matrices.
     *
     * @param begin      An iterator to the position of the first body.
     * @param end        An iterator to the position one past the last body.
     */
    template<typename body_iterator>
    inline void update_body_indices( body_iterator begin, body_iterator end )
    {
      size_t idx = 0u;
      for(body_iterator body = begin;body!=end;++body)
        body->set_idx( idx++ );
    } 
    
  }// end namespace detail
  
} //namespace prox

// PROX_UPDATE_BODY_INDICES_H 
#endif
