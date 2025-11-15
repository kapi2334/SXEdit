
#pragma once

namespace sxEditCore
{
    class IResizeable
    {
    public:
        /*
         * Resizes the current object by adding to the existing dimentions the given newWidth and newHeight.
         * Returns true if the resizing succeeded, false otherwise.
         */
        virtual bool resize(int addedWidth, int addedHeight) = 0;

        virtual ~IResizeable() = default;
    };
}