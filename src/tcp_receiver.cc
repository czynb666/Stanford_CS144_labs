#include "tcp_receiver.hh"
#include <iostream>

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message, Reassembler& reassembler, Writer& inbound_stream )
{
  // Your code here.

  if (message.SYN) {
    ISN = message.seqno;
    ACKNO = message.seqno + 1;
    absoluteACKNO = 1;
  }

  if (!ISN.has_value())
    return;

  int payload_length = message.payload.length();

  reassembler.insert(message.seqno.unwrap(ISN.value(), absoluteACKNO.value()) - !message.SYN, 
                     message.payload.release(), 
                     message.FIN, 
                     inbound_stream);

  ACKNO = Wrap32::wrap(reassembler.getFirstUnassembledIndex(), ISN.value()) + 1;
  absoluteACKNO = reassembler.getFirstUnassembledIndex() + 1;

  if (message.FIN)
    FINpos = message.seqno + !!message.SYN + payload_length;

  if (FINpos.has_value() && ACKNO == FINpos.value()) {
    ACKNO = ACKNO.value() + 1;
    absoluteACKNO = absoluteACKNO.value() + 1;
  }

}

TCPReceiverMessage TCPReceiver::send( const Writer& inbound_stream ) const
{
  // Your code here.

  return TCPReceiverMessage{ACKNO, (uint16_t)(min(inbound_stream.available_capacity(), (uint64_t)UINT16_MAX))};
}
