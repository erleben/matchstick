#ifndef KDOP_SELECT_CONTACT_POINT_ALGORITHM_H
#define KDOP_SELECT_CONTACT_POINT_ALGORITHM_H

#include <geometry.h>

#include <string>

namespace kdop
{

  class SelectContactPointAlgorithm
  {
  public:

    typedef enum {
        use_growth
      , use_most_opposing_surfaces
      , use_closest_points
    } algorithm_type;

  protected:

    static algorithm_type & get_algorithm_choice()
    {
      static algorithm_type choice = use_most_opposing_surfaces;
      return choice;
    }

  public:

    static bool is_using_closest_point()
    {
      return get_algorithm_choice() == use_closest_points;
    }

    static void set_algorithm(algorithm_type const & choice)
    {
      get_algorithm_choice() = choice;
    }

    static void set_algorithm(std::string const & choice)
    {
      if(choice.compare("opposing")==0)
        get_algorithm_choice() = use_most_opposing_surfaces;
      else if(choice.compare("growth")==0)
        get_algorithm_choice() = use_growth;
      else if(choice.compare("closest")==0)
        get_algorithm_choice() = use_closest_points;
      else
      {
        assert(false || !"set_algorithm(): unrecognized choice-value");
      }
    }

  public:

    template<typename V>
    static bool call_algorithm(
                      geometry::Tetrahedron<V> const & A
                    , geometry::Tetrahedron<V> const & B
                    , geometry::ContactsCallback<V> & callback
                    , std::vector<bool> const & surface_A
                    , std::vector<bool> const & surface_B
                    , std::vector<V> const & structure_map_A
                    , std::vector<V> const & structure_map_B
                    )
    {
      switch ( get_algorithm_choice() )
      {
        case use_most_opposing_surfaces:
          return geometry::contacts_tetrahedron_tetrahedron(
                                                            A
                                                            , B
                                                            , callback
                                                            , surface_A
                                                            , surface_B
                                                            , structure_map_A
                                                            , structure_map_B
                                                            , geometry::MOST_OPPOSING_SURFACES()
                                                            );
        case use_growth:
          return geometry::contacts_tetrahedron_tetrahedron(
                                                            A
                                                            , B
                                                            , callback
                                                            , surface_A
                                                            , surface_B
                                                            , structure_map_A
                                                            , structure_map_B
                                                            , geometry::GROWTH()
                                                            );
        case use_closest_points:
          return geometry::contacts_tetrahedron_tetrahedron(
                                                            A
                                                            , B
                                                            , callback
                                                            , surface_A
                                                            , surface_B
                                                            , structure_map_A
                                                            , structure_map_B
                                                            , geometry::CLOSEST_POINTS()
                                                            );
      }

      assert(false || !"call_algorithm(): unrecognized algorithm choice");
      
      return false;
    }

  };


}// end namespace kdop

// KDOP_SELECT_CONTACT_POINT_ALGORITHM_H
#endif
