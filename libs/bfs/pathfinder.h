#pragma once

#include <queue>
#include <memory>
#include <list>
#include <stdint.h>
#include <limits>

#include <sgraphics/geometry/olc.h>

namespace std {
template<class T> struct hash;
}

namespace math{

//!
//! \brief The PathFinder class
//!
class Graph
{
    template<class T>
    friend struct std::hash;

    //!
    //! \brief The Node struct
    //!
    struct Node
    {
        Node(std::size_t key, std::list<std::size_t> neighbours, uint32_t weight = std::numeric_limits<uint32_t>::max())
            : key_(key),
              weight_(weight),
              neighbours_(std::move(neighbours))
        {}

        std::size_t key_;
        uint32_t weight_;
        std::list<std::size_t> neighbours_;
    };

    using nodes_type = std::vector<std::unique_ptr<Node>>;
    using point_type = olc::vf2d;

public:

    //!
    //! \brief Graph
    //! \param hashId
    //!
    Graph(uint32_t hashId = 0)
        : hashId_(hashId) {}

    //!
    //! \brief findWay
    //! \param start
    //! \param finish
    //! \return
    //!
    std::list<point_type> findWay(point_type p1, point_type p2);

    //!
    //! \brief addNode
    //! \param node
    //!
    void addNode(point_type node, std::list<point_type> neighbours);

private:

    //!
    //! \brief getNode
    //! \param key
    //! \return
    //!
    Node* getNode(uint32_t key);

private:

    nodes_type nodes_;  //!<
    uint32_t hashId_;

};

}
