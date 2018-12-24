#ifndef BROAD_OBJECT_H
#define BROAD_OBJECT_H

namespace broad
{
  
  /**
   * Broad Phase Object Interface.
   * This class defines a common interface that all broad phse collision objects must
   * implement.
   *
   * @tparam T    The precision of the broad phase collision detection system, often either float or double.
   */
  template<typename T>
  class Object
    {
    public:
      
      Object()
      : m_seen_by(0)
      {}
      
      virtual ~Object(){}
            
      /**
       * Get bounding box of object.
       * This method is used as a call-back method to ask a simulator for updated values of an objects bounding box.
       *
       *
       * @param mx    Upon return this argument should hold the current world-space minimum x-coordinate of the geometry of the object.
       * @param my    Upon return this argument should hold tThe current world-space minimum y-coordinate of the geometry of the object.
       * @param mz    Upon return this argument should hold tThe current world-space minimum z-coordinate of the geometry of the object.
       * @param Mx    Upon return this argument should hold tThe current world-space maximum x-coordinate of the geometry of the object.
       * @param My    Upon return this argument should hold tThe current world-space maximum y-coordinate of the geometry of the object.
       * @param Mz    Upon return this argument should hold tThe current world-space maximum z-coordinate of the geometry of the object.
       */
      virtual void get_box(T & mx,T & my,T & mz,T & Mx,T & My,T & Mz) const = 0;
      
    public: // 2011-01-14 Kenny: code review, this should be protected scope and m_seen_by should be accessed through the accessor class. However, since this is a hacky way of guarding against multiple report the best resolution might be to leave it be for now.
      
      Object<T> * m_seen_by;   ///< Object pointer used to guard against multiple
                               ///< reported overlap pairs in the grid based algorithm.
                               ///< This is not the most pretty solution but it is very
                               ///< efficient as it allows for a constant time test to guard
                               ///< against multiple reported overlaps. On the downside it
                               ///< increases the object memory footprint and the technique
                               ///< itself is inherently sequential (thus not parallizable).
                               
       
    };
  
} //namespace broad

// BROAD_OBJECT_H
#endif 
