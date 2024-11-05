/*!	\file	RPNEvaluator.cpp
	\brief	RPN Evaluator class implementation.
	\author	Garth Santor
	\date	2021-10-29
	\copyright	Garth Santor, Trinh Han

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.11.01
	C++ 20 validated
	Changed to GATS_TEST

Version 2012.11.13
	C++ 11 cleanup

Version 2009.12.10
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

#include <ee/RPNEvaluator.hpp>
#include <ee/integer.hpp>
#include <ee/operation.hpp>
#include <ee/operator.hpp> 
#include <cassert>
#include <stack>
#include <stdexcept>
#include <vector>

[[nodiscard]] Operand::pointer_type RPNEvaluator::evaluate(TokenList const& rpnExpression) {
    // Check if the input TokenList is empty and throw an exception if it is
    if (rpnExpression.empty()) {
        throw std::runtime_error("Error: insufficient operands");
    }

    std::stack<Operand::pointer_type> operandStack;

    for (auto const& token : rpnExpression) {
        if (is<Operand>(token)) {
            operandStack.push(convert<Operand>(token));
        }
        else if (is<Operator>(token)) {
            auto op = convert<Operator>(token);
            if (operandStack.size() < op->number_of_args()) {
                throw std::runtime_error("Error: insufficient operands");
            }

            std::vector<Operand::pointer_type> operands;
            for (unsigned i = 0; i < op->number_of_args(); ++i) {
                operands.push_back(operandStack.top());
                operandStack.pop();
            }

            Operand::pointer_type result = op->evaluate(operands);
            operandStack.push(result);
        }
        else {
            throw std::runtime_error("Unknown token type encountered");
        }
    }

    if (operandStack.size() != 1) {
        throw std::runtime_error("Error: too many operands");
    }

    return operandStack.top();
}