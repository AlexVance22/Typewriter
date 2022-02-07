#pragma once

struct AppData;
struct Document;
struct File;


void saveFile(AppData& app, const Document& doc);
void exportFile(AppData& app, const Document& doc, File outfile);
void importFile(AppData& app, Document& doc);