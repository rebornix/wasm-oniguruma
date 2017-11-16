#include "onig-scanner.h"
#include <iostream>
#include <emscripten.h>
using namespace emscripten;

OnigScanner::OnigScanner(std::vector<std::string> sources)
    : searcher(NULL)
{
    int length = sources.size();
    regExps.resize(length);

    std::vector<std::string>::iterator iter = sources.begin();
    int index = 0;
    while (iter < sources.end())
    {
        regExps[index] = new OnigRegExp(*iter);
        ++index;
        ++iter;
    }

    searcher = new OnigSearcher(regExps);
}

CaptureResult* OnigScanner::FindNextMatchSync(int _string, int utf16_length, size_t v16StartLocation)
{
    uintptr_t p = _string;
    char *_sstring = reinterpret_cast<char *>(p);

    OnigString* source = new OnigString(_sstring, utf16_length);

    OnigResult *bestResult = searcher->Search(source, v16StartLocation);
    if (bestResult != NULL)
    {
        return new CaptureResult(bestResult, source);
    }
    else
    {
        return NULL;
    }
}

std::wstring UTF8to16(const char *in)
{
    std::wstring out;
    if (in == NULL)
        return out;

    unsigned int codepoint;
    while (*in != 0)
    {
        unsigned char ch = static_cast<unsigned char>(*in);
        if (ch <= 0x7f)
            codepoint = ch;
        else if (ch <= 0xbf)
            codepoint = (codepoint << 6) | (ch & 0x3f);
        else if (ch <= 0xdf)
            codepoint = ch & 0x1f;
        else if (ch <= 0xef)
            codepoint = ch & 0x0f;
        else
            codepoint = ch & 0x07;
        ++in;
        if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
        {
            if (codepoint > 0xffff)
            {
                out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
                out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
            }
            else if (codepoint < 0xd800 || codepoint >= 0xe000)
                out.append(1, static_cast<wchar_t>(codepoint));
        }
    }
    return out;
}


void test(int _string, int utf16_length)
{
    uintptr_t p = _string;
    char *_sstring = reinterpret_cast<char *>(p);
    printf("str %s, length %d\n", _sstring, std::strlen(_sstring));
    size_t utf8_length_ = std::strlen(_sstring);
    printf("1st %c\n", *_sstring);
    printf("2nd %c\n", *(_sstring + 1));
    printf("3rd %c\n", *(_sstring + 2));
    printf("4th %c\n", *(_sstring + 3));
    printf("5th %c\n", *(_sstring + 4));
    printf("6th %c\n", *(_sstring + 5));
    printf("7th %c\n", *(_sstring + 6));
    printf("8th %c\n", *(_sstring + 7));
    printf("9th %c\n", *(_sstring + 8));

    std::cout << _sstring << std::endl;
    std::wstring utf16Val = UTF8to16(_sstring);
    size_t utf16_length_ = utf16Val.length();
    std::cout << utf16_length_ << std::endl;

    int *utf16OffsetToUtf8 = new int[utf16_length_ + 1];
    utf16OffsetToUtf8[utf16_length_] = utf8_length_;
    int *utf8OffsetToUtf16 = new int[utf8_length_ + 1];
    utf8OffsetToUtf16[utf8_length_] = utf16_length_;


    int i8 = 0;
    for (int i16 = 0, len = utf16_length_; i16 < len; i16++)
    {
        uint16_t in = utf16Val[i16];
        unsigned int codepoint = in;
        bool wasSurrogatePair = false;

        if (in >= 0xd800 && in <= 0xdbff)
        {
            // Hit a high surrogate, try to look for a matching low surrogate
            if (i16 + 1 < len)
            {
                uint16_t next = utf16Val[i16 + 1];
                if (next >= 0xdc00 && next <= 0xdfff)
                {
                    // Found the matching low surrogate
                    codepoint = (((in - 0xd800) << 10) + 0x10000) | (next - 0xdc00);
                    wasSurrogatePair = true;
                }
            }
        }

        utf16OffsetToUtf8[i16] = i8;

        if (codepoint <= 0x7f) {
            utf8OffsetToUtf16[i8++] = i16;
        } else if (codepoint <= 0x7ff) {
            utf8OffsetToUtf16[i8++] = i16;
            utf8OffsetToUtf16[i8++] = i16;
        } else if (codepoint <= 0xffff) {
            utf8OffsetToUtf16[i8++] = i16;
            utf8OffsetToUtf16[i8++] = i16;
            utf8OffsetToUtf16[i8++] = i16;
        } else {
            utf8OffsetToUtf16[i8++] = i16;
            utf8OffsetToUtf16[i8++] = i16;
            utf8OffsetToUtf16[i8++] = i16;
            utf8OffsetToUtf16[i8++] = i16;
        }

        if (wasSurrogatePair) {
            utf16OffsetToUtf8[i16 + 1] = utf16OffsetToUtf8[i16];
            i16++;
        }
    }

    for (int i16 = 0, len = utf16_length_; i16 < len; i16++) {
        printf("u16: %d, u8: %d\n", i16, utf16OffsetToUtf8[i16]);
    }
}

EMSCRIPTEN_BINDINGS(test) {
    // this may or may not work, let's try directly binding the static funcs
    function("test", &test, allow_raw_pointers());
}

EMSCRIPTEN_BINDINGS(CaptureIndice) {
    class_<CaptureIndice>("CaptureIndice")
        .constructor<int, int, int, int>(allow_raw_pointers())
        .property("index", &CaptureIndice::getIndex, &CaptureIndice::setIndex)
        .property("start", &CaptureIndice::getStart, &CaptureIndice::setStart)
        .property("end", &CaptureIndice::getEnd, &CaptureIndice::setEnd)
        .property("length", &CaptureIndice::getLength, &CaptureIndice::setLength)
          ;
};

EMSCRIPTEN_BINDINGS(CaptureResult) {
    class_<CaptureResult>("CaptureResult")
        .constructor<OnigResult *, OnigString*>(allow_raw_pointers())
        .property("index", &CaptureResult::getIndex, &CaptureResult::setIndex)
        // returning array/vector is a bummer, here we return two functions.
        .function("Count", &CaptureResult::Count)
        .function("IndiceAt", &CaptureResult::IndiceAt, allow_raw_pointers())

        //   .property("captureIndices", &CaptureResult::getCaptureIndices, &CaptureResult::setCaptureIndices)
        ;
};

EMSCRIPTEN_BINDINGS(OnigScanner) {
    register_vector<std::string>("VectorString");
    class_<OnigScanner>("OnigScanner")
        .constructor<std::vector<std::string>>(allow_raw_pointers())
    .function("_findNextMatchSync", &OnigScanner::FindNextMatchSync, allow_raw_pointers())
    ;
}