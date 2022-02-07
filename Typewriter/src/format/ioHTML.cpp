#include "PCH.h"
#include "ioHTML.h"

#include "core/EditState.h"
#include "core/AppData.h"

using namespace rapidxml;


Document inHTML(char* source)
{
	Document result;

	auto doc = std::make_unique<xml_document<>>();
	doc->parse<0>(source);

	xml_node<>* firstpage = doc->first_node("html")->first_node("body")->first_node("div");

	xml_node<>* titlenode = firstpage->first_node();
	result.title = titlenode->value();
	result.subtitle = titlenode->next_sibling()->value();

	for (xml_node<>* pagenode = firstpage->next_sibling(); pagenode; pagenode = pagenode->next_sibling())
	{
		for (xml_node<>* div = pagenode->first_node(); div; div = div->next_sibling())
		{
			xml_attribute<>* attr = div->first_attribute();
			if (strncmp(attr->value(), "SceneNum", 32) == 0)
				continue;

			result.content.emplace_back();
			result.content.back().text = div->value();
			result.content.back().state = StateOps::StringToState(attr->value());
		}
	}

	return result;
}

std::string outHTML(const Document& doc, const std::string& filename)
{
	std::stringstream result;

	//fs::copy("res/standalone.css", outfile.concat("css"), fs::copy_options::overwrite_existing);

	result << "<html>\n<head>\n\t<link rel=\"stylesheet\" href=\"" << filename
		<< ".css\"/>\n</head>\n<body>\n\t<div class=\"Page\">\n\t\t<div class=\"Title\">"
		<< doc.title << "</div>\n\t\t<div class=\"Subtitle\">" << doc.subtitle
		<< "</div>\n\t</div>\n\t<div class=\"Page\">\n";

	uint32_t scenecount = 1;

	for (const auto& [text, state] : doc.content)
	{
		if (state == EditState::Scene)
		{
			result << "\t\t<div class=\"SceneNum\">";

			for (size_t i = 0; i < 3 - std::to_string(scenecount).size(); i++)
				result << "&nbsp;";

			result << scenecount++ << " </div><div class=\"Scene\">" << text << "</div>\n";
		}
		else
			result << "\t\t<div class=\"" << StateOps::StateToString(state) << "\">" << text << "</div>\n";
	}

	result << "\t</div>\n</body>\n</html>\0";

	return result.str();
}