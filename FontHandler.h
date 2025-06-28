#pragma once

#include <Windows.h>

namespace sxEditCore {
    class FontHandler {
    private:
        HFONT font;
        LOGFONTA logFont; // Keeping up font info to provide base for changes 
        
        COLORREF _fontColor;

        void recreateFont() {
            if (font) {
                DeleteObject(font);
            }
            font = CreateFontA(
                logFont.lfHeight,
                logFont.lfWidth,
                logFont.lfEscapement,
                logFont.lfOrientation,
                logFont.lfWeight,
                logFont.lfItalic,
                logFont.lfUnderline,
                logFont.lfStrikeOut,
                logFont.lfCharSet,
                logFont.lfOutPrecision,
                logFont.lfClipPrecision,
                logFont.lfQuality,
                logFont.lfPitchAndFamily,
                logFont.lfFaceName
            );
        }

    public:
        int maxFontSize = 120; //Sets the maximum settable font size;
        int _spaceBetweenChars = 15; //Put it as font variable
        FontHandler(LPCSTR fontName, int size, COLORREF textColor = RGB(255,255,255),
                    int weight = FW_NORMAL, bool italic = false, bool underline = false,
                    bool strikeout = false,
                    int slopeX = 0, int slopeY = 0) {

            ZeroMemory(&logFont, sizeof(LOGFONTA));
            logFont.lfHeight = size;
            logFont.lfWidth = size;
            logFont.lfEscapement = slopeX;
            logFont.lfOrientation = slopeY;
            logFont.lfWeight = weight;
            logFont.lfItalic = italic;
            logFont.lfUnderline = underline;
            logFont.lfStrikeOut = strikeout;
            logFont.lfCharSet = ANSI_CHARSET;
            logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
            logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
            logFont.lfQuality = DEFAULT_QUALITY;
            logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
            strncpy_s(logFont.lfFaceName, fontName, LF_FACESIZE - 1);

            recreateFont();
            _fontColor = textColor; 
        }

        ~FontHandler() {
            if (font) {
                DeleteObject(font);
            }
        }

        //Sets up new font size
        void updateFontSize(int size){
            if(size >= 1 && size <= maxFontSize){
                logFont.lfHeight = size;
                logFont.lfWidth = size;
            }

            recreateFont();
        }
        //Switches up (true -> false, false-> true) font's italic property.
        void switchItalic(bool isItalic){
            logFont.lfItalic = !logFont.lfItalic;
            recreateFont();
        }
        //Switches up (true -> false, false-> true) font's underline property.
        void switchUnderline(bool isUnderlined){
            logFont.lfUnderline= !logFont.lfUnderline;
            recreateFont();
        }
        //Switches up (true -> false, false-> true) font's strikeout property.
        void switchStrikeout(bool isStrikedout){
            logFont.lfStrikeOut= !logFont.lfStrikeOut;
            recreateFont();
        }
        void setColor(COLORREF newColor){
            _fontColor = newColor;
        }


        HFONT getFont() const {
            return font;
        }
        
        //Returns size of a font 
        int getSize(){
            return logFont.lfHeight;
        }
        //Reutrns color of a font
        COLORREF getTextColor(){
            return _fontColor;
        }

    };
}

