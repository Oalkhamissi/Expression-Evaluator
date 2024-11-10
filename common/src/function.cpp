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
#include <ee/expression_evaluator.hpp>
#include <algorithm>
#include <cmath>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/math/special_functions.hpp>
#include <boost/math/constants/constants.hpp>

// Max function implementation
Operand::pointer_type Max::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto left = operands[0];
    auto right = operands[1];

    if (auto leftInt = std::dynamic_pointer_cast<Integer>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Integer, Integer -> Integer
            return std::static_pointer_cast<Operand>(
                make<Integer>(std::max(leftInt->value(), rightInt->value())));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Integer, Real -> Real
            return std::static_pointer_cast<Operand>(
                make<Real>(std::max(Real::value_type(leftInt->value()), rightReal->value())));
        }
    }
    else if (auto leftReal = std::dynamic_pointer_cast<Real>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Real, Integer -> Real
            return std::static_pointer_cast<Operand>(
                make<Real>(std::max(leftReal->value(), Real::value_type(rightInt->value()))));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Real, Real -> Real
            return std::static_pointer_cast<Operand>(
                make<Real>(std::max(leftReal->value(), rightReal->value())));
        }
    }
    throw std::runtime_error("Invalid operand types for Max function");
}

// Min function implementation
Operand::pointer_type Min::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    auto left = operands[0];
    auto right = operands[1];

    if (auto leftInt = std::dynamic_pointer_cast<Integer>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Integer, Integer -> Integer
            return std::static_pointer_cast<Operand>(
                make<Integer>(std::min(leftInt->value(), rightInt->value())));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Integer, Real -> Real
            return std::static_pointer_cast<Operand>(
                make<Real>(std::min(Real::value_type(leftInt->value()), rightReal->value())));
        }
    }
    else if (auto leftReal = std::dynamic_pointer_cast<Real>(left)) {
        if (auto rightInt = std::dynamic_pointer_cast<Integer>(right)) {
            // Real, Integer -> Real
            return std::static_pointer_cast<Operand>(
                make<Real>(std::min(leftReal->value(), Real::value_type(rightInt->value()))));
        }
        else if (auto rightReal = std::dynamic_pointer_cast<Real>(right)) {
            // Real, Real -> Real
            return std::static_pointer_cast<Operand>(
                make<Real>(std::min(leftReal->value(), rightReal->value())));
        }
    }
    throw std::runtime_error("Invalid operand types for Min function");
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

Operand::pointer_type Result::evaluate(std::vector<Operand::pointer_type> const& operands) const {
    // Ensure exactly one operand is passed
    if (operands.size() != 1) {
        throw std::runtime_error("Error: Result function requires exactly one argument.");
    }

    // Get the index as an integer
    auto indexOperand = std::dynamic_pointer_cast<Integer>(operands[0]);
    if (!indexOperand) {
        throw std::runtime_error("Error: Result index must be an integer.");
    }
    auto index = static_cast<size_t>(indexOperand->value());

    // Retrieve the result by index
    // Assuming an instance of ExpressionEvaluator is accessible for retrieving previous results
    // Update this logic if necessary based on how ExpressionEvaluator is managed in your project

    throw std::runtime_error("Error: Cannot retrieve previous results without access to ExpressionEvaluator.");
}