#include "PCH.h"
#include "outPDF.h"


PDFConvertError::PDFConvertError(const char* msg, uint32_t stage)
	: std::exception(msg), m_stage(stage)
{}

uint32_t PDFConvertError::getStage() const noexcept
{
	return m_stage;
}


static wkhtmltopdf_global_settings* setGlobalSettings(const char* output)
{
	auto globSettings = wkhtmltopdf_create_global_settings();

	if (!globSettings)
		return nullptr;

	bool globSuccess = wkhtmltopdf_set_global_setting(globSettings, "page", "-") &&
		wkhtmltopdf_set_global_setting(globSettings, "margin.top", "1in") &&
		wkhtmltopdf_set_global_setting(globSettings, "margin.bottom", "1in") &&
		wkhtmltopdf_set_global_setting(globSettings, "margin.left", "0in") &&
		wkhtmltopdf_set_global_setting(globSettings, "margin.right", "0in") &&
		wkhtmltopdf_set_global_setting(globSettings, "colorMode", "Grayscale") &&
		wkhtmltopdf_set_global_setting(globSettings, "out", output);

	if (!globSuccess)
	{
		wkhtmltopdf_destroy_global_settings(globSettings);
		return nullptr;
	}

	return globSettings;
}

static wkhtmltopdf_object_settings* setObjectSettings(const char* input)
{
	auto objSettings = wkhtmltopdf_create_object_settings();

	if (!objSettings)
		return nullptr;

	bool objSuccess = wkhtmltopdf_set_object_setting(objSettings, "web.defaultEncoding", "utf-8") &&
		wkhtmltopdf_set_object_setting(objSettings, "web.enableIntelligentShrinking", "false") &&
		wkhtmltopdf_set_object_setting(objSettings, "web.userStyleSheet", "res/style.css") &&
		wkhtmltopdf_set_object_setting(objSettings, "page", input);

	if (!objSuccess)
	{
		wkhtmltopdf_destroy_object_settings(objSettings);
		return nullptr;
	}

	return objSettings;
}


void outPDF(const std::string& infile, const std::string& outfile)
{
	const std::string source = infile + ".html";
	const std::string output = outfile + ".pdf";


	const auto globSettings = setGlobalSettings(output.c_str());
	if (!globSettings)
		throw PDFConvertError("Failed to convert to PDF", 0);

	const auto pdfConverter = wkhtmltopdf_create_converter(globSettings);
	if (!pdfConverter)
		throw PDFConvertError("Failed to convert to PDF", 1);


	const auto objSettings = setObjectSettings(source.c_str());
	if (!objSettings)
	{
		wkhtmltopdf_destroy_converter(pdfConverter);
		throw PDFConvertError("Failed to convert to PDF", 2);
	}

	wkhtmltopdf_add_object(pdfConverter, objSettings, nullptr);

	if (!wkhtmltopdf_convert(pdfConverter))
	{
		wkhtmltopdf_destroy_converter(pdfConverter);
		throw PDFConvertError("Failed to convert to PDF", 3);
	}
}