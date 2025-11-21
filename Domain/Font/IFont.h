#pragma once;
#include "../IPosition.h";

namespace sxEditCore::Font
{
    template <typename T>
    class IFont
    {
    public:
        virtual sxEditCore::IPosition *getFontOverallSize() const = 0;
        virtual T *getSystemFontObject() = 0;
        virtual ~IFont() = default;
    };
}