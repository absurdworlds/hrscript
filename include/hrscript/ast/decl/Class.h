/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_ClassDeclaration_
#define _hrscript_AST_ClassDeclaration_
#include <hrscript/ast/Declaration.h>
#include <hrscript/ast/Statement.h>
namespace hrscript {
namespace ast {
/* TODO: give this class a more suitable name
 * (prototype is not a new type, but rather just a template for
 * an instance, or at least it seemes to be so)
 */
class TypeDeclaration : public Declaration {
public:
	virtual ~TypeDeclaration();

	virtual void accept(ast::Visitor& visitor) = 0;
}

class ClassDeclaration : public TypeDeclaration {
public:
	virtual ~ClassDeclaration();

	virtual void accept(ast::Visitor& visitor);
private:
	std::string name;
	std::vector<VariableDeclaration*> body;
};

class PrototypeDeclaration : public TypeDeclaration {
public:
	virtual ~ClassDeclaration();

	virtual void accept(ast::Visitor& visitor);
protected:
	std::string name;
	TypeDeclaration* base;
	// It seems like prototype allows only assignments inside it,
	// while instance additionally allows at least function calls
	std::vector<Statement*> stmts;
};

class InstanceDeclaration : public Declaration {
public:
	virtual ~ClassDeclaration();

	virtual void accept(ast::Visitor& visitor);
private:
	std::string name;
	TypeDeclaration* base;
	std::vector<Statement*> stmts;
};
} // namespace ast
} // namespace hrscript
#endif//_hrscript_AST_ClassDeclaration_
