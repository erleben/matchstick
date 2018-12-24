#ifndef CONTENT_IO_BASE_H
#define CONTENT_IO_BASE_H

#include <tinyxml.h>

namespace content
{
    namespace details
    {
        
        /**
         * descr
         *
         * @param  element
         * @param  attrib_name
         * @param  mandatory
         *
         * @return                  
         */
        std::string read_string(  TiXmlElement const * element
                                , std::string const & attrib_name
                                , bool const & mandatory = false
                                );
        
        /**
         * descr
         *
         * @param  element
         * @param  attrib_name
         * @param  default_value
         * @param  mandatory
         *
         * @return                  
         */        
        float read_float(  TiXmlElement const * element
                         , std::string const & attrib_name
                         , float const & default_value= 0.0f
                         , bool const & mandatory = false
                         );
        
        /**
         * descr
         *
         * @param  element
         * @param  attrib_name
         * @param  default_value
         * @param  mandatory
         *
         * @return                  
         */
        int read_integer(  TiXmlElement const * element
                         , std::string const & attrib_name
                         , int const & default_value= 0
                         , bool const & mandatory = false
                         );
        
        /**
         * descr
         *
         * @param  parent
         * @param  name
         * @param  mandatory
         *
         * @return                  
         */
        TiXmlElement const * get_singleton_child(  TiXmlNode const * parent
                                                 , std::string const & name
                                                 , bool const & mandatory = false
                                                 );
        
    } // namespace details

    
}// namespace content
// CONTENT_IO_BASE_H
#endif
