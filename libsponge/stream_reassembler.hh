#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using std::string;
using std::vector;

class StreamSegment {
  private:
    size_t _index;
    string _data;
    bool _eof;
  public:
    StreamSegment(size_t index, const string &data, bool eof);
    // start index (inclusive)
    size_t start() const {return _index;}
    // end index (exclusive)
    size_t end() const {return _index + _data.length();}
    // segment size
    size_t size() const { return _data.length(); }
    const string & data() const {return _data;}
    // Check if the current segment overlaps with another segment
    bool has_overlap(const StreamSegment & another);
    // Merge two overlapping segments
    StreamSegment merge(const StreamSegment & another);
    bool eof() const { return _eof; }
};

class SegmentBuffer {
  private:
    vector<StreamSegment> _segments;  //!< An ordered list of stream segments. No overlap.  
  public:
    SegmentBuffer(): _segments() {}
    size_t unassembled_bytes() const;
    const StreamSegment & peek() const { return _segments[0];}
    void pop() { _segments.erase(_segments.begin(), _segments.begin() + 1); }
    void add(const StreamSegment & segment);
    bool empty() const { return _segments.empty(); }
    size_t size() const { return _segments.size(); }

};

//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
class StreamReassembler {
  private:
    // Your code here -- add private members as necessary.
    ByteStream _output;         //!< The reassembled in-order byte stream
    size_t _capacity;           //!< The maximum number of bytes
    size_t _next_byte;          //!< The index of the next byte to be assembled
    SegmentBuffer _buff;
    bool _eof;

  public:
    //! \brief Construct a `StreamReassembler` that will store up to `capacity` bytes.
    //! \note This capacity limits both the bytes that have been reassembled,
    //! and those that have not yet been reassembled.
    StreamReassembler(const size_t capacity);

    //! \brief Receives a substring and writes any newly contiguous bytes into the stream.
    //!
    //! If accepting all the data would overflow the `capacity` of this
    //! `StreamReassembler`, then only the part of the data that fits will be
    //! accepted. If the substring is only partially accepted, then the `eof`
    //! will be disregarded.
    //!
    //! \param data the string being added
    //! \param index the index of the first byte in `data`
    //! \param eof whether or not this segment ends with the end of the stream
    void push_substring(const std::string &data, const uint64_t index, const bool eof);

    //! \name Access the reassembled byte stream
    //!@{
    const ByteStream &stream_out() const { return _output; }
    ByteStream &stream_out() { return _output; }
    //!@}

    //! The number of bytes in the substrings stored but not yet reassembled
    //!
    //! \note If the byte at a particular index has been submitted twice, it
    //! should only be counted once for the purpose of this function.
    size_t unassembled_bytes() const;
    bool eof() const { return _eof; }

    //! \brief Is the internal state empty (other than the output stream)?
    //! \returns `true` if no substrings are waiting to be assembled
    bool empty() const;
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH