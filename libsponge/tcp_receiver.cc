#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

bool TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader & header = seg.header();
    bool eof = false;
    size_t padding = 1;
    if(header.syn && !_isn.has_value()) {
        _isn = header.seqno;
        _ack = header.seqno;
        // Initial checkpoint should be zero
        // Took me many ours to find this bug
        _checkpoint = 0;
        padding = 0;
    }

    // Conncetion has not started yet
    if(!_isn.has_value()) {
        return false;
    }

    uint64_t low = unwrap(_ack, _isn.value(), _checkpoint);
    uint64_t index = unwrap(header.seqno, _isn.value(), _checkpoint);
    uint64_t high = max(window_size(), size_t(1)) + low;
    
    // std::cout << "ack=" << _ack 
    //     << " isn=" << _isn.value() 
    //     << " seqno=" << header.seqno
    //     << " window_size=" << window_size() 
    //     << " checkpoint=" << _checkpoint
    //     << " low=" << low 
    //     << " index=" << index 
    //     << " high=" << high << std::endl;

    _checkpoint = index;
    std::cout << seg.header().summary() << std::endl;

    if(index + max(size_t(1), seg.length_in_sequence_space()) <= low || index >= high) {
        // std::cout << "Outside the window" << std::endl;
        return false;
    }

    if(header.fin) {
        eof = true;
    }
    std::cout << index - padding << std::endl;
    _reassembler.push_substring(seg.payload().copy(), index - padding, eof);
    // std::cout << "bytes_written=" << stream_out().bytes_written() << std:: endl;
    _ack = wrap(stream_out().bytes_written() + 1 + (eof ? 1 : 0), _isn.value());
    
    // size_t padding = 1;
    return true;
}

std::optional<WrappingInt32> TCPReceiver::ackno() const {
    if(_isn.has_value()) {
        return {_ack};
    } else {
        return {};
    }
}

size_t TCPReceiver::window_size() const { 
    // Implement with public methods when possible
    return stream_out().remaining_capacity(); 
}
