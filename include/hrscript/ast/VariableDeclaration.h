/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_FuncDeclaration_
#define _hrscript_AST_FuncDeclaration_
#include <string>
#include <hrscript/ast/Declaration.h>
#include <hrscript/ast/Expression.h>
namespace hrscript {
namespace ast {
class VariableDeclaration : public Declaration {
public:
	VariableDeclaration(std::string id/*, Expression* val = 0*/);
	virtual ~VariableDeclaration();

	virtual accept(ast::Visitor& visitor) = 0;
private:
	std::string name;
	// IIRC Daedalus doesn't support variable initializaion
	//Expression* val;
}

class ConstantDeclaration : public Declaration {
public:
	ConstantDeclaration(std::string id, Expression* val);
	virtual ~ConstantDeclaration();

	virtual accept(ast::Visitor& visitor) = 0;
private:
	std::string name;
	Expression* val;
}
} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_FuncDeclaration_