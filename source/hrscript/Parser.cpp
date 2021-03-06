/*
 * Copyright (C) 2015  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <cassert>
#include <vector>

#include <hrscript/ast/decl/Variable.h>
#include <hrscript/ast/decl/Function.h>

#include <hrscript/ast/Expression.h>
#include <hrscript/ast/UnaryExpr.h>
#include <hrscript/ast/BinaryExpr.h>
#include <hrscript/ast/CallExpr.h>
#include <hrscript/ast/NumberExpr.h>
#include <hrscript/ast/StringExpr.h>
#include <hrscript/ast/IdentifierExpr.h>

#include <hrscript/ast/Statement.h>
#include <hrscript/ast/IfElseStatement.h>
#include <hrscript/ast/StatementBlock.h>

#include <hrscript/utility/PrintToken.h>
#include <hrscript/diagnostic/DiagnosticsEngine.h>
#include <hrscript/parser/Parser.h>

namespace aw {
namespace script {
uptr<ast::Declaration>
ErrorDeclaration(std::string msg)
{
	//print(msg);
	return nullptr;
}

uptr<ast::Declaration>
Parser::parseDeclaration()
{
	uptr<ast::Declaration> decl;

	switch(token.getType()) {
	case kw_var:
		decl = parseVariableDeclaration();
		break;
	case kw_const:
		decl = parseConstantDeclaration();
		break;
	case kw_func:
		decl = parseFunctionDefinition();
		break;
	case kw_class:
		decl = parseClassDeclaration();
		break;
	default:
		return nullptr;
	}

	/* TODO: do not forget about global variables
	if (token.getType() != tok_semicolon)
		return nullptr;
		*/

	return decl;
}

/*
 * variableDecl ::= 'var' id id
 */
uptr<ast::Variable>
Parser::parseVariableDeclaration()
{
	// Read variable type
	if (!isTypeName(token))
		return nullptr;

	// Read variable name
	if (!isIdentifier(getNextToken()))
		return nullptr;
	
	// TODO: symbol table lookup
	std::string name = token.getData();

	// Variable* var = new Variable(/*symbol*/, /*thingy*/)// TODO ;

	return ast::Variable::create(name);
}

/*
 * constantDecl ::= 'const' id id '=' expr
 */
uptr<ast::Declaration>
Parser::parseConstantDeclaration()
{
	return ErrorDeclaration("NYI");
#if 0
	// Read variable type
	if (!isTypeName(getNextToken()))
		return 0;

	// Read variable name
	if (!isIdentifier(getNextToken()))
		return 0;

	// TODO: symbol table lookup
	std::string name = token.getData();

	// Read constant initializer
	if (getNextToken().getType() != tok_equals)
		return 0;

	ast::Expression* initializer = parseExpression();

	// Constant* constant = new Constant(/*symbol*/, /*thingy*/); // TODO 

	return new ConstantDeclaration(name, initializer);
#endif
}

bool Parser::match(TokenType expected)
{
	if (token.getType() != expected)
		return false;

	// consume token
	getNextToken();
	return true;
}

/*
 * functionDecl ::= 'func' id id '(' args ')'
 *         args ::= arg (',' args)?
 *          arg ::= variableDecl
 */
uptr<ast::FunctionProto>
Parser::parseFunctionPrototype()
{
	// Return type
	if (!isTypeName(token))
		return nullptr;

	std::string ret = token.getData();

	// Function name
	if (!isIdentifier(getNextToken()))
		return nullptr;

	// TODO: symbol table lookup
	std::string name = token.getData();

	// consume identifier
	getNextToken();
	if (!match(tok_l_paren))
		return nullptr;

	// Argument list
	std::vector<uptr<ast::Variable>> args;

	// TODO: 'var int a'  or  'int a'
	// while ((token == tok_identifier) || (token == kw_var)) {
	while (match(kw_var)) {
		auto arg = parseVariableDeclaration();
		if (!arg)
			return nullptr;

		args.push_back(std::move(arg));

		getNextToken();

		if (token.getType() == tok_r_paren)
			break;

		if (!match(tok_comma))
			return unexpectedTokenError(tok_comma);
	}
	
	if (!match(tok_r_paren)) {
		Diagnostic msg(Location(), Diagnostic::ExpectedVariableDecl);
		diag.report(msg);
		// error(Diagnostic::err_unexpected_token);
		return nullptr;
	}

	return ast::FunctionProto::create(name, ret, std::move(args));
}
/*
std::nullptr_t Parser::error(Diagnostic::ID id)
{
	return nullptr;
}*/

/*
 * functionDef  ::= functionDecl '{' stmts '}'
 */
uptr<ast::Declaration>
Parser::parseFunctionDefinition()
{
	getNextToken(); // consume 'func'

	auto proto = parseFunctionPrototype();

	if (!proto)
		return nullptr;

	if (token.getType() == tok_semicolon)
		return std::move(proto);

	auto body = parseStatementBlock();

	if (!body)
		return nullptr;

	return ast::Function::create(
	        std::move(proto), std::move(body));
}

/*
 *      classDecl ::= 'class' id
 *          class ::= classDecl '{' classBodyDecls '}'
 * classBodyDecl  ::= functionDecl | varDecl
 * classBodyDecls ::= classBodyDecl ';' classBodyDecls?
 */
uptr<ast::Declaration>
Parser::parseClassDeclaration()
{
	return ErrorDeclaration("NYI");
#if 0
	// Class name
	if (!isIdentifier(getNextToken()))
		return 0;

	std::string name = token.getData();

	if (!getNextToken().getType() != tok_l_brace)
		return new ClassDeclaration(name);

	// Class members
	std::vector<Variable*> members;
	while (getNextToken().getType() == kw_var) {
		auto var = parseVariableDeclaration();
		if (var == 0)
			return 0;

		members.push_back(var);

		getNextToken();

		if (token != tok_semicolon)
			return 0;
	}

	if (!getNextToken().getType() != tok_r_brace)
		return 0;

	return new ClassDeclaration(name, members);
#endif
}

uptr<ast::Statement>
Parser::parseStatement()
{
	switch (token.getType()) {
	case kw_if:
		getNextToken(); // consume 'if'
		return parseBranchStatement();
	case tok_l_brace:
		return parseStatementBlock();
	default:
		return parseExprStatement();
	}
}

uptr<ast::Statement>
Parser::parseExprStatement()
{
	auto expr = parseExpression();

	if (!match(tok_semicolon)) {
		Diagnostic msg(Location(),
		               Diagnostic::ExpectedSemicolonAfterExpression);
		diag.report(msg);
		return nullptr;
	}

	return std::move(expr);
}

uptr<ast::StatementBlock>
Parser::parseStatementBlock()
{
	if (!match(tok_l_brace))
		return nullptr; // expected '{'

	std::vector<uptr<ast::Statement>> statements;
	while (!match(tok_r_brace)) {
		auto statement = parseStatement();
		if(!statement)
			return nullptr;

		statements.push_back(std::move(statement));
	}

	return std::make_unique<ast::StatementBlock>(std::move(statements));
}

uptr<ast::Statement>
Parser::parseBranchStatement()
{
	if (!match(tok_l_paren))
		return unexpectedTokenError(tok_l_paren);

	uptr<ast::Expression> ifExpr = parseExpression();
	if (!ifExpr)
		return nullptr;

	if (!match(tok_r_paren))
		return unexpectedTokenError(tok_r_paren);

	uptr<ast::Statement> ifBody = parseStatement();
	if (!ifBody)
		return nullptr;

	uptr<ast::Statement> elseBody = nullptr;

	if (match(kw_else)) {
		elseBody = parseStatement();

		if (!elseBody)
			return nullptr;
	}

	return std::make_unique<ast::IfElseStatement>(
	        std::move(ifExpr), std::move(ifBody), std::move(elseBody));
}

/********************** Expressions **********************/
uptr<ast::Expression>
Parser::parseExpression()
{
	// Parse left hand side
	uptr<ast::Expression> lhs = parseUnaryExpr();

	if (!lhs)
		return nullptr;

	return parseBinaryExpr(std::move(lhs), prec::Unknown);
}

uptr<ast::Expression>
Parser::parsePrimaryExpr()
{
	switch(token.getType()) {
	case tok_l_paren:
		return parseParenExpr();
	case tok_identifier:
		return parseIdentifierExpr();
	case tok_numeric_constant:
		return parseNumberExpr();
	case tok_string_literal:
		return parseStringExpr();
	default:
		return 0; // expected expression
	}
}

uptr<ast::Expression>
Parser::parseParenExpr()
{
	if (!match(tok_l_paren))
		return unexpectedTokenError(tok_l_paren); // Expected (

	uptr<ast::Expression> expr = parseExpression();

	if (!match(tok_r_paren))
		return unexpectedTokenError(tok_r_paren);

	return expr;
}

uptr<ast::Expression>
Parser::parseBinaryExpr(uptr<ast::Expression> LHS,
                        prec::Level minPrec)
{
	while (1) {
		prec::Level curPrec = getOperatorPrecedence(token);
		if (curPrec < minPrec)
			return LHS;

		Token opcode = token;
		getNextToken(); // consume operator

		uptr<ast::Expression> RHS = parseUnaryExpr();
		if(!RHS)
			return nullptr;


		prec::Level nextPrec = getOperatorPrecedence(token);
		bool isRightAssoc = isRightAssociative(token);

		if (curPrec < nextPrec ||
		   (curPrec == nextPrec && isRightAssoc)) {
			RHS = parseBinaryExpr(
			       std::move(RHS),
			       prec::Level(curPrec + !isRightAssoc));
			if (!RHS)
				return nullptr;
		}

		LHS = std::make_unique<ast::BinaryExpr>(
		       opcode.getType(), std::move(LHS), std::move(RHS));
	}
}

uptr<ast::Expression>
Parser::parseUnaryExpr()
{
	if (!isOperator(token))
		return parsePrimaryExpr();

	Token opcode = token;
	getNextToken(); // consume operator

	uptr<ast::Expression> operand = parseUnaryExpr();
	if (!operand)
		return nullptr;

	return std::make_unique<ast::UnaryExpr>(
	        opcode.getType(), std::move(operand));
}

uptr<ast::Expression>
Parser::parseIdentifierExpr()
{
	std::string name = token.getData();

	getNextToken(); // consume identifier
	if (match(tok_l_paren))
		return parseCallExpr(name);
	
	// TODO: postfix operators

	return std::make_unique<ast::IdentifierExpr>(name);
}

uptr<ast::Expression>
Parser::parseCallExpr(std::string func)
{
	std::vector<uptr<ast::Expression>> args;

	if (!match(tok_r_paren)) {
		while (true) {
			auto arg = parseExpression();

			if (!arg)
				return nullptr;

			args.push_back(std::move(arg));

			if (match(tok_r_paren))
				break;

			if (!match(tok_comma))
				return unexpectedTokenError(tok_comma); // expected ,
		}
	}

	return std::make_unique<ast::CallExpr>(
	        std::move(func), std::move(args));
}

uptr<ast::Expression>
Parser::parseStringExpr()
{
	assert(token == tok_string_literal && "parseStringExpr called when there's no string literal!");

	Token tok = token;
	// Consume string
	getNextToken();

	return std::make_unique<ast::StringExpr>(tok.getData());
}

uptr<ast::Expression>
Parser::parseNumberExpr()
{
	assert(token == tok_numeric_constant && "parseNumberExpr called when there's no number!");

	// store token, because we need to consume it
	Token tok = token;
	// consume number
	getNextToken();

	return std::make_unique<ast::NumberExpr>(tok.getData());
}

// Print out diagnostic and return nullptr
// Assumes that it is called after failed match()
std::nullptr_t Parser::unexpectedTokenError(TokenType expected)
{
	Diagnostic msg(Location(), Diagnostic::UnexpectedToken);
	msg << spellToken(expected) << spellToken(token);
	diag.report(msg);

	return nullptr;
}
} // namespace hrscript
} // namespace aw
