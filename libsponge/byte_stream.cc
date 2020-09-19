#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : _stream(), _size(0), _capacity(capacity), _nwritten(0), _nread(0), _input_ended(0) {}

size_t ByteStream::write(const string &data) {
    size_t count = 0;
    for(size_t i=0; i<data.length() && remaining_capacity() > 0; i++) {
        _stream.push_back(data[i]);
        count++;
        _size++;
    }
    _nwritten += count;
    return count;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    // What if len is too big? Not safe.
    return string(_stream.begin(), _stream.begin() + len);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    // What if len is too big? Not safe.
    _stream.erase(_stream.begin(), _stream.begin() + len);
    _nread += len;
    _size -= len;
}

void ByteStream::end_input() {
    _input_ended = true;
}

bool ByteStream::input_ended() const { return _input_ended; }

size_t ByteStream::buffer_size() const { return _size; }

bool ByteStream::buffer_empty() const { return _size == 0; }

bool ByteStream::eof() const { return input_ended() && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _nwritten; }

size_t ByteStream::bytes_read() const { return _nread; }

size_t ByteStream::remaining_capacity() const { return _capacity - _size; }
