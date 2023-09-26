#include "HuffmanBase.hpp"



//This is the Huffman tree class that allows you to make an object of it
class HuffmanTree : public HuffmanTreeBase
{
    public:
        std::string compress(const std::string inputStr);
        std::string serializeTree() const;
        std::string decompress(const std::string inputCode, const std::string serializedTree);
        
};
