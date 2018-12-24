#ifndef PROX_GUI_SELECT_TOOL_H
#define PROX_GUI_SELECT_TOOL_H

#include <tiny.h>

#include <content.h>

#include <cmath>

namespace prox_gui
{

  class SelectTool
  {
  public:

    typedef tiny::MathTypes<float> MT;
    typedef MT::real_type          T;
    typedef MT::vector3_type       V;
    typedef MT::matrix3x3_type     M;
    typedef MT::quaternion_type    Q;
    typedef MT::coordsys_type      C;
    typedef MT::value_traits       VT;

  protected:

    size_t         m_id;                ///< Identifier of current selection. Only valid if m_is_selected is true.
    bool           m_has_selected;       ///< Boolean flag indicating whether something is selected or not.

    V m_ray_direction;
    V m_ray_origin;
    V m_hit_point;
    T m_hit_distance;

    float m_anchor_x;   ///< Selection anchor point x coordinate.
    float m_anchor_y;   ///< Selection anchor point y coordinate.
    float m_anchor_z;   ///< Selection anchor point z coordinate.

  public:

    size_t const & get_selected_id() const { return m_id;           }
    bool   const & has_selected()    const { return m_has_selected; }

  protected:

  public:

    SelectTool()
    {}

    virtual ~SelectTool()
    {
      clear();
    }

  public:

    void clear()
    {
      m_id = 0;
      m_has_selected = false;
    }

    void select(V const & p, V const & r, content::API * engine)
    {
      m_has_selected = false;

      m_ray_origin      = p;
      m_ray_direction   = r;
      m_hit_distance    = VT::infinity();
      m_hit_point       = V::zero();

      if(!engine)
        return;

      float hit_x    = 0.0f;
      float hit_y    = 0.0f;
      float hit_z    = 0.0f;
      float distance = 0.0f;

      bool const did_hit = engine->compute_raycast(
                                                   p(0), p(1), p(2),
                                                   r(0), r(1), r(2),
                                                   m_id,
                                                   hit_x, hit_y, hit_z,
                                                   distance
                                                   );

      if(! did_hit)
        return;

      m_hit_point    = V::make( hit_x, hit_y, hit_z);
      m_hit_distance = distance;

      engine->get_rigid_body_position( m_id, m_anchor_x, m_anchor_y, m_anchor_z );

      m_has_selected = true;
    }

    void deselect()
    {
      m_has_selected = false;
    }

    void move_selection(V const & p, V const & r, V const & dof, content::API * engine)
    {
      if(! m_has_selected)
        return;

      //--- scale ray direction vectors to start at p and end at view plane
      T const s1    =  tiny::inner_prod(m_ray_direction, dof);
      T const s2    =  tiny::inner_prod(r, dof);
      V const r1    =  m_ray_direction/s1;
      V const r2    =  r/s2;

      //--- Determine how the displacment on view plane scale to displacement in world
      T const scale =  m_hit_distance*s1;

      assert(scale>VT::zero() || !"move_selection(): internal error");

      V const d =  (r2 - r1)*scale;

      float x = m_anchor_x + d(0);
      float y = m_anchor_y + d(1);
      float z = m_anchor_z + d(2);

      engine->set_rigid_body_position( m_id, x, y, z );

    }

  };

}//namespace prox_gui

// PROX_GUI_SELECT_TOOL_H
#endif
