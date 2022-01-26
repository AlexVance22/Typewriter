#include "PCH.h"
#include "Serializer.h"

#include "ioHTML.h"
#include "OutPDF.h"
#include "OutIMG.h"
#include "system/Dialogs.h"

#include "Application.h"


bool Serializer::saveFile(Application* app)
{
	const std::string outfile = "user/" + app->m_projName;
	
	if (outHTML(app->m_page, app->m_title, app->m_subtitle, outfile))
	{
		app->m_saved = true;
		return true;
	}
	else return false;
}

bool Serializer::saveAsFile(Application* app, const std::string& outfile)
{
	if (fs::exists("user/" + outfile + ".html"))
	{
		// display message
		return false;
	}

	if (outHTML(app->m_page, app->m_title, app->m_subtitle, "user/" + outfile))
	{
		app->m_newProject = false;
		app->m_projName = outfile;
		app->m_saved = true;
		return true;
	}

	return false;
}

bool Serializer::exportFile(Application* app)
{
	const char* filter = "PDF Document (*.pdf)\0*.pdf\0HTML Document (*.html + *.css)\0*.html;*css\0";
							//PNG Image (*.png)\0*.png\0JPG Image (*.jpg)\0*.jpg\0SVG Image (*.svg)\0*.svg\0";
	auto [outpath, outtype] = saveFileName(app->m_window.getSystemHandle(), filter, app->m_projName);

	if (outpath.empty())
		return false;

	std::string source = "user/" + app->m_projName;
	std::string output = outpath.replace_extension().generic_u8string();

	if (outHTML(app->m_page, app->m_title, app->m_subtitle, source))
		app->m_saved = true;
	else
		return false;

	switch (outtype)
	{
	case 1:
		return outPDF(source, output);
	case 2:
		return outHTML(app->m_page, app->m_title, app->m_subtitle, output, true);
	/*
	case 3:
		outIMG(source, output, "png");
		break;
	case 4:
		outIMG(source, output, "jpg");
		break;
	case 5:
		outIMG(source, output, "svg");
		break;
	*/
	default:
		return false;
	}
}

bool Serializer::importFile(Application* app, const std::string& infile)
{
	const std::string projName = (infile == "") ? "empty" : infile;

	if (inHTML(app->m_page, app->m_title, app->m_subtitle, ("user/" + projName + ".html").c_str()))
	{
		app->m_newProject = (infile == "");
		app->m_projName = projName;
		app->m_page.resetView();
		app->m_titlePage.setTitle(app->m_title);
		app->m_titlePage.setSubtitle(app->m_subtitle);
		app->m_titlePage.setEnabled(true);
		app->m_saved = true;
		return true;
	}
	return false;
}