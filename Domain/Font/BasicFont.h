#pragma once;

namespace sxEditCore::Font
{
    template <typename T>
    class BasicFont : public sxEditCore::Font::IFont<T>
    {
    private:
        unsigned int _fontSize = 12;
        T *_systemFontObject = nullptr;

    public:
        unsigned int getFontOverallSize() const
        {
            return this->_fontSize;
        }
        T *getSystemFontObject()
        {
            return this->_systemFontObject;
        }
        BasicFont(unsigned int size, T *systemFontObject)
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