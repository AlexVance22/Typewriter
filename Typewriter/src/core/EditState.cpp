#include "PCH.h"
#include "EditState.h"


const std::unordered_map<std::string, EditState> StateOps::s_invstringLUT = {
	{ "None",			EditState::None },
	{ "Scene",			EditState::Scene },
	{ "Directorial",	EditState::Directorial },
	{ "Name",			EditState::Name },
	{ "Parenthetical",	EditState::Parenthetical },
	{ "Speech",			EditState::Speech }
};

const std::unordered_map<EditState, std::string> StateOps::s_stringLUT = {
	{ EditState::None,			"None" },
	{ EditState::Scene,			"Scene" },
	{ EditState::Directorial,	"Directorial" },
	{ EditState::Name,			"Name" },
	{ EditState::Parenthetical,	"Parenthetical" },
	{ EditState::Speech,		"Speech"}
};

const std::unordered_map<EditState, EditState> StateOps::s_stateLUT = {
	{ EditState::None,			EditState::Scene },
	{ EditState::Scene,			EditState::Directorial },
	{ EditState::Directorial,	EditState::Name },
	{ EditState::Name,			EditState::Speech },
	{ EditState::Parenthetical,	EditState::Speech },
	{ EditState::Speech,		EditState::Name },
};