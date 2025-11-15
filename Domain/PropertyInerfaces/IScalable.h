#pragma once;
namespace sxEditCore
{
    class IScalable
    {
    public:
        /*
         * Scales the current object by the given scaleFactor.
         * Returns true if the scaling succeeded, false otherwise.
         */
        virtual bool scale(float scaleFactor) = 0;

        virtual ~IScalable() = default;
    };
}