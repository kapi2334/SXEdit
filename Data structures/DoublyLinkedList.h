#pragma once
#include <cstdlib>
#include <iostream>
namespace sxEditCore::dataStructures{


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
            int cacheTreshold = 20;
            dlCachedNode cachedNode; 
            
            dlNode *First; //Pointer to first node (head)
            dlNode *Last; //Pointer to last node (tail) 

            int size; //Acual size of this list


            /*It is used to obtain a node at a specific position - using the node stored in memory.
             *Returns nullptr when cashing attempt failed - fe. if CahedNode is empty, 
             or if targetted node and cahed one are too away.
             * */
            dlNode* tryToCache(int nodeIndex){
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
            dlNode* getNode(int index){
                //Index validity checks.
                if(index < 0 ) return nullptr;
                if(index >= size) return nullptr;
                //Cases when index is first or last
                if(index == 0) return First;
                if(index == (size - 1)) return Last;
                //Reading from cached node
                dlNode* outNode = tryToCache(index);
                if(outNode != nullptr){
                //Success
                    //std::cout << "Node from cashe returned: " << outNode->value<<"\n";
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
                      //  std::cout << "Node from front side returned: " << outNode->value<<"\n";
                        return outNode;
                        
                    
                    }
                    else{
                    //Targeted node is closer to the end of the list
                        outNode = Last;
                        for(int i = size - 1; i > index; i--){
                            outNode = outNode->prev;
                        }
                        //std::cout << "Node from back side returned: " << outNode->value<<"\n";
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
            char deleteNode(int index){
                //Obtaining node to delete
                dlNode* node = getNode(index);
                //Prev node -> next = next node, next node -> prev = prev node
                node->prev->next = node-> next;
                node->next->prev = node->prev;
                char delval = node->value;
                delete node;
                return delval;
            }
            //Return value of node with specified index.
            char get(int index){ 
                dlNode* out = getNode(index);
                if(out == nullptr) return errorChar;
                return out->value;
            }
            int getSize(){
                return size;
            }

    }; 

}
