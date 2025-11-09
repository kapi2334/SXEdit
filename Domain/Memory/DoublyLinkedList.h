#pragma once
#include <cstdlib>
#include <iostream>
namespace sxEditCore::Memory{


    class dlList{
        private:
            //Single node declaration
            struct dlNode{
                char value;
                struct dlNode* next;
                struct dlNode* prev;
            } ; 

            //Cahed node declaration
            struct dlCachedNode{
                int index = -1;
                dlNode *nodeAddress= nullptr;
            };  

            //TO DO: CALCULATING THE THERSHOLD VALUE DEPENDING ON THE SIZE VARIABLE
            const int cacheTreshold = 20;
            mutable dlCachedNode cachedNode; 

            dlNode *First; //Pointer to first node (head)
            dlNode *Last; //Pointer to last node (tail) 

            int size; //Acual size of this list


            /*It is used to obtain a node at a specific position - using the node stored in memory.
             *Returns nullptr when cashing attempt failed - fe. if CahedNode is empty, 
             or if targetted node and cahed one are too away.
             * */
            dlNode* tryToCache(int nodeIndex) const{
                if(cachedNode.nodeAddress == nullptr ) return nullptr; //If cahed node doesnt exist
                if(nodeIndex >= size || nodeIndex < 0) return nullptr; //If index is outside the array
                if(std::abs(nodeIndex - cachedNode.index) < cacheTreshold){ //Success - finding node using one in cashe can be done.
                                                                            //Cashe types
                    if(nodeIndex - cachedNode.index > 0 ){
                        //Iterating to node from cached one using next property.

                        while(cachedNode.index < nodeIndex){
                            cachedNode.index++;
                            cachedNode.nodeAddress = cachedNode.nodeAddress->next;
                        }
                        return cachedNode.nodeAddress;
                    }
                    else if(nodeIndex - cachedNode.index == 0 ){
                        //Requested node its that one in the cache
                        return cachedNode.nodeAddress;
                    }
                    else if(nodeIndex - cachedNode.index < 0 ){
                        //Iterating to node from cahed one using prev property.

                        while(cachedNode.index > nodeIndex){

                            cachedNode.index--;
                            cachedNode.nodeAddress = cachedNode.nodeAddress->prev;
                        }
                        return cachedNode.nodeAddress;
                    }

                }
                return nullptr;        


            }
            /*Returns address of node which is located at given index.
             *Function is optimized to begin node finding operation from most efficient node. 
             */
            dlNode* getNode(int index) const{
                //Index validity checks.
                if(size == 0 || (First == nullptr && Last == nullptr)) return nullptr;
                if(index < 0 ) return nullptr;
                if(index >= size) return nullptr;
                //Cases when index is first or last
                if(index == 0) return First;
                if(index == (size - 1)) return Last;
                //Reading from cached node
                dlNode* outNode = tryToCache(index);
                if(outNode != nullptr){
                    //Success
                    return outNode;
                }
                else{
                    //Failed
                    if(index <= size/2){
                        //Targeted node is closer to the front of the list 
                        outNode = First;
                        for(int i = 0; i < index; i++){
                            outNode = outNode->next;
                        }
                        return outNode;


                    }
                    else{
                        //Targeted node is closer to the end of the list
                        outNode = Last;
                        for(int i = size - 1; i > index; i--){
                            outNode = outNode->prev;
                        }
                        return outNode;
                    }
                }


            }

        public:
            //Char that is returned(displayed) when error occured in node (fe nullptr was returned).
            char errorChar = '#';
            dlList(){
                size = 0; 
                First = nullptr;
                Last = nullptr;
            }
            
            //Adds value (input) to the front of the List
            int pushFront(char input){
                dlNode *newNode = new dlNode;
                newNode->value = input; 
                newNode->prev = nullptr;
                newNode->next = First;
                if(Last == nullptr) Last = newNode;
                if(First != nullptr){
                    First->prev = newNode;
                }
                First = newNode;
                size++;
                //Updating cache
                cachedNode.nodeAddress = newNode;
                cachedNode.index = 0;
                return 0;
            }
            //Adds value (input) to the back of the list
            int pushBack(char input){
                dlNode *newNode = new dlNode;
                newNode->value = input;
                newNode->next = nullptr;
                
                if(First == nullptr){
                    newNode->prev = nullptr;
                    First = newNode;
                    Last = newNode;
                    size++;
                    //Updating cache
                    cachedNode.nodeAddress = newNode;
                    cachedNode.index = 0;
                    return 0;
                }
                
                newNode -> prev = Last;
                Last -> next = newNode;
                Last = newNode;
                size++;
                //Updating cache
                cachedNode.nodeAddress = newNode;
                cachedNode.index = size - 1;
                return size - 1;
            }
            int pushAtIndex(int index, char input){
                //Adds char(input) at specified position (index)
                if(index > this->getSize()|| index < 0){
                    std::cout<<"Error occured.\n";
                    return -1;
                }
                if(index == 0){ //First element
                    this->pushFront(input);
                    return index;
                }else if(index == this->getSize()){ //Last element
                    this->pushBack(input);
                    return index;
                }
                //In the middle
                dlNode* newNode = new dlNode();
                size++;
                newNode->value = input;
                dlNode* oldNode = this->getNode(index);
                /*
                newNode->next = oldNode->next;
                newNode->prev = oldNode;
                oldNode->next->prev = newNode;
                oldNode->next = newNode;
                */
               newNode-> next = oldNode;
               newNode-> prev = oldNode -> prev;
               oldNode->prev -> next = newNode;
               oldNode->prev = newNode;
               
               std::cout<<"Pushed in the middle.\n";
               //Updating cache
               cachedNode.nodeAddress = newNode;
               cachedNode.index = index;
               return index;
            }
            //Deletes node with given index.
            char deleteNode(int index){
                //Obtaining node to delete
                dlNode* node = getNode(index);
                if(node == nullptr) return errorChar;
                char delval = node->value;
                if(size == 1){
                    delete node;
                    First = nullptr;
                    Last = nullptr;
                    //Clearing cache
                    cachedNode.nodeAddress = nullptr; 
                    cachedNode.index = -1;
                    size = 0;
                    return delval;
                } 
                size--;
                //Prev node -> next = next node, next node -> prev = prev node
                if(node -> prev == nullptr){ //First in list
                    First = node->next;
                    node->next->prev = nullptr;
                    //Updating cache
                    cachedNode.nodeAddress = node->next;
                    cachedNode.index = 0;
                }else if(node -> next == nullptr){ //Last in list
                    node->prev->next = nullptr;
                    Last = node->prev;
                    //Updating cache
                    cachedNode.nodeAddress = node->prev;
                    cachedNode.index = size-1;
                }else{ //In the middle of a list
                    node->prev->next = node-> next;
                    node->next->prev = node->prev;
                    //Updating cache
                    cachedNode.nodeAddress = node->prev;
                    cachedNode.index = index - 1;
                }
                delete node;
                return delval;
            }
            //Return value of node with specified index.
            char get(int index) const{ 
                dlNode* out = getNode(index);
                if(out == nullptr) return errorChar;
                return out->value;
            }
            char& getReference(int index) { 
                dlNode* out = getNode(index);
                if(out == nullptr) return errorChar;
                return out->value;
            }
            const char& getReference(int index) const{ 
                dlNode* out = getNode(index);
                if(out == nullptr) return errorChar;
                return out->value;
            }
            //Clears whole contents of this list 
            void clear(){
                if(size != 0 && Last != nullptr){
                    //Deleting objects 
                    while(Last != nullptr){
                        dlNode* node = Last;
                        Last = node->prev;
                        delete node;
                        node = nullptr;
                    }
                    size = 0;
                    Last = nullptr;
                    First = nullptr;
                    //Clearing cache
                    cachedNode.nodeAddress = nullptr;
                    cachedNode.index = - 1;
                }
            }
            int getSize() const{
                return size;
            }
            ~dlList(){
                this->clear();
            }
            
            // Konstruktor kopiujący
            dlList(const dlList& other) : dlList() {
                dlNode* curr = other.First;
                while (curr != nullptr) {
                    pushBack(curr->value);
                    curr = curr->next;
                }
            }
            
            // Operator przypisania kopiującego
            dlList& operator=(const dlList& other) {
                if (this == &other) return *this;
                clear();
                dlNode* curr = other.First;
                while (curr != nullptr) {
                    pushBack(curr->value);
                    curr = curr->next;
                }
                return *this;
            }

            // Konstruktor przenoszący
            dlList(dlList&& other) noexcept 
                : First(other.First), Last(other.Last), size(other.size), cachedNode(other.cachedNode) {
                    other.First = nullptr;
                    other.Last = nullptr;
                    other.size = 0;
                    other.cachedNode.nodeAddress = nullptr;
                    other.cachedNode.index = -1;
                }

            // Operator przypisania przenoszącego
            dlList& operator=(dlList&& other) noexcept {
                if (this == &other) return *this;
                clear();
                First = other.First;
                Last = other.Last;
                size = other.size;
                cachedNode = other.cachedNode;
                other.First = nullptr;
                other.Last = nullptr;
                other.size = 0;
                other.cachedNode.nodeAddress = nullptr;
                other.cachedNode.index = -1;
                return *this;
            }

    }; 

}
