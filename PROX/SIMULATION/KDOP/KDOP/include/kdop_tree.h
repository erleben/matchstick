#ifndef KDOP_TREE_H
#define KDOP_TREE_H

#include <types/geometry_dop.h>

#include <vector>
#include <cassert>

namespace kdop
{

  inline size_t UNDEFINED() { return 0xFFFF; }

  template<typename T, size_t K>
  class Node
  {
  public:

    typedef geometry::DOP<T,K> volume_type;

  public:

    volume_type m_volume;
    size_t      m_parent;   // Index of parent node, undefined for root node
    size_t      m_start;    // Index of first child node
    size_t      m_end;      // Index of last child node, if node is a leaf then start==end and start has the index value of the geometry entity (tetrahedron) covered by the node.

  public:

    Node()
    : m_volume()
    , m_parent( UNDEFINED() )
    , m_start( UNDEFINED() )
    , m_end( UNDEFINED() )
    {}

    ~Node(){}

    Node(Node const & node)
    {
      *this = node;
    }

    Node& operator=( Node const & node)
    {
      if( this != &node)
      {
        this->m_volume = node.m_volume;
        this->m_parent = node.m_parent;
        this->m_start  = node.m_start;
        this->m_end    = node.m_end;
      }
      return *this;
    }
  public:

    bool is_root() const
    {
      return m_parent == UNDEFINED() && m_start!=UNDEFINED() && m_end!=UNDEFINED();
    }

    bool is_leaf() const
    {
      //return m_parent != UNDEFINED() && m_start==m_end;
      return m_start==m_end;
    }

    bool is_undefined() const
    {
      // If all indices are undefined then this node is neither a root, a leaf or an internal node of a tree.
      return m_parent == UNDEFINED() && m_start==UNDEFINED() && m_end==UNDEFINED();
    }


  };

  template<typename T, size_t K>
  class SubTree
  {
  public:

    typedef Node<T,K> node_type;

  public:

    std::vector<node_type> m_nodes;
    size_t m_height;

  public:

    SubTree()
    : m_nodes()
    , m_height(0)
    {}

    ~SubTree(){}

    SubTree( SubTree const & stree)
    {
      *this = stree;
    }

    SubTree& operator=( SubTree const & stree)
    {

      if( this != &stree) {
        this->m_nodes = stree.m_nodes;
        this->m_height = stree.m_height;
      }

      return *this;
    }
  };

  template<typename T, size_t K>
  class Tree
  {
  public:

    typedef geometry::DOP<T,K>     volume_type;
    typedef SubTree<T,K>           subtree_type;

  public:

    geometry::DOP<T,K>                       m_root;            ///< A root kDOP that contains the whole tree.

    std::vector< std::vector<subtree_type> > m_chunk_levels;    ///< Levels of chuncks, each level
                                                                ///< has more refined details. The lowest
                                                                ///< level, back(), are the leaves pointing to the
                                                                ///< tetrahedrons of the mesh.
                                                                ///< We call the lowest level, branches, and
                                                                ///< the intermediate levels are called super
                                                                ///< chuncks (they are chunks of chunks).

  public:

    Tree()
    : m_root()
    , m_chunk_levels()
    {}

    ~Tree(){}

    Tree( Tree const & tree)
    {
      *this = tree;
    }

    Tree& operator= (Tree const & tree)
    {
      if( this != &tree)
      {
        this->m_root         = tree.m_root;
        this->m_chunk_levels = tree.m_chunk_levels;
      }
      return *this;
    }

  public:

    void clear()
    {
      this->m_root = volume_type();
      this->m_chunk_levels.clear();
    }

    typename std::vector< std::vector<subtree_type> >::reference branches()
    {
      assert(this->m_chunk_levels.size() > 0 || !"branches(): internal error, no branches exist");

      return this->m_chunk_levels.back();
    }

    typename std::vector< std::vector<subtree_type> >::const_reference branches() const
    {
      assert(this->m_chunk_levels.size() > 0 || !"branches(): internal error, no branches exist");

      return m_chunk_levels.back();
    }

    typename std::vector< std::vector<subtree_type> >::reference super_chunks(size_t const & level)
    {
      assert(level < m_chunk_levels.size() || !"super_chunks: out of bounds");

      return m_chunk_levels[level];
    }

    typename std::vector< std::vector<subtree_type> >::const_reference super_chunks(size_t const &  level) const
    {
      assert(level < m_chunk_levels.size() || !"super_chunks: out of bounds");

      return m_chunk_levels[level];
    }

    size_t number_of_levels() const
    {
      return m_chunk_levels.size();
    }

  };

}// namespace kdop

// KDOP_TREE_H
#endif