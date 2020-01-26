#include "stella/core/tile.h"
#include "stella/core/grid.h"

#include "../catch.hpp"

namespace {
    using Grid = stella::core::Grid<stella::core::Tile>;
    using Tile = stella::core::Tile;

    auto tile = Tile();
    auto tile2 = Tile(32);
    auto tile3 = Tile(1);
    auto tile4 = Tile(-4);
    auto tile5 = Tile(0);
    auto grid = Grid(30, 35);
}

TEST_CASE("Operations on the Tile class")
{
    SECTION("Tile is initialized properly")
    {
        CHECK(tile.value == 0);
        CHECK(tile2.value == 32);
        CHECK(tile3.value == 1);
        CHECK(tile4.value == -4);
        CHECK(tile5.value == 0);

        CHECK(tile.collidable == false);
        CHECK(tile2.collidable == false);
        CHECK(tile.visible == false);
        CHECK(tile2.visible == false);
        CHECK(tile.x == 0);
        CHECK(tile2.x == 0);
        CHECK(tile.y == 0);
        CHECK(tile2.y == 0);
        CHECK(tile.z == 0);
        CHECK(tile2.z == 0);
    }

    SECTION("Tile is modified and reseted properly")
    {
        tile.collidable = true;
        tile.visible = true;
        tile.value = 5;
        tile.x = 6;
        tile.y = 4;
        tile.z = 10;
        tile.active_edges.set(0);
        tile.custom_edges.set(1);
        tile.solid_edges.set(2);

        CHECK(tile.collidable == true);
        CHECK(tile.visible == true);
        CHECK(tile.value == 5);
        CHECK(tile.x == 6);
        CHECK(tile.y == 4);
        CHECK(tile.z == 10);
        CHECK(tile.active_edges.test(0) == 1);
        CHECK(tile.custom_edges.test(1) == 1);
        CHECK(tile.solid_edges.test(2) == 1);

        tile.reset();

        CHECK(tile.collidable == false);
        CHECK(tile.visible == true);
        CHECK(tile.value == 0);
        CHECK(tile.x == 6);
        CHECK(tile.y == 4);
        CHECK(tile.z == 10);
        CHECK(tile.active_edges.test(0) == 0);
        CHECK(tile.custom_edges.test(1) == 0);
        CHECK(tile.solid_edges.test(2) == 0);
    }
    
}
TEST_CASE("Operations on the Grid class")
{
    SECTION("Set and get values on the Grid")
    {
        grid.set_value(4, 3, tile);
        grid.set_value(10, 18, tile2);
        grid.set_value(0, 0, tile3);
        grid.set_value(29, 29, tile4);

        CHECK(grid.get_value(4,3) == tile);
        CHECK(grid.get_value(10,18) == tile2);
        CHECK(grid.get_value(0,0) == tile3);
        CHECK(grid.get_value(29,29) == tile4);
    }

    SECTION("Resize Grid")
    {
        CHECK(grid.width() == 30);
        CHECK(grid.height() == 35);

        grid.resize(0, 2, 3, 0);
        
        CHECK(grid.width() == 32);
        CHECK(grid.height() == 38);
        CHECK(grid.get_value(31, 15).value == 0);
        CHECK(grid.get_value(30, 14).value == 0);
        CHECK(grid.get_value(10, 34).value == 0);
        CHECK(grid.get_value(11, 34).value == 0);

        grid.resize(-5, 0, 0, -4);

        CHECK(grid.width() == 28);
        CHECK(grid.height() == 33);
        CHECK(grid.get_value(20, 32).value == 0);
        CHECK(grid.get_value(19, 31).value == 0);
        CHECK(grid.get_value(27, 13).value == 0);
        CHECK(grid.get_value(26, 22).value == 0);


        // Trying to resize to opposite sides
        // CHECK(grid.resize(5, 3, 3, 34));

        // CHECK(grid.width() == 28);
        // CHECK(grid.height() == 33);
        // CHECK(grid.get_value(20, 32).value == 0);
        // CHECK(grid.get_value(19, 31).value == 0);
        // CHECK(grid.get_value(27, 13).value == 0);
        // CHECK(grid.get_value(26, 22).value == 0);
    }
}
