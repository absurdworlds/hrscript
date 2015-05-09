/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _daedalus_AST_StringExpr_
#define _daedalus_AST_StringExpr_
#include <daedalus/ast/Expression.h>

namespace daedalus {
namespace ast {
/*! Expression for numeric literals */
class StringExpr : public Expression {
public:
	StringExpr(std::string value);

	virtual ~StringExpr();

	virtual accept(ast::Visitor& visitor) = 0;
private:
	std::string value;
};

} // namespace ast
} // namespace daedalus
#endif//_daedalus_AST_StringExpr_
