#ifndef CONTENT_IO_CACHE_H
#define CONTENT_IO_CACHE_H

#include <content_api_input.h>
#include <content_api_output.h>
#include <content_io_point.h>
#include <content_io_vertex.h>
#include <content_io_tetrahedron.h>
#include <content_io_transform.h>
#include <content_io_motion.h>
#include <content_io_rigid_body_state.h>

#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>   // needed for std::runtime_error

namespace content
{
  namespace details
  {
    /**
     * The Data  Cache Class.
     * During parsing of XML files one may need to keep temporary data or query the
     * physics engine about information. This class provides a data handler for the
     * XML parser. An instance of this class is passed along to all functions in the
     * XML parser library and they can use the class to add data or remove data as
     * they wishes.
     */
    class Cache
      {
      protected:
        
        content::Input              * m_input;    ///< A pointer to the physics engine manager      
        content::Output             * m_output;   ///< A pointer to the physics engine manager      
        
        size_t                        m_index;        ///< A  cached index value.
        std::map<std::string, size_t> m_name_to_idx;  ///< Internal data structure used to map names to indices
        
        std::map<std::string, Motion >        m_motions;            ///< Internal data structure used to keep a collection of motions.
        std::map<std::string, Transform >     m_transforms;         ///< Internal data structure used to keep a collection of transforms.
        std::map<std::string, RigidBodyState> m_rigid_body_states;  ///< Internal data structure used to keep a collection of rigid body states 
        std::vector<Point>                    m_points;             ///< Temporary storage for points.
        std::vector<Vertex>                    m_vertices;          ///< Temporary storage for vertices.
        std::vector<Tetrahedron>               m_tetrahedra;        ///< Temporary storage for tetrahedra.

      public:

        std::map<size_t,size_t>  m_force_index_lut;
        
        /**
         * Get a pointer to the current physics manager.
         *
         * @return A pointer to the manager.
         */
        content::Input const  * input()  const { return m_input; }
        content::Input        * input()        { return m_input; }
        content::Output const * output() const { return m_output; }
        content::Output       * output()       { return m_output; }
        
        size_t const & get_idx()  {  return m_index;  }
        void set_idx(size_t const & value) { m_index = value; }
        
        /**
         * Get index for name.
         *
         * @param name    A textual name.
         *
         * @return      The index value for the given name.
         */
        size_t const & get_idx_from_name( std::string const & name ) const
        {
          std::map<std::string, size_t>::const_iterator iter = m_name_to_idx.find( name );
          if(iter==m_name_to_idx.end())
            throw std::runtime_error("no entry existed for " + name);          
          return iter->second;
        }
        
        /**
         * Set index for name.
         *
         * @param name    A textual name.
         * @param idx     The index value.
         */
        void set_idx_for_name( std::string const & name, size_t const & idx )
        {
          std::map<std::string, size_t>::const_iterator iter = m_name_to_idx.find( name );
          if(iter!=m_name_to_idx.end())
            throw std::runtime_error("entry already existed for " + name);          
          m_name_to_idx[ name ] = idx;
        }
        
        /**
         * Name to Transform mapping.
         *
         * @param name    A textual name.
         *
         * @return        A reference to the transform that corresponds to the given name.
         */
        Transform const & get_transform_from_name( std::string const & name ) const
        {
          std::map<std::string, Transform>::const_iterator iter = m_transforms.find( name );
          if(iter==m_transforms.end())
            throw std::runtime_error("no entry existed for " + name);          
          return iter->second;
        }
        
        void set_transform_for_name( std::string const & name, Transform const & T )
        {
          std::map<std::string, Transform>::const_iterator iter = m_transforms.find( name );
          if(iter!=m_transforms.end())
            throw std::runtime_error("entry already existed for " + name);          
          m_transforms[ name ] = T;
        }
        
        /**
         * Name to Motion mapping.
         *
         * @param name    A textual name.
         *
         * @return        A reference to the motion that corresponds to the given name.
         */
        Motion const & get_motion_from_name( std::string const & name ) const
        {
          std::map<std::string, Motion>::const_iterator iter = m_motions.find( name );
          if(iter==m_motions.end())
            throw std::runtime_error("no entry existed for " + name);          
          return iter->second;
        }
        
        void set_motion_for_name( std::string const & name, Motion const & M )
        {
          std::map<std::string, Motion>::const_iterator iter = m_motions.find( name );
          if(iter!=m_motions.end())
            throw std::runtime_error("entry already existed for " + name);          
          m_motions[ name ] = M;
        }
        
        /**
         * Name to rigid body state mapping.
         *
         * @param name    A textual name.
         *
         * @return        A reference to the state that corresponds to the given name.
         */
        RigidBodyState const & get_state_from_name( std::string const & name ) const
        {
          std::map<std::string, RigidBodyState>::const_iterator iter = m_rigid_body_states.find( name );
          if(iter==m_rigid_body_states.end())
            throw std::runtime_error("no entry existed for " + name);          
          return iter->second;
        }
        
        void set_state_for_name( std::string const & name, RigidBodyState const & S )
        {
          std::map<std::string, RigidBodyState>::const_iterator iter = m_rigid_body_states.find( name );
          if(iter!=m_rigid_body_states.end())
            throw std::runtime_error("entry already existed for " + name);          
          m_rigid_body_states[ name ] = S;
        }

      public:

        /**
         * Clear all points currently saved.
         */
        void clear_points() { m_points.clear(); }
        
        /**
         * Add new point to internal storage.
         *
         * @param P     The new point data to be added.
         */
        void add_point( Point const & p ) {  m_points.push_back(p); }
        
        /**
         * Get pointer to array of points in internal storage.
         *
         * @return   A pointer to the first element in an array of points
         */
        Point const * get_points() const { return m_points.empty() ? 0 : &m_points[0]; }
        
        /**
         * Get Number of Points in Internal Storage.
         *
         * @return  The number of points.
         */
        size_t size_points() const { return m_points.size(); }

      public:

        void clear_vertices() { m_vertices.clear(); }
        void add_vertex( Vertex const & v ) {  m_vertices.push_back(v); }
        Vertex const * get_vertices() const { return m_vertices.empty() ? 0 : &m_vertices[0]; }
        size_t size_vertices() const { return m_vertices.size(); }

      public:

        void clear_tetrahedra() { m_tetrahedra.clear(); }
        void add_tetrahedron( Tetrahedron const & t ) {  m_tetrahedra.push_back(t); }
        Tetrahedron const * get_tetrahedra() const { return m_tetrahedra.empty() ? 0 : &m_tetrahedra[0]; }
        size_t size_tetrahedra() const { return m_tetrahedra.size(); }

      protected:
        
        Cache(){}
        
      public:
        
        Cache(content::Input * input)
        : m_input( input )
        , m_output(0)
        , m_index(0u)
        , m_name_to_idx()
        , m_motions()
        , m_transforms()
        , m_rigid_body_states()
        , m_points()
        {
          assert( input || !"input manager was null");
        }

        Cache(content::Output * output)
        : m_input(0)
        , m_output( output )
        , m_index(0u)
        , m_name_to_idx()
        , m_motions()
        , m_transforms()
        , m_rigid_body_states()
        , m_points()
        {
          assert( output || !"output manager was null");
        }
        
        ~Cache(){}
        
      };
    
  }// namespace details
  
}// namespace content

// CONTENT_IO_CACHE_H
#endif
