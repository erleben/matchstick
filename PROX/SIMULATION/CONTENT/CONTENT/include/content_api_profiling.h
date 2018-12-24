#ifndef CONTENT_API_PROFILING_H
#define CONTENT_API_PROFILING_H

namespace content
{
  
  class Profiling
  {
  public:
    
    Profiling(){}
    virtual ~Profiling(){}

  public:

    /**
     * Get the total number of contact in the configuration from the last
     * invocation of the collision detection engine.
     *
     * @return  The number of contacts.
     */
    virtual size_t get_number_of_contacts() = 0;
    
    /**
     * The position of the contact point.
     *
     * @param contact_number         The number identifying the contact.
     * @param x                      The x-coordinate of the poisition.
     * @param y                      The y-coordinate of the poisition.
     * @param z                      The z-coordinate of the poisition.
     */
    virtual void get_contact_position( size_t const & contact_number, float & x, float & y, float & z) = 0;
    
    /**
     * The normal of the contact point.
     *
     * @param contact_number         The number identifying the contact.
     * @param x                      The x-coordinate of the normal.
     * @param y                      The y-coordinate of the normal.
     * @param z                      The z-coordinate of the normal.
     */
    virtual void get_contact_normal( size_t const & contact_number, float & x, float & y, float & z) = 0;

    /**
     * The characteristic structure direction vector of the contact point.
     *
     * @param contact_number         The number identifying the contact.
     * @param x                      The x-coordinate of the normal.
     * @param y                      The y-coordinate of the normal.
     * @param z                      The z-coordinate of the normal.
     */
    virtual void get_contact_structure_direction( size_t const & contact_number, float & x, float & y, float & z) = 0;


    /**
     * The depth (gap/penetration) of the contact point.
     *
     * @param contact_number         The number identifying the contact.
     * @param depth                  The depth value of the contact.
     */
    virtual void get_contact_depth( size_t const & contact_number, float & depth) = 0;


  };
  
}// namespace content

// CONTENT_API_PROFILING_H
#endif
