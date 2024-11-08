/*! \file RPNEvaluator.cpp
    \brief RPN Evaluator class implementation.
    \author Garth Santor
    \date 2021-10-29
    \copyright Garth Santor

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

============================================================= */

#include <ee/RPNEvaluator.hpp>
#include <ee/integer.hpp>
#include <ee/real.hpp>
#include <ee/operation.hpp>
#include <ee/operator.hpp>
#include <ee/function.hpp>
#include <cassert>
#include <stack>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/math/special_functions.hpp>
#include <boost/math/constants/constants.hpp>

// Helper function to pop an Integer or Real from the stack, with type-checking
template <typename T>
typename T::value_type pop_operand_from_stack(std::stack<Operand::pointer_type>& operandStack) {
    if (operandStack.empty()) {
        throw std::runtime_error("Error: Operand stack is empty");
    }
    auto operand = std::dynamic_pointer_cast<T>(operandStack.top());
    operandStack.pop();
    if (!operand) {
        throw std::runtime_error("Invalid operand type: Expected " + std::string(typeid(T).name()));
    }
    return operand->value();
}

[[nodiscard]] Operand::pointer_type RPNEvaluator::evaluate(TokenList const& rpnExpression) {
    if (rpnExpression.empty()) {
        throw std::runtime_error("Error: insufficient operands");
    }

    std::stack<Operand::pointer_type> operandStack;

    for (auto const& token : rpnExpression) {
        if (is<Operand>(token)) {
            operandStack.push(convert<Operand>(token));
        }
        else if (is<Operator>(token)) {
            if (is<Identity>(token)) {
                auto value = operandStack.top(); operandStack.pop();
                if (auto intValue = std::dynamic_pointer_cast<Integer>(value)) {
                    operandStack.push(std::make_shared<Integer>(intValue->value()));
                }
                else if (auto realValue = std::dynamic_pointer_cast<Real>(value)) {
                    operandStack.push(std::make_shared<Real>(realValue->value()));
                }
                else {
                    throw std::runtime_error("Invalid operand type for identity");
                }
            }
            else if (is<Negation>(token)) {
                auto value = operandStack.top(); operandStack.pop();
                if (auto intValue = std::dynamic_pointer_cast<Integer>(value)) {
                    operandStack.push(std::make_shared<Integer>(-intValue->value()));
                }
                else if (auto realValue = std::dynamic_pointer_cast<Real>(value)) {
                    operandStack.push(std::make_shared<Real>(-realValue->value()));
                }
                else {
                    throw std::runtime_error("Invalid operand type for negation");
                }
            }
            else if (is<Addition>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();
                if (auto rightInt = std::dynamic_pointer_cast<Integer>(right), leftInt = std::dynamic_pointer_cast<Integer>(left); rightInt && leftInt) {
                    operandStack.push(std::make_shared<Integer>(leftInt->value() + rightInt->value()));
                }
                else if (auto rightReal = std::dynamic_pointer_cast<Real>(right), leftReal = std::dynamic_pointer_cast<Real>(left); rightReal && leftReal) {
                    operandStack.push(std::make_shared<Real>(leftReal->value() + rightReal->value()));
                }
                else {
                    throw std::runtime_error("Invalid operand types for addition");
                }
            }
            else if (is<Subtraction>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();
                if (auto rightInt = std::dynamic_pointer_cast<Integer>(right), leftInt = std::dynamic_pointer_cast<Integer>(left); rightInt && leftInt) {
                    operandStack.push(std::make_shared<Integer>(leftInt->value() - rightInt->value()));
                }
                else if (auto rightReal = std::dynamic_pointer_cast<Real>(right), leftReal = std::dynamic_pointer_cast<Real>(left); rightReal && leftReal) {
                    operandStack.push(std::make_shared<Real>(leftReal->value() - rightReal->value()));
                }
                else {
                    throw std::runtime_error("Invalid operand types for subtraction");
                }
            }
            else if (is<Multiplication>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();
                if (auto rightInt = std::dynamic_pointer_cast<Integer>(right), leftInt = std::dynamic_pointer_cast<Integer>(left); rightInt && leftInt) {
                    operandStack.push(std::make_shared<Integer>(leftInt->value() * rightInt->value()));
                }
                else if (auto rightReal = std::dynamic_pointer_cast<Real>(right), leftReal = std::dynamic_pointer_cast<Real>(left); rightReal && leftReal) {
                    operandStack.push(std::make_shared<Real>(leftReal->value() * rightReal->value()));
                }
                else {
                    throw std::runtime_error("Invalid operand types for multiplication");
                }
            }
            else if (is<Division>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();
                if (auto rightInt = std::dynamic_pointer_cast<Integer>(right), leftInt = std::dynamic_pointer_cast<Integer>(left); rightInt && leftInt) {
                    if (rightInt->value() == 0) throw std::runtime_error("Division by zero");
                    operandStack.push(std::make_shared<Integer>(leftInt->value() / rightInt->value()));
                }
                else if (auto rightReal = std::dynamic_pointer_cast<Real>(right), leftReal = std::dynamic_pointer_cast<Real>(left); rightReal && leftReal) {
                    if (rightReal->value() == 0) throw std::runtime_error("Division by zero");
                    operandStack.push(std::make_shared<Real>(leftReal->value() / rightReal->value()));
                }
                else {
                    throw std::runtime_error("Invalid operand types for division");
                }
            }
            else if (is<Modulus>(token)) {
                auto right = pop_operand_from_stack<Integer>(operandStack);
                auto left = pop_operand_from_stack<Integer>(operandStack);
                operandStack.push(std::make_shared<Integer>(left % right));
            }
            else if (is<Power>(token)) {
                auto exponent = operandStack.top(); operandStack.pop();
                auto base = operandStack.top(); operandStack.pop();

                if (auto baseInt = std::dynamic_pointer_cast<Integer>(base), exponentInt = std::dynamic_pointer_cast<Integer>(exponent); baseInt && exponentInt) {
                    operandStack.push(std::make_shared<Integer>(boost::multiprecision::pow(baseInt->value(), static_cast<unsigned int>(exponentInt->value()))));
                }
                else {
                    // Convert operands to Real if they are not Integer
                    Real::value_type baseValue = (is<Integer>(base)) ? static_cast<Real::value_type>(convert<Integer>(base)->value()) : convert<Real>(base)->value();
                    Real::value_type exponentValue = (is<Integer>(exponent)) ? static_cast<Real::value_type>(convert<Integer>(exponent)->value()) : convert<Real>(exponent)->value();
                    operandStack.push(std::make_shared<Real>(boost::multiprecision::pow(baseValue, exponentValue)));
                }
            }
            else if (is<Factorial>(token)) {
                auto value = pop_operand_from_stack<Integer>(operandStack);
                operandStack.push(std::make_shared<Integer>(Integer::factorial(value)));
            }
            else {
                throw std::runtime_error("Unsupported operator");
            }
        }
         else if (is<Function>(token)) {
             // Handle single-argument functions
             if (is<Abs>(token)) {
                 auto value = operandStack.top();
                 operandStack.pop();
                 if (auto intValue = std::dynamic_pointer_cast<Integer>(value)) {
                     operandStack.push(std::make_shared<Integer>(boost::multiprecision::abs(intValue->value())));
                 }
                 else if (auto realValue = std::dynamic_pointer_cast<Real>(value)) {
                     operandStack.push(std::make_shared<Real>(boost::multiprecision::abs(realValue->value())));
                 }
                 else {
                     throw std::runtime_error("Invalid operand type for absolute value");
                 }
             }
             else if (is<Arccos>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::acos(value)));
             }
             else if (is<Arcsin>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::asin(value)));
             }
             else if (is<Arctan>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::atan(value)));
             }
             else if (is<Ceil>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::ceil(value)));
             }
             else if (is<Floor>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::floor(value)));
             }
             else if (is<Ln>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::log(value)));
             }
             else if (is<Lb>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::log2(value)));
             }
             else if (is<Sin>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::sin(value)));
             }
             else if (is<Cos>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::cos(value)));
             }
             else if (is<Tan>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::tan(value)));
             }
             else if (is<Sqrt>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::sqrt(value)));
             }
             else if (is<Exp>(token)) {
                 auto value = pop_operand_from_stack<Real>(operandStack);
                 operandStack.push(std::make_shared<Real>(boost::multiprecision::exp(value)));
             }
             else if (is<Arctan2>(token)) {
                 auto x_operand = std::dynamic_pointer_cast<Real>(operandStack.top());
                 operandStack.pop();
                 auto y_operand = std::dynamic_pointer_cast<Real>(operandStack.top());
                 operandStack.pop();

                 Real::value_type result_value = boost::multiprecision::atan2(
                     static_cast<Real::value_type>(y_operand->value()),
                     static_cast<Real::value_type>(x_operand->value())
                 );

                 operandStack.push(std::make_shared<Real>(result_value));
             }
             else if (is<Max>(token)) {
                 auto right = operandStack.top(); operandStack.pop();
                 auto left = operandStack.top(); operandStack.pop();
                 if (auto rightInt = std::dynamic_pointer_cast<Integer>(right), leftInt = std::dynamic_pointer_cast<Integer>(left); rightInt && leftInt) {
                     operandStack.push(std::make_shared<Integer>(std::max(leftInt->value(), rightInt->value())));
                 }
                 else if (auto rightReal = std::dynamic_pointer_cast<Real>(right), leftReal = std::dynamic_pointer_cast<Real>(left); rightReal && leftReal) {
                     operandStack.push(std::make_shared<Real>(std::max(leftReal->value(), rightReal->value())));
                 }
                 else {
                     throw std::runtime_error("Invalid operand types for max function");
                 }
             }
             else if (is<Min>(token)) {
                 auto right = operandStack.top(); operandStack.pop();
                 auto left = operandStack.top(); operandStack.pop();
                 if (auto rightInt = std::dynamic_pointer_cast<Integer>(right), leftInt = std::dynamic_pointer_cast<Integer>(left); rightInt && leftInt) {
                     operandStack.push(std::make_shared<Integer>(std::min(leftInt->value(), rightInt->value())));
                 }
                 else if (auto rightReal = std::dynamic_pointer_cast<Real>(right), leftReal = std::dynamic_pointer_cast<Real>(left); rightReal && leftReal) {
                     operandStack.push(std::make_shared<Real>(std::min(leftReal->value() , rightReal->value())));
                 }
                 else {
                     throw std::runtime_error("Invalid operand types for min function");
                 }
             }
             else if (is<Pow>(token)) {
                 auto exponent = operandStack.top(); operandStack.pop();
                 auto base = operandStack.top(); operandStack.pop();
                 if (auto baseInt = std::dynamic_pointer_cast<Integer>(base), exponentInt = std::dynamic_pointer_cast<Integer>(exponent); baseInt && exponentInt) {
                     operandStack.push(std::make_shared<Integer>(boost::multiprecision::pow(baseInt->value(), static_cast<unsigned>(exponentInt->value()))));
                 }
                 else if (auto baseReal = std::dynamic_pointer_cast<Real>(base), exponentReal = std::dynamic_pointer_cast<Real>(exponent); baseReal && exponentReal) {
                     operandStack.push(std::make_shared<Real>(boost::multiprecision::pow(baseReal->value(), exponentReal->value())));
                 }
                 else {
                     throw std::runtime_error("Invalid operand types for Pow function");
                 }
             }
             else {
                 throw std::runtime_error("Unsupported function");
             }
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