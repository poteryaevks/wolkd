#pragma once

#include <boost/noncopyable.hpp>

#include <list>
#include <functional>

#include "ztypes.h"


//!
//! \brief
//!  \author Потеряев К.С.
//!
template <class CollisionImpl>
class ICollision : private boost::noncopyable
{

protected:

    using list_type = std::list<std::reference_wrapper<const zgame::rect>>;

public:

    //!
    //! \brief isCollison
    //! \return
    //!
    bool operator()(zgame::rect& rect, std::vector<std::shared_ptr<zgame::rect>> rects) noexcept {

        return impl().calcImpl(rect, rects);
    }

    //!
    //! \brief getContacts
    //! \return
    //!
    list_type getContacts() const noexcept {

        return impl().getContactsImpl();
    }

    //!
    //! \brief Деструктор
    //!
    ~ICollision() = default;

    //!
    ICollision(float time)
        : time_(time)
    {}

private:

    CollisionImpl& impl(){

        return static_cast<CollisionImpl&>(*this);
    }

protected:

    float time_;
};


//!
//! \brief The StaticCollision class
//!
class StaticCollision : public ICollision<StaticCollision>
{

    friend class ICollision<StaticCollision>;

    StaticCollision(float time)
        : ICollision<StaticCollision>(time)
    {}

private:

    bool calcImpl(zgame::rect& rect, const std::vector<zgame::rect>& rects) noexcept {

        for (auto& r : rects){

            if(olc::aabb::RectVsRect(&r, &rect)){

                if(rect.pos != r.pos){

                    rect.pos -= rect.vel * time_;
                    return true;
                }
            }
        }

        return false;
    }

    list_type getContactsImpl() const noexcept {

        return list_type({ std::ref(dumpRect_) });
    }

private:

    zgame::rect dumpRect_;
};

//!
//! \brief The StaticCollision class
//!
class DynamicCollision : public ICollision<DynamicCollision>
{

    friend class ICollision<DynamicCollision>;

public:

    DynamicCollision(float time)
        : ICollision<DynamicCollision>(time)
    {}

private:

    //!
    //! \brief calcImpl
    //! \param rect
    //! \param rects
    //! \return
    //!
    bool calcImpl(zgame::rect& rect, std::vector<std::shared_ptr<zgame::rect>>  rects) noexcept {

        std::vector<zgame::rect*> contacts;
        olc::vf2d cp, cn;
        float t = 0;

        for (const auto& r : rects){

            if (olc::aabb::DynamicRectVsRect(&rect, time_, *r, cp, cn, t))
                contacts.push_back(const_cast<zgame::rect*>(r.get()));
        }

        for (auto& contact : contacts){

            if (olc::aabb::DynamicRectVsRect(&rect, time_, *contact, cp, cn, t)){

                if (cn.y > 0) rect.contact[0] = contact; else rect.contact[0] = nullptr;
                if (cn.x < 0) rect.contact[1] = contact; else rect.contact[1] = nullptr;
                if (cn.y < 0) rect.contact[2] = contact; else rect.contact[2] = nullptr;
                if (cn.x > 0) rect.contact[3] = contact; else rect.contact[3] = nullptr;

                rect.vel += cn * olc::vf2d(std::abs(rect.vel.x), std::abs(rect.vel.y)) * (1 - t);
                return true;
            }
        }

        return false;
    }

    std::list<std::reference_wrapper<const zgame::rect>> getContactsImpl() const noexcept {

        return list_type({ std::ref(dumpRect_) });
    }

private:

    zgame::rect dumpRect_;
};
