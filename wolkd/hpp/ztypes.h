#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <map>
#include <memory>

#include <sgraphics/geometry/olc.h>
#include <sgraphics/convertion/convertion.h>

#include <QtCore/QFlags>


//!
//!
//!
namespace zgame{

using uint = std::uint32_t;
using id_type = std::uint32_t;
using rect = olc::aabb::rect<float>;
using vect_type = olc::vf2d;
using point_type = vect_type;


enum eObjectCategory {
    MAP = 1,
    PLAYER = 2,
    NPC = 4,
    ENEMY = 8,
};


enum eObjectCollisionType{

    NONE,
    SOLID
};

//static const rect CAR1         { {192, 160   }, { 32, 32 } };
//static const rect TREE         { {128, 0     }, { 32, 38 } };
//static const rect GROUND       { {0, 0       }, { 32, 32 } };
//static const zgame::rect HOUSE_WALL   { {0, 178     }, { 16, 16 } };
//static const zgame::rect HOUSE_L      { {16, 16 * 7 }, { 48, 48 } };
//static const zgame::rect HOUSE_R      { {16 * 4, 112}, { 48, 48 } };
//static const zgame::rect ROAD_H       { {224, 0     }, { 32, 32 } };
//static const zgame::rect ROAD_V       { {224, 32    }, { 32, 32 } };
//static const zgame::rect ROAD_r       { {160, 0     }, { 32, 32 } };
//static const rect GWALL_H      { {18, 54     }, { 44, 8  } };
//static const zgame::rect GWALL_V      { {0, 48      }, { 16, 48 } };
//static const zgame::rect TANK         { {0, 32      }, { 16, 16 } };
//static const zgame::rect GATE         { {88, 48,    }, { 16, 16 } };
//static const zgame::rect ROOF         { {16*7, 16*7 }, { 48, 64 } };
//static const rect MONUMENT     { {16*4, 0    }, { 16, 16 } };
//static const zgame::rect TRAIL        { {16*4, 16   }, { 16, 16 } };

//!
//! \brief The eErrorState enum
//!
enum eErrorState {
    NO_ERROR,
    TRANSPARENT_ERROR
};

//!
//! \brief The eOrientation enum
//!
enum eOrientation{
    RIGHT = 2,
    UP = 3,
    DOWN = 0,
    LEFT = 1
};

//!
//! \brief areEqual
//! \param f1
//! \param f2
//! \param eps
//! \return
//!
inline bool areEqual(const float& f1, const float& f2, double eps = 0.001)
{
    return std::fabs(f1 - f2) < eps ? true : false;
}


//!
//! \brief areEqual
//! \param p1
//! \param eps
//! \return
//!
inline bool areEqual(const point_type& p1, const point_type& p2, double eps = 0.001)
{
    bool x_eq{ false };
    bool y_eq{ false };

    std::abs(p1.x - p2.x) < eps ? x_eq = true : x_eq = false;
    std::abs(p1.y - p2.y) < eps ? y_eq = true : y_eq = false;

    return (x_eq && y_eq);
}


//!
//! \brief areEqual
//! \param r1
//! \param r2
//! \param eps
//! \return
//!
inline bool areEqual(const rect& r1, const rect& r2, double eps = 0.001)
{
    return areEqual(r1.pos, r2.pos, eps) && areEqual(r1.size, r2.size, eps);
}


//!
//! \brief toAngle
//! \param vec
//! \return
//!
inline float toAngle(const vect_type& vec)
{
    float angle = std::abs(
                std::atan(vec.y / vec.x)
                );

    if(std::isnan(angle))
        return 0;

    angle *= 180 / M_PI;

    if      (vec.x >= 0 && vec.y >= 0) { angle += 0;          }
    else if (vec.x <= 0 && vec.y <= 0) { angle += 180;        }
    else if (vec.x <= 0 && vec.y >= 0) { angle = 180 - angle; }
    else                               { angle = 360 - angle; }

    return angle;
}


//!
//! \brief toOrientation
//! \param vel
//! \return
//!
inline eOrientation toOrientation(const vect_type& vel)
{
    float angle = toAngle(vel);

    if      (angle >= 45 && angle <= 135) { return zgame::DOWN;    }
    else if (angle > 135 && angle <= 225) { return zgame::LEFT;  }
    else if (angle > 225 && angle <= 315) { return zgame::UP;  }
    else                                  { return zgame::RIGHT; }
}



}

Q_DECLARE_FLAGS(ObjectsCategory, zgame::eObjectCategory)



