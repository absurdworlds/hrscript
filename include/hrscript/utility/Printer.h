/*
 * Copyright (C) 2015  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef hrscript_printer_h
#define hrscript_printer_h
#include <aw/hdf/Writer.h>
#include <hrscript/ast/Visitor.h>

namespace aw {
namespace script {
class Printer : public ast::Visitor {
public:
	Printer(aw::io::WriteStream& out);
	virtual ~Printer() = default;

	virtual void visit(ast::FunctionProto& node);
	virtual void visit(ast::Function& node);
	virtual void visit(ast::Variable& node);
	virtual void visit(ast::StatementBlock& node);
	virtual void visit(ast::IfElseStatement& node);
	virtual void visit(ast::NumberExpr& node);
	virtual void visit(ast::StringExpr& node);
	virtual void visit(ast::IdentifierExpr& node);
	virtual void visit(ast::CallExpr& node);
	virtual void visit(ast::UnaryExpr& node);
	virtual void visit(ast::BinaryExpr& node);

private:
	void printSignature(ast::FunctionProto& node);

	aw::hdf::Writer writer;
};
} // namespace script
} // namespace aw
#endif//hrscript_printer_h
