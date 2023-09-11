#include "reassembler.hh"
#include <iostream>

using namespace std;

Reassembler::Reassembler() : caughtLastDatagram(false), firstUnassembledIndex(0), totalStringLength(0), buffer()  {}

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  // Your code here.

  // first insert this datagram to Reassembler::buffer, after it's done, deal with output.
  // this splits this function into two individual parts.

  caughtLastDatagram |= is_last_substring;

  if (first_index < firstUnassembledIndex) {
    if (first_index + data.size() < firstUnassembledIndex)
      return;
    uint64_t offset = firstUnassembledIndex - first_index;
    first_index += offset;
    data.erase(0, offset);
  }

  if (first_index + data.size() > firstUnassembledIndex + output.available_capacity()) {
    if (first_index > firstUnassembledIndex + output.available_capacity())
      return;
    uint64_t offset = first_index + data.size() - firstUnassembledIndex - output.available_capacity();
    data.erase(data.size() - offset, data.size());
  }

  pair<uint64_t, string> info = {first_index, data};

  auto p = buffer.lower_bound({first_index, data});

  while (p != buffer.end() && info.first + info.second.size() >= p->first) {
    int lengthFromP = max(0, (int)(p->first + p->second.size() - info.first - info.second.size()));
    info = {first_index, data + p->second.substr(p->second.size() - lengthFromP, p->second.size())};// suffix of p
    totalStringLength -= p->second.size();
    p = buffer.erase(p);
  }

  if (p != buffer.begin()) {
    --p;

    // there's no need to loop here, since the overlapping up front occurs once at most.
    if (p->first + p->second.size() >= info.first) {
      int lengthFromInfo = max(0, (int)(info.first + info.second.size() - p->first - p->second.size()));
      info = {p->first, p->second + info.second.substr(info.second.size() - lengthFromInfo, info.second.size())};// suffix of info
      totalStringLength -= p->second.size();
      p = buffer.erase(p);
    }
  }

  totalStringLength += info.second.size();
  buffer.insert(info);


  if (buffer.begin()->first == firstUnassembledIndex) {

    output.push(buffer.begin()->second);

    totalStringLength -= buffer.begin()->second.size();
    firstUnassembledIndex += buffer.begin()->second.size();

    buffer.erase(buffer.begin());

    if (buffer.empty() && caughtLastDatagram)
      output.close();
  }


}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return totalStringLength;
}
