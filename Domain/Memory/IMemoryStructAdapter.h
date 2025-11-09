#pragma once

namespace sxEditCore::Memory{
    class IMemoryStructAdapter{
        public:
            //Adds value (input) to the front of the List
            virtual int pushFront(char input) = 0;
            //Adds value (input) to the back of the list    
            virtual int pushBack(char input) = 0;
            //Adds char(input) at specified position (index)
            virtual int pushAtIndex(int index, char input) = 0;
            //Deletes first node in the list
            virtual int deleteFront() = 0;
            //Deletes last node in the list
            virtual int deleteBack() = 0;
            //Deletes node with given index.
            virtual char deleteAtIndex(int index) = 0;
            //Returns value of node with specified index.
            virtual int size() = 0;
            //Defines operator[] for setting values at specified index.
            virtual char& operator[](size_t index) = 0;
            //Defines operator[] for getting values at specified index.
            virtual char& const operator[](size_t index) const = 0;
            //Default virtual destructor
            virtual ~IMemoryStructAdapter()=default;
    };
}