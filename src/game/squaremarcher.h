#ifndef SQUAREMARCHER_H
#define SQUAREMARCHER_H

#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

class SquareMarcher
{

public:

   SquareMarcher(
      uint32_t w,
      uint32_t h,
      const std::vector<int32_t>& tiles,
      const std::vector<int32_t>& collidingTiles
   );

   void printMap();
   void dumpMap();


   enum class Direction {
      None,
      Up,
      Down,
      Left,
      Right
   };

   enum class PixelLocation {
      None        = 0x00,
      TopLeft     = 0x01,
      TopRight    = 0x02,
      BottomLeft  = 0x04,
      BottomRight = 0x08
   };

   struct Path
   {
      std::vector<sf::Vector2i> mPolygon;
      std::vector<Direction> mDirs;

      void printPoly();
      void printDirs();
   };

   std::vector<Path> mPaths;


private:

   void scan();
   Path march(uint32_t startX, uint32_t startY);
   void updateDirection();
   void updatePosition();
   bool isColliding(uint32_t x, uint32_t y);
   bool isVisited(uint32_t x, uint32_t y);
   void serialize(const std::string& filename);
   void deserialize(const std::string& filename);
   void debugPaths();


private:

   std::map<int, Direction> mMap;
   uint32_t mWidth = 0u;
   uint32_t mHeight = 0u;
   std::vector<int32_t> mTiles;
   std::vector<int32_t> mCollidingTiles;
   std::vector<int32_t> mVisited;

   uint32_t mX = 0;
   uint32_t mY = 0;
   Direction mDirCurrent = Direction::None;
   Direction mDirPrevious = Direction::None;
};

#endif // SQUAREMARCHER_H
