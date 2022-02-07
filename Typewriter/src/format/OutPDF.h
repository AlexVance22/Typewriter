#pragma once


class PDFConvertError : public std::exception
{
private:
	uint32_t m_stage = 0;

public:
	PDFConvertError(const char* msg, uint32_t stage);

	uint32_t getStage() const noexcept;
};


void outPDF(const std::string& infile, const std::string& outfile);