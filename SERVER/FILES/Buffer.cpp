#include "Buffer.h"

void Buffer::writeBytes(const std::vector<unsigned char>& bytes)
{
    m_data.insert(m_data.end(), bytes.begin(), bytes.end());
}

std::vector<unsigned char> Buffer::getData() const
{
    return m_data;
}

Buffer::Buffer(const std::vector<unsigned char>& data)
    : m_data(data)
{}
