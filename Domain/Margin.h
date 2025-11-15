#pragma once;
#include "PropertyInerfaces/IScalable.h";
#include "PropertyInerfaces/IResizeable.h";
#include "SxException.h";
namespace sxEditCore
{
    class Margin : public IScalable, public IResizeable
    {
    private:
        bool arePropertiesValid(int left, int right, int top, int bottom)
        {
            if (left >= 0 && right >= 0 && top >= 0 && bottom >= 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    public:
        int left = 0;
        int right = 0;
        int top = 0;
        int bottom = 0;

        Margin(int left, int right, int top, int bottom)
        {
            if (!arePropertiesValid(left, right, top, bottom))
            {
                left = 0;
                right = 0;
                top = 0;
                bottom = 0;
                this->left = left;
                this->right = right;
                this->top = top;
                this->bottom = bottom;
                throw new sxException("Given margin properties are invalid. All margin properties must be non-negative integers. Margin properties have been set to 0.", ErrorLevel::Warning, "OUTOFBOUNDS02");
            }
            this->left = left;
            this->right = right;
            this->top = top;
            this->bottom = bottom;
        }

        Margin()
        {
            this->left = 0;
            this->right = 0;
            this->top = 0;
            this->bottom = 0;
        }
        /*
         * Scales the current object by the given scaleFactor.
         * Returns true if the scaling succeeded, false otherwise.
         */
        bool scale(float scaleFactor)
        {
            int newLeft = (int)(this->left * scaleFactor);
            int newRight = (int)(this->right * scaleFactor);
            int newTop = (int)(this->top * scaleFactor);
            int newBottom = (int)(this->bottom * scaleFactor);
            if (arePropertiesValid(newLeft, newRight, newTop, newBottom))
            {
                this->left = newLeft;
                this->right = newRight;
                this->top = newTop;
                this->bottom = newBottom;
                return true;
            }
            else
            {
                return false;
            }
        }
        /*
         * Resizes the current object by adding to the existing dimentions the given newWidth and newHeight.
         * Returns true if the resizing succeeded, false otherwise.
         */
        bool resize(int addedWidth, int addedHeight)
        {
            int newLeft = this->left + addedWidth;
            int newRight = this->right + addedWidth;
            int newTop = this->top + addedHeight;
            int newBottom = this->bottom + addedHeight;
            if (arePropertiesValid(newLeft, newRight, newTop, newBottom))
            {
                this->left = newLeft;
                this->right = newRight;
                this->top = newTop;
                this->bottom = newBottom;
                return true;
            }
            else
            {
                return false;
            }
        }
    };
}