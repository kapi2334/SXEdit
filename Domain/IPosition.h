#pragma once;
namespace sxEditCore
{
    /*
     * Interface representing a position in a fixed reference frame
     * (e.g., screen coordinates, grid coordinates).
     */
    class IPosition
    {
    public:
        /*
         * Updates the position to the given (x, y) coordinates.
         * Returns true if the update succeeded, false otherwise.
         */
        virtual bool update(float x, float y) = 0;
        virtual float getX() const = 0;
        virtual float getY() const = 0;
    };
}