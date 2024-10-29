#pragma once
#include <cstdint>

namespace Endianness {
    inline bool isLittleEndian() {
        uint16_t number = 0x1;
        return *(reinterpret_cast<uint8_t*>(&number)) == 0x1;
    }

    inline uint16_t swapUint16(uint16_t value) {
        return (value << 8) | (value >> 8);
    }

    inline uint32_t swapUint32(uint32_t value) {
        return  ((value << 24) & 0xFF000000) |
                ((value << 8)  & 0x00FF0000) |
                ((value >> 8)  & 0x0000FF00) |
                ((value >> 24) & 0x000000FF);
    }

    inline uint16_t hostToNetworkUint16(uint16_t value) {
        if (isLittleEndian())
            return swapUint16(value);
        else
            return value;
    }

    inline uint16_t networkToHostUint16(uint16_t value) {
        if (isLittleEndian())
            return swapUint16(value);
        else
            return value;
    }

    inline uint32_t hostToNetworkUint32(uint32_t value) {
        if (isLittleEndian())
            return swapUint32(value);
        else
            return value;
    }

    inline uint32_t networkToHostUint32(uint32_t value) {
        if (isLittleEndian())
            return swapUint32(value);
        else
            return value;
    }
}
