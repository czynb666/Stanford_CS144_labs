#pragma once

#include "reassembler.hh"
#include "tcp_receiver_message.hh"
#include "tcp_sender_message.hh"

class TCPReceiver
{

private:

  std::optional<Wrap32> ISN {};
  std::optional<Wrap32> ACKNO {};
  std::optional<Wrap32> FINpos {};
  std::optional<uint64_t> absoluteACKNO {};

public:
  /*
   * The TCPReceiver receives TCPSenderMessages, inserting their payload into the Reassembler
   * at the correct stream index.
   */
  void receive( TCPSenderMessage message, Reassembler& reassembler, Writer& inbound_stream );

  /* The TCPReceiver sends TCPReceiverMessages back to the TCPSender. */
  TCPReceiverMessage send( const Writer& inbound_stream ) const;
};
