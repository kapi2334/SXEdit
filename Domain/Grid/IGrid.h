#pragma once;
#include "../IPosition.h";
#include "../SystemPosition.h";
#include "GridPosition.h";
namespace sxEditCore::Grid
{
    class IGrid
    {
    public:
        /*
         * Converts system coordinates (sysX, sysY) to grid coordinates.
         * Returns a GridPosition object representing the grid coordinates.
         */
        virtual sxEditCore::Grid::GridPosition convertValueOnGrid(sxEditCore::IPosition &initialPosition) = 0;

        /*
         * Converts grid coordinates (gridX, gridY) to system coordinates.
         * Returns a SystemPosition object representing the system coordinates.
         */
        virtual sxEditCore::SystemPosition getRealObjectValue(sxEditCore::Grid::GridPosition &positionOnGrid) = 0;

        /*
         * Updates the grid size to the given newSize (in system coordinates).
         * Returns the updated size as a SystemPosition object.
         */
        virtual sxEditCore::SystemPosition updateGridSize(sxEditCore::SystemPosition &newSize) = 0;

        /**
         * @brief  Gets the size of a single cell in the grid.
         * @return GridPosition*
         */
        virtual GridPosition *getSingleCellSize() = 0;
        virtual ~IGrid() = default;
    };
}