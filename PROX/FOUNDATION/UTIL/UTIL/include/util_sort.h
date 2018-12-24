#ifndef UTIL_SORT_H
#define UTIL_SORT_H

#include <vector>

namespace util
{

  template <typename T>
  inline void sort(
                     T const & l1
                   , T const & l2
                   , T const & l3
                   , T & lmin
                   , T & lmed
                   , T & lmax
                   )
  {
    if (l1 < l2)
    {
      if (l1 < l3)
      {
        if (l2 < l3)
        {
          lmin = l1;
          lmed = l2;
          lmax = l3;
        }
        else
        {
          lmin = l1;
          lmed = l3;
          lmax = l2;
        }
      }
      else
      {
        lmin = l3;
        lmed = l1;
        lmax = l2;
      }
    }
    else
    {
      if (l2 < l3)
      {
        if (l1 < l3)
        {
          lmin = l2;
          lmed = l1;
          lmax = l3;
        }
        else
        {
          lmin = l2;
          lmed = l3;
          lmax = l1;
        }
      }
      else
      {
        lmin = l3;
        lmed = l2;
        lmax = l1;
      }
    }
  }


  template <typename T>
  inline void sort(
                   T const & v0
                   , T const & v1
                   , T const & v2
                   , T const & v3
                   , std::vector<unsigned int> & pi
                   )
  {

    std::vector<T> v;

    v.resize(4u);
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    v[3] = v3;

    pi.resize(4u);
    pi[0] = 0;
    pi[1] = 0;
    pi[2] = 0;
    pi[3] = 0;

    static unsigned int const pis[24][4] = {
      {0, 1, 2, 3},
      {0, 1, 3, 2},
      {0, 2, 1, 3},
      {0, 2, 3, 1},
      {0, 3, 1, 2},
      {0, 3, 2, 1},
      {1, 0, 2, 3},
      {1, 0, 3, 2},
      {1, 2, 0, 3},
      {1, 2, 3, 0},
      {1, 3, 0, 2},
      {1, 3, 2, 0},
      {2, 1, 0, 3},
      {2, 1, 3, 0},
      {2, 0, 1, 3},
      {2, 0, 3, 1},
      {2, 3, 1, 0},
      {2, 3, 0, 1},
      {3, 1, 2, 0},
      {3, 1, 0, 2},
      {3, 2, 1, 0},
      {3, 2, 0, 1},
      {3, 0, 1, 2},
      {3, 0, 2, 1}
    };

    for(unsigned int i = 0; i < 24;++i)
    {
      unsigned int const & i0 = pis[i][0];
      unsigned int const & i1 = pis[i][1];
      unsigned int const & i2 = pis[i][2];
      unsigned int const & i3 = pis[i][3];

      if( (v[i0] <= v[i1]) && (v[i1] <= v[i2]) && (v[i2] <= v[i3]) )
      {
        pi[0] = i0;
        pi[1] = i1;
        pi[2] = i2;
        pi[3] = i3;
        return;
      }
    }
  }

}//end of namespace util

// UTIL_SORT_H
#endif
