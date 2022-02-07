#include "PCH.h"
#include "OutIMG.h"

#include "ConvertHTML.h"


bool outIMG(const std::string& infile, const std::string& outfile, const std::string& extension)
{
	std::string source = infile + ".html";
	std::string output = outfile + "." + extension;

	ConvertHTML::imageInputFile(source.c_str());
	ConvertHTML::imageOutputFile(output.c_str());
	ConvertHTML::imageSetting("screenWidth", "900");
	ConvertHTML::imageSetting("fmt", extension.c_str());
	ConvertHTML::imageSetting("web.userStyleSheet", "res/style.css");

	return ConvertHTML::pdfConvert();
}