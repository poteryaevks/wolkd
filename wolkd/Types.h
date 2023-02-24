#pragma once

#include <vector>
#include <map>
#include <memory>

#include <sgraphics/engine/BaseGame.hpp>
#include <sgraphics/geometry/olc.h>
#include <sgraphics/convertion/convertion.h>
#include <sgraphics/geometry/types.hpp>

#include <QtCore/QFlags>

namespace game
{
    using IdType = std::uint32_t;
    using FRectType = olc::aabb::rect<float>;
    using FRectPtr = FRectType*;
    using FRectRef = std::reference_wrapper<const FRectType>;
    using FRectRefs = std::vector<FRectRef>;
    using FRectPtrs = std::vector<FRectPtr>;
    using FVectType = olc::vf2d;
    using PointType = FVectType;
    using Duration = sgraphics::BaseGame::Duration;

    enum eObjectCategory
    {
        MAP = 1,
        PLAYER = 2,
        NPC = 4,
        ENEMY = 8,
    };

    enum CollisionType
    {
        NONE,
        SOLID
    };

    // static const rect CAR1         { {192, 160   }, { 32, 32 } };
    // static const rect TREE         { {128, 0     }, { 32, 38 } };
    // static const rect GROUND       { {0, 0       }, { 32, 32 } };
    // static const FRectType HOUSE_WALL   { {0, 178     }, { 16, 16 } };
    // static const FRectType HOUSE_L      { {16, 16 * 7 }, { 48, 48 } };
    // static const FRectType HOUSE_R      { {16 * 4, 112}, { 48, 48 } };
    // static const FRectType ROAD_H       { {224, 0     }, { 32, 32 } };
    // static const FRectType ROAD_V       { {224, 32    }, { 32, 32 } };
    // static const FRectType ROAD_r       { {160, 0     }, { 32, 32 } };
    // static const rect GWALL_H      { {18, 54     }, { 44, 8  } };
    // static const FRectType GWALL_V      { {0, 48      }, { 16, 48 } };
    // static const FRectType TANK         { {0, 32      }, { 16, 16 } };
    // static const FRectType GATE         { {88, 48,    }, { 16, 16 } };
    // static const FRectType ROOF         { {16*7, 16*7 }, { 48, 64 } };
    // static const rect MONUMENT     { {16*4, 0    }, { 16, 16 } };
    // static const FRectType TRAIL        { {16*4, 16   }, { 16, 16 } };

    enum eErrorState
    {
        NO_ERROR,
        TRANSPARENT_ERROR
    };

    enum eOrientation
    {
        RIGHT = 2,
        UP = 3,
        DOWN = 0,
        LEFT = 1
    };

    inline bool areEqual(const float &f1, const float &f2, double eps = 0.001)
    {
        return std::fabs(f1 - f2) < eps ? true : false;
    }

    inline bool areEqual(const PointType &p1, const PointType &p2, double eps = 0.001)
    {
        bool x_eq{false};
        bool y_eq{false};

        std::abs(p1.x - p2.x) < eps ? x_eq = true : x_eq = false;
        std::abs(p1.y - p2.y) < eps ? y_eq = true : y_eq = false;

        return (x_eq && y_eq);
    }

    inline bool areEqual(const FRectType &r1, const FRectType &r2, double eps = 0.001)
    {
        return areEqual(r1.pos, r2.pos, eps) && areEqual(r1.size, r2.size, eps);
    }

    inline float toAngle(const FVectType &vec)
    {
        float angle = std::abs(
            std::atan(vec.y / vec.x));

        if (std::isnan(angle))
            return 0;

        angle *= 180 / M_PI;

        if (vec.x >= 0 && vec.y >= 0)
        {
            angle += 0;
        }
        else if (vec.x <= 0 && vec.y <= 0)
        {
            angle += 180;
        }
        else if (vec.x <= 0 && vec.y >= 0)
        {
            angle = 180 - angle;
        }
        else
        {
            angle = 360 - angle;
        }

        return angle;
    }

    inline eOrientation toOrientation(const FVectType &vel)
    {
        float angle = toAngle(vel);

        if (angle >= 45 && angle <= 135)
        {
            return eOrientation::DOWN;
        }
        else if (angle > 135 && angle <= 225)
        {
            return eOrientation::LEFT;
        }
        else if (angle > 225 && angle <= 315)
        {
            return eOrientation::UP;
        }
        else
        {
            return eOrientation::RIGHT;
        }
    }
}

Q_DECLARE_FLAGS(ObjectsCategory, game::eObjectCategory)
