// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "stdafx.h"
#include "binary_message_parser.h"
#include "signalrclient/signalr_exception.h"

namespace signalr
{
    namespace binary_message_parser
    {
        bool try_parse_message(const unsigned char* message, size_t length, size_t* length_prefix_length, size_t* length_of_message)
        {
            if (length == 0)
            {
                return false;
            }

            size_t num_bytes = 0;
            size_t message_length = 0;
            size_t length_prefix_buffer = std::min((size_t)5, length);

            unsigned char byte_read;
            do
            {
                byte_read = message[num_bytes];
                message_length = message_length | (((size_t)(byte_read & 0x7f)) << (num_bytes * 7));
                num_bytes++;
            }
            while (num_bytes < length_prefix_buffer && ((int)byte_read & 0x80) != 0);

            // size bytes are missing
            if ((byte_read & 0x80) != 0 && (num_bytes < 5))
            {
                throw signalr_exception("partial messages are not supported.");
            }

            if ((byte_read & 0x80) != 0 || (num_bytes == 5 && byte_read > 7))
            {
                // TODO: throw message over 2gb not supported
                throw signalr_exception("messages over 2GB are not supported.");
            }

            // not enough data with the given length prefix
            if (length < message_length + num_bytes)
            {
                throw signalr_exception("partial messages are not supported.");
            }

            *length_prefix_length = num_bytes;
            *length_of_message = message_length;
            return true;
        }
    }
}