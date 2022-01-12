#include "PCH.h"
#include "ConvertHTML.h"


wkhtmltopdf_global_settings* ConvertHTML::s_pdfSettings = nullptr;
wkhtmltoimage_global_settings* ConvertHTML::s_imgSettings = nullptr;

wkhtmltopdf_object_settings* ConvertHTML::s_objSettings = nullptr;

wkhtmltopdf_converter* ConvertHTML::s_pdfConverter = nullptr;
wkhtmltoimage_converter* ConvertHTML::s_imgConverter = nullptr;


void ConvertHTML::pdfInitSettingsGlobal()
{
	s_pdfSettings = wkhtmltopdf_create_global_settings();

	wkhtmltopdf_set_global_setting(s_pdfSettings, "page", "-");
	wkhtmltopdf_set_global_setting(s_pdfSettings, "out", "-");
	wkhtmltopdf_set_global_setting(s_pdfSettings, "margin.top", "1in");
	wkhtmltopdf_set_global_setting(s_pdfSettings, "margin.bottom", "1in");
	wkhtmltopdf_set_global_setting(s_pdfSettings, "margin.left", "0in");
	wkhtmltopdf_set_global_setting(s_pdfSettings, "margin.right", "0in");
}

void ConvertHTML::pdfInitSettingsObject()
{
	s_objSettings = wkhtmltopdf_create_object_settings();

	wkhtmltopdf_set_object_setting(s_objSettings, "web.defaultEncoding", "utf-8");
	wkhtmltopdf_set_object_setting(s_objSettings, "web.enableIntelligentShrinking", "false");
}

void ConvertHTML::imgInitSettings()
{
	s_imgSettings = wkhtmltoimage_create_global_settings();

	wkhtmltoimage_set_global_setting(s_imgSettings, "in", "-");
	wkhtmltoimage_set_global_setting(s_imgSettings, "out", "-");
	wkhtmltoimage_set_global_setting(s_imgSettings, "fmt", "png");
	wkhtmltoimage_set_global_setting(s_imgSettings, "smartWidth", "false");
	wkhtmltoimage_set_global_setting(s_imgSettings, "web.defaultEncoding", "utf-8");
	wkhtmltoimage_set_global_setting(s_imgSettings, "web.enableIntelligentShrinking", "false");
}


void ConvertHTML::init()
{
	wkhtmltopdf_init(0);
	wkhtmltoimage_init(0);
}


bool ConvertHTML::pdfSetting(const char* key, const char* val, bool global)
{
	if (global)
	{
		if (!s_pdfSettings)
			pdfInitSettingsGlobal();

		return wkhtmltopdf_set_global_setting(s_pdfSettings, key, val);
	}
	else
	{
		if (!s_objSettings)
			pdfInitSettingsObject();

		return wkhtmltopdf_set_object_setting(s_objSettings, key, val);
	}
}

bool ConvertHTML::pdfInputFile(const char* filepath)
{
	return pdfSetting("page", filepath, false);
}

bool ConvertHTML::pdfOutputFile(const char* filepath)
{
	return pdfSetting("out", filepath);
}

bool ConvertHTML::pdfConvert()
{
	if (!s_pdfSettings)
		pdfInitSettingsGlobal();

	if (!s_objSettings)
		pdfInitSettingsObject();

	s_pdfConverter = wkhtmltopdf_create_converter(s_pdfSettings);
	wkhtmltopdf_add_object(s_pdfConverter, s_objSettings, nullptr);

	if (!wkhtmltopdf_convert(s_pdfConverter))
	{
		pdfReset();
		return false;
	}

	pdfReset();
	return true;
}

void ConvertHTML::pdfReset()
{
	s_pdfSettings = nullptr;
	s_objSettings = nullptr;

	wkhtmltopdf_destroy_converter(s_pdfConverter);
}


bool ConvertHTML::imageSetting(const char* key, const char* val)
{
	if (!s_imgSettings)
		imgInitSettings();

	return wkhtmltoimage_set_global_setting(s_imgSettings, key, val);
}

bool ConvertHTML::imageInputFile(const char* filepath)
{
	return imageSetting("in", filepath);
}

bool ConvertHTML::imageOutputFile(const char* filepath)
{
	return imageSetting("out", filepath);
}

bool ConvertHTML::imageConvert()
{
	if (!s_imgSettings)
		return false;

	s_imgConverter = wkhtmltoimage_create_converter(s_imgSettings, nullptr);

	if (!wkhtmltoimage_convert(s_imgConverter))
	{
		imageReset();
		return false;
	}

	imageReset();
	return true;
}

void ConvertHTML::imageReset()
{
	s_imgSettings = nullptr;

	wkhtmltoimage_destroy_converter(s_imgConverter);
}