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
        int _x = 0;
        int _y = 0;

    public:
        SystemPosition(int _x, int _y)
        {
            this->update(_x, _y);
        }
        SystemPosition()
        {
            this->_x = 0;
            this->_y = 0;
        }
        /*
         * Updates the position to the given (_x, _y) coordinates.
         * Returns true if the update succeeded, false otherwise.
         */
        bool update(int _x, int _y)
        {
            if (_x >= 0 && _y >= 0)
            {
                this->_x = _x;
                this->_y = _y;
                return true;
            }
            else
            {
                std::string errMsg = "Given coordinates to the SystemPosition are invalid: _x: " + std::to_string(_x) + "_y: " + std::to_string(_y) + ".";
                throw new sxException(errMsg, ErrorLevel::Warning, "OUTOFBOUNDS01");
            }
        }
        virtual int getX() const
        {
            return _x;
        }

        virtual int getY() const
        {
            return _y;
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
            if (this->_x == other._x && this->_y == other._y)
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