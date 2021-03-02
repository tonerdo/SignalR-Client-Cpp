// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "stdafx.h"
#include "binary_message_formatter.h"
#include <cassert>

namespace signalr
{
    namespace binary_message_formatter
    {
        void write_length_prefix(std::string& payload)
        {
            char buffer[5];

            size_t length = payload.length();
            size_t length_num_bytes = 0;
            do
            {
                assert(length_num_bytes < 5);
                buffer[length_num_bytes] = (char)(length & 0x7f);
                length >>= 7;
                if (length > 0)
                {
                    buffer[length_num_bytes] |= 0x80;
                }
                length_num_bytes++;
            } while (length > 0);

            payload.insert(0, buffer, length_num_bytes);
        }
    }
}