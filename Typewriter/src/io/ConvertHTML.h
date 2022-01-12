#pragma once


class ConvertHTML
{
private:
	static wkhtmltopdf_global_settings* s_pdfSettings;
	static wkhtmltoimage_global_settings* s_imgSettings;

	static wkhtmltopdf_object_settings* s_objSettings;

	static wkhtmltopdf_converter* s_pdfConverter;
	static wkhtmltoimage_converter* s_imgConverter;

private:
	static void pdfInitSettingsGlobal();
	static void pdfInitSettingsObject();
	static void imgInitSettings();

public:
	static void init();

	static bool pdfSetting(const char* key, const char* val, bool global = true);
	static bool pdfInputFile(const char* filepath);
	static bool pdfOutputFile(const char* filepath);
	static bool pdfConvert();
	static void pdfReset();

	static bool imageSetting(const char* key, const char* val);
	static bool imageInputFile(const char* filepath);
	static bool imageOutputFile(const char* filepath);
	static bool imageConvert();
	static void imageReset();
};