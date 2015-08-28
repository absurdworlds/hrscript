/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_UnaryExpr_
#define _hrscript_AST_UnaryExpr_
#include <hrscript/ast/Expression.h>

namespace hrscript {
namespace ast {

class UnaryExpr : public Expression {
public:
	UnaryExpr(int op, Expression* operand)
		: op(op), expr(operand)
	{
	}

	virtual Expression* getOperand();
	virtual int getOperation();

	virtual void accept(ast::Visitor& visitor)
	{
		visitor.visit(*this);
	}
private:
	Expression* expr;
	int op;
};

} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_UnaryExpr_
