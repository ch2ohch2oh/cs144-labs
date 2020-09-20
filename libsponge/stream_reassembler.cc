#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamSegment::StreamSegment(size_t index, const string &data, bool eof) : _index(index), _data(data), _eof{eof} {}

bool StreamSegment::has_overlap(const StreamSegment &another) {
    if (this->start() > another.end() || this->end() < another.start()) {
        return false;
    } else {
        return true;
    }
}

StreamSegment StreamSegment::merge(const StreamSegment &another) {
    if (!this->has_overlap(another)) {
        std::cerr << "Cannot merge two segments with no overlap" << std::endl;
    }
    size_t new_start = min(this->start(), another.start());
    size_t new_end = max(this->end(), another.end());
    string buf(new_end - new_start, '#');
    buf.replace(this->start() - new_start, this->size(), this->data());
    buf.replace(another.start() - new_start, another.size(), another.data());
    return StreamSegment(new_start, buf, this->eof() || another.eof());
}

void SegmentBuffer::add(const StreamSegment &segment) {
    StreamSegment new_seg = segment;
    // Find intersections first
    for (auto seg = _segments.begin(); seg != _segments.end();) {
        if (seg->has_overlap(new_seg)) {
            new_seg = seg->merge(new_seg);
            seg = _segments.erase(seg);
        } else {
            seg++;
        }
    }
    // Insert and maintain order
    vector<StreamSegment>::iterator seg;
    for (seg = _segments.begin(); seg != _segments.end(); seg++) {
        if (seg->start() > new_seg.start()) {
            break;
        }
    }
    _segments.insert(seg, new_seg);
    // std::cerr << "Size of _segments = " << _segments.size() << std::endl;
}

size_t SegmentBuffer::unassembled_bytes() const {
    size_t total = 0;
    for (auto seg = _segments.begin(); seg != _segments.end(); seg++) {
        total += seg->size();
    }
    return total;
}

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity), _capacity(capacity), _next_byte(0), _buff(), _eof(false) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.

void StreamReassembler::push_substring(const std::string &data, const size_t index, const bool eof) {
    string new_data{};
    size_t new_index{};

    // std::cout << "_next_byte = " << _next_byte << " current data = " << data
    //     << " eof = " << eof << std::endl;

    if (data.length() == 0 && (!eof || index < _next_byte)) {
        return;
    } else if (data.length() > 0 && index + data.length() <= _next_byte) {
        return;
    } else if (index <= _next_byte) {
        new_data = data.substr(_next_byte - index, data.length() - (_next_byte - index));
        new_index = _next_byte;
    } else {
        new_data = data;
        new_index = index;
    }
    _buff.add(StreamSegment(new_index, new_data, eof));
    // std::cout << "_buff.size() = " << _buff.size() << std::endl;
    while (_buff.size() > 0 && _buff.peek().start() == _next_byte && _output.remaining_capacity() > 0) {
        const StreamSegment &next = _buff.peek();

        size_t nwritten = _output.write(next.data());
        // std::cout << "nwritten = " << nwritten << " next.data() = " << next.data() << " next.eof = "
        //     << next.eof() << std::endl;
        _next_byte += nwritten;
        if (nwritten == next.data().size()) {
            if (next.eof()) {
                _output.end_input();
            }
            _buff.pop();
        } else if (nwritten > 0) {
            _buff.pop();
            _buff.add(StreamSegment(_next_byte, next.data().substr(nwritten), next.eof()));
        }
    }
    // std::cout << "Exiting push_string" << std::endl;
}

size_t StreamReassembler::unassembled_bytes() const { return _buff.unassembled_bytes(); }

bool StreamReassembler::empty() const { return _buff.unassembled_bytes() == 0; }