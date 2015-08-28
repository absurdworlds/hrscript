/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_lexer_
#define _hrscript_lexer_
#include <hrscript/lexer/OctetStream.h>
#include <hrscript/lexer/Token.h>
namespace hrscript {
class Lexer {
public:
	Lexer(OctetStream& stream)
		: stream(stream)
	{
	}

	void init();
	Token getCurrentToken();
	Token getNextToken();

private:
	bool lexIllegalToken(Token& token);
	bool lexIdentifier(Token& token);
	bool lexStringLiteral(Token& token);
	bool lexNumericConstant(Token& token);
	bool lexNextToken(Token& token);
	void skipLineComment();
	void skipBlockComment();
	void handleComment();

	Token curToken;
	OctetStream& stream;
};
} // namespace hrscript
#endif//_hrscript_lexer_