#include "PCH.h"
#include "Serializer.h"

#include "core/AppData.h"
#include "core/Exceptions.h"

#include "system/Dialogs.h"
#include "ioHTML.h"
#include "OutPDF.h"


void saveFile(AppData& app, const Document& doc)
{
	std::ofstream stream(app.filepath + ".html");
	if (!stream.is_open())
		throw FileIOError("Failed to save file at specified location");

	stream << outHTML(doc, app.filename);
	
	app.savestate = SaveState::Saved;
}

void exportFile(AppData& app, const Document& doc, File outfile)
{
	const std::string output = outfile.path.replace_extension().generic_string();
	const std::string content = outHTML(doc, app.filename);

	switch (outfile.type)
	{
	case 1:
		if (!outPDF(app.filepath, output))
			throw FileIOError("Failed to export file to specified location");
	case 2: {
		std::ofstream stream(output + ".html");
		if (!stream.is_open())
			throw FileIOError("Failed to export file to specified location");

		stream << content;
		break; }
	default:
		throw FileIOError("Cannot export specified file type");
	}
}

void importFile(AppData& app, Document& doc)
{
	std::ifstream stream(app.filepath + ".html");
	if (!stream.is_open())
		throw FileIOError("Failed to open file at specified location");

	std::stringstream ss;
	ss << stream.rdbuf();
	std::string buf = ss.str();

	doc = inHTML(buf.data());
	
	app.savestate = (app.filename == "empty") ? SaveState::New : SaveState::Saved;
}