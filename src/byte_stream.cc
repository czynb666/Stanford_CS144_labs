#include <stdexcept>
#include <iostream>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), totalPushedSize(0), totalPoppedSize(0),
                                              buffer(""), isClosed(false), hasError(false) {}

void Writer::push( string data )
{
  // Your code here.

  uint64_t tmpPushSize = min(data.size(), available_capacity());
  
  buffer += data.substr(0, tmpPushSize);
  totalPushedSize += tmpPushSize;
}

void Writer::close()
{
  // Your code here.

  isClosed = true;
}

void Writer::set_error()
{
  // Your code here.

  hasError = true;
}

bool Writer::is_closed() const
{
  // Your code here.
  return isClosed;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - buffer.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return totalPushedSize;
}

string_view Reader::peek() const
{
  // Your code here.
  return string_view(buffer);
}

bool Reader::is_finished() const
{
  // Your code here.
  return isClosed && buffer.empty();
}

bool Reader::has_error() const
{
  // Your code here.
  return hasError;
}

void Reader::pop( uint64_t len )
{
  // Your code here.

  uint64_t tmpPopSize = min(len, bytes_buffered());
  buffer.erase(buffer.begin(), buffer.begin() + tmpPopSize);
  totalPoppedSize += tmpPopSize;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return buffer.size();
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return totalPoppedSize;
}
