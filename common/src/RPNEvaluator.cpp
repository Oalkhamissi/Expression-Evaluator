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
#include <ee/boolean.hpp>
#include <ee/operation.hpp>
#include <ee/operator.hpp>
#include <ee/function.hpp>
#include <ee/variable.hpp>
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

// Helper function to pop an Operand::pointer_type from the stack
inline typename T::value_type pop_operand_from_stack(std::stack<Operand::pointer_type>& operandStack) {
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
                auto operand = operandStack.top();
                operandStack.pop();

                if (auto var = std::dynamic_pointer_cast<Variable>(operand)) {
                    if (!var->value()) {  // Check if the variable has been assigned a value
                        throw std::runtime_error("Error: variable not initialized");
                    }
                }

                // Proceed with negation if operand is an Integer or Real
                if (auto intValue = std::dynamic_pointer_cast<Integer>(operand)) {
                    operandStack.push(std::make_shared<Integer>(-intValue->value()));
                }
                else if (auto realValue = std::dynamic_pointer_cast<Real>(operand)) {
                    operandStack.push(std::make_shared<Real>(-realValue->value()));
                }
                else {
                    throw std::runtime_error("Invalid operand type for negation");
                }
            }
            else if (is<Assignment>(token)) {
                auto valueOperand = operandStack.top(); operandStack.pop();
                auto variableOperand = operandStack.top(); operandStack.pop();

                auto variable = std::dynamic_pointer_cast<Variable>(variableOperand);
                if (!variable) {
                    throw std::runtime_error("Error: assignment to a non-variable.");
                }

                // Ensure the value is properly assigned based on operand type
                if (auto boolValue = std::dynamic_pointer_cast<Boolean>(valueOperand)) {
                    variable->set(std::make_shared<Boolean>(boolValue->value()));
                }
                else if (auto intValue = std::dynamic_pointer_cast<Integer>(valueOperand)) {
                    variable->set(std::make_shared<Integer>(intValue->value()));
                }
                else if (auto realValue = std::dynamic_pointer_cast<Real>(valueOperand)) {
                    variable->set(std::make_shared<Real>(realValue->value()));
                }
                else {
                    throw std::runtime_error("Error: Unsupported assignment operand type.");
                }

                operandStack.push(variable); // Push the variable with the new value back onto the stack
            }
            else if (is<Addition>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (auto leftInt = std::dynamic_pointer_cast<Integer>(left), rightInt = std::dynamic_pointer_cast<Integer>(right); leftInt && rightInt) {
                    operandStack.push(std::make_shared<Integer>(leftInt->value() + rightInt->value()));
                }
                else {
                    Real::value_type leftValue = (is<Integer>(left)) ? static_cast<Real::value_type>(convert<Integer>(left)->value()) : convert<Real>(left)->value();
                    Real::value_type rightValue = (is<Integer>(right)) ? static_cast<Real::value_type>(convert<Integer>(right)->value()) : convert<Real>(right)->value();
                    operandStack.push(std::make_shared<Real>(leftValue + rightValue));
                }
            }
            else if (is<Subtraction>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (auto leftInt = std::dynamic_pointer_cast<Integer>(left), rightInt = std::dynamic_pointer_cast<Integer>(right); leftInt && rightInt) {
                    operandStack.push(std::make_shared<Integer>(leftInt->value() - rightInt->value()));
                }
                else {
                    Real::value_type leftValue = (is<Integer>(left)) ? static_cast<Real::value_type>(convert<Integer>(left)->value()) : convert<Real>(left)->value();
                    Real::value_type rightValue = (is<Integer>(right)) ? static_cast<Real::value_type>(convert<Integer>(right)->value()) : convert<Real>(right)->value();
                    operandStack.push(std::make_shared<Real>(leftValue - rightValue));
                }
            }
            else if (is<Multiplication>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (auto leftInt = std::dynamic_pointer_cast<Integer>(left), rightInt = std::dynamic_pointer_cast<Integer>(right); leftInt && rightInt) {
                    operandStack.push(std::make_shared<Integer>(leftInt->value() * rightInt->value()));
                }
                else {
                    Real::value_type leftValue = (is<Integer>(left)) ? static_cast<Real::value_type>(convert<Integer>(left)->value()) : convert<Real>(left)->value();
                    Real::value_type rightValue = (is<Integer>(right)) ? static_cast<Real::value_type>(convert<Integer>(right)->value()) : convert<Real>(right)->value();
                    operandStack.push(std::make_shared<Real>(leftValue * rightValue));
                }
            }
            else if (is<Division>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (auto leftInt = std::dynamic_pointer_cast<Integer>(left), rightInt = std::dynamic_pointer_cast<Integer>(right); leftInt && rightInt) {
                    if (rightInt->value() == 0) throw std::runtime_error("Division by zero");
                    operandStack.push(std::make_shared<Integer>(leftInt->value() / rightInt->value()));
                }
                else {
                    Real::value_type leftValue = (is<Integer>(left)) ? static_cast<Real::value_type>(convert<Integer>(left)->value()) : convert<Real>(left)->value();
                    Real::value_type rightValue = (is<Integer>(right)) ? static_cast<Real::value_type>(convert<Integer>(right)->value()) : convert<Real>(right)->value();
                    if (rightValue == 0) throw std::runtime_error("Division by zero");
                    operandStack.push(std::make_shared<Real>(leftValue / rightValue));
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
                    if (exponentInt->value() < 0) {
                        operandStack.push(std::make_shared<Real>(Real::value_type(1.0) / boost::multiprecision::pow(static_cast<Real::value_type>(baseInt->value()), static_cast<unsigned int>(-exponentInt->value()))));
                    }
                    else {
                        operandStack.push(std::make_shared<Integer>(boost::multiprecision::pow(baseInt->value(), static_cast<unsigned int>(exponentInt->value()))));
                    }
                }
                else {
                    Real::value_type baseValue = (is<Integer>(base)) ? static_cast<Real::value_type>(convert<Integer>(base)->value()) : convert<Real>(base)->value();
                    Real::value_type exponentValue = (is<Integer>(exponent)) ? static_cast<Real::value_type>(convert<Integer>(exponent)->value()) : convert<Real>(exponent)->value();
                    operandStack.push(std::make_shared<Real>(boost::multiprecision::pow(baseValue, exponentValue)));
                }
            }
            else if (is<And>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                auto lhs = std::dynamic_pointer_cast<Boolean>(left);
                auto rhs = std::dynamic_pointer_cast<Boolean>(right);

                if (!lhs || !rhs) {
                    throw std::runtime_error("Error: AND operation requires boolean operands.");
                }

                operandStack.push(std::make_shared<Boolean>(lhs->value() && rhs->value()));
}
            else if (is<Or>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                auto lhs = std::dynamic_pointer_cast<Boolean>(left);
                auto rhs = std::dynamic_pointer_cast<Boolean>(right);

                if (!lhs || !rhs) {
                    throw std::runtime_error("Error: OR operation requires boolean operands.");
                }

                operandStack.push(std::make_shared<Boolean>(lhs->value() || rhs->value()));
            }
            else if (is<Not>(token)) {
                auto operand = operandStack.top();
                operandStack.pop();

                // Ensure operand is non-null and of Boolean type
                if (!operand) {
                    throw std::runtime_error("Error: Null operand encountered in NOT operation.");
                }

                std::shared_ptr<Boolean> value;
                if (is<Variable>(operand)) {
                    auto variable = std::dynamic_pointer_cast<Variable>(operand);
                    if (!variable->value()) {
                        throw std::runtime_error("Error: variable not initialized in NOT operation.");
                    }
                    value = std::dynamic_pointer_cast<Boolean>(variable->value());
                }
                else {
                    value = std::dynamic_pointer_cast<Boolean>(operand);
                }

                if (!value) {
                    throw std::runtime_error("Error: NOT operation requires a boolean operand.");
                }

                operandStack.push(std::make_shared<Boolean>(!value->value()));

            }
            else if (is<Nand>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();
                auto lhs = std::dynamic_pointer_cast<Boolean>(left);
                auto rhs = std::dynamic_pointer_cast<Boolean>(right);

                if (!lhs || !rhs) {
                    throw std::runtime_error("Error: Null operand encountered in Nand operation.");
                }

                operandStack.push(std::make_shared<Boolean>(!(lhs->value() && rhs->value())));
            }
            else if (is<Nor>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();
                auto lhs = std::dynamic_pointer_cast<Boolean>(left);
                auto rhs = std::dynamic_pointer_cast<Boolean>(right);

                if (!lhs || !rhs) {
                    throw std::runtime_error("Error: Null operand encountered in Nor operation.");
                }

                operandStack.push(std::make_shared<Boolean>(!(lhs->value() || rhs->value())));
            }
            else if (is<Xor>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();
                auto lhs = std::dynamic_pointer_cast<Boolean>(left);
                auto rhs = std::dynamic_pointer_cast<Boolean>(right);

                if (!lhs || !rhs) {
                    throw std::runtime_error("Error: Null operand encountered in Xor operation.");
                }

                operandStack.push(std::make_shared<Boolean>(lhs->value() != rhs->value()));
            }
            else if (is<Xnor>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();
                auto lhs = std::dynamic_pointer_cast<Boolean>(left);
                auto rhs = std::dynamic_pointer_cast<Boolean>(right);

                if (!lhs || !rhs) {
                    throw std::runtime_error("Error: Null operand encountered in Xnor operation.");
                }

                operandStack.push(std::make_shared<Boolean>(lhs->value() == rhs->value()));
            }
            else if (is<Equality>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (!left || !right) {
                    throw std::runtime_error("Error: Null operand encountered in Equality operation.");
                }

                bool result = (is<Integer>(left) && is<Integer>(right))
                    ? convert<Integer>(left)->value() == convert<Integer>(right)->value()
                    : (is<Real>(left) && is<Real>(right))
                    ? convert<Real>(left)->value() == convert<Real>(right)->value()
                    : (is<Boolean>(left) && is<Boolean>(right))
                    ? convert<Boolean>(left)->value() == convert<Boolean>(right)->value()
                    : (is<Integer>(left) && is<Real>(right))
                    ? static_cast<Real::value_type>(convert<Integer>(left)->value()) == convert<Real>(right)->value()
                    : (is<Real>(left) && is<Integer>(right))
                    ? convert<Real>(left)->value() == static_cast<Real::value_type>(convert<Integer>(right)->value())
                    : throw std::runtime_error("Unsupported operand types for Equality");

                operandStack.push(std::make_shared<Boolean>(result));
                }
            else if (is<Inequality>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (!left || !right) {
                    throw std::runtime_error("Error: Null operand encountered in Inequality operation.");
                }

                bool result = (is<Integer>(left) && is<Integer>(right))
                    ? convert<Integer>(left)->value() != convert<Integer>(right)->value()
                    : (is<Real>(left) && is<Real>(right))
                    ? convert<Real>(left)->value() != convert<Real>(right)->value()
                    : (is<Boolean>(left) && is<Boolean>(right))
                    ? convert<Boolean>(left)->value() != convert<Boolean>(right)->value()
                    : (is<Integer>(left) && is<Real>(right))
                    ? static_cast<Real::value_type>(convert<Integer>(left)->value()) != convert<Real>(right)->value()
                    : (is<Real>(left) && is<Integer>(right))
                    ? convert<Real>(left)->value() != static_cast<Real::value_type>(convert<Integer>(right)->value())
                    : throw std::runtime_error("Unsupported operand types for Inequality");

                operandStack.push(std::make_shared<Boolean>(result));
            }
            else if (is<Greater>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (!left || !right) {
                    throw std::runtime_error("Error: Null operand encountered in Greater operation.");
                }

                bool result = (is<Integer>(left) && is<Integer>(right))
                    ? convert<Integer>(left)->value() > convert<Integer>(right)->value()
                    : (is<Real>(left) && is<Real>(right))
                    ? convert<Real>(left)->value() > convert<Real>(right)->value()
                    : (is<Integer>(left) && is<Real>(right))
                    ? static_cast<Real::value_type>(convert<Integer>(left)->value()) > convert<Real>(right)->value()
                    : (is<Real>(left) && is<Integer>(right))
                    ? convert<Real>(left)->value() > static_cast<Real::value_type>(convert<Integer>(right)->value())
                    : (is<Boolean>(left) && is<Boolean>(right))
                    ? (convert<Boolean>(left)->value() ? 1 : 0) > (convert<Boolean>(right)->value() ? 1 : 0)
                    : throw std::runtime_error("Unsupported operand types for Greater");

                operandStack.push(std::make_shared<Boolean>(result));
            }
            else if (is<GreaterEqual>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (!left || !right) {
                    throw std::runtime_error("Error: Null operand encountered in GreaterEqual operation.");
                }

                bool result = (is<Integer>(left) && is<Integer>(right))
                    ? convert<Integer>(left)->value() >= convert<Integer>(right)->value()
                    : (is<Real>(left) && is<Real>(right))
                    ? convert<Real>(left)->value() >= convert<Real>(right)->value()
                    : (is<Integer>(left) && is<Real>(right))
                    ? static_cast<Real::value_type>(convert<Integer>(left)->value()) >= convert<Real>(right)->value()
                    : (is<Real>(left) && is<Integer>(right))
                    ? convert<Real>(left)->value() >= static_cast<Real::value_type>(convert<Integer>(right)->value())
                    : (is<Boolean>(left) && is<Boolean>(right))
                    ? (convert<Boolean>(left)->value() ? 1 : 0) >= (convert<Boolean>(right)->value() ? 1 : 0)
                    : throw std::runtime_error("Unsupported operand types for GreaterEqual");

                operandStack.push(std::make_shared<Boolean>(result));
            }
            else if (is<Less>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (!left || !right) {
                    throw std::runtime_error("Error: Null operand encountered in Less operation.");
                }

                bool result = (is<Integer>(left) && is<Integer>(right))
                    ? convert<Integer>(left)->value() < convert<Integer>(right)->value()
                    : (is<Real>(left) && is<Real>(right))
                    ? convert<Real>(left)->value() < convert<Real>(right)->value()
                    : (is<Integer>(left) && is<Real>(right))
                    ? static_cast<Real::value_type>(convert<Integer>(left)->value()) < convert<Real>(right)->value()
                    : (is<Real>(left) && is<Integer>(right))
                    ? convert<Real>(left)->value() < static_cast<Real::value_type>(convert<Integer>(right)->value())
                    : (is<Boolean>(left) && is<Boolean>(right))
                    ? (convert<Boolean>(left)->value() ? 1 : 0) < (convert<Boolean>(right)->value() ? 1 : 0)
                    : throw std::runtime_error("Unsupported operand types for Less");

                operandStack.push(std::make_shared<Boolean>(result));
            }
            else if (is<LessEqual>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (!left || !right) {
                    throw std::runtime_error("Error: Null operand encountered in LessEqual operation.");
                }

                bool result = (is<Integer>(left) && is<Integer>(right))
                    ? convert<Integer>(left)->value() <= convert<Integer>(right)->value()
                    : (is<Real>(left) && is<Real>(right))
                    ? convert<Real>(left)->value() <= convert<Real>(right)->value()
                    : (is<Integer>(left) && is<Real>(right))
                    ? static_cast<Real::value_type>(convert<Integer>(left)->value()) <= convert<Real>(right)->value()
                    : (is<Real>(left) && is<Integer>(right))
                    ? convert<Real>(left)->value() <= static_cast<Real::value_type>(convert<Integer>(right)->value())
                    : (is<Boolean>(left) && is<Boolean>(right))
                    ? (convert<Boolean>(left)->value() ? 1 : 0) <= (convert<Boolean>(right)->value() ? 1 : 0)
                    : throw std::runtime_error("Unsupported operand types for LessEqual");

                operandStack.push(std::make_shared<Boolean>(result));
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

                if (auto leftInt = std::dynamic_pointer_cast<Integer>(left), rightInt = std::dynamic_pointer_cast<Integer>(right); leftInt && rightInt) {
                    operandStack.push(std::make_shared<Integer>(std::max(leftInt->value(), rightInt->value())));
                }
                else {
                    Real::value_type leftValue = (is<Integer>(left)) ? static_cast<Real::value_type>(convert<Integer>(left)->value()) : convert<Real>(left)->value();
                    Real::value_type rightValue = (is<Integer>(right)) ? static_cast<Real::value_type>(convert<Integer>(right)->value()) : convert<Real>(right)->value();
                    operandStack.push(std::make_shared<Real>(std::max(leftValue, rightValue)));
                }
            }
            else if (is<Min>(token)) {
                auto right = operandStack.top(); operandStack.pop();
                auto left = operandStack.top(); operandStack.pop();

                if (auto leftInt = std::dynamic_pointer_cast<Integer>(left), rightInt = std::dynamic_pointer_cast<Integer>(right); leftInt && rightInt) {
                    operandStack.push(std::make_shared<Integer>(std::min(leftInt->value(), rightInt->value())));
                }
                else {
                    Real::value_type leftValue = (is<Integer>(left)) ? static_cast<Real::value_type>(convert<Integer>(left)->value()) : convert<Real>(left)->value();
                    Real::value_type rightValue = (is<Integer>(right)) ? static_cast<Real::value_type>(convert<Integer>(right)->value()) : convert<Real>(right)->value();
                    operandStack.push(std::make_shared<Real>(std::min(leftValue, rightValue)));
                }
            }
            else if (is<Pow>(token)) {
                auto exponent = operandStack.top(); operandStack.pop();
                auto base = operandStack.top(); operandStack.pop();

                if (auto baseInt = std::dynamic_pointer_cast<Integer>(base), exponentInt = std::dynamic_pointer_cast<Integer>(exponent); baseInt && exponentInt) {
                    // Integer ^ Integer
                    operandStack.push(std::make_shared<Integer>(boost::multiprecision::pow(baseInt->value(), static_cast<unsigned int>(exponentInt->value()))));
                }
                else {
                    // Convert operands to Real if they are not both Integer
                    Real::value_type baseValue = (is<Integer>(base)) ? static_cast<Real::value_type>(convert<Integer>(base)->value()) : convert<Real>(base)->value();
                    Real::value_type exponentValue = (is<Integer>(exponent)) ? static_cast<Real::value_type>(convert<Integer>(exponent)->value()) : convert<Real>(exponent)->value();
                    operandStack.push(std::make_shared<Real>(boost::multiprecision::pow(baseValue, exponentValue)));
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