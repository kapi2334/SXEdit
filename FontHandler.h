#include <Windows.h>
#include <winnt.h>

namespace sxEditCore {
    class FontHandler {
    private:
        HFONT font;
        LOGFONTA logFont; // trzymamy dane czcionki, by móc ją modyfikować

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
        FontHandler(LPCSTR fontName, int size,
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
        }

        ~FontHandler() {
            if (font) {
                DeleteObject(font);
            }
        }

        HFONT getFont() const {
            return font;
        }
        
        //Sets up new font size
        void updateFontSize(int size){
            logFont.lfHeight = size;
            logFont.lfWidth = size;

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

    };
}

