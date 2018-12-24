#ifndef PROX_COLLISION_DETECTION_H
#define PROX_COLLISION_DETECTION_H

#include <broad.h>
#include <broad_statistics.h>
#include <geometry.h>

#include <narrow.h>

#include <prox_contact_point.h>
#include <prox_params.h>
#include <prox_rigid_body.h>

#include <util_profiling.h>

#include <cassert>
#include <vector>

namespace prox
{

  namespace detail
  {

    /**
     * Contact Point Generation Callback Functor.
     * This functor acts as the glue between the contact point generation algorithm and the Prox contact point data types and containers.
     *
     * The functor hides the nasty details and data types from the contact point generation library.
     */
    template<typename M>
    class ContactCallbackFunctor
    : public geometry::ContactsCallback<typename M::vector3_type>
    {
    public:

      typedef prox::RigidBody<M>         body_type;
      typedef prox::ContactPoint<M>      contact_type;
      typedef typename M::real_type      T;
      typedef typename M::vector3_type   V;
      typedef typename M::value_traits   VT;

    protected:

      body_type                   * m_body_i;   ///< A pointer to body i of the contact.
      body_type                   * m_body_j;   ///< A pointer to body j of the contact.
      std::vector< contact_type > * m_results;  ///< A pointer to a contact point container where all generated contacts should be added to.

    public:

      ContactCallbackFunctor()
      : m_body_i(0)
      , m_body_j(0)
      , m_results(0)
      {}

      ContactCallbackFunctor(body_type * A, body_type * B, std::vector< prox::ContactPoint<M> > & results)
      : m_body_i(A)
      , m_body_j(B)
      , m_results(&results)
      {
        assert(A || !"ContactCallbackFunctor(...) body A was null");
        assert(B || !"ContactCallbackFunctor(...) body B was null");
      }

      ~ContactCallbackFunctor(){}

      ContactCallbackFunctor& operator=( ContactCallbackFunctor const & callback)
      {
        if (this != &callback)
        {
          this->m_body_i  = callback.m_body_i;
          this->m_body_j  = callback.m_body_j;
          this->m_results = callback.m_results;
        }
        return *this;
      }

      /*
       * Add new contact callback function.
       * The contact point generation library invokes this function
       * call whenever it has discovered a new contact that should be stored.
       *
       * @param p    The position of the new contact point.
       * @param n    The unit normal of the new contact point.
       * @param d    The penetration distance meassure of the contact point.
       */
      void operator()( V const & p, V const & n, T const & d, V const & Sa, V const & Sb)
      {
        assert( this->m_body_i              || !"operator(): body i is null" );
        assert( this->m_body_j              || !"operator(): body j is null" );
        assert( this->m_results             || !"operator(): results is null");
        assert( tiny::norm(n) > VT::zero()  || !"operator(): normal is zero" );
          
          
        contact_type contact;

        contact.set_position( p );
        contact.set_depth( d );
        contact.set_normal( n );
        contact.set_body_i( this->m_body_i );
        contact.set_body_j( this->m_body_j );
        contact.set_Sa(Sa);
        contact.set_Sb(Sb);
        contact.set_S(n);

        this->m_results->push_back( contact );
      }
    };

  }// namespace detail

  /**
   * Inovoke collision detection system.
   * This function basically wraps the entire collision detection system into one functional ``unit''.
   *
   * @tparam contact_container   The type of contact container
   * @tparam M                   The math types used
   *
   * @param data                 Collision detection system data that are reused from invokation to invokation.
   * @param contacts             Upon return this container will hold all current contact points in the configuration.
   * @param tag                  Tag dispatcher for transfering math types to collision detection system.
   */
  template< typename M>
  inline void collision_detection(
                                  std::vector< RigidBody< M > > & bodies
                                  , broad::System<typename M::real_type> & broad_system
                                  , narrow::System<typename M::tiny_types> & narrow_system
                                  , std::vector< ContactPoint<M> > & contacts
                                  , Params<M> const & params
                                  , M const & /*tag*/
  )
  {
    typedef typename M::tiny_types                                  tiny_types;
    typedef typename M::real_type                                   T;
    typedef typename prox::RigidBody<M>                             body_type;
    typedef          std::vector<body_type>                         body_container;
    typedef typename body_container::iterator                       body_iterator;
    typedef typename narrow::Geometry<tiny_types>                   geometry_type;
    //typedef typename narrow::System<tiny_types>::geometry_iterator  geometry_iterator;
    typedef typename broad::System<T>::overlap_type                 overlap_type;
    typedef          std::vector< overlap_type >                    overlap_container;
    typedef typename overlap_container::iterator                    overlap_iterator;
    typedef          detail::ContactCallbackFunctor<M>              callback_type;
    typedef typename std::vector< ContactPoint<M> >::iterator       contact_iterator;

    START_TIMER("collision_detection_time");

    //--- First we preprocess data structures for doing collision detection ----
    {
      START_TIMER("preprocessing_time");

      //--- Update kDOP BVHs to reflect changes in tetramesh geometry ----------
      START_TIMER("kdop_work_pool_time");

      std::vector< narrow::UpdateWorkItem< tiny_types > > kdop_bvh_update_work_pool;

      kdop_bvh_update_work_pool.reserve( bodies.size() ); // Make sure all space we may need is pre-allocated.

      for(body_iterator body = bodies.begin(); body != bodies.end(); ++body)
      {
        geometry_type const & geometry = narrow_system.get_geometry( body->get_geometry_idx() );

        if(!geometry.has_shape() )
          continue;

          narrow::UpdateWorkItem<tiny_types> work_item = narrow::UpdateWorkItem<tiny_types>(
                                                                                                          *body
                                                                                                          , geometry
                                                                                                          , body->get_position()
                                                                                                          , body->get_orientation()
                                                                                                          );
          kdop_bvh_update_work_pool.push_back( work_item );
      }
      STOP_TIMER("kdop_work_pool_time");

      START_TIMER("kdop_update_time");
      if( ! kdop_bvh_update_work_pool.empty() )
      {
          narrow::update_kdop_bvh(  kdop_bvh_update_work_pool );
      }
      STOP_TIMER("kdop_update_time");

      // 2013-07-06 Kenny code review: This will clear all data in
      //            the broad phase collision detection system. This
      //            may not be the most efficient approach, as all
      //            configuration data needs to be build up from stracth
      //            again. Ideally only newly added bodies or geometry
      //            changed bodies should be cleared/added to the broad
      //            phase system.
      broad_system.clear();

      //--- Update body radius and connect their AABBs to the broad phase system -
      for(body_iterator body = bodies.begin(); body != bodies.end(); ++body)
      {
        geometry_type const & geometry = narrow_system.get_geometry( body->get_geometry_idx() );

        body->set_radius( geometry.get_radius() );

        // 2013-07-06 Kenny code review: Here we re-connect all bodies
        //            to the broad phase collision detection system.
        //            See my review comment above about efficiency.
        broad_system.connect( &(*body)  );
      }

      // 2015-03-03 Kenny code review: This optimal spacing requires
      // sorting of all objects, so it runs O(n lg n). However, grid
      // algorithm is trying to run in O(n). Hence, one could argue that
      // a sweep-line algorithm would be better as its performance do
      // not depend on the obejct sizes.
      broad_system.compute_optimal_cell_spacing(); 

      STOP_TIMER("preprocessing_time");
    }

    //--- Second we perform broad phase collision detection --------------------
    overlap_container overlaps;
    {
      START_TIMER("broad_time");

      // We use the broad phase collision detection system to quickly determine pairs of overlapping bodies

      float efficiency = 0.0f;

      if( params.use_all_pair() )
      {
         broad::find_overlaps( broad_system, overlaps, efficiency, broad::all_pair_algorithm() );
      }
      else
      {
         broad::find_overlaps( broad_system, overlaps, efficiency, broad::grid_algorithm() );
      }

      STOP_TIMER("broad_time");

#ifdef USE_PROFILING
      RECORD("broad_efficiency", efficiency);
      broad::Statistics stats = broad::make_statistics( broad_system );
      RECORD("broad_fill_ratio", stats.m_fill_ratio);
      RECORD("broad_min_size_obj", stats.m_min_obj);
      RECORD("broad_max_size_obj", stats.m_max_obj);
      RECORD("broad_avg_size_obj", stats.m_avg_obj);
      RECORD("broad_std_size_obj", stats.m_std_obj);
#endif

    }

    //--- Third we perform narrow phase collision detction ---------------------
    {
      START_TIMER("narrow_time");

      //--- Make sure we do not carry any old contact information around. ------
      contacts.clear();

      std::vector< narrow::TestPair<tiny_types> > narrow_test_pairs;

      std::vector< callback_type > callbacks;  // 2014-10-19 Kenny: Argh, I hate this design choice.... really ugly
      callbacks.resize( overlaps.size() );
      typename std::vector< callback_type >::iterator callback = callbacks.begin();

      for(overlap_iterator o = overlaps.begin(); o != overlaps.end(); ++o, ++callback)
      {
        body_type * bodyA = static_cast<body_type *>(o->first);    // 2009-11-25 Kenny: hmm can we not get rid of static casts?
        body_type * bodyB = static_cast<body_type *>(o->second);   // 2009-11-25 Kenny: hmm can we not get rid of static casts?

        //--- Verify if we need to test the two bodies or if we can skip them --
        if (bodyA->is_fixed() && bodyB->is_fixed())
          continue;
        if (bodyA->is_fixed() && bodyB->is_scripted())
          continue;
        if (bodyA->is_scripted() && bodyB->is_fixed())
          continue;
        if (bodyA->is_scripted() && bodyB->is_scripted())
          continue;

        *callback = callback_type( bodyA, bodyB, contacts );

        narrow::TestPair<tiny_types> narrow_pair = narrow::TestPair<tiny_types>(
                                                                                *bodyA
                                                                                , *bodyB
                                                                                , bodyA->get_position()
                                                                                , bodyA->get_orientation()
                                                                                , bodyB->get_position()
                                                                                , bodyB->get_orientation()
                                                                                , (*callback)
                                                                                );

        narrow_test_pairs.push_back( narrow_pair );
      }

      narrow::dispatch( narrow_system, narrow_test_pairs );

      STOP_TIMER("narrow_time");
    }

    //--- Forth phase we perform contact point reduction -----------------------
    if(params.stepper_params().contact_reduction())
    {
      START_TIMER("contact_reduction_time");

      std::sort(contacts.begin(), contacts.end());
      auto last = std::unique(contacts.begin(), contacts.end());
      contacts.erase(last, contacts.end());

      STOP_TIMER("contact_reduction_time");
    }

    //--- Fifth phase we collect statistics on contact points ------------------
    {
      RECORD("contacts", contacts.size() );

      util::Profiling::Monitor * penetration_monitor = util::Profiling::get_monitor("penetration");

      penetration_monitor->clear();

      for(contact_iterator contact = contacts.begin(); contact != contacts.end(); ++contact)
      {
        RECORD("penetration", contact->get_depth() );
      }

      RECORD("avg_penetration", penetration_monitor->get_avg() );
      RECORD("min_penetration", penetration_monitor->get_min() );
      RECORD("max_penetration", penetration_monitor->get_max() );

      penetration_monitor->clear();
    }

    STOP_TIMER("collision_detection_time");
  }
} // namespace prox

// PROX_COLLISION_DETECTION_H
#endif
