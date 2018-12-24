#ifndef GEOMETRY_CONTACTS_CALLBACK_H
#define GEOMETRY_CONTACTS_CALLBACK_H

namespace geometry
{
  
    template<typename V>
    class ContactsCallback
    {
    public:

      ContactsCallback(){}
      virtual ~ContactsCallback(){}

    public:

        /**
         * Callback interface for reporting newly found contact points.
         *
         * @param point     The contact point in WCS.
         * @param normal    The contact point normal in WCS.
         * @param distance  The penetration distance measure, negative if overlapping and positive if separation.
         * @param sA        Structure direction of body A
         * @param sB        Structure direction of body B
         */
        virtual void operator()( 
                                   V const & point
                                  , V const & normal
                                  , typename V::real_type const & distance
                                  , V const & sA
                                  , V const & sB
                                 ) = 0;
    };
  
}//namespace geometry

//GEOMETRY_CONTACTS_CALLBACK_H
#endif
