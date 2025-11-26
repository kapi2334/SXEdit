#pragma once
#include <memory>
#include "../IPosition.h"
#include "../SystemPosition.h"

namespace sxEditCore::Font
{
    template <typename T>
    class BasicFont : public sxEditCore::Font::IFont
    {
    private:
        /**
         * Represents overall font size (width,height) (e.g., for monospaced fonts, width and height are equal).
         */
        std::unique_ptr<IPosition> fontSize = std::make_unique<sxEditCore::SystemPosition>(0.0f, 0.0f);
        T *systemFontObject = nullptr;

    public:
        IPosition *getFontSize() const
        {
            return &this->fontSize;
        }
        T *getSystemFontObject()
        {
            return this->systemFontObject;
        }
        BasicFont(std::unique_ptr<IPosition> &fontSize, T *systemFontObject)
        {
            this->fontSize = fontSize;
            if (systemFontObject == nullptr)
            {
                systemFontObject = new T();
            }
            this->systemFontObject = systemFontObject;
        }
    };

}