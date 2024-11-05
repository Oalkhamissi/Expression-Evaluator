/*!	\file	parser.cpp
	\brief	Parser class implementation.
	\author	Garth Santor
	\date	2021-10-29
	\copyright	Garth Santor, Trinh Han
=============================================================
Revision History
-------------------------------------------------------------

Version 2021.11.01
	C++ 20 validated
	Changed to GATS_TEST

Version 2014.10.31
	Visual C++ 2013

Version 2012.11.13
	C++ 11 cleanup

Version 2009.12.02
	Alpha release.


=============================================================

Copyright Garth Santor/Trinh Han

The copyright to the computer program(s) herein
is the property of Garth Santor/Trinh Han, Canada.
The program(s) may be used and/or copied only with
the written permission of Garth Santor/Trinh Han
or in accordance with the terms and conditions
stipulated in the agreement/contract under which
the program(s) have been supplied.
=============================================================*/

#include <ee/parser.hpp>
#include <ee/function.hpp>
#include <ee/operand.hpp>
#include <ee/operator.hpp>
#include <ee/pseudo_operation.hpp>
#include <stack>
#include <stdexcept>
#include <sstream>

[[nodiscard]] TokenList Parser::parse(TokenList const& infixTokens) {
    TokenList outputQueue;
    std::stack<Token::pointer_type> operatorStack;

    for (auto const& token : infixTokens) {
        if (is<Operand>(token)) {
            outputQueue.push_back(token); // Directly push operands to output
        }
        else if (is<Operator>(token)) {
            auto op = convert<Operator>(token);
            // Handle operator precedence and associativity
            while (!operatorStack.empty() && is<Operator>(operatorStack.top())) {
                auto topOp = convert<Operator>(operatorStack.top());

                // Check precedence and associativity for operator stack management
                bool isLeftAssoc = is<LAssocOperator>(topOp);
                bool higherPrecedence = topOp->precedence() > op->precedence();
                bool equalPrecedence = topOp->precedence() == op->precedence();

                if (higherPrecedence || (isLeftAssoc && equalPrecedence)) {
                    outputQueue.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                else {
                    break;
                }
            }
            operatorStack.push(op);
        }
        else if (is<LeftParenthesis>(token)) {
            operatorStack.push(token);
        }
        else if (is<RightParenthesis>(token)) {
            while (!operatorStack.empty() && !is<LeftParenthesis>(operatorStack.top())) {
                outputQueue.push_back(operatorStack.top());
                operatorStack.pop();
            }
            if (operatorStack.empty() || !is<LeftParenthesis>(operatorStack.top())) {
                throw std::runtime_error("Mismatched parentheses");
            }
            operatorStack.pop(); // Remove the left parenthesis
        }
        else {
            throw std::runtime_error("Unknown token type");
        }
    }

    // Pop remaining operators in the stack to the output queue
    while (!operatorStack.empty()) {
        if (is<Parenthesis>(operatorStack.top())) {
            throw std::runtime_error("Mismatched parentheses");
        }
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }

    return outputQueue;
}
