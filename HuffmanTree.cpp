#include "HuffmanTree.hpp"
#include "HeapQueue.hpp"
#include "HuffmanBase.hpp"

#include <map>
#include <iostream>
#include <stack>


//This is a huffman node object that will be used to store the root once it is found for the tree
HuffmanNode *tree_root;


//This is the function that creates the map that is used to help encode the string
void preorder(HuffmanNode* x, std::string str, std::map<char,std::string> &myMap)
{
    if(x->isLeaf())
    {
        myMap[x->getCharacter()] = str;
        return;
    }
    
    preorder(x->left, str+"0", myMap);
    preorder(x->right, str+"1", myMap);
}

//This is the function that creates the postorder of the tree to be used in the function serializeTree
void postorder(HuffmanNode* x, std::string& str) 
{
    if(x == nullptr) return;

    postorder(x->left, str);
    postorder(x->right, str);

    if(x->isLeaf())
    {
        str += "L";
        str += x->getCharacter();
    } 
    else 
        str += "B";
} 

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

//This is the function that will intake a string an output it encoded using huffmans methds of compression
std::string HuffmanTree::compress(const std::string inputStr)
{
    int size = inputStr.length();
    std::map<char,int> List;
    for( int i=0; i<size; i++) //This is the for loop that will go through the given string and track the frequency of numbers into a map
    {
        char tempChar = inputStr[i];
        List[tempChar]++;
    }
    std::map<char,int>::iterator itr;

    HeapQueue<HuffmanNode*, HuffmanNode::Compare> queue;

    for(itr = List.begin(); itr != List.end(); itr++) //This is the for loop that will put all of the items in the map into the heap queue
    {
        HuffmanNode* temp = new HuffmanNode(itr->first, itr->second);
        queue.insert(temp);
        
    }
    while(queue.size() != 1) //This is the while loop that will create the tree using the heap 
    {
        HuffmanNode *temp1 = queue.min();
        queue.removeMin();
        HuffmanNode *temp2 = queue.min();
        queue.removeMin();
        HuffmanNode *inter = new HuffmanNode('\0',temp1->getFrequency() + temp2->getFrequency(),NULL, temp1, temp2);
        queue.insert(inter);
    }

     std::string blank = "";

    std::map<char,std::string> Binary;

    tree_root = queue.min();
    preorder(queue.min(), blank, Binary); //This function call will set the map given to have all of the chars binary representations 
    std::string binary = "";
    for(int i=0; i<size; i++) //This for loop is used to create the actual string of binary that we will return
    {
        char temp = inputStr[i];
        binary += Binary[temp];
    }
    return binary;
}


//This is the function that will output the tree used to compres the string in a postorder form
std::string  HuffmanTree::serializeTree() const
{      
    std::string string;
    postorder(tree_root, string); //This function call will update the string to be the postorder tree that was used to compress the string
    return string;
}


//This is the function that is used to decode a compressed string into it original state using a serialized Tree given in the parameters
std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree)
{
    std::string str = "";

    std::stack<HuffmanNode*> tree;

    for (size_t i = 0; i < serializedTree.size(); i++) //This for loop is used to recreate the tree from the serialized Tree given to us 
    {
        if(serializedTree[i] == 'L')
        {
            i++;
            HuffmanNode* temp = new HuffmanNode(serializedTree[i],0);
            tree.push(temp);
        }
        else if(serializedTree[i] == 'B')
        {
            HuffmanNode* right = tree.top();
            tree.pop();
            HuffmanNode* left = tree.top();
            tree.pop();
            HuffmanNode* inter = new HuffmanNode('\0',0,nullptr, left, right);
            tree.push(inter);
        }
    } 

    HuffmanNode* tempNode = tree.top();
    for (size_t i = 0; i < inputCode.size();i++) //This for loop is the one that will acctually uncompress the string using the tree created above by transversing through the tree
    {
        if(inputCode[i] == '0')
        {
            tempNode = tempNode->left;
        }
        else if(inputCode[i] == '1')
        {
            tempNode = tempNode->right;
        }
        if(tempNode->isLeaf())
        {
            str += tempNode->getCharacter();
            tempNode = tree.top();
        }
    }
    return str;
}
