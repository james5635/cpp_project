#include "task.h"
#include <sstream>
string Task::toJson() const
{
    ostringstream oss;
    oss << "{"
        << "\"id\": " << id << ", "
        << "\"description\": \"" << description << "\", "
        << "\"status\": \"" << status << "\", "
        << "\"createAt\": \"" << createdAt << "\", "
        << "\"updatedAt\": \"" << updatedAt << "\" "
        << "}";
    return oss.str();
}

Task Task::fromJson(const string &json)
{
    Task t;

    auto extractValue = [&](const std::string &key) -> std::string
    {
        size_t keyPos = json.find("\"" + key + "\"");
        if (keyPos == std::string::npos)
            throw std::runtime_error("Key '" + key + "' not found");

        // Find colon after key
        size_t colonPos = json.find(":", keyPos);
        if (colonPos == std::string::npos)
            throw std::runtime_error("Malformed JSON: missing ':' for key '" + key + "'");

        // Start from first non-space/quote after colon
        size_t start = colonPos + 1;
        while (start < json.size() && (std::isspace(json[start]) || json[start] == '"'))
            ++start;

        // Find end (either quote, comma, or closing brace)
        size_t end = start;
        bool inQuotes = (json[start - 1] == '"');
        if (inQuotes)
        {
            end = json.find("\"", start);
        }
        else
        {
            while (end < json.size() && json[end] != ',' && json[end] != '}')
                ++end;
        }

        return json.substr(start, end - start);
    };
    t.id = std::stoi(extractValue("id"));
    t.description = extractValue("description");
    t.status = extractValue("status");
    t.createdAt = extractValue("createAt");
    t.updatedAt = extractValue("updatedAt");
    return t;
}
