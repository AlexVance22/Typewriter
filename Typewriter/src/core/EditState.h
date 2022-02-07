#pragma once


enum class EditState
{
	None = 0,
	Scene,
	Directorial,
	Name,
	Parenthetical,
	Speech
};


class StateOps
{
private:
	const static std::unordered_map<std::string, EditState> s_invstringLUT;
	const static std::unordered_map<EditState, std::string> s_stringLUT;
	const static std::unordered_map<EditState, EditState> s_stateLUT;

public:
	static const std::string& StateToString(EditState state) { return s_stringLUT.at(state); }
	static EditState StringToState(const std::string& str) { return s_invstringLUT.at(str); }
	static EditState StateToNext(EditState state) { return s_stateLUT.at(state); }
};