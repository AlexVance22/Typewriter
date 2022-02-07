#include "PCH.h"
#include "outPDF.h"

#include "ConvertHTML.h"


bool outPDF(const std::string& infile, const std::string& outfile)
{
	const std::string source = infile + ".html";
	const std::string output = outfile + ".pdf";

	const bool success = ConvertHTML::pdfInputFile(source.c_str()) &&
						ConvertHTML::pdfOutputFile(output.c_str()) &&
						ConvertHTML::pdfSetting("colorMode", "Grayscale") &&
						ConvertHTML::pdfSetting("web.userStyleSheet", "res/style.css", false) &&
						ConvertHTML::pdfConvert();

	return success;
}