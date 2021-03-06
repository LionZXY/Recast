/**
 * @file Node.h
 * @author LionZXY
 * @project Recast-server
 * @date 20.06.17
 * @email nikita@kulikof.ru
 **/
#ifndef RECAST_SERVER_NODE_H
#define RECAST_SERVER_NODE_H

#include <set>
#include "utils/Parcel.hpp"
#include "spells/events/IEventListener.hpp"

enum NodeType {
    USUALLY = 0,
    ENERGY = 1,
    HEATER = 2,
    AIM = 3,
    GENERATOR = 4
};

class SpellNode  {
public:
    SpellNode(NodeType type, float x, float y, float z) : x(x), y(y), z(z), type(type), nowInTick(false) {}

    ~SpellNode();

    virtual bool isEnergyNode() { return false; }

    /**
     * Link node if not already exist
     * The contract involves adding the current node from the calling party
     *
     * @param otherNode
     */
    void connectNode(SpellNode *otherNode);

    /**
     * Main tick method. Not execute tick on parent node
     *
     * @param listener
     * @param callable
     */
    void tick(IEventListener &listener, SpellNode *callable);

    bool inTick() const { return nowInTick; }

    float getDistance(const SpellNode *otherNode) const;

    NodeType getType() const { return type; }

    void iterrator(std::function<void(SpellNode *)> next);

    std::set<SpellNode *> &getConnectedNodes() { return connectedNodes; }

    float getX() const { return x; }

    float getY() const { return y; }

    float getZ() const { return z; }


    static void write(Parcel &in, SpellNode * obj);
    static SpellNode *read(Parcel &out);

protected:
    float x, y, z; //// Relative coordinates from root node
    std::set<SpellNode *> connectedNodes;
    bool nowInTick; //// Inside var for iterrator between nodes
    NodeType type;

    virtual inline void onTick(IEventListener &listener, SpellNode *callable) {};
};

#endif //RECAST_SERVER_NODE_H
