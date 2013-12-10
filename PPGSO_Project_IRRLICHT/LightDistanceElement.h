#include <irrlicht.h>

using namespace irr;
using namespace core;

class LightDistanceElement
    {
    public:
        LightDistanceElement() {};

        LightDistanceElement(scene::ISceneNode* n, f64 d)
            : node(n), distance(d) { }

        scene::ISceneNode* node;
        f64 distance;

        // Lower distance elements are sorted to the start of the array
        bool operator < (const LightDistanceElement& other) const
        {
            return (distance < other.distance);
        }
    };