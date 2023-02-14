#pragma once

#pragma once

#include <boost/noncopyable.hpp>

#include <functional>

#include "../Types.h"
#include "../ICollision.h"

namespace game
{
    class StaticCollision : public ICollision
    {
    public:
        StaticCollision() = default;

        bool Calculate(FRectType &rect, const std::vector<FRectPtr> &rects, float time) override
        {
            for (auto &r : rects)
            {
                if (olc::aabb::RectVsRect(r, &rect))
                {
                    if (rect.pos != r->pos)
                    {
                        rect.pos -= rect.vel * time;
                        return true;
                    }
                }
            }

            return false;
        }

        RectsType GetContacts() const override { return {}; }
    };

    class DynamicCollision : public ICollision
    {
    public:
        DynamicCollision() = default;

        bool Calculate(FRectType &rect, const std::vector<FRectPtr> &rects, float time) override
        {
            std::vector<FRectType *> contacts;
            olc::vf2d cp, cn;
            float t = 0;

            for (const auto &r : rects)
            {
                if (olc::aabb::DynamicRectVsRect(&rect, time, *r, cp, cn, t))
                    contacts.push_back(const_cast<FRectType *>(r));
            }

            for (auto &contact : contacts)
            {

                if (olc::aabb::DynamicRectVsRect(&rect, time, *contact, cp, cn, t))
                {
                    rect.contact[0] = cn.y > 0 ? contact : nullptr;
                    rect.contact[1] = cn.x < 0 ? contact : nullptr;
                    rect.contact[2] = cn.y < 0 ? contact : nullptr;
                    rect.contact[3] = cn.x > 0 ? contact : nullptr;

                    rect.vel += cn * olc::vf2d(std::abs(rect.vel.x), std::abs(rect.vel.y)) * (1 - t);
                    return true;
                }
            }

            return false;
        }

        RectsType GetContacts() const override { return {}; }
    };
}
