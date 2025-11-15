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
        SystemPosition gridSize = SystemPosition(0, 0);
        sxEditCore::Font::IFont<std::any> *_fontReferenceObject = nullptr;

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
            this->_fontReferenceObject = fontObject;
        }
    };
}