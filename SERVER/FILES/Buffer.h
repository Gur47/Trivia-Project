#pragma once
#include <vector>

// buffer for handling byte operations
class Buffer
{
public:
    Buffer() = default;
    Buffer(const std::vector<unsigned char>& data);
    ~Buffer() = default;

    void writeBytes(const std::vector<unsigned char>& bytes);
    std::vector<unsigned char> getData() const;

private:
    std::vector<unsigned char> m_data;
};
