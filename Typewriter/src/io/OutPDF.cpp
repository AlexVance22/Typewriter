#include "PCH.h"
#include "outPDF.h"

#include "ConvertHTML.h"


bool outPDF(const std::string& infile, const std::string& outfile)
{
	std::string source = infile + ".html";
	std::string output = outfile + ".pdf";

	ConvertHTML::pdfInputFile(source.c_str());
	ConvertHTML::pdfOutputFile(output.c_str());
	ConvertHTML::pdfSetting("colorMode", "Grayscale");
	ConvertHTML::pdfSetting("web.userStyleSheet", "res/style.css", false);

	return ConvertHTML::pdfConvert();
}