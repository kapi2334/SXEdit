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
        IPosition _fontSize = new IPosition(12, 12);
        T *_systemFontObject = nullptr;

    public:
        IPosition *getFontSize() const
        {
            return &this->_fontSize;
        }
        T *getSystemFontObject()
        {
            return this->_systemFontObject;
        }
        BasicFont(IPosition &fontSize, T *systemFontObject)
        {
            this->_fontSize = fontSize;
            if (systemFontObject == nullptr)
            {
                systemFontObject = new T();
            }
            this->_systemFontObject = systemFontObject;
        }
        ~BasicFont()
        {
            if (this->_systemFontObject != nullptr)
            {
                delete this->_systemFontObject;
                this->_systemFontObject = nullptr;
            }
        }
        BasicFont(BasicFont &other)
        {
            this->_fontSize = other._fontSize;
            this->_systemFontObject = other._systemFontObject;
        }
        BasicFont &operator=(const BasicFont &other)
        {
            if (this != &other)
            {
                this->_fontSize = other._fontSize;
                this->_systemFontObject = other._systemFontObject;
                delete this->_systemFontObject;
                this->_systemFontObject = nullptr;
            }
            return *this;
        }
        BasicFont(BasicFont &&other) noexcept
        {
            this->_fontSize = other._fontSize;
            this->_systemFontObject = other._systemFontObject;
            delete this->_systemFontObject;
            this->_systemFontObject = nullptr;
            this->fontSize = 0;
        }
        BasicFont &operator=(BasicFont &&other) noexcept
        {
            if (this != &other)
            {
                this->_fontSize = other._fontSize;
                this->_systemFontObject = other._systemFontObject;
                delete this->_systemFontObject;
                this->_systemFontObject = nullptr;
            }
            return *this;
        }
    };

}