#include "pathfinder.h"

#include <algorithm>
#include <unordered_set>

namespace std {

template<>
struct hash<math::Graph::point_type>
{

public:

    //!
    //! \brief hash
    //! \param hashId
    //!
    hash(uint32_t hashId)
        : hashId_(hashId) {}

    //!
    //! \brief operator ()
    //! \param p
    //! \return
    //!
    std::size_t operator()(const math::Graph::point_type &p) const {

        return (p.y * hashId_ + p.x);
    }

private:

    uint32_t hashId_;

};
}



namespace math{


Graph::Node* Graph::getNode(uint32_t key){

    auto it = std::find_if(nodes_.begin(),
                           nodes_.end(),
                           [&key](const auto& node)
    { return node->key_ == key; }
    );

    if(it == nodes_.end()) return nullptr;
    else                   return it->get();
}

std::list<Graph::point_type> Graph::findWay(point_type p1, point_type p2)
{
    std::uint32_t start = std::hash<point_type>(hashId_)(p1);
    std::uint32_t finish = std::hash<point_type>(hashId_)(p2);

    for(auto& node : nodes_)
        node->weight_ = std::numeric_limits<uint32_t>::max();

    Node* currentNode = getNode(start);
    if(currentNode == nullptr)
        return std::list<point_type>();

    Node* startNode = currentNode;
    std::uint32_t currentWeight {0};
    std::queue<Node*> queue;  // очередь обработки вершин
    currentNode->weight_ = currentWeight;
    queue.push(currentNode);

    // ..set weights..
    while(!queue.empty()){

        currentNode = queue.front();
        queue.pop();

        for(auto key : currentNode->neighbours_){

            Node* neighbour = getNode(key);
            if(neighbour != nullptr
                    && neighbour->weight_ == std::numeric_limits<uint32_t>::max()){

                neighbour->weight_ = currentWeight;
                queue.push(neighbour);
            }
        }

        currentWeight++;
    }

    currentNode = getNode(finish);
    if(currentNode == nullptr)
        return std::list<point_type>();

    // ..find way..
    std::list<point_type> way;

    while(currentNode != startNode
          && currentNode->weight_ != std::numeric_limits<uint32_t>::max()){

        point_type point ({ currentNode->key_ / hashId_, currentNode->key_ % hashId_} );

        way.push_back(point);
        const auto& neighbours = currentNode->neighbours_;

        auto it = std::min_element(neighbours.begin(),
                                   neighbours.end(),
                                   [this](const auto& l, const auto& r){

            Node* nodeR = this->getNode(r);
            Node* nodeL = this->getNode(l);
            if(nodeR == nullptr || nodeL == nullptr)
                return false;
            else
                return nodeL->weight_ < nodeR->weight_;
        });

        if(it == neighbours.end())
            return std::list<point_type>();

        currentNode = getNode(*it);
        if(currentNode  == nullptr)
            break;
    }

    return std::move(way);
}

void Graph::addNode(point_type node, std::list<point_type> neighbours)
{
    using hashed_type = point_type;

    std::list<std::size_t> neighbourKeys;
    for(const auto& neighbour : neighbours){

        neighbourKeys.push_back(
                    std::hash<hashed_type>(hashId_)(neighbour)
                    );
    }


    auto res= std::hash<hashed_type>(hashId_)(node);

    nodes_.push_back(
                std::make_unique<Node>(std::hash<hashed_type>(hashId_)(node),
                                       std::move(neighbourKeys))
                );
}


}
