#pragma once

struct Document;


[[nodiscard]] Document inHTML(char* source);

[[nodiscard]] std::string outHTML(const Document& doc, const std::string& filename);