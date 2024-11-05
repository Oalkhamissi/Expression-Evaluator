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
#include <ee/integer.hpp>
#include <ee/real.hpp>
#include <ee/variable.hpp>
#include <cassert>

#include <stdexcept>



// Addition Operator
Operand::pointer_type Addition::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    return std::static_pointer_cast<Operand>(make<Integer>(lhs->value() + rhs->value()));
}

// Subtraction Operator
Operand::pointer_type Subtraction::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);  // Ensure exactly two operands
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    return std::static_pointer_cast<Operand>(make<Integer>(lhs->value() - rhs->value()));
}

// Multiplication Operator
Operand::pointer_type Multiplication::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);  // Ensure exactly two operands
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    return std::static_pointer_cast<Operand>(make<Integer>(lhs->value() * rhs->value()));
}

// Division Operator
Operand::pointer_type Division::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);  // Ensure exactly two operands
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    if (rhs->value() == 0) {
        throw std::runtime_error("Division by zero");  // Handle division by zero
    }
    return std::static_pointer_cast<Operand>(make<Integer>(lhs->value() / rhs->value()));
}

// Factorial Operator
Operand::pointer_type Factorial::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 1);  // Factorial is a unary operator
    auto operand = convert<Integer>(operands[0]);

    // Implement factorial logic here
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
    assert(operands.size() == 1);  // Not is a unary operator
    auto operand = convert<Integer>(operands[0]);
    return std::static_pointer_cast<Operand>(make<Integer>(!operand->value()));
}

// And Operator
Operand::pointer_type And::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2); // Ensure exactly two operands for binary AND operation
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    return std::static_pointer_cast<Operand>(make<Integer>(lhs->value() && rhs->value()));
}

// Nand Operator
Operand::pointer_type Nand::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    bool result = !(lhs->value() && rhs->value());
    return std::static_pointer_cast<Operand>(make<Integer>(result ? 1 : 0));
}

// Or Operator
Operand::pointer_type Or::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2); // Ensure exactly two operands for binary OR operation
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    return std::static_pointer_cast<Operand>(make<Integer>(lhs->value() || rhs->value()));
}
// Nor Operator
Operand::pointer_type Nor::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    bool result = !(lhs->value() || rhs->value());
    return std::static_pointer_cast<Operand>(make<Integer>(result ? 1 : 0));
}
// Xor Operator
Operand::pointer_type Xor::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2); // Ensure exactly two operands for binary XOR operation
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);

    // Perform XOR by converting each operand to a boolean value
    bool result = (lhs->value() != 0) ^ (rhs->value() != 0);
    return std::static_pointer_cast<Operand>(make<Integer>(result ? 1 : 0));
}
// Xnor Operator
Operand::pointer_type Xnor::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    bool result = !(lhs->value() ^ rhs->value());
    return std::static_pointer_cast<Operand>(make<Integer>(result ? 1 : 0));
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
    assert(operands.size() == 2); // Ensure exactly two operands for equality comparison

    // Convert both operands to Integer (or appropriate type)
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);

    // Return true (1) if values are equal, false (0) otherwise
    bool result = (lhs->value() == rhs->value());
    return std::static_pointer_cast<Operand>(make<Integer>(result ? 1 : 0));
}

// Greater Operator
Operand::pointer_type Greater::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2); // Ensure exactly two operands for comparison

    // Convert both operands to Integer
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);

    // Perform the comparison
    bool result = (lhs->value() > rhs->value());
    return std::static_pointer_cast<Operand>(make<Integer>(result ? 1 : 0));
}

// GreaterEqual Operator
Operand::pointer_type GreaterEqual::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2); // Ensure exactly two operands for comparison

    // Convert both operands to Integer
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);

    // Perform the greater-than-or-equal comparison
    bool result = (lhs->value() >= rhs->value());
    return std::static_pointer_cast<Operand>(make<Integer>(result ? 1 : 0));
}

// Inequality Operator
Operand::pointer_type Inequality::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    bool result = (lhs->value() != rhs->value());
    return std::static_pointer_cast<Operand>(make<Integer>(result ? 1 : 0));
}

// Less Operator
Operand::pointer_type Less::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    bool result = (lhs->value() < rhs->value());
    return std::static_pointer_cast<Operand>(make<Integer>(result ? 1 : 0));
}

// LessEqual Operator
Operand::pointer_type LessEqual::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    assert(operands.size() == 2);
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);
    bool result = (lhs->value() <= rhs->value());
    return std::static_pointer_cast<Operand>(make<Integer>(result ? 1 : 0));
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
    assert(operands.size() == 2); // Ensure exactly two operands for exponentiation
    auto lhs = convert<Integer>(operands[0]);
    auto rhs = convert<Integer>(operands[1]);

    // Ensure that rhs->value() is non-negative to avoid unexpected behavior
    if (rhs->value() < 0) {
        throw std::runtime_error("Negative exponent not supported for integer power operation.");
    }

    // Cast rhs->value() to unsigned int to be compatible with boost::multiprecision::pow
    Integer::value_type result = boost::multiprecision::pow(lhs->value(), static_cast<unsigned int>(rhs->value()));
    return std::static_pointer_cast<Operand>(make<Integer>(result));
}