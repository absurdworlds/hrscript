/*
 * Copyright (C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <hrscript/common/types.h>
#include <hrscript/lexer/Lexer.h>
namespace hrscript {
Lexer::Lexer(OctetStream& stream)
	: stream(stream)
{
	// Setup keywords
	kwmap
	.add("const", KEYWORD(const))
	.add("var", KEYWORD(var))
	.add("func", KEYWORD(func))
	.add("if", KEYWORD(if))
	.add("else", KEYWORD(else))
	.add("class", KEYWORD(class))
	.add("prototype", KEYWORD(prototype))
	.add("instance", KEYWORD(instance))
	.add("return", KEYWORD(return))
	.add("void", KEYWORD(void))
	.add("float", KEYWORD(float))
	.add("int", KEYWORD(int))
	.add("string", KEYWORD(string))
	.add("import", KEYWORD(import))
	.add("import", KEYWORD(export));

	// Extract first token
	getNextToken();
}

void Lexer::init()
{
}

Token Lexer::getCurrentToken()
{
	return curToken;
}

/*!
 * Extract next token from stream
 */
Token Lexer::getNextToken()
{
	lexNextToken(curToken);
	return getCurrentToken();
}

bool Lexer::lexIdentifier(Token& token)
{
	char c;
	std::string id;

	stream.getCurrent(c);
	while (isalnum(c) || c == '_') {
		id += c;
		stream.getNext(c);
	}

	token.setData(id);

	// Check if token is a reserved keyword
	auto kind = kwmap.get(token.getData());

	if (!kind)
		kind = TOKEN(identifier);

	token.setType(kind);
	
	return true;
}

bool Lexer::lexNumericConstant(Token& token)
{
	std::string num;
	char c;
	char prev;
	stream.getCurrent(c);

	while (isalnum(c) || c == '.') {
		num += c;
		prev = c;
		stream.getNext(c);
	}

	if ((c == '-' || c == '+') && (prev == 'e' || prev == 'E')) {
		do {
			num += c;
			stream.getNext(c);
		} while (isalnum(c) || c == '.');
	}

	token.setData(num);
	token.setType(tok_numeric_constant);

	return true;
}

bool Lexer::lexStringLiteral(Token& token)
{
	std::string str;
	char c;
	stream.getCurrent(c);

	while (c != '"') {
		if (c == '\\') {
			stream.getNext(c);
		}
		str += c;
		stream.getNext(c);
	}

	stream.getNext(c); // consume '"'

	token.setData(str);
	token.setType(tok_string_literal);

	return true;
}

bool Lexer::lexIllegalToken(Token& token)
{
	std::string str;
	char c;

	while (!isspace(c)) {
		str += c;
		stream.getNext(c);
	}

	token.setData(str);
	token.setType(tok_illegal);

	return true;
}

void Lexer::skipLineComment()
{
	char c;
	// crude comment handling
	do {
		stream.getNext(c);
	} while (c != '\n');
}

void Lexer::skipBlockComment()
{
	while (true) {
		char c;
		char prev;
		stream.getNext(c);
		// TODO: Inefficient! Check multiple chars at once
		while (c != '/' && c != 0) {
			prev = c;
			stream.getNext(c);
		}
		if (prev == '*' || c == 0) {
			stream.getNext(c);
			break;
		}
	}
}

#if 0
void Lexer::handleComment()
{
	char c;

	stream.peek(c);

	assert((c == '*' || c == '/')
	       && "Lexer::handleComment called without a comment");
}
#endif

void Lexer::handleComment()
{
	char c;

checkForComment:
	stream.peek(c);

	if (c == '*') {
		skipBlockComment();
	} else if (c == '/') {
		skipLineComment();
	} else {
		// Not a comment - we're done.
		return;
	}
	
	// Instead of going through everything again, we do everything here.
	// Skip whitespace and check for more comments
	stream.getCurrent(c);
	while(isspace(c))
		stream.getNext(c);

	// Manually optimise tail call
	goto checkForComment;
}

/*!
 * This function is an actual lexer implementation.
 * It reads characters from stream and produces tokens
 */
bool Lexer::lexNextToken(Token& tok)
{
	char c;

lexNextToken:
	stream.getCurrent(c);

	while(isspace(c))
		stream.getNext(c);

	switch (c) {
	case 0:
		tok.setType(tok_eof);
		return true;
	/* Numeric constants */
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return lexNumericConstant(tok);
	/* String literal */
	case '"':
		stream.getNext(c); // consume '"'
		return lexStringLiteral(tok);
	/* Identifier */
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
	case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
	case 'v': case 'w': case 'x': case 'y': case 'z':
	case '_':
		return lexIdentifier(tok);
	/* Operators */
	case '^':
		tok.setType(tok_caret);
		break;
	case '~':
		tok.setType(tok_tilde);
		break;
	case ',':
		tok.setType(tok_comma);
		break;
	case '.':
		tok.setType(tok_dot);
		break;
	case ';':
		tok.setType(tok_semicolon);
		break;
	case '%':
		tok.setType(tok_percent);
		break;
	case '{':
		tok.setType(tok_l_brace);
		break;
	case '}':
		tok.setType(tok_r_brace);
		break;
	case '[':
		tok.setType(tok_l_square);
		break;
	case ']':
		tok.setType(tok_r_square);
		break;
	case '(':
		tok.setType(tok_l_paren);
		break;
	case ')':
		tok.setType(tok_r_paren);
		break;
	case '&':
		stream.peek(c);
		if (c == '&') {
			tok.setType(tok_amp_amp);
			stream.getNext(c);
		} else {
			tok.setType(tok_amp);	
		}
		// TODO: does daedalus have '&=' operator?
		break;
	case '|':
		stream.peek(c);
		if (c == '|') {
			tok.setType(tok_pipe_pipe);
			stream.getNext(c);
		} else {
			tok.setType(tok_pipe);
		}
		// TODO: does daedalus have '|= operator?
	case '!':
		stream.peek(c);
		if (c == '=') {
			tok.setType(tok_bang_equal);
			stream.getNext(c);
		} else {
			tok.setType(tok_bang);
		}
		break;
	case '*':
		stream.peek(c);
		if (c == '=') {
			tok.setType(tok_ast_equal);
			stream.getNext(c);
		} else {
			tok.setType(tok_ast);
		}
		break;
	case '/':
		// Look for comments first
		handleComment();

		// Check what we have, after we're done with comments
		// If we have '/', continue handling this case.
		// If we have something different, restart lexer.
		// TODO: I could've just restarted the lexer regardless,
		// is this optimization necessary?
		stream.getCurrent(c);
		if (c != '/')
			// We didn't lex anything, restart the lexer.
			goto lexNextToken;

		stream.peek(c);
		if (c == '=') {
			tok.setType(tok_slash_equal);
			stream.getNext(c);
		} else {
			tok.setType(tok_slash);
		}
		break;
	case '=':
		stream.peek(c);
		if (c == '=') {
			tok.setType(tok_equal_equal);
			stream.getNext(c);
		} else {
			tok.setType(tok_equal);
		}
		break;
	case '+':
		stream.peek(c);
		if (c == '+') {
			tok.setType(tok_plus_plus);
			stream.getNext(c);
		} else if (c == '=') {
			tok.setType(tok_plus_equal);
			stream.getNext(c);
		} else {
			tok.setType(tok_plus);
		}
		break;
	case '-':
		stream.peek(c);
		if (c == '-') {
			tok.setType(tok_minus_minus);
			stream.getNext(c);
		} else if (c == '=') {
			tok.setType(tok_minus_equal);
			stream.getNext(c);
		} else {
			tok.setType(tok_minus);
		}
		break;
	case '<':
		stream.peek(c);
		if (c == '<') {
			tok.setType(tok_less_less);
			stream.getNext(c);
		} else if (c == '=') {
			tok.setType(tok_less_equal);
			stream.getNext(c);
		} else {
			tok.setType(tok_less);
		}
		break;
	case '>':
		stream.peek(c);
		if (c == '>') {
			tok.setType(tok_greater_greater);
			stream.getNext(c);
		} else if (c == '=') {
			tok.setType(tok_less_equal);
			stream.getNext(c);
		} else {
			tok.setType(tok_less);
		}
		break;
	/* Illegal token */
	default:
		// TODO: handle unicode
		return lexIllegalToken(tok);
	}

	stream.getNext(c);
	return true;
}
} // namespace hrscript
