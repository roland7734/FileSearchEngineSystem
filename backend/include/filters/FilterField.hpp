// FilterField.hpp

#ifndef FILTERFIELD_HPP
#define FILTERFIELD_HPP

enum class FilterField {
    Content,
    Path,
    Size,
    AccessTime,
    MimeType
};

inline FilterField parseFilterField(const std::string& key) {
    if (key == "content") return FilterField::Content;
    if (key == "path") return FilterField::Path;
    if (key == "size") return FilterField::Size;
    if (key == "accesstime") return FilterField::AccessTime;
    if (key == "mimetype") return FilterField::MimeType;
    throw std::invalid_argument("Unrecognized key in query: " + key);
}

#endif // FILTERFIELD_HPP
