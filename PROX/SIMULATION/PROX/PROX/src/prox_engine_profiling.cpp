#include <prox_engine.h>
#include <prox_engine_data.h>

#include <cassert>

namespace prox
{

  size_t Engine::get_number_of_contacts()
  {
    assert( m_data || !"Engine::get_number_of_contacts(): Internal error, data null pointer");

    return m_data->m_contacts.size();
  }

  void Engine::get_contact_position(
                                    size_t const & contact_number
                                    , float & x
                                    , float & y
                                    , float & z
                                    )
  {
    assert( m_data || !"Engine::get_contact_position(): Internal error, data null pointer");

    assert( contact_number < m_data->m_contacts.size() || !"Engine::get_contact_position(): internal error: contact index out of bounds");
    
    x = m_data->m_contacts[contact_number].get_position()(0);
    y = m_data->m_contacts[contact_number].get_position()(1);
    z = m_data->m_contacts[contact_number].get_position()(2);
  }

  void Engine::get_contact_normal(
                                  size_t const & contact_number
                                  , float & x
                                  , float & y
                                  , float & z
                                  )
  {
    assert( m_data || !"Engine::get_contact_normal(): Internal error, data null pointer");

    assert( contact_number < m_data->m_contacts.size() || !"Engine::get_contact_normal(): internal error: contact index out of bounds");

    x = m_data->m_contacts[contact_number].get_normal()(0);
    y = m_data->m_contacts[contact_number].get_normal()(1);
    z = m_data->m_contacts[contact_number].get_normal()(2);
  }

  void Engine::get_contact_structure_direction(
                                     size_t const & contact_number
                                     , float & x
                                     , float & y
                                     , float & z
                                     )
  {
    assert( m_data || !"Engine::get_contact_structure(): Internal error, data null pointer");

    assert( contact_number < m_data->m_contacts.size() || !"Engine::get_contact_structure(): internal error: contact index out of bounds");

    x = m_data->m_contacts[contact_number].get_S()(0);
    y = m_data->m_contacts[contact_number].get_S()(1);
    z = m_data->m_contacts[contact_number].get_S()(2);
  }

  
  void Engine::get_contact_depth(
                                 size_t const & contact_number
                                 , float & depth
                                 )
  {
    assert( m_data || !"Engine::get_contact_depth(): Internal error, data null pointer");

    assert( contact_number < m_data->m_contacts.size() || !"Engine::get_contact_depth(): internal error: contact index out of bounds");

    depth = m_data->m_contacts[contact_number].get_depth();
  }

}// namespace prox
