#pragma one
#include "IMemoryStructAdapter.h"
#include "DoublyLinkedList.h"
namespace sxEditCore::Memory{
    class DoublyLinkedListAdapter: public IMemoryStructAdapter{
        private:
            dlList* internalList; //Internal doubly linked list instance
        public:
            //Adds value (input) to the front of the List
            int pushFront(char input){
                return internalList->pushFront(input);
            }
            //Adds value (input) to the back of the list    
            int pushBack(char input) {
                return internalList->pushBack(input);
            }
            //Adds char(input) at specified position (index)
            int pushAtIndex(int index, char input) {
                return internalList->pushAtIndex(index, input);
            }
            //Deletes first node in the list
            int deleteFront() {
                return internalList->deleteNode(0);
            }
            //Deletes last node in the list
            int deleteBack() {
                return internalList->deleteNode(internalList->getSize() - 1);
            }
            //Deletes node with given index->
            char deleteAtIndex(int index) {
                return internalList->deleteNode(index);
            }
            //Returns value of node with specified index->
            int size() {
                return internalList->getSize();
            }
            //Defines operator[] for setting values at specified index->
            char& operator[](size_t index) {
                int tempFix = static_cast<int>(index);
                return internalList->getReference(tempFix);
            }
            //Defines operator[] for getting values at specified index->
            char& const operator[](size_t index) const {
                int tempFix = static_cast<int>(index);
                return internalList->getReference(tempFix);
            }

            DoublyLinkedListAdapter(){
                internalList = new dlList();
            }

    };
}