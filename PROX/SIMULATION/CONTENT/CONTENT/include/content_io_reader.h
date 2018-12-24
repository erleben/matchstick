#ifndef CONTENT_IO_READER_H
#define CONTENT_IO_READER_H

#include <content_api_input.h>

#include <string>

namespace content
{
  
  /**
   * Read Physics XML data.
   *
   * @param  filename   Path and filename for the XML file to be read.
   * @param  input      A pointer to a physics engine input API.
   *
   * @return            If the XML file was read sucessfully then the
   *                    return value is true otherwise it is false.
   */
  bool xml_read( std::string const & filename, content::Input * input );

}// namespace content

// CONTENT_IO_READER_H
#endif
