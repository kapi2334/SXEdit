#pragma once;
#include "../IPosition.h";
#include "../SystemPosition.h"
#include "../SxException.h";
#include <string>
namespace sxEditCore::Grid{
    class GridPosition: public sxEditCore::IPosition{
        public:
            int x = 0;
            int y = 0;
            
            GridPosition(int x, int y){
                update(x,y);
            }

            GridPosition(){
                this->x = 0;
                this->y = 0;
            }
            
            /*
            * Updates the position to the given (x, y) coordinates.
            * Returns true if the update succeeded, false otherwise.
            */
           bool update(int newX, int newY) {
               if(x >= 0 && y >= 0){
                   this->x = x;
                   this->y = y;
                   return true;
                } else {
                    std::string errMsg = "Given coordinates to the SystemPosition are invalid: x: " + std::to_string(x) + "y: " + std::to_string(y) + ".";
                    throw new sxException(errMsg, ErrorLevel::Warning, "OUTOFBOUNDS01");
                }
            }
            /*
            * Converts the current GridPosition to a SystemPosition representation.
            * Provided grid object handles the conversion.
            * Return the converted SystemPosition. 
            */
            sxEditCore::SystemPosition toSystemPosition(sxEditCore::Grid::GridPosition& activeGridObject){
                return activeGridObject.toSystemPosition(*this); 
            }

            bool operator==(const GridPosition& other) const {
                if(this->x == other.x && this->y == other.y){
                    return true;
                }else{
                    return false;
                }
            }
    };
}