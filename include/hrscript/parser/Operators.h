/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_OperatorPrecedence_
#define _hrscript_OperatorPrecedence_
#include <hrscript/lexer/Token.h>

namespace aw {
namespace script {
namespace prec {
enum Level {
	None           = -1,
	Unknown        = 0,
	Assignment     = 1,  // =, any= (except relational)
	LogicalOr      = 2,
	LogicalAnd     = 3,
	BitwiseOr      = 4,
	BitwiseXor     = 5,
	BitwiseAnd     = 6,
	Equality       = 7,  // ==, !=
	Relational     = 8,  // >=, <=, <, >
	Shift          = 9,  // <<, >>
	Additive       = 10, // +, -
	Multiplicative = 11  // *, *, %
};
} // namespace prec

prec::Level getOperatorPrecedence(Token tok)
{
	switch(tok.getType()) {
	default:
		// Not an operator
		return prec::None;
	case tok_equal:
	case tok_ast_equal:
	case tok_plus_equal:
	case tok_minus_equal:
	case tok_slash_equal:
		return prec::Assignment;
	case tok_pipe_pipe:
		return prec::LogicalOr;
	case tok_amp_amp:
		return prec::LogicalAnd;
	case tok_pipe:
		return prec::BitwiseOr;
	case tok_caret:
		return prec::BitwiseXor;
	case tok_amp:
		return prec::BitwiseAnd;
	case tok_equal_equal:
	case tok_bang_equal:
		return prec::Equality;
	case tok_less:
	case tok_less_equal:
	case tok_greater:
	case tok_greater_equal:
		return prec::Relational;
	case tok_plus:
	case tok_minus:
		return prec::Additive;
	case tok_ast:
	case tok_slash:
		return prec::Multiplicative;	
	}
}

bool isRightAssociative(Token tok)
{
	return getOperatorPrecedence(tok) == prec::Assignment;
}

bool isBinaryOperator(Token tok)
{
	return getOperatorPrecedence(tok) > prec::Unknown;
}

bool isOperator(Token tok)
{
	return isBinaryOperator(tok);
}

} // namespace script
} // namespace aw
#endif //_hrscript_OperatorPrecedence_
