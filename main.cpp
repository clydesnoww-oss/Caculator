#include <iostream>
#include<string>
#include<unordered_map>
#include <deque>
#include <vector>
#include <cmath>
struct sOperator
{
	uint8_t prcedence = 0;
	uint8_t argument = 0;
};

struct sSymbol
{
	std::string symbol = " ";

	enum class Type : uint8_t
	{
		UNKNOWN,
		NUMERIC_LITERAL,
		OPERATOR,
		PARENTHESES_OPEN,
		PARENTHESES_CLOSE,

	} type = sSymbol::Type::UNKNOWN;


	sOperator operatorr;
};
sSymbol symbolPev { "0", sSymbol::Type::NUMERIC_LITERAL,0,0 };
int main()
{
	std::string Expression;
	std::unordered_map<char, sOperator> mapOps;
	mapOps['^'] = { .prcedence = 5 , .argument = 2 };
	mapOps['/'] = { .prcedence = 4 , .argument = 2 };
	mapOps['*'] = { .prcedence = 3 , .argument = 2 };
	mapOps['+'] = { .prcedence = 2 , .argument = 2 };
	mapOps['-'] = { .prcedence = 1 , .argument = 2 };

	char operatorr;
	std::cout << "To Quit Press Q Else Press Any Other Key: ";
	std::cin >> operatorr;

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


	std::deque<sSymbol> holdingStack;
	std::deque<sSymbol> OutputStack;

	while (operatorr != 'q' && operatorr != 'Q')
	{
		std::cout << "Enter Expression: ";
		std::getline(std::cin, Expression);

		if (Expression.empty()) continue;

		if (!Expression.empty() && (Expression[0] == '-' || Expression[0] == '+')) Expression.insert(0, "0");  

		for (size_t i = 0; i < Expression.size(); i++) {
			if ((Expression[i] == '-' || Expression[i] == '+') &&
				(i == 0 || Expression[i - 1] == '(' || Expression[i - 1] == '+' ||
					Expression[i - 1] == '-' || Expression[i - 1] == '*' || Expression[i - 1] == '/')) {

				// Insert a '0' before the unary so parser sees it as 0-Number
				Expression.insert(i, "0");
				i++; // skip past the inserted '0'
			}
		}


		for (size_t i = 0; i < Expression.size() ; )
		{
			char c = Expression[i];
			if (std::isspace(c)) { i++; continue; }
			if (std::isdigit(c))
			{
				std::string number;
				while (i < Expression.size() && std::isdigit(Expression[i]))
				{
					number += Expression[i];
					i++;
				}
				OutputStack.push_back({ number, sSymbol::Type::NUMERIC_LITERAL });
				symbolPev = OutputStack.back();
				continue; 
			}
			else if (c == '(')
			{
				if ((i + 1) < Expression.size() && symbolPev.type == sSymbol::Type::NUMERIC_LITERAL && symbolPev.type != sSymbol::Type::OPERATOR &&  symbolPev.type == sSymbol::Type::PARENTHESES_CLOSE)
				{
					Expression.insert(i + 1, "*");
				}

				holdingStack.push_back({ std::string(1,c) , sSymbol::Type::PARENTHESES_OPEN });
				symbolPev = holdingStack.back();

			}
			else if (c == ')')
			{
				if (Expression[i+1] == '(' && symbolPev.type != sSymbol::Type::OPERATOR)
				{
					Expression.insert(i + 2, "*");
				}

				while (!holdingStack.empty() && holdingStack.back().type != sSymbol::Type::PARENTHESES_OPEN)
				{
					OutputStack.push_back(holdingStack.back());
					holdingStack.pop_back();
				}

				if (holdingStack.empty())
				{
					std::cout << "Error:: Bad Parantheses Expression " << std::string(1, c) << '\n';
					return 0;
				}
				if (!holdingStack.empty() && holdingStack.back().type == sSymbol::Type::PARENTHESES_OPEN)
				{

					holdingStack.pop_back();
				}

				symbolPev = { std::string(1,c) , sSymbol::Type::PARENTHESES_CLOSE };
			}

			else if (mapOps.contains(c))
			{
				sOperator newOps = mapOps[c];
				if (c == '-' || c == '+' ) {
					if (symbolPev.type == sSymbol::Type::NUMERIC_LITERAL || symbolPev.type == sSymbol::Type::PARENTHESES_CLOSE) {
						// Binary operator
						newOps.argument = 2;
						newOps.prcedence = 1; // or standard for +/-
					}
					else if (symbolPev.type != sSymbol::Type::NUMERIC_LITERAL && symbolPev.type != sSymbol::Type::PARENTHESES_CLOSE )
					{
						// Unary operator
						newOps.argument = 2;
						newOps.prcedence = 7;
			
					}
				}
				while (!holdingStack.empty() && holdingStack.back().type != sSymbol::Type::PARENTHESES_OPEN)
				{
					if (holdingStack.back().type == sSymbol::Type::OPERATOR)
					{
						const auto& holdingstack_op_precedence = holdingStack.front().operatorr;
						if (holdingstack_op_precedence.prcedence >= newOps.prcedence)
						{
							OutputStack.push_back(holdingStack.back());
							holdingStack.pop_back();
						}
						else
							break;
					}
				}


				holdingStack.push_back({ std::string(1,c) , sSymbol::Type::OPERATOR,newOps });

				symbolPev = holdingStack.back();
			}
			else
			{
				std::cout << "Error::Bad Arguments Expression! " << std::string(1, c) << '\n';
				break;
			}

			i++;
		}


		while (!holdingStack.empty())
		{
			OutputStack.push_back(holdingStack.back());
			holdingStack.pop_back();
		}

		std::cout << "Heres Your Parsed Expression: ";
		for (const auto& d : OutputStack)
		{
			std::cout << d.symbol;
		}
		std::cout << '\n';

		std::deque <double> solveMedium;
		for (const auto& tempfSolve : OutputStack)
		{
			switch (tempfSolve.type)
			{
			case sSymbol::Type::NUMERIC_LITERAL:
			{
				solveMedium.push_front(std::stod(tempfSolve.symbol));
			}break;

			case sSymbol::Type::OPERATOR:
			{

				std::vector<double> mem(tempfSolve.operatorr.argument);
				if (solveMedium.empty())
				{
					std::cout << "Error:: Bad Expression\n";
				}
				else 
				{
					for (uint8_t i = 0; i < tempfSolve.operatorr.argument; i++)
					{

						mem[i] = solveMedium.front();
						solveMedium.pop_front();

					}
				}
				double result = 0.0;

				if (tempfSolve.operatorr.argument == 2)
				{
					switch (tempfSolve.symbol[0])
					{
					case '^':
						result = std::pow(mem[1] ,mem[0]);
						break;
					case '/':
						result = mem[1] / mem[0];
						break;

					case '*':
						result = mem[1] * mem[0];
						break;

					case '+':
						result = mem[1] + mem[0];
						break;

					case '-':
						result = mem[1] - mem[0];
						break;

					default:
						// Unknown operator
						break;
					}
				}
				
				if (tempfSolve.operatorr.argument == 1)
				{
					mem[1] = 0;
					switch (tempfSolve.symbol[0])
					{
					case '-':
						result =  - mem[0];
						break;

					case '+':
						result = + mem[0];
						break;
					default:
						break;
					}

				}
				solveMedium.push_front(result);
			}
			}


		}
		std::cout << "Result = " << std::to_string(solveMedium.front()) << '\n';
		//std::cout << "Do You Want To Quit: ";
		//std::cin >> operatorr;
		//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		OutputStack.clear();
		holdingStack.clear();

	}


	
}
