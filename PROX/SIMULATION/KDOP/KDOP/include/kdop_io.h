#ifndef KDOP_IO_H
#define KDOP_IO_H

#include <kdop_tree.h>
#include <types/geometry_dop.h>

#include <iosfwd>

namespace kdop
{
  namespace io
  {

    template<typename T, size_t K>
    inline std::ostream & operator<<( std::ostream & output, geometry::DOP<T, K> const & dop)
    {
      size_t const N = K/2;
      for(size_t k =  0u; k < N; ++k)
      {
        output << "\tinterval = " << k << " lower = " << dop(k).lower() << " upper = " << dop(k).upper() << std::endl;
      }
      return output;
    }

    namespace details
    {

      template<typename T, size_t K>
      inline void print_subtree(std::ostream & output, kdop::Node<T, K> const & node, kdop::SubTree<T,K> const & tree)
      {
        output << "(root,leaf,undef) = (" << node.is_root() << ", " << node.is_leaf() << ", " << node.is_undefined() << ")" << std::endl;
        output << node.m_volume << std::endl;
        output << "\tparent      = " << node.m_parent << std::endl;
        output << "\tchild start = " << node.m_start  << std::endl;
        output << "\tchild end   = " << node.m_end    << std::endl;

        if (node.is_leaf())
          return;

        for(size_t i= node.m_start; i <= node.m_end; ++i)
        {
          print_subtree( output, tree.m_nodes[i], tree);
        }
      }

    }// namespace details


    template<typename T, size_t K>
    inline std::ostream & operator<<( std::ostream & output, kdop::SubTree<T, K> const & tree)
    {
      details::print_subtree(output, tree.m_nodes[0], tree );

      return output;
    }

    template<typename T, size_t K>
    inline std::ostream & operator<<( std::ostream & output, kdop::Tree<T, K> const & tree)
    {
      output << "root" << std::endl;
      output << tree.m_root << std::endl;

      size_t const B = tree.branches().size();
      output << "#branches = " << B << std::endl;

      for(size_t i = 0; i < B;++i)
      {
        output << "branch " << i << ":" << std::endl;
        output <<  tree.branches()[i] << std::endl;
      }
      return output;
    }

  }// namespace io

}// namespace kdop

// KDOP_IO_H
#endif