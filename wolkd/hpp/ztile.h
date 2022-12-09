#pragma once

#include "ztypes.h"

#include <memory>
#include <functional>

#include <sgraphics/engine/isprite.h>
#include <sgraphics/engine/polygon_funcs.h>
#include <sgraphics/geometry/olc.h>

#include <almaz/misc/factory.h>


//!
//! \brief The ZSprite class
//!
class ZTile : public sg::engine::ISprite<ZTile>
{
    friend class ISprite<ZTile>;

public:

    ZTile(std::shared_ptr<zgame::rect> rect,
          const std::string& path,
          const std::array<uint8_t, 3>& rgb  = {0, 0, 0})
        : ISprite<ZTile>(path, rgb),
          rect_(rect)
    {}

    virtual ~ZTile() {}

    //!
    //! \brief getType
    //! \return
    //!
    virtual zgame::eObjectCollisionType getType() = 0;

    //!
    //! \brief show
    //!
    virtual void show(const zgame::vect_type&, float) noexcept = 0;

    //!
    //! \brief getRect
    //! \return
    //!
    zgame::rect getOffsettedRect() {

        zgame::rect rect = *rect_;
        rect.pos += offset_;
        return rect;
    }

    //!
    //! \brief getRect
    //! \return
    //!
    std::shared_ptr<zgame::rect> getRect() {

        return rect_;
    }

    //!
    //! \brief setRect
    //! \param rect
    //!
    void setRect(zgame::rect rect) { *rect_ = std::move(rect); }

    //!
    //! \brief setRect
    //! \param rect
    //!
    void setOffset(const zgame::vect_type& offset) { offset_ = offset; }

protected:

    std::shared_ptr<zgame::rect> rect_;
    zgame::vect_type offset_ ;
};


using TileConstuctor = std::function<std::unique_ptr<ZTile>(
std::shared_ptr<zgame::rect>, const std::string&, const std::array<uint8_t, 3>&
)>;


static almaz::misc::IFactory<ZTile, char, TileConstuctor, std::shared_ptr<zgame::rect>, const std::string&, const std::array<uint8_t, 3>&> TILE_FACTORY;

//!
//! \brief The CarTile class
//!
class CarTile final : public ZTile
{

public:

    CarTile(std::shared_ptr<zgame::rect> rect,
            const std::string& path,
            const std::array<uint8_t, 3>& rgb  = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {}

    ~CarTile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::SOLID; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        renderCopy(GROUND, rect);
        renderCopy(ROAD_H, rect);
        renderCopy(CAR1, rect);
    }

private:

    const zgame::rect CAR1   { {192, 160   }, { 32, 32 } };
    const zgame::rect GROUND { {0, 0       }, { 32, 32 } };
    const zgame::rect ROAD_H { {224, 0     }, { 32, 32 } };
};

//!
//! \brief The CarTile class
//!
class TreeTile final : public ZTile
{

public:

    TreeTile(std::shared_ptr<zgame::rect> rect,
             const std::string& path,
             const std::array<uint8_t, 3>& rgb  = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {
//        prevRect_ = rect_;
//        rect_->size.x *= 2;
//        rect_->size.y *= 2;

//        rect_->pos.x -= rect_->size.x;
//        rect_->pos.y -= rect_->size.y;
    }

    ~TreeTile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::SOLID; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        renderCopy(GROUND, rect);
        renderCopy(TREE, rect);
    }

private:

    const zgame::rect GROUND { {0, 0       }, { 32, 32 } };
    const zgame::rect TREE   { {128, 0     }, { 32, 38 } };
};

//!
//! \brief The GWallTile class
//!
class GWallTile final : public ZTile
{

public:

    GWallTile(std::shared_ptr<zgame::rect> rect,
              const std::string& path,
              const std::array<uint8_t, 3>& rgb  = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {}

    ~GWallTile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::SOLID; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        renderCopy(GROUND, rect);
        renderCopy(GWALL_H, rect);
    }

private:

    const zgame::rect GWALL_H { {18, 54     }, { 44, 8  } };
    const zgame::rect GROUND  { {0, 0       }, { 32, 32 } };

};

class TankTile final : public ZTile
{
public:

    TankTile(std::shared_ptr<zgame::rect> rect,
             const std::string& path,
             const std::array<uint8_t, 3>& rgb = {0, 0, 0})
        : ZTile(rect, path, rgb),
          count_(0)
    {}

    ~TankTile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::SOLID; }

    virtual void show(const zgame::vect_type& offset, float tp) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        updateSprite_ += tp;

        if(updateSprite_ > 1 / FPS_){

            updateSprite_ = 0;
            count_++;
        }

        renderCopy(GROUND, rect);
        zgame::rect srcRect ({
                                 {(float)(16 * ((int32_t)count_ % 3)), (float)32},
                                 {16, 16}
                             });

        renderCopy(srcRect, rect);
    }

private:

    float updateSprite_;
    Uint32 count_;
    static constexpr float FPS_ { 5 };
    const zgame::rect GROUND  { {0, 0 }, { 32, 32 } };
};


//!
//! \brief The MonumentTile class
//!
class MonumentTile final : public ZTile
{

public:

    MonumentTile(std::shared_ptr<zgame::rect> rect,
                 const std::string& path,
                 const std::array<uint8_t, 3>& rgb = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {}

    ~MonumentTile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::SOLID; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        renderCopy(GROUND, rect);
        renderCopy(MONUMENT, rect);
    }

private:

    const zgame::rect MONUMENT { {16*4, 0    }, { 16, 16 } };
    const zgame::rect GROUND   { {0, 0 }, { 32, 32 } };
};

//!
//! \brief The MonumentTile class
//!
class NoneTile final : public ZTile
{

public:

    NoneTile(std::shared_ptr<zgame::rect> rect,
             const std::string& path,
             const std::array<uint8_t, 3>& rgb = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {}

    ~NoneTile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::NONE; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        renderCopy(GROUND, rect);
    }

private:

    const zgame::rect GROUND   { {0, 0 }, { 32, 32 } };
};

//!
//! \brief The HouseLeftTile class
//!
class HouseLeftTile final : public ZTile
{

public:

    HouseLeftTile(std::shared_ptr<zgame::rect> rect,
                  const std::string& path,
                  const std::array<uint8_t, 3>& rgb = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {
        prevRect_ = *rect_;
        rect_->size.x *= 3;
        rect_->size.y *= 3;

        rect_->pos.x -= prevRect_.size.x * 2;
        rect_->pos.y -= prevRect_.size.y * 2;
    }

    ~HouseLeftTile() = default;


    virtual zgame::eObjectCollisionType getType() override { return zgame::SOLID; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();
        zgame::rect temp =  prevRect_;
        temp.pos += offset;

        renderCopy(GROUND, temp);
        renderCopy(HOUSE_L, rect);
    }

private:

    zgame::rect prevRect_;

    const zgame::rect HOUSE_L  { {16, 16 * 7 }, { 48, 48 } };
    const zgame::rect GROUND   { {0, 0 }, { 32, 32 } };
};


//!
//! \brief The HouseRightTile class
//!
class HouseRightTile final : public ZTile
{

public:

    HouseRightTile(std::shared_ptr<zgame::rect> rect,
                   const std::string& path,
                   const std::array<uint8_t, 3>& rgb = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {
        prevRect_ = *rect_;
        rect_->size.x *= 3;
        rect_->size.y *= 3;

        rect_->pos.x -= prevRect_.size.x * 2;
        rect_->pos.y -= prevRect_.size.y * 2;
    }

    ~HouseRightTile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::SOLID; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        zgame::rect temp =  prevRect_;
        temp.pos += offset;

        renderCopy(GROUND, temp);
        renderCopy(HOUSE_R, rect);
    }

private:

    zgame::rect prevRect_;
    const zgame::rect GROUND   { {0, 0 }, { 32, 32 } };
    const zgame::rect HOUSE_R  { {16 * 4, 112}, { 48, 48 } };
};



//!
//! \brief The RoadTile class
//!
class GroundTile final : public ZTile
{

public:

    GroundTile(std::shared_ptr<zgame::rect> rect,
               const std::string& path,
               const std::array<uint8_t, 3>& rgb = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {}

    ~GroundTile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::NONE; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        renderCopy(GROUND, rect);
    }

private:

    const zgame::rect GROUND   { {0, 0 }, { 32, 32 } };
};

//!
//! \brief The RoadTile class
//!
class RoadITile final : public ZTile
{

public:

    RoadITile(std::shared_ptr<zgame::rect> rect,
              const std::string& path,
              const std::array<uint8_t, 3>& rgb = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {}

    ~RoadITile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::NONE; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{


        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        renderCopy(GROUND, rect);
        renderCopy(ROAD_V, rect);
    }

private:

    const zgame::rect ROAD_V { {224, 32    }, { 32, 32 } };
    const zgame::rect GROUND { {0, 0 }, { 32, 32 } };
};


//!
//! \brief The Road_Tile class
//!
class Road_Tile final : public ZTile
{

public:

    Road_Tile(std::shared_ptr<zgame::rect> rect,
              const std::string& path,
              const std::array<uint8_t, 3>& rgb = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {}

    ~Road_Tile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::NONE; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        renderCopy(GROUND, rect);
        renderCopy(ROAD_H, rect);
    }

private:

    const zgame::rect GROUND { {0, 0 }, { 32, 32 } };
    const zgame::rect ROAD_H { {224, 0 }, { 32, 32 } };
};

//!
//! \brief The Road_Tile class
//!
class RoadrTile final : public ZTile
{

public:

    RoadrTile(std::shared_ptr<zgame::rect> rect,
              const std::string& path,
              const std::array<uint8_t, 3>& rgb = {0, 0, 0})
        : ZTile(rect, path, rgb)
    {}

    ~RoadrTile() = default;

    virtual zgame::eObjectCollisionType getType() override { return zgame::NONE; }

    virtual void show(const zgame::vect_type& offset, float) noexcept override{

        setOffset(offset);
        zgame::rect rect = getOffsettedRect();

        renderCopy(GROUND, rect);
        renderCopy(ROAD_r, rect);
    }

private:

    const zgame::rect GROUND  { {0, 0 }, { 32, 32 } };
    const zgame::rect ROAD_r  { {160, 0     }, { 32, 32 } };
};

////////////////////////factory////////////////////////


const bool CarTileReg = TILE_FACTORY.Register('1',
                                              [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb = {0, 0, 0}){
        return std::make_unique<CarTile>(rect, path, rgb);
        });


const bool TreeTileReg = TILE_FACTORY.Register('#',
                                               [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<TreeTile>(rect, path, rgb);
        });


const bool GWallTileReg = TILE_FACTORY.Register('w',
                                                [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<GWallTile>(rect, path, rgb);
        });


const bool TankTileReg = TILE_FACTORY.Register('b',
                                               [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<TankTile>(rect, path, rgb);
        });


const bool MonumentTileReg = TILE_FACTORY.Register('+',
                                                   [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<MonumentTile>(rect, path, rgb);
        });


const bool GroundTileReg = TILE_FACTORY.Register('.',
                                                 [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<GroundTile>(rect, path, rgb);
        });


const bool Road_TileReg = TILE_FACTORY.Register('-',
                                                [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<Road_Tile>(rect, path, rgb);
        });


const bool RoadITileReg = TILE_FACTORY.Register('|',
                                                [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<RoadITile>(rect, path, rgb);
        });


const bool RoadrTileReg = TILE_FACTORY.Register('r',
                                                [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<RoadrTile>(rect, path, rgb);
        });

const bool NoneTileReg = TILE_FACTORY.Register('n',
                                               [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<NoneTile>(rect, path, rgb);
        });

const bool HouseLReg = TILE_FACTORY.Register('L',
                                             [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<HouseLeftTile>(rect, path, rgb);
        });

const bool HouseRReg = TILE_FACTORY.Register('J',
                                             [](std::shared_ptr<zgame::rect> rect, const std::string& path, const std::array<uint8_t, 3>& rgb= {0, 0, 0}){
        return std::make_unique<HouseRightTile>(rect, path, rgb);
        });


