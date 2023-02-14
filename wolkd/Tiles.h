#pragma once

#include "Types.h"

#include <memory>
#include <functional>
#include <filesystem>

#include <sgraphics/engine/ISprite.hpp>
#include <sgraphics/geometry/olc.h>
#include <sgraphics/engine/Engine.hpp>

#include <alm/misc/factory.h>

namespace game
{
    class ITile
    {
    public:
        using Ptr = std::unique_ptr<ITile>;
        ITile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : sprite_(sg::GetEngine().CreateSprite(path, rgb)),
              rect_(rect)
        {
        }

        virtual ~ITile() = default;
        virtual CollisionType getType() = 0;
        virtual void show(const FVectType &, const Duration &) noexcept = 0;

        const FRectType& GetRect() { return offSettedRect_; }

    protected:
        inline FRectType GetOffSettedRect(const FVectType &offset)
        {
            FRectType rect = rect_;
            rect.pos += offset;
            offSettedRect_ = rect;
            return rect;
        }

        sg::ISprite::Ptr sprite_;
        FRectType rect_;
        FRectType offSettedRect_;
    };

    using TileConstuctor = std::function<ITile::Ptr(FRectType, const std::string &, const sg::RgbType &)>;
    static alm::misc::IFactory<ITile, char, TileConstuctor, FRectType, const std::string &, const sg::RgbType &> TILE_FACTORY;

    class CarTile final : public ITile
    {
    public:
        CarTile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
        }

        ~CarTile() = default;
        CollisionType getType() override { return SOLID; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            sprite_->RenderCopy(GROUND, rect);
            sprite_->RenderCopy(ROAD_H, rect);
            sprite_->RenderCopy(CAR1, rect);
        }

    private:
        const FRectType CAR1{{192, 160}, {32, 32}};
        const FRectType GROUND{{0, 0}, {32, 32}};
        const FRectType ROAD_H{{224, 0}, {32, 32}};
    };

    class TreeTile final : public ITile
    {
    public:
        TreeTile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
            //        prevRect_ = rect_;
            //        rect_.size.x *= 2;
            //        rect_.size.y *= 2;
            //        rect_.pos.x -= rect_.size.x;
            //        rect_.pos.y -= rect_.size.y;
        }

        ~TreeTile() = default;
        CollisionType getType() override { return SOLID; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            sprite_->RenderCopy(GROUND, rect);
            sprite_->RenderCopy(TREE, rect);
        }

    private:
        const FRectType GROUND{{0, 0}, {32, 32}};
        const FRectType TREE{{128, 0}, {32, 38}};
    };

    class GWallTile final : public ITile
    {

    public:
        GWallTile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
        }

        ~GWallTile() = default;

        CollisionType getType() override { return SOLID; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            sprite_->RenderCopy(GROUND, rect);
            sprite_->RenderCopy(GWALL_H, rect);
        }

    private:
        const FRectType GWALL_H{{18, 54}, {44, 8}};
        const FRectType GROUND{{0, 0}, {32, 32}};
    };

    class TankTile final : public ITile
    {
    public:
        TankTile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
        }

        ~TankTile() = default;
        CollisionType getType() override { return SOLID; }
        void show(const FVectType &offset, const Duration &duration) noexcept override
        {
            using namespace std::chrono_literals;
            auto rect = GetOffSettedRect(offset);
            elapsedTime_ += duration;

            if (elapsedTime_ > 100ms)
            {
                elapsedTime_ = 0s;
                count_++;
            }

            sprite_->RenderCopy(GROUND, rect);
            FRectType srcRect({{(float)(16 * ((int32_t)count_ % 3)), (float)32}, {16, 16}});
            sprite_->RenderCopy(srcRect, rect);
        }

    private:
        Duration elapsedTime_{};
        std::uint32_t count_{};
        static constexpr float FPS_{5};
        const FRectType GROUND{{0, 0}, {32, 32}};
    };

    class MonumentTile final : public ITile
    {
    public:
        MonumentTile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
        }

        ~MonumentTile() = default;

        CollisionType getType() override { return SOLID; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            sprite_->RenderCopy(GROUND, rect);
            sprite_->RenderCopy(MONUMENT, rect);
        }

    private:
        const FRectType MONUMENT{{16 * 4, 0}, {16, 16}};
        const FRectType GROUND{{0, 0}, {32, 32}};
    };

    class NoneTile final : public ITile
    {
    public:
        NoneTile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
        }

        ~NoneTile() = default;

        CollisionType getType() override { return NONE; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            sprite_->RenderCopy(GROUND, rect);
        }

    private:
        const FRectType GROUND{{0, 0}, {32, 32}};
    };

    class HouseLeftTile final : public ITile
    {
    public:
        HouseLeftTile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
            prevRect_ = rect_;
            rect_.size.x *= 3;
            rect_.size.y *= 3;
            rect_.pos.x -= prevRect_.size.x * 2;
            rect_.pos.y -= prevRect_.size.y * 2;
        }

        ~HouseLeftTile() = default;

        CollisionType getType() override { return SOLID; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            FRectType temp = prevRect_;
            temp.pos += offset;
            sprite_->RenderCopy(GROUND, temp);
            sprite_->RenderCopy(HOUSE_L, rect);
        }

    private:
        FRectType prevRect_;
        const FRectType HOUSE_L{{16, 16 * 7}, {48, 48}};
        const FRectType GROUND{{0, 0}, {32, 32}};
    };

    class HouseRightTile final : public ITile
    {
    public:
        HouseRightTile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
            prevRect_ = rect_;
            rect_.size.x *= 3;
            rect_.size.y *= 3;

            rect_.pos.x -= prevRect_.size.x * 2;
            rect_.pos.y -= prevRect_.size.y * 2;
        }

        ~HouseRightTile() = default;

        CollisionType getType() override { return SOLID; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            FRectType temp = prevRect_;
            temp.pos += offset;

            sprite_->RenderCopy(GROUND, temp);
            sprite_->RenderCopy(HOUSE_R, rect);
        }

    private:
        FRectType prevRect_;
        const FRectType GROUND{{0, 0}, {32, 32}};
        const FRectType HOUSE_R{{16 * 4, 112}, {48, 48}};
    };

    class GroundTile final : public ITile
    {
    public:
        GroundTile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
        }

        ~GroundTile() = default;

        CollisionType getType() override { return NONE; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            sprite_->RenderCopy(GROUND, rect);
        }

    private:
        const FRectType GROUND{{0, 0}, {32, 32}};
    };

    class RoadITile final : public ITile
    {
    public:
        RoadITile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
        }

        ~RoadITile() = default;

        CollisionType getType() override { return NONE; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            sprite_->RenderCopy(GROUND, rect);
            sprite_->RenderCopy(ROAD_V, rect);
        }

    private:
        const FRectType ROAD_V{{224, 32}, {32, 32}};
        const FRectType GROUND{{0, 0}, {32, 32}};
    };

    class Road_Tile final : public ITile
    {
    public:
        Road_Tile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
        }

        ~Road_Tile() = default;

        CollisionType getType() override { return NONE; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            sprite_->RenderCopy(GROUND, rect);
            sprite_->RenderCopy(ROAD_H, rect);
        }

    private:
        const FRectType GROUND{{0, 0}, {32, 32}};
        const FRectType ROAD_H{{224, 0}, {32, 32}};
    };

    class RoadrTile final : public ITile
    {
    public:
        RoadrTile(FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
            : ITile(rect, path, rgb)
        {
        }

        ~RoadrTile() = default;
        CollisionType getType() override { return NONE; }
        void show(const FVectType &offset, const Duration &) noexcept override
        {
            auto rect = GetOffSettedRect(offset);
            sprite_->RenderCopy(GROUND, rect);
            sprite_->RenderCopy(ROAD_r, rect);
        }

    private:
        const FRectType GROUND{{0, 0}, {32, 32}};
        const FRectType ROAD_r{{160, 0}, {32, 32}};
    };

    ////////////////////////factory////////////////////////

    const bool CarTileReg = TILE_FACTORY.Register('1',
                                                  [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                  {
                                                      return std::make_unique<CarTile>(rect, path, rgb);
                                                  });

    const bool TreeTileReg = TILE_FACTORY.Register('#',
                                                   [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                   {
                                                       return std::make_unique<TreeTile>(rect, path, rgb);
                                                   });

    const bool GWallTileReg = TILE_FACTORY.Register('w',
                                                    [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                    {
                                                        return std::make_unique<GWallTile>(rect, path, rgb);
                                                    });

    const bool TankTileReg = TILE_FACTORY.Register('b',
                                                   [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                   {
                                                       return std::make_unique<TankTile>(rect, path, rgb);
                                                   });

    const bool MonumentTileReg = TILE_FACTORY.Register('+',
                                                       [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                       {
                                                           return std::make_unique<MonumentTile>(rect, path, rgb);
                                                       });

    const bool GroundTileReg = TILE_FACTORY.Register('.',
                                                     [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                     {
                                                         return std::make_unique<GroundTile>(rect, path, rgb);
                                                     });

    const bool Road_TileReg = TILE_FACTORY.Register('-',
                                                    [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                    {
                                                        return std::make_unique<Road_Tile>(rect, path, rgb);
                                                    });

    const bool RoadITileReg = TILE_FACTORY.Register('|',
                                                    [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                    {
                                                        return std::make_unique<RoadITile>(rect, path, rgb);
                                                    });

    const bool RoadrTileReg = TILE_FACTORY.Register('r',
                                                    [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                    {
                                                        return std::make_unique<RoadrTile>(rect, path, rgb);
                                                    });

    const bool NoneTileReg = TILE_FACTORY.Register('n',
                                                   [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                   {
                                                       return std::make_unique<NoneTile>(rect, path, rgb);
                                                   });

    const bool HouseLReg = TILE_FACTORY.Register('L',
                                                 [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                 {
                                                     return std::make_unique<HouseLeftTile>(rect, path, rgb);
                                                 });

    const bool HouseRReg = TILE_FACTORY.Register('J',
                                                 [](FRectType rect, const std::filesystem::path &path, const sg::RgbType &rgb = {})
                                                 {
                                                     return std::make_unique<HouseRightTile>(rect, path, rgb);
                                                 });
}
