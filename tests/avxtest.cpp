#include <cassert>
#include <iostream>

#include "avxcodec.h"


void basictest(uint32_t distinct, uint32_t length) {
    uint64_t * buf = new uint64_t[length];
    for(size_t i = 0; i < length; i++) {
        buf[i] = (i % distinct) * UINT64_C(0xcb9fe8c7cff9982a) + 77777 ;// made up
    }
    AVXDictCODEC codec;
    dictionary_coded_t t (codec.compress(buf, length));
    uint64_t * newbuf = new uint64_t[length];

    size_t newlength = codec.uncompress(t,newbuf);
    assert(length == newlength);
    for(size_t i = 0; i < length; i++) {
        assert(buf[i] == newbuf[i]);
    }
}

void fasttest(uint32_t distinct, uint32_t length) {
    uint64_t * buf = new uint64_t[length];
    for(size_t i = 0; i < length; i++) {
        buf[i] = (i % distinct) * UINT64_C(0xcb9fe8c7cff9982a) + 77777 ;// made up
    }
    AVXDictCODEC codec;
    dictionary_coded_t t (codec.compress(buf, length) );
    uint64_t * newbuf = new uint64_t[length];

    size_t newlength = AVXDictCODEC::fastuncompress(t,newbuf);
    assert(length == newlength);
    for(size_t i = 0; i < length; i++) {
        assert(buf[i] == newbuf[i]);
    }
    delete[] newbuf;
    delete[] buf;
}


int main() {
    for(uint32_t length = 256; length <= 65536; length *=2) {
        for(uint32_t distinct = 1; distinct <= 65536; distinct *=2) {
            basictest(distinct, length);
            fasttest(distinct, length);
        }
        std::cout << ".";
        std::cout.flush();
    }
    std::cout << std::endl;
    std::cout << "AVX code might be ok. " << std::endl;
    return 0;
}
