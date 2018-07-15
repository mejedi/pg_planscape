//nlohmann/json
#include "json.h"

static std::size_t extra_space(const std::string& s)
{
    std::size_t result = 0;

    for (const auto& c : s)
    {
        switch (c)
        {
            case '"':
            case '\\':
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t':
            {
                // from c (1 byte) to \x (2 bytes)
                result += 1;
                break;
            }

            default:
            {
                if (c >= 0x00 and c <= 0x1f)
                {
                    // from c (1 byte) to \uxxxx (6 bytes)
                    result += 5;
                }
                break;
            }
        }
    }

    return result;
}

std::string json_escape_string(const std::string& s)
{
    const auto space = extra_space(s);
    if (space == 0)
    {
        return s;
    }

    // create a result string of necessary size
    std::string result(s.size() + space, '\\');
    std::size_t pos = 0;

    for (const auto& c : s)
    {
        switch (c)
        {
            // quotation mark (0x22)
            case '"':
            {
                result[pos + 1] = '"';
                pos += 2;
                break;
            }

            // reverse solidus (0x5c)
            case '\\':
            {
                // nothing to change
                pos += 2;
                break;
            }

            // backspace (0x08)
            case '\b':
            {
                result[pos + 1] = 'b';
                pos += 2;
                break;
            }

            // formfeed (0x0c)
            case '\f':
            {
                result[pos + 1] = 'f';
                pos += 2;
                break;
            }

            // newline (0x0a)
            case '\n':
            {
                result[pos + 1] = 'n';
                pos += 2;
                break;
            }

            // carriage return (0x0d)
            case '\r':
            {
                result[pos + 1] = 'r';
                pos += 2;
                break;
            }

            // horizontal tab (0x09)
            case '\t':
            {
                result[pos + 1] = 't';
                pos += 2;
                break;
            }

            default:
            {
                if (c >= 0x00 and c <= 0x1f)
                {
                    // print character c as \uxxxx
                    sprintf(&result[pos + 1], "u%04x", int(c));
                    pos += 6;
                    // overwrite trailing null character
                    result[pos] = '\\';
                }
                else
                {
                    // all other characters are added as-is
                    result[pos++] = c;
                }
                break;
            }
        }
    }

    return result;
}
