// Copyright Doug Moen 2016-2017.
// Distributed under The MIT License.
// See accompanying file LICENCE.md or https://opensource.org/licenses/MIT

#include <curv/string.h>

using namespace aux;
namespace curv {

const char String::name[] = "string";

Shared<String>
String::make(const char* str, size_t len)
{
    void* raw = malloc(sizeof(String) + len);
    if (raw == nullptr)
        throw std::bad_alloc();
    String* s = new(raw) String();
    memcpy(s->data_, str, len);
    s->data_[len] = '\0';
    s->size_ = len;
    return Shared<String>{s};
}

Shared<String>
String_Builder::get_string()
{
    auto s = str(); // copies the data
    return String::make(s.data(), s.size()); // copies the data again
}

void
String::print(std::ostream& out) const
{
    out << '"';
    for (size_t i = 0; i < size_; ++i) {
        char c = data_[i];
        if (c == '$' || c == '"')
            out << c;
        out << c;
    }
    out << '"';
}

} // namespace curv
