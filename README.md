Project: Expression Evaluator
Course	INFO-3140 Object-Oriented Analysis and Modeling
Professor	Garth Santor
Due	Friday, December 6th, 2024
Weight	15%
Project Description
Complete the Visual C++ 2022/C++ 23 console application project that implements the Expression Evaluator base code (provided) that reads integers and outputs the result.   On the existing code base, you must implement the infix to postfix algorithm (Shunting yard algorithm by Edsger Dijkstra), then implement a postfix to result algorithm (Postfix algorithm). 
Advanced requirements are graded only if the base (shunting yard and postfix algorithm) is complete and all test cases are working.   The advanced requirements are to implement:
1.	Real data type.
2.	Mixed integer and real expressions.
3.	Boolean data type along with its appropriate operators (AND, OR, NOT, XOR, NAND, NOR, XNOR) and real data types from the console and outputs the result.  
4.	Errors in the expression should be reported to the users and the program should continue operation (so program does not crash).
5.	Variables
6.	Stored Results (requires variable)
Note: Additional details, algorithms, design discussion will be provided in class.
	
Bonus features:
1.	Add the Gregorian data type from Project 1.
2.	You’ll need to add the packaged day, month, and year objects (integer only)
3.	You’ll need operation objects to handle the day/month/year functions and the Gregorian constructor.
4.	Create a specialized Gregorian operand (called now) that initializes to the current date.
5.	You do not need to handle time of day.
6.	Your objective is to be able to process expressions such as:
Gregorian(2019,10,10) + months(2) - days(5)
now > Gregorian(2019,10,8)+days(20)






 
Grading Criteria
This project will be graded using the following criteria.
Document Requirements		
Using this document: Highlight in Yellow the functional requirements which you have implemented.   Put in BOLD AND ITALICS the RPN architecture that you have chosen to implement here.
Big If
Operand Polymorph
Operator Polymorph
Double Dispatch
Lookup Table 	-10% if missing	
Requirements 		
Empty & Unknown: 00 a-d	5%	Only 2 unknown exceptions I cannot figure out.
 

Integer only: 01 a-e	5%	
Integer & Unary: 02 a-f	10%	
Integer & Binary: 03 a-q	20%	
Integer & Binary & Named & Right: 04 a-c	5%	
Integer & Binary & Parenthesis: 05 a-f	5%	
Integer & Binary & Functions: 06 a-m	10%	
Real: 07 aa-ch	10%	
Mixed: 08 a-t	5%	
Boolean: 09 aa-bn	5%	
Relational: 10 a-p	5%	
Variable: 11 a-h	5%	 
Result: 12 a	5%	I am passing the test but getting an Unknown exception.
Application	5%	
Gregorian	+5%	=
Non-functional requirements		
Penalties from C & C++ Grading Guide v2.5.0	various	
Late submission
•	One to five days late
•	More than five days late	
-10%/day
-100%	
Total	100%	
Submission
1.	Submit entire Visual Studio project directory to Fanshawe Online
a.	Delete all debug and release directories. 
b.	Submit in a .ZIP, .7z archive file.
