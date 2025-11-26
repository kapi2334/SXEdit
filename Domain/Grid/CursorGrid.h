#pragma once;
#include "IGrid.h";
#include "../Font/IFont.h";
#include "../Margin.h";
#include <any>
namespace sxEditCore::Grid
{
    class CursorGrid : public IGrid
    {
    private:
        /**
         * Represents the size of the grid in system coordinates - shoud be updated when window changes size.
         */
        SystemPosition gridSize = SystemPosition(0, 0);
        /**
         * Pointer to the font object. It's size is uised to calculate cell size.
         */
        sxEditCore::Font::IFont<std::any> *fontReferenceObject = nullptr;
        /**
         * Represents size of a single letter in the grid (oncluding margins).
         */
        GridPosition singleCellSize = GridPosition(0, 0);
        /**
         *  Margin between letters in the grid cells.
         */
        sxEditCore::Margin marginBetweenLetters = sxEditCore::Margin(2, 2, 2, 2);
        /**
         *  Margin between text area and border of the grid.
         */
        sxEditCore::Margin marginBetweenTextAreaAndBorder = sxEditCore::Margin(4, 4, 4, 4);

    public:
        /*
         * Converts the given GridPosition to a SystemPosition representation.
         * Returns the converted SystemPosition.
         */
        sxEditCore::SystemPosition getRealObjectValue(const GridPosition &gridPos)
        {
        }
        /*
         * Converts system coordinates (sysX, sysY) to grid coordinates.
         * Returns a GridPosition object representing the grid coordinates.
         */
        virtual sxEditCore::Grid::GridPosition convertValueOnGrid(sxEditCore::IPosition &initialPosition)
        {
        }
        /*
         * Updates the grid size to the given newSize (in system coordinates).
         * Returns the updated size as a SystemPosition object.
         */
        virtual sxEditCore::SystemPosition updateGridSize(sxEditCore::SystemPosition &newSize)
        {
            gridSize = newSize;
            return gridSize;
        }
        GridPosition *getSingleCellSize()
        {
            int cellWidth = fontReferenceObject->getFontOverallSize()->getX() + marginBetweenLetters.left;
            int cellHeight = fontReferenceObject->getFontOverallSize()->getY() + marginBetweenLetters.top;
            return &GridPosition(cellWidth, cellHeight);
        }
        /*
        Simulation of a cursor grid with margins.
        +--------------------+
        |11111111111111111111|
        |1+--------------+1  |
        |1|    2         |1  |
        |1|2  [L] [L]    |1  |
        |1|2             |1  |
        |1+--------------+1  |
        |11111111111111111111|
        +--------------------+
                    1 - Margin between text area and border
                    2 - Margin between letters
                    L - Lingle letter
                */
        CursorGrid(sxEditCore::Font::IFont<std::any> *fontObject = nullptr, SystemPosition initialGridSize = SystemPosition(0, 0), sxEditCore::Margin &marginBetweenLetters, sxEditCore::Margin &marginBetweenTextAreaAndBorder) : gridSize(initialGridSize)
        {
            if (fontObject == nullptr)
            {
                throw new sxException("Given font object for CursorGrid is null.", ErrorLevel::Critical, "PTR01");
            }
            this->fontReferenceObject = fontObject;
            this->gridSize = initialGridSize;
        }
    };
}