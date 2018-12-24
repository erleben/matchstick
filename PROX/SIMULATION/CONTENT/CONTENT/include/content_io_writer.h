#ifndef CONTENT_IO_WRITER_H
#define CONTENT_IO_WRITER_H

#include <content_api_output.h>

#include <string>

namespace content
{
  
  /**
   * Write Physics XML data.
   *
   * @param  filename   Path and filename for the XML file to be written.
   * @param  output     A pointer to a physics engine output API.
   *
   * @return            If the XML file was written sucessfully then the
   *                    return value is true otherwise it is false.
   */
  bool xml_write( std::string const & filename, content::Output * output );
  
}// namespace content

// CONTENT_IO_WRITER_H
#endif
