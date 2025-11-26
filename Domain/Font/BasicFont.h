#pragma once;
#include "../IPosition.h";

namespace sxEditCore::Font
{
    template <typename T>
    class BasicFont : public sxEditCore::Font::IFont<T>
    {
    private:
        /**
         * Represents overall font size (width,height) (e.g., for monospaced fonts, width and height are equal).
         */
        IPosition fontSize = new IPosition(12, 12);
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
        BasicFont(IPosition &fontSize, T *systemFontObject)
        {
            this->fontSize = fontSize;
            if (systemFontObject == nullptr)
            {
                systemFontObject = new T();
            }
            this->systemFontObject = systemFontObject;
        }
        ~BasicFont()
        {
            if (this->systemFontObject != nullptr)
            {
                delete this->systemFontObject;
                this->systemFontObject = nullptr;
            }
        }
        BasicFont(BasicFont &other)
        {
            this->fontSize = other.fontSize;
            this->systemFontObject = other.systemFontObject;
        }
        BasicFont &operator=(const BasicFont &other)
        {
            if (this != &other)
            {
                this->fontSize = other.fontSize;
                this->systemFontObject = other.systemFontObject;
                delete this->systemFontObject;
                this->systemFontObject = nullptr;
            }
            return *this;
        }
        BasicFont(BasicFont &&other) noexcept
        {
            this->fontSize = other.fontSize;
            this->systemFontObject = other.systemFontObject;
            delete this->systemFontObject;
            this->systemFontObject = nullptr;
            this->fontSize = 0;
        }
        BasicFont &operator=(BasicFont &&other) noexcept
        {
            if (this != &other)
            {
                this->fontSize = other.fontSize;
                this->systemFontObject = other.systemFontObject;
                delete this->systemFontObject;
                this->systemFontObject = nullptr;
            }
            return *this;
        }
    };

}