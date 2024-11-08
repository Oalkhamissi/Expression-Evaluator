/*!	\file	function.cpp
	\brief	Function class implementations.
	\author	Garth Santor
	\date	2021-10-29
	\copyright	Garth Santor, Trinh Han

=============================================================
Declarations of the Function classes derived from Operation.

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.10.02
	C++ 20 validated

Version 2019.11.05
	C++ 17 cleanup

Version 2016.11.02
	Added 'override' keyword where appropriate.

Version 2014.10.30
	Removed binary function

Version 2012.11.13
	C++ 11 cleanup

Version 2009.11.26
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


#include <ee/function.hpp>
#include <ee/integer.hpp>
#include <ee/real.hpp>
#include <algorithm>
#include <cmath>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/math/special_functions.hpp>
#include <boost/math/constants/constants.hpp>

// Max function implementation
Operand::pointer_type Max::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto lhs = std::dynamic_pointer_cast<Integer>(operands[0])->value();
    auto rhs = std::dynamic_pointer_cast<Integer>(operands[1])->value();
    return std::make_shared<Integer>(std::max(lhs, rhs));
}

// Min function implementation
Operand::pointer_type Min::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto lhs = std::dynamic_pointer_cast<Integer>(operands[0])->value();
    auto rhs = std::dynamic_pointer_cast<Integer>(operands[1])->value();
    return std::make_shared<Integer>(std::min(lhs, rhs));
}

// Pow function implementation
Operand::pointer_type Pow::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto base = std::dynamic_pointer_cast<Integer>(operands[0])->value();
    auto exponent = std::dynamic_pointer_cast<Integer>(operands[1])->value();
    return std::make_shared<Integer>(boost::multiprecision::pow(base, static_cast<unsigned>(exponent)));
}

// Sin function implementation
Operand::pointer_type Sin::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::sin(value));
}

// Cos function implementation
Operand::pointer_type Cos::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::cos(value));
}

// Tan function implementation
Operand::pointer_type Tan::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::tan(value)); // Use Boost's tan
}

// Exp function implementation
Operand::pointer_type Exp::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::exp(value));
}

// Sqrt function implementation
Operand::pointer_type Sqrt::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::sqrt(value));
}

// Ln function implementation (natural logarithm)
Operand::pointer_type Ln::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::log(value));
}

// Abs function implementation
Operand::pointer_type Abs::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::abs(value));
}

// Arccos function implementation
Operand::pointer_type Arccos::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::acos(value));
}

// Arcsin function implementation
Operand::pointer_type Arcsin::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::asin(value));
}

// Arctan function implementation
Operand::pointer_type Arctan::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::atan(value));
}

// Ceil function implementation
Operand::pointer_type Ceil::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::ceil(value));
}

// Floor function implementation
Operand::pointer_type Floor::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::floor(value));
}

// Lb function implementation (log base 2)
Operand::pointer_type Lb::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::log2(value));
}

// Log function implementation (log base 10)
Operand::pointer_type Log::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto value = std::dynamic_pointer_cast<Real>(operands[0])->value();
    return std::make_shared<Real>(boost::multiprecision::log10(value));
}

// Arctan2 function implementation
Operand::pointer_type Arctan2::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto y = std::dynamic_pointer_cast<Real>(operands[0])->value();
    auto x = std::dynamic_pointer_cast<Real>(operands[1])->value();
    return std::make_shared<Real>(boost::multiprecision::atan2(y, x));
}