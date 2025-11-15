#pragma once;

namespace sxEditCore::Font{
    template <typename T>
    class IFont{
        public:
            virtual int getFontOverallSize() const = 0;
            virtual T* getSystemFontObject() = 0;
            virtual ~IFont() = default;
    };
}