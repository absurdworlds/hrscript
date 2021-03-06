/*
 * Copyright (C) 2015   Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef _hrscript_AST_StatementBlock_
#define _hrscript_AST_StatementBlock_
#include <vector>

#include <hrscript/ast/Statement.h>

namespace aw {
namespace script {
namespace ast {

class StatementBlock : public Statement {
public:
	StatementBlock(std::vector<std::unique_ptr<Statement>> statements)
		: statements(std::move(statements))
	{
	}

	virtual ~StatementBlock() = default;

	virtual void accept(ast::Visitor& visitor)
	{
		visitor.visit(*this);
	}

	std::vector<uptr<Statement>>& getStatements()
	{
		return statements;
	}
private:
	std::vector<std::unique_ptr<Statement>> statements;
};
} // namespace ast
} // namespace script
} // namespace aw
#endif//_hrscript_AST_StatementBlock_
