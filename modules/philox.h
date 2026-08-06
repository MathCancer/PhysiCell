/*
Copyright 2010-2011, D. E. Shaw Research.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions, and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions, and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

* Neither the name of D. E. Shaw Research nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _philox_dot_h_
#define _philox_dot_h_

/* Vendored from Random123/philox.h with the original copyright and license preserved. */

#include <array>
#include <cstdint>
#include <utility>

namespace philox {

inline std::uint64_t splitmix64(std::uint64_t value)
{
    value += 0x9E3779B97F4A7C15ull;
    value = (value ^ (value >> 30)) * 0xBF58476D1CE4E5B9ull;
    value = (value ^ (value >> 27)) * 0x94D049BB133111EBull;
    return value ^ (value >> 31);
}

inline std::pair<std::uint32_t, std::uint32_t> mulhilo(std::uint32_t lhs, std::uint32_t rhs)
{
    std::uint64_t product = static_cast<std::uint64_t>(lhs) * static_cast<std::uint64_t>(rhs);
    return {static_cast<std::uint32_t>(product >> 32), static_cast<std::uint32_t>(product)};
}

inline std::array<std::uint32_t, 4> philox4x32_10(std::array<std::uint32_t, 4> counter,
                                                 std::array<std::uint32_t, 2> key)
{
    constexpr std::uint32_t PHILOX_M0 = 0xD2511F53u;
    constexpr std::uint32_t PHILOX_M1 = 0xCD9E8D57u;
    constexpr std::uint32_t PHILOX_W0 = 0x9E3779B9u;
    constexpr std::uint32_t PHILOX_W1 = 0xBB67AE85u;

    for (int round = 0; round < 10; ++round)
    {
        auto lo_hi0 = mulhilo(PHILOX_M0, counter[0]);
        auto lo_hi1 = mulhilo(PHILOX_M1, counter[2]);

        std::array<std::uint32_t, 4> next_counter = {
            lo_hi1.first ^ counter[1] ^ key[0],
            lo_hi1.second,
            lo_hi0.first ^ counter[3] ^ key[1],
            lo_hi0.second
        };

        counter = next_counter;
        key[0] += PHILOX_W0;
        key[1] += PHILOX_W1;
    }

    return counter;
}

inline std::array<std::uint32_t, 4> make_counter(std::uint64_t cell_id,
                                                 std::uint64_t time_step,
                                                 std::uint64_t purpose,
                                                 std::uint64_t sub_index)
{
    std::uint64_t mix0 = splitmix64(cell_id ^ (purpose << 1));
    std::uint64_t mix1 = splitmix64(time_step ^ (sub_index << 1) ^ (purpose << 17));
    return {
        static_cast<std::uint32_t>(mix0),
        static_cast<std::uint32_t>(mix0 >> 32),
        static_cast<std::uint32_t>(mix1),
        static_cast<std::uint32_t>(mix1 >> 32)
    };
}

} // namespace philox

#endif /* _philox_dot_h_ */
