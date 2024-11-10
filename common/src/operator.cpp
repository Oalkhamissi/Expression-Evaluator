/*!	\file	operator.cpp
	\brief	Operator classes implementations.
	\author	Garth Santor
	\date	2021-10-29
	\copyright	Garth Santor, Trinh Han

=============================================================
Implementation of the Operator class derived from Token and the
supporting utilities.

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.10.02
	C++ 20 validated

Version 2019.11.05
	C++ 17 cleanup

Version 2012.11.13
	C++ 11 cleanup

Version 0.1.0: 2010-11-08
	Added exception for assignment to a non-variable.

Version 2009.12.14
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

#include <ee/operator.hpp>
#include <ee/operand.hpp>
#include <ee/boolean.hpp>
#include <ee/integer.hpp>
#include <ee/real.hpp>
#include <ee/variable.hpp>
#include <cassert>

#include <stdexcept>



// Addition Operator
Operand::pointer_type Addition::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);

    auto left = operands[0];
    auto right = operands[1];

    if (auto leftInt = std::dynamic_pointer_cast<Integer>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Integer + Integer -> Integer
            return std::static_pointer_cast<Operand>(make<Integer>(leftInt->value() + rightInt->value()));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Integer + Real -> Real
            return std::static_pointer_cast<Operand>(make<Real>(Real::value_type(leftInt->value()) + rightReal->value()));
        }
    }
    else if (auto leftReal = std::dynamic_pointer_cast<Real>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Real + Integer -> Real
            return std::static_pointer_cast<Operand>(make<Real>(leftReal->value() + Real::value_type(rightInt->value())));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Real + Real -> Real
            return std::static_pointer_cast<Operand>(make<Real>(leftReal->value() + rightReal->value()));
        }
    }
    throw std::runtime_error("Invalid operand types for addition");
}

// Subtraction Operator
Operand::pointer_type Subtraction::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);

    auto left = operands[0];
    auto right = operands[1];

    if (auto leftInt = std::dynamic_pointer_cast<Integer>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Integer - Integer -> Integer
            return std::static_pointer_cast<Operand>(make<Integer>(leftInt->value() - rightInt->value()));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Integer - Real -> Real
            return std::static_pointer_cast<Operand>(make<Real>(Real::value_type(leftInt->value()) - rightReal->value()));
        }
    }
    else if (auto leftReal = std::dynamic_pointer_cast<Real>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Real - Integer -> Real
            return std::static_pointer_cast<Operand>(make<Real>(leftReal->value() - Real::value_type(rightInt->value())));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Real - Real -> Real
            return std::static_pointer_cast<Operand>(make<Real>(leftReal->value() - rightReal->value()));
        }
    }
    throw std::runtime_error("Invalid operand types for subtraction");
}

// Multiplication Operator
Operand::pointer_type Multiplication::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);

    auto left = operands[0];
    auto right = operands[1];

    if (auto leftInt = std::dynamic_pointer_cast<Integer>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Integer * Integer -> Integer
            return std::static_pointer_cast<Operand>(make<Integer>(leftInt->value() * rightInt->value()));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Integer * Real -> Real
            return std::static_pointer_cast<Operand>(make<Real>(Real::value_type(leftInt->value()) * rightReal->value()));
        }
    }
    else if (auto leftReal = std::dynamic_pointer_cast<Real>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Real * Integer -> Real
            return std::static_pointer_cast<Operand>(make<Real>(leftReal->value() * Real::value_type(rightInt->value())));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Real * Real -> Real
            return std::static_pointer_cast<Operand>(make<Real>(leftReal->value() * rightReal->value()));
        }
    }
    throw std::runtime_error("Invalid operand types for multiplication");
}

// Division Operator
Operand::pointer_type Division::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);

    auto left = operands[0];
    auto right = operands[1];

    // Check for division by zero
    if ((std::dynamic_pointer_cast<Integer>(right) && std::dynamic_pointer_cast<Integer>(right)->value() == 0) ||
        (std::dynamic_pointer_cast<Real>(right) && std::dynamic_pointer_cast<Real>(right)->value() == 0)) {
        throw std::runtime_error("Division by zero");
    }

    if (auto leftInt = std::dynamic_pointer_cast<Integer>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Integer / Integer -> Integer
            return std::static_pointer_cast<Operand>(make<Integer>(leftInt->value() / rightInt->value()));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Integer / Real -> Real
            return std::static_pointer_cast<Operand>(make<Real>(Real::value_type(leftInt->value()) / rightReal->value()));
        }
    }
    else if (auto leftReal = std::dynamic_pointer_cast<Real>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Real / Integer -> Real
            return std::static_pointer_cast<Operand>(make<Real>(leftReal->value() / Real::value_type(rightInt->value())));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Real / Real -> Real
            return std::static_pointer_cast<Operand>(make<Real>(leftReal->value() / rightReal->value()));
        }
    }
    throw std::runtime_error("Invalid operand types for division");
}


// Factorial Operator
Operand::pointer_type Factorial::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 1); 
    auto operand = convert<Integer>(operands[0]);

    Integer::value_type result = 1;
    for (Integer::value_type i = 1; i <= operand->value(); ++i) {
        result *= i;
    }
    return std::static_pointer_cast<Operand>(make<Integer>(result));
}

// Identity Operator
Operand::pointer_type Identity::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 1);  // Identity is a unary operator
    return operands[0];  // Return the operand itself
}

// Negation Operator
Operand::pointer_type Negation::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 1);  // Negation is a unary operator
    auto operand = convert<Integer>(operands[0]);
    return std::static_pointer_cast<Operand>(make<Integer>(-operand->value()));
}

// Not Operator
Operand::pointer_type Not::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 1);
    auto operand = std::dynamic_pointer_cast<Boolean>(operands[0]);
    return std::make_shared<Boolean>(!operand->value());
}

// And Operator
Operand::pointer_type And::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Boolean>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Boolean>(operands[1]);
    return std::make_shared<Boolean>(lhs->value() && rhs->value());
}

// Nand Operator
Operand::pointer_type Nand::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Boolean>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Boolean>(operands[1]);
    return std::make_shared<Boolean>(!(lhs->value() && rhs->value()));
}

// Or Operator
Operand::pointer_type Or::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Boolean>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Boolean>(operands[1]);
    return std::make_shared<Boolean>(lhs->value() || rhs->value());
}

// Nor Operator
Operand::pointer_type Nor::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Boolean>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Boolean>(operands[1]);
    return std::make_shared<Boolean>(!(lhs->value() || rhs->value()));
}

// Xor Operator
Operand::pointer_type Xor::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Boolean>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Boolean>(operands[1]);
    return std::make_shared<Boolean>(lhs->value() != rhs->value());
}

// Xnor Operator
Operand::pointer_type Xnor::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Boolean>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Boolean>(operands[1]);
    return std::make_shared<Boolean>(lhs->value() == rhs->value());
}
// Assignment Operator
Operand::pointer_type Assignment::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2); // Ensure exactly two operands for assignment

    // Convert the first operand to Variable and the second to Integer (or appropriate type)
    auto variable = convert<Variable>(operands[0]);
    auto value = operands[1]; // This could be Integer, Real, or any Operand type

    if (!variable) {
        throw std::runtime_error("Assignment target must be a variable");
    }

    // Set the value in the variable
    variable->set(value);

    // Return the assigned value as a result
    return value;
}

// Equality Operator
Operand::pointer_type Equality::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Integer>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Integer>(operands[1]);
    return std::make_shared<Boolean>(lhs->value() == rhs->value());
}

// Inequality Operator
Operand::pointer_type Inequality::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Integer>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Integer>(operands[1]);
    return std::make_shared<Boolean>(lhs->value() != rhs->value());
}

// Greater Operator
Operand::pointer_type Greater::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Integer>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Integer>(operands[1]);
    return std::make_shared<Boolean>(lhs->value() > rhs->value());
}

// GreaterEqual Operator
Operand::pointer_type GreaterEqual::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Integer>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Integer>(operands[1]);
    return std::make_shared<Boolean>(lhs->value() >= rhs->value());
}

// Less Operator
Operand::pointer_type Less::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Integer>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Integer>(operands[1]);
    return std::make_shared<Boolean>(lhs->value() < rhs->value());
}

// LessEqual Operator
Operand::pointer_type LessEqual::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = std::dynamic_pointer_cast<Integer>(operands[0]);
    auto rhs = std::dynamic_pointer_cast<Integer>(operands[1]);
    return std::make_shared<Boolean>(lhs->value() <= rhs->value());
}

// Modulus Operator
Operand::pointer_type Modulus::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    if (rhs->value() == 0) {
        throw std::runtime_error("Division by zero");
    }
    return std::static_pointer_cast<Operand>(make<Integer>(lhs->value() % rhs->value()));
}

// Power Operator
Operand::pointer_type Power::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);

    if (auto lhs_int = std::dynamic_pointer_cast<Integer>(operands[0])) {
        if (auto rhs_int = std::dynamic_pointer_cast<Integer>(operands[1])) {
            // Integer exponentiation
            Integer::value_type result = boost::multiprecision::pow(lhs_int->value(), static_cast<unsigned int>(rhs_int->value()));
            return std::make_shared<Integer>(result);
        }
        else if (auto rhs_real = std::dynamic_pointer_cast<Real>(operands[1])) {
            // Mixed Integer base and Real exponent
            Real::value_type result = boost::multiprecision::pow(static_cast<Real::value_type>(lhs_int->value()), rhs_real->value());
            return std::make_shared<Real>(result);
        }
    }
    else if (auto lhs_real = std::dynamic_pointer_cast<Real>(operands[0])) {
        if (auto rhs_int = std::dynamic_pointer_cast<Integer>(operands[1])) {
            // Real base and Integer exponent
            Real::value_type result = boost::multiprecision::pow(lhs_real->value(), static_cast<Real::value_type>(rhs_int->value()));
            return std::make_shared<Real>(result);
        }
        else if (auto rhs_real = std::dynamic_pointer_cast<Real>(operands[1])) {
            // Real exponentiation
            Real::value_type result = boost::multiprecision::pow(lhs_real->value(), rhs_real->value());
            return std::make_shared<Real>(result);
        }
    }

    throw std::runtime_error("Unsupported operand types for Power operator");
}