#include "PCH.h"
#include "ioHTML.h"

#include "Application.h"

#include "gui/Page.h"


bool inHTML(Page& page, sf::String& title, sf::String& subtitle, const char* infile)
{
	page.clear();

	auto doc = std::make_unique<rapidxml::xml_document<>>();
	rapidxml::file<> f(infile);
	doc->parse<0>(f.data());

	rapidxml::xml_node<>* firstpage = doc->first_node("html")->first_node("body")->first_node("div");


	rapidxml::xml_node<>* titlenode = firstpage->first_node();
	title = titlenode->value();
	subtitle = titlenode->next_sibling()->value();


	for (rapidxml::xml_node<>* pagenode = firstpage->next_sibling(); pagenode; pagenode = pagenode->next_sibling())
	{
		for (rapidxml::xml_node<>* div = pagenode->first_node(); div; div = div->next_sibling())
		{
			rapidxml::xml_attribute<>* attr = div->first_attribute();
			if (strncmp(attr->value(), "SceneNum", 32) == 0)
				continue;

			page.addDivision(Application::strToState(attr->value()));
			page.getActive().setText(div->value());
		}
	}

	return true;
}


static bool allowOverwrite(const std::string& outhtml)
{
	if (fs::exists(outhtml))
	{
		std::cout << "Overwrite existing file? [y/N]\n";
		char response;
		std::cin >> response;
		return response == 'y';
	}

	return true;
}

static void dumpHeader(std::ofstream& f, fs::path outfile, bool standalone)
{
	f << "<html>\n<head>\n\t<link rel=\"stylesheet\" href=\"";

	if (standalone)
	{
		fs::copy("res/standalone.css", outfile.concat("css"), fs::copy_options::overwrite_existing);

		f << outfile.filename();
	}
	else
		f << "../res/standalone";

	f << ".css\"/>\n</head>\n";
}

bool outHTML(const Page& page, const sf::String& title, const sf::String& subtitle, const fs::path& outfile, bool standalone)
{
	const std::string outhtml = outfile.generic_u8string() + ".html";

	if (standalone)
	{
		if (!allowOverwrite(outhtml))
			return false;
	}

	std::ofstream f(outhtml);
	if (!f.is_open())
		return false;

	dumpHeader(f, outfile, standalone);

	uint32_t scenecount = 1;

	f << "<body>\n\t<div class=\"Page\">\n\t\t<div class=\"Title\">" << (std::string)title
		<< "</div>\n\t\t<div class=\"Subtitle\">" << (std::string)subtitle <<
		"</div>\n\t</div>\n\t<div class=\"Page\">\n";

	for (const auto& div : page)
	{
		const EditState state = div.getState();
		const sf::String text = div.getText();

		if (state == EditState::Scene)
		{
			f << "\t\t<div class=\"SceneNum\">";

			for (size_t i = 0; i < 3 - std::to_string(scenecount).size(); i++)
				f << "&nbsp;";

			f << scenecount++ << " </div><div class=\"Scene\">" << (std::string)text << "</div>\n";
		}
		else
			f << "\t\t<div class=\"" << Application::stateToStr(state) << "\">" << (std::string)text << "</div>\n";
	}

	f << "\t</div>\n</body>\n</html>\0";

	f.close();

	return true;
}