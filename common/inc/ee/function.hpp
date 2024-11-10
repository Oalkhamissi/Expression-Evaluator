#pragma once
/*!	\file	function.hpp
	\brief	Function classes declarations.
	\author	Garth Santor
	\date	2021-10-29
	\copyright	Garth Santor, Trinh Han

=============================================================
Declarations of the Function classes derived from Operation.  
Includes the subclasses:
	OneArgFunction
		Abs
		Arccos
		Arcsin
		Arctan
		Ceil
		Cos
		Exp
		Floor
		Lb
		Ln
		Log
		Result
		Sin
		Sqrt
		Tan

	TwoArgFunction
		Arctan2
		Max
		Min
		Pow

	ThreeArgFunction

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

#include <ee/operation.hpp>
#include <ee/integer.hpp>
#include <vector>


/*! Function token base class. */
class Function : public Operation {
public:
    // Declare the evaluate function to allow derived classes to override it
    virtual Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const {
        throw std::runtime_error("Evaluate not implemented for this function.");
    }
};

        /*! One argument function token base class. */
        class OneArgFunction : public Function {
        public:
            [[nodiscard]] virtual unsigned number_of_args() const override { return 1; }
        };

                /*! Absolute value function token. */
                class Abs : public OneArgFunction {
                public:
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Arc cosine function token. */
                class Arccos : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Arc sine function token. */
                class Arcsin : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Arc tangent function token. Argument is the slope. */
                class Arctan : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Ceil function token. */
                class Ceil : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Cosine function token. */
                class Cos : public OneArgFunction { 
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Exponential function token. pow(e,x), where 'e' is the euler constant and 'x' is the exponent. */
                class Exp : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Floor function token. */
                class Floor : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Logarithm base 2 function token. */
                class Lb : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Natural logarithm function token. */
                class Ln : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Logarithm base 10 function token. */
                class Log : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Previous result token. Argument is the 1-base index of the result. */
                class Result : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Sine function token. */
                class Sin : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Square root token. */
                class Sqrt : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Tangent function. */
                class Tan : public OneArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };


        /*! Two argument function token base class. */
        class TwoArgFunction : public Function {
        public:
            [[nodiscard]] virtual unsigned number_of_args() const override { return 2; }
        };

                /*! 2-parameter arc tangent function token.
                    First argument is the change in Y, second argument is the change in X. */
                class Arctan2 : public TwoArgFunction {
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Maximum of 2 elements function token. */
                class Max : public TwoArgFunction {
                public:
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Minimum of 2 elements function token. */
                class Min : public TwoArgFunction {
                public:
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

                /*! Pow function token. First argument is the base, second is the exponent. */
                class Pow : public TwoArgFunction {
                public:
                    Operand::pointer_type evaluate(std::vector<Operand::pointer_type> const& operands) const override;
                };

        /*! Three argument function token base class. */
        class ThreeArgFunction : public Function {
        public:
            virtual unsigned number_of_args() const override { return 3; }
        };