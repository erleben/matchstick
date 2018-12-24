#include <content_io_writer.h>
#include <content_io_utils.h>
#include <util_string_helper.h>

namespace content
{
  namespace details
  {
    std::string get_geometry_name(size_t const & idx, Cache & data)
    {
      std::string name = data.output()->get_geometry_name( idx );
      if( name.size() == 0)
      {
        name = "geometry" + util::to_string(idx);
      }
      return name;
    }

    std::string get_rigid_body_name(size_t const & idx, Cache & data)
    {
      std::string name = data.output()->get_rigid_body_name( idx );
      if( name.size() == 0)
      {
        name = "body" + util::to_string(idx);
      }
      return name;
    }
    
    std::string get_material_name(size_t const & idx, Cache & data)
    {
      std::string name = data.output()->get_material_name( idx );
      if( name.size() == 0)
      {
        name = "material" + util::to_string(idx);
      }
      return name;
    }

    std::string get_pin_force_name(size_t const & idx, Cache & data)
    {
      return "pin_" + util::to_string(idx);
    }

    std::string get_scripted_motion_name(size_t const & idx, Cache & data)
    {
      return "scripted_motion_" + util::to_string(idx);
    }

    /**
     * XML Document creation routine. Useful lower­level routine for when
     * you want to keep a reference to the created doc.
     *
     * @param doc          Reference to XML doc to be written.
     * @param data         Reference to any cached data.
     *
     * @return             If succesfully written to the specified file then the
     *                     return value is true otherwise it is false.
     *
     * @example
     * @code
     *
     * template<typename skin_type>
     * void embed_extra_xml( ... , TiXmlDocument & doc)
     * {
     *   TiXmlElement * some_element = TiXmlHandle(&doc).FirstChild(...).Element();
     *   for( ; some_element ; some_element=some_element->NextSiblingElement(...) )
     *   {
     *     std::string name = ...;
     *     std::string value = ...;
     *     some_element->SetAttribute( name , value );
     *   }
     * }
     *
     * TiXmlDocument doc;
     * content::make_doc(doc, data);
     * embed_extra_xml( ... , doc);
     * doc.SaveFile("example.xml");
     *
     * @endcode
     */
    bool make_doc(TiXmlDocument & doc, Cache & data)
    {
      TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
      TiXmlElement * physics  = new TiXmlElement( "physics" );
      doc.LinkEndChild(decl);
      doc.LinkEndChild(physics);
      TiXmlElement * params        = new TiXmlElement( "params" );
      TiXmlElement * materials     = new TiXmlElement( "materials" );
      TiXmlElement * configuration = new TiXmlElement( "configuration" );
      TiXmlElement * channels      = new TiXmlElement( "channels" );
      physics->LinkEndChild(params);
      physics->LinkEndChild(materials);
      physics->LinkEndChild(configuration);
      physics->LinkEndChild(channels);
      // Add elements to params
      {
        float value = data.output()->get_collision_envelope();
        TiXmlElement * envelope = new TiXmlElement( "envelope" );
        envelope->SetDoubleAttribute( "value", value );
        params->LinkEndChild(envelope);
      }
      {
        float value = data.output()->get_time_step();
        TiXmlElement * timestep = new TiXmlElement( "timestep" );
        timestep->SetDoubleAttribute( "value", value );
        params->LinkEndChild(timestep);
      }
      // Add material elements to materials
      {
        std::vector<size_t> indices;
        size_t N = data.output()->get_number_of_materials();
        indices.resize(N);
        data.output()->get_material_indices( &indices[0] );
        for(size_t i = 0;i<N;++i)
        {
          size_t material_idx = indices[i];
          std::string name = get_material_name( material_idx, data);
          TiXmlElement * material = new TiXmlElement( "material" );
          material->SetAttribute( "name", name );
          materials->LinkEndChild(material);
          
        }
      }
      // Add properties to materials
      {
        TiXmlElement * properties      = new TiXmlElement( "properties" );
        materials->LinkEndChild(properties);
        
        size_t N = data.output()->get_number_of_properties();
        std::vector<size_t> first_indices;
        std::vector<size_t> second_indices;
        first_indices.resize( N );
        second_indices.resize( N );
        data.output()->get_material_property_indices( &first_indices[0], &second_indices[0] );
        
        for(size_t i = 0;i<N;++i)
        {
          size_t      const first_idx   = first_indices[i];
          size_t      const second_idx  = second_indices[i];
          std::string const first_name  = get_material_name(  first_idx, data);
          std::string const second_name = get_material_name( second_idx, data);
          
          TiXmlElement * property = new TiXmlElement( "property" );
          property->SetAttribute( "materials", "("+ first_name+","+second_name+")" );
          
          float e     = data.output()->get_coefficient_of_restitution( first_idx, second_idx );
          property->SetDoubleAttribute( "restitution", e );
          properties->LinkEndChild(property);
          
          TiXmlElement * ani_friction = new TiXmlElement( "anisotropic_friction" );
          float mux   = 0.0f;
          float muy   = 0.0f;
          float muz   = 0.0f;
          data.output()->get_coefficients_of_anisotropic_friction( first_idx, second_idx, mux, muy, muz );
          ani_friction->SetDoubleAttribute( "x", mux );
          ani_friction->SetDoubleAttribute( "y", muy );
          ani_friction->SetDoubleAttribute( "z", muz );
          property->LinkEndChild(ani_friction);

          TiXmlElement * iso_friction = new TiXmlElement( "isotropic_friction" );
          float mu   = 0.0f;
          data.output()->get_coefficient_of_isotropic_friction( first_idx, second_idx, mu );
          iso_friction->SetDoubleAttribute( "mu", mu );
          property->LinkEndChild(iso_friction);
        }
        
      }
      // add geometries to configuration
      {
        TiXmlElement * geometries = new TiXmlElement( "geometries" );
        configuration->LinkEndChild(geometries);
        
        size_t N = data.output()->get_number_of_geometries();
        std::vector<size_t> indices;
        indices.resize(N);
        data.output()->get_geometry_indices( &indices[0] );
        for(size_t i = 0;i<N;++i)
        {
          size_t geometry_idx = indices[i];
          std::string name = get_geometry_name( geometry_idx, data);
          TiXmlElement * geometry = new TiXmlElement( "geometry" );
          geometry->SetAttribute( "name", name );
          geometries->LinkEndChild(geometry);

          size_t N = 0u;
          size_t K = 0u;
          data.output()->get_tetramesh_shape(geometry_idx, N, K);

          if( N==0 || K==0) // Empty tetramesh, no need to save anything
            continue;

          TiXmlElement * tetramesh = new TiXmlElement( "tetramesh" );
          geometry->LinkEndChild(tetramesh);

          std::vector<size_t> vertices;
          std::vector<size_t> tetrahedra;
          std::vector<float>  coordinates;

          vertices.resize(N);
          tetrahedra.resize(4*K);
          coordinates.resize(3*N);

          data.output()->get_tetramesh_shape(geometry_idx, &vertices[0], &tetrahedra[0], &coordinates[0] );

          for(size_t n =0u ; n < N ; ++n)
          {
            TiXmlElement * vertex = new TiXmlElement( "vertex" );
            vertex->SetAttribute( "idx", vertices[n] );
            vertex->SetDoubleAttribute( "x", coordinates[3*n] );
            vertex->SetDoubleAttribute( "y", coordinates[3*n+1] );
            vertex->SetDoubleAttribute( "z", coordinates[3*n+2] );
            tetramesh->LinkEndChild(vertex);
          }
          for(size_t k =0u ; k < K ; ++k)
          {
            TiXmlElement * tetrahedron = new TiXmlElement( "tetrahedron" );

            size_t const i_idx = tetrahedra[4*k + 0];
            size_t const j_idx = tetrahedra[4*k + 1];
            size_t const k_idx = tetrahedra[4*k + 2];
            size_t const m_idx = tetrahedra[4*k + 3];

            assert(i_idx!=j_idx || !"i and j was the same");
            assert(i_idx!=k_idx || !"i and k was the same");
            assert(i_idx!=m_idx || !"i and m was the same");
            assert(j_idx!=k_idx || !"j and k was the same");
            assert(j_idx!=m_idx || !"j and m was the same");
            assert(k_idx!=m_idx || !"k and m was the same");

            tetrahedron->SetAttribute( "i", i_idx );
            tetrahedron->SetAttribute( "j", j_idx );
            tetrahedron->SetAttribute( "k", k_idx );
            tetrahedron->SetAttribute( "m", m_idx );

            tetramesh->LinkEndChild(tetrahedron);
          }
        }
      }
      // add forces to configuration
      {
        TiXmlElement * forces = new TiXmlElement( "forces" );
        configuration->LinkEndChild(forces);
        
        // Add gravity
        TiXmlElement * gravity = new TiXmlElement( "gravity" );
        float x = 0.0f;
        float y = 1.0f;
        float z = 0.0f;
        data.output()->get_gravity_up(x, y, z);
        float acc = 0.0f;
        data.output()->get_gravity_acceleration(acc);

        gravity->SetDoubleAttribute("x", x);
        gravity->SetDoubleAttribute("y", y);
        gravity->SetDoubleAttribute("z", z);
        gravity->SetDoubleAttribute("acc", acc);
        forces->LinkEndChild(gravity);

        // Add damping
        TiXmlElement * damping = new TiXmlElement( "damping" );
        float linear  = 0.01f;
        float angular = 0.01f;
        data.output()->get_damping_parameters(linear,angular);

        damping->SetDoubleAttribute("linear", linear);
        damping->SetDoubleAttribute("angular", angular);
        forces->LinkEndChild(damping);


        size_t const number_of_pins = data.output()->get_number_of_pin_forces();
        if(number_of_pins>0u)
        {
          std::vector<size_t> pin_indices;
          pin_indices.resize(number_of_pins);
          data.output()->get_pin_force_indices(&pin_indices[0]);

          for(size_t p =0u; p < number_of_pins; ++p)
          {
            size_t const & pin_idx = pin_indices[p];

            float tau;
            float target_x;
            float target_y;
            float target_z;
            float anchor_x;
            float anchor_y;
            float anchor_z;

            data.output()->get_pin_tau(pin_idx, tau);
            data.output()->get_pin_target(pin_idx, target_x, target_y, target_z);
            data.output()->get_pin_anchor(pin_idx, anchor_x, anchor_y, anchor_z);

            TiXmlElement * pin = new TiXmlElement( "pin" );

            std::string name = get_pin_force_name(pin_idx, data);

            data.set_idx_for_name(name, pin_idx);

            pin->SetAttribute("name", name);

            pin->SetDoubleAttribute("target_x", target_x);
            pin->SetDoubleAttribute("target_y", target_y);
            pin->SetDoubleAttribute("target_z", target_z);

            pin->SetDoubleAttribute("anchor_x", anchor_x);
            pin->SetDoubleAttribute("anchor_y", anchor_y);
            pin->SetDoubleAttribute("anchor_z", anchor_z);

            pin->SetDoubleAttribute("tau", tau);

            forces->LinkEndChild(pin);

          }
        }

        // Todo add support for other force types here
        //
        // ........
        //


      }
      // add scripted motions to configuration
      {
        TiXmlElement * scripted_motions = new TiXmlElement( "scripted_motions" );
        configuration->LinkEndChild(scripted_motions);

        size_t const number_of_motions = data.output()->get_number_of_scripted_motions();
        if(number_of_motions>0u)
        {
          std::vector<size_t> motion_indices;
          motion_indices.resize(number_of_motions);
          data.output()->get_scripted_motion_indices(&motion_indices[0]);

          for(size_t p =0u; p < number_of_motions; ++p)
          {
            size_t const & motion_idx = motion_indices[p];


            if (data.output()->is_scripted_motion_keyframe(motion_idx))
            {
              TiXmlElement * keyframe_motion = new TiXmlElement( "keyframe" );

              std::string name = get_scripted_motion_name(motion_idx, data);

              keyframe_motion->SetAttribute("name", name);

              size_t numer_of_positions    = data.output()->get_number_of_key_frame_positions(motion_idx);
              if (numer_of_positions>0u)
              {
                std::vector<float> time_array;
                std::vector<float> x_array;
                std::vector<float> y_array;
                std::vector<float> z_array;

                time_array.resize(numer_of_positions);
                x_array.resize(numer_of_positions);
                y_array.resize(numer_of_positions);
                z_array.resize(numer_of_positions);

                data.output()->get_key_frame_positions(
                                                       motion_idx
                                                       , &time_array[0]
                                                       , &x_array[0]
                                                       , &y_array[0]
                                                       , &z_array[0]
                                                       );

                for(size_t tick=0u; tick<numer_of_positions;++tick)
                {
                  TiXmlElement * position = new TiXmlElement( "position" );
                  position->SetDoubleAttribute("time", time_array[tick]);
                  position->SetDoubleAttribute("x", x_array[tick]);
                  position->SetDoubleAttribute("y", y_array[tick]);
                  position->SetDoubleAttribute("z", z_array[tick]);

                  keyframe_motion->LinkEndChild(position);
                }
              }


              size_t numer_of_orientations = data.output()->get_number_of_key_frame_orientations(motion_idx);
              if (numer_of_orientations>0u)
              {
                std::vector<float> time_array;
                std::vector<float> qs_array;
                std::vector<float> qx_array;
                std::vector<float> qy_array;
                std::vector<float> qz_array;

                time_array.resize(numer_of_orientations);
                qs_array.resize(numer_of_orientations);
                qx_array.resize(numer_of_orientations);
                qy_array.resize(numer_of_orientations);
                qz_array.resize(numer_of_orientations);

                data.output()->get_key_frame_orientations(
                                                          motion_idx
                                                          , &time_array[0]
                                                          , &qs_array[0]
                                                          , &qx_array[0]
                                                          , &qy_array[0]
                                                          , &qz_array[0]
                                                          );

                for(size_t tick=0u; tick<numer_of_orientations;++tick)
                {
                  TiXmlElement * orientation = new TiXmlElement( "orientation" );
                  orientation->SetDoubleAttribute("time", time_array[tick]);
                  orientation->SetDoubleAttribute("qs", qx_array[tick]);
                  orientation->SetDoubleAttribute("qx", qx_array[tick]);
                  orientation->SetDoubleAttribute("qy", qy_array[tick]);
                  orientation->SetDoubleAttribute("qz", qz_array[tick]);

                  keyframe_motion->LinkEndChild(orientation);
                }

              }



              scripted_motions->LinkEndChild(keyframe_motion);
            }
            else if (data.output()->is_scripted_motion_oscilation(motion_idx))
            {
              TiXmlElement * oscilation_motion = new TiXmlElement( "oscilation" );

              std::string name = get_scripted_motion_name(motion_idx, data);

              oscilation_motion->SetAttribute("name", name);

              float amplitude;
              float frequency;
              float phase;
              float dir_x;
              float dir_y;
              float dir_z;

              data.output()->get_scripted_oscilation_paramters(motion_idx, amplitude, frequency, phase, dir_x, dir_y, dir_z);

              oscilation_motion->SetDoubleAttribute("amplitude", amplitude);
              oscilation_motion->SetDoubleAttribute("frequency", frequency);
              oscilation_motion->SetDoubleAttribute("phase",     phase    );
              oscilation_motion->SetDoubleAttribute("dir_x",     dir_x    );
              oscilation_motion->SetDoubleAttribute("dir_y",     dir_y    );
              oscilation_motion->SetDoubleAttribute("dir_z",     dir_z    );

              scripted_motions->LinkEndChild(oscilation_motion);
            }

          }
        }

      }
      // Add objects to configuration
      {
        TiXmlElement * objects = new TiXmlElement( "objects" );
        configuration->LinkEndChild(objects);
        
        size_t N = data.output()->get_number_of_rigid_bodies();
        std::vector<size_t> indices;
        indices.resize(N);
        data.output()->get_rigid_body_indices( &indices[0] );
        for(size_t i = 0;i<N;++i)
        {
          size_t object_idx = indices[i];
          std::string name = get_rigid_body_name( object_idx, data);
          TiXmlElement * object = new TiXmlElement( "object" );
          object->SetAttribute( "name", name );
          object->SetAttribute( "type", "rigid" );   // 2009-12-29 kenny: currently only rigid objects are supported!
          objects->LinkEndChild(object);
          
          // add state element
          TiXmlElement * state = new TiXmlElement( "state" );

          size_t material_idx = data.output()->get_rigid_body_material(object_idx);
          if(material_idx != content::UNDEFINED)
          {
            std::string material_name = get_material_name( material_idx, data);
            state->SetAttribute("material", material_name);
          }
          bool active   = data.output()->get_rigid_body_active(object_idx);
          bool fixed    = data.output()->get_rigid_body_fixed(object_idx);

          state->SetAttribute("active", ((active) ? "true" : "false"));
          state->SetAttribute("fixed", ((fixed) ? "true" : "false") );

          bool scripted = data.output()->has_scripted_motion(object_idx);

          state->SetAttribute("scripted", ((scripted) ? "true" : "false") );

          if(scripted)
          {
            size_t motion_idx = data.output()->get_scripted_motion(object_idx);
            std::string scripted_motion_name = get_scripted_motion_name( motion_idx, data);
            state->SetAttribute("scripted_motion", scripted_motion_name);
          }

          object->LinkEndChild(state);
          
          TiXmlElement * transform = new TiXmlElement( "transform" );
          float x = 0.0f;
          float y = 0.0f;
          float z = 0.0f;
          data.output()->get_rigid_body_position(object_idx, x, y, z);
          float qs = 0.0f;
          float qx = 0.0f;
          float qy = 0.0f;
          float qz = 0.0f;
          data.output()->get_rigid_body_orientation(object_idx, qs, qx, qy, qz);
          transform->SetDoubleAttribute("x", x);
          transform->SetDoubleAttribute("y", y);
          transform->SetDoubleAttribute("z", z);
          transform->SetDoubleAttribute("qs", qs);
          transform->SetDoubleAttribute("qx", qx);
          transform->SetDoubleAttribute("qy", qy);
          transform->SetDoubleAttribute("qz", qz);
          state->LinkEndChild(transform);
          
          TiXmlElement * motion = new TiXmlElement( "motion" );
          float vx = 0.0f;
          float vy = 0.0f;
          float vz = 0.0f;
          data.output()->get_rigid_body_velocity(object_idx, vx, vy, vz);
          float wx = 0.0f;
          float wy = 0.0f;
          float wz = 0.0f;
          data.output()->get_rigid_body_spin(object_idx, wx, wy, wz);
          motion->SetDoubleAttribute("vx", vx);
          motion->SetDoubleAttribute("vy", vy);
          motion->SetDoubleAttribute("vz", vz);
          motion->SetDoubleAttribute("wx", wx);
          motion->SetDoubleAttribute("wy", wy);
          motion->SetDoubleAttribute("wz", wz);
          state->LinkEndChild(motion);
          
          TiXmlElement * mass = new TiXmlElement( "mass" );
          float M = data.output()->get_rigid_body_mass(object_idx);
          mass->SetDoubleAttribute("value", M);
          state->LinkEndChild(mass);
          
          TiXmlElement * inertia = new TiXmlElement( "inertia" );
          float Ixx = 0.0f;
          float Iyy = 0.0f;
          float Izz = 0.0f;
          data.output()->get_rigid_body_inertia(object_idx, Ixx, Iyy, Izz);
          inertia->SetDoubleAttribute("xx", Ixx);
          inertia->SetDoubleAttribute("yy", Iyy);
          inertia->SetDoubleAttribute("zz", Izz);
          state->LinkEndChild(inertia);
          
          // add collision shape element
          size_t geometry_idx = data.output()->get_rigid_body_collision_geometry( object_idx );
          std::string geometry_name = get_geometry_name( geometry_idx, data);
          TiXmlElement * shape = new TiXmlElement( "shape" );
          shape->SetAttribute( "ref", geometry_name );
          object->LinkEndChild(shape);

          TiXmlElement * structure = new TiXmlElement( "structure" );
          size_t N = 0u;
          data.output()->get_material_structure_map_size(object_idx, N );
          structure->SetAttribute("size", N);
          if(N>0u)
          {
            std::vector<size_t>  indices;
            std::vector<float>   structure_map;
            indices.resize(N);
            structure_map.resize(3*N);

            data.output()->get_material_structure_map_data(object_idx, &indices[0], &structure_map[0] );
            for(size_t n =0u ; n < N ; ++n)
            {
              TiXmlElement * dir = new TiXmlElement( "dir" );
              dir->SetAttribute( "idx", indices[n] );
              dir->SetDoubleAttribute( "x", structure_map[3*n] );
              dir->SetDoubleAttribute( "y", structure_map[3*n+1] );
              dir->SetDoubleAttribute( "z", structure_map[3*n+2] );
              structure->LinkEndChild(dir);
            }
          }
          object->LinkEndChild(structure);

          //--- Add applied forces if any --------------------------------------
          size_t number_of_forces = data.output()->get_number_of_connected_forces( object_idx );

          if (number_of_forces>0)
          {
            TiXmlElement * forces = new TiXmlElement( "forces" );
            object->LinkEndChild(forces);

            std::vector<size_t> force_indices;
            force_indices.resize(number_of_forces);
            data.output()->get_connected_force_indices(object_idx, &force_indices[0]);

            for(size_t f = 0u; f < number_of_forces; ++f )
            {
              size_t const force_idx = force_indices[f];

              TiXmlElement * apply = new TiXmlElement( "apply" );

              std::string const name = get_pin_force_name(force_idx, data);

              apply->SetAttribute( "ref", name );
              forces->LinkEndChild(apply);
            }
          }

        }
      }
      return true;
    }
    
  }// namespace details
  
  bool xml_write( std::string const & filename, content::Output * output )
  {
    // build document
    TiXmlDocument doc;
    details::Cache data( output );
    
    if (! content::details::make_doc(doc, data) )
      return false;
    
    // write the document
#ifdef TIXML_USE_STL
    doc.SaveFile(filename);
#else
    doc.SaveFile(filename.c_str());
#endif
    
    return true;
  }
  
}// namespace content
