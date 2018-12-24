#ifndef CONTENT_IO_UTILS_H
#define CONTENT_IO_UTILS_H

#include <tinyxml.h>
#include <content_io_transform.h>
#include <content_io_motion.h>
#include <content_io_cache.h>

#include <string>

namespace content
{
  namespace details
  {
    
    /**
     * Read a collection of XML elements.
     * This is a convenience function which makes it easier to write code for parsing
     * XML elements. It is equivalent to a conditional for_each kind of construct.
     *
     * @param tag        The XML element holding the collection that is to be read.
     * @param item       The name of the XML elements in the collection that should be read.
     * @param dispatch   A callback function that is invoked on each XML element in the collection.
     * @param data       The current Data class instance, used for passing along information.
     *
     * @return           If succesfull then the return value is true otherwise it is false.
     */
    bool read_collection (TiXmlElement const * tag, std::string const & item, bool (*dispatch)(TiXmlElement const *, Cache &), Cache & data );
    
   /**
     * Read boolean value from XML element.
     *
     * @param element         A pointer to the XML element from which the boolean attribute should be read.
     * @param attrib_name     The name of the attribute holding the boolean value.
     * @param default_value   The default value if data can not be found.
     * @param mandatory       Boolean flag indicating whether the data is mandatory.  
     *
     * @return                The resulting boolean value.
     */    
    bool read_bool(TiXmlElement const * element, std::string const & attrib_name, bool const & default_value = true, bool const & mandatory = false);
    
    /**
    * Read Transform data from XML element.
    *
    * @param element         A pointer to the XML element.
    * @param data            The current Data class instance, used for passing along information.
    * @param default_value   The default value of the transform if the data can not be found.
    *
    * @return                The resulting transform data.
    */
    Transform read_transform(TiXmlElement const * element, Cache & data, Transform const & default_value = Transform());

    /**
     * Read Motion data from XML element.
     *
     * @param element         A pointer to the XML element.
     * @param data            The current Data class instance, used for passing along information.
     * @param default_value   The default value of the motion if the data can not be found.
     *
     * @return                The resulting motion data.
     */
    Motion read_motion(TiXmlElement const * element, Cache & data, Motion const & default_value = Motion());
    
  } // namespace details
}// namespace content

// CONTENT_IO_UTILS_H
#endif
