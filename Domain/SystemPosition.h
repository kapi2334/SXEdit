#pragma once;
#include "IPosition.h";
#include "SxException.h";
#include "Grid/IGrid.h";
#include "Grid/GridPosition.h";
#include <string>
namespace sxEditCore
{
    class SystemPosition : public IPosition
    {
        int x = 0;
        int y = 0;

    public:
        SystemPosition(int x, int y)
        {
            this->update(x, y);
        }
        SystemPosition()
        {
            this->x = 0;
            this->y = 0;
        }
        /*
         * Updates the position to the given (x, y) coordinates.
         * Returns true if the update succeeded, false otherwise.
         */
        bool update(int x, int y)
        {
            if (x >= 0 && y >= 0)
            {
                this->x = x;
                this->y = y;
                return true;
            }
            else
            {
                std::string errMsg = "Given coordinates to the SystemPosition are invalid: x: " + std::to_string(x) + "y: " + std::to_string(y) + ".";
                throw new sxException(errMsg, ErrorLevel::Warning, "OUTOFBOUNDS01");
            }
        }
        virtual int getX() const
        {
            return x;
        }

        virtual int getY() const
        {
            return y;
        }
        /*
         * Converts the current SystemPosition to a GridPosition representation.
         * Provided grid object handles the conversion.
         * Return the converted GridPosition.
         */
        sxEditCore::Grid::GridPosition toGridPosition(sxEditCore::Grid::IGrid &activeGridObject)
        {
            return activeGridObject.convertValueOnGrid(*this);
        }
        bool operator==(const SystemPosition &other) const
        {
            if (this->x == other.x && this->y == other.y)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    };
}