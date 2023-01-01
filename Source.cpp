#include "Header.h"



int main() {

	std::cout << "write an equation: " << std::endl;
	std::string eq;
	std::getline(std::cin, eq);
	
	//std::cout << eq << std::endl;
	std::vector<Token> tokens = tokenize(eq);
	//valid(tokens);

	Token ans = eval(tokens);
	std::cout << "float: " << ans.f << std::endl;
	std::cout << "int: " << ans.i << std::endl;
	std::cout << "done" << std::endl;
	return 0;
}

std::vector<Token> tokenize(const std::string& s) {
	std::vector<Token> v;
	int len = s.length();
	int i = 0;
	while (i<len)
	{
		if (s[i] == '(')
		{
			v.push_back({ 0,0,PARO,OP_TYPE });
			i++;
			continue;
		}
		if (s[i] == ')')
		{
			v.push_back({ 0,0,PARC,OP_TYPE });
			i++;
			continue;
		}
		if (s[i] == '+')
		{
			v.push_back({ 0,0,ADD,OP_TYPE });
			i++;
			continue;
		}



		if (s[i] == '/')
		{
			v.push_back({ 0,0,DIV,OP_TYPE });
			i++;
			continue;
		}
		if (s[i] == '*')
		{
			v.push_back({ 0,0,MUL,OP_TYPE });
			i++;
			continue;
		}

		if (s[i] == '-')
		{
			if (is_sign(s, i))
			{
				v.push_back({ 0,0,MINUS,OP_TYPE });
			}
			else
			{
				v.push_back({ 0,0,SUB,OP_TYPE });
			}
			i++;
			continue;
		}

		if (std::isdigit(s[i])) //check if int
		{
			



			int val;
			val = is_int_digit_in_string(s, i);
			if (val != NULL)
			{
				v.push_back({ val,0,NOOP,INT_TYPE });


				continue;
			}
			
		}

		if (std::isdigit(s[i])) //check if float
		{
			float val;
			val = is_float_digit_in_string(s, i);
			
			v.push_back({ 0,val,NOOP,FLOAT_TYPE });


			continue;
		}
		
	}

	return v;
}

int is_int_digit_in_string(const std::string& s, int &i) {
	int istart = i;
	int iend = i;
	while (std::isdigit(s[iend]) || s[iend] == '.')
	{
		if (s[iend] == '.')
		{
			return NULL;
		}
		iend++;
	}

	std::string newstring = s;
	int newval = std::stoi(newstring.substr(istart, iend));
	i = iend;
	return newval;
}

float is_float_digit_in_string(const std::string& s, int& i) {
	int istart = i;
	int iend = i;
	while (std::isdigit(s[iend])||s[iend]=='.')
	{
		iend++;
	}
	std::string newstring = s;
	float newval = std::stof(newstring.substr(istart, iend));
	i = iend;
	return newval;
}




bool is_sign(const std::string& s, int i) {
	if (i == 0 || s[i - 1] == '(' ||
		s[i - 1] == '+' ||
		s[i - 1] == '-' ||
		s[i - 1] == '*' ||
		s[i - 1] == '/')
	{
		return true;
	}return false;
}

bool valid(const std::vector<Token>) {
	return true;
}

Token eval(const std::vector<Token> &t) {

#pragma region out of recursion
	if (t.size() == 1) return t[0];
#pragma endregion

#pragma region instuff in par
	for (int i = 0; i < t.size(); i++)
	{
		if (t[i].type == OP_TYPE && t[i].op == PARC)
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (t[j].type == OP_TYPE && t[j].op == PARO)
				{
					std::vector<Token> v;
					for (int k = j + 1; k < i; k++) v.push_back(t[k]);
					Token instuff = eval(v);
					v.clear();
					for (int k = 0; k < j; k++) v.push_back(t[k]);
					v.push_back(instuff);
					for (int k = i + 1; k < t.size(); k++) v.push_back(t[k]);
					return eval(v);
				}

			}
			mythrow("unmatching parantices");
		}
	}
#pragma endregion

#pragma region minus handling
	for (size_t i = 0; i < t.size(); i++)
	{
		if (t[i].op == MINUS)
		{
			std::vector<Token> v;
			for (size_t k = 0; k < i; k++) v.push_back(t[k]);
			Token tok = t[i + 1];
			tok.i *= -1;
			tok.f *= -1;
			v.push_back(tok);
			for (size_t k = i + 2; k < t.size(); k++) v.push_back(t[k]);
			if (t.size()==1)
			{
				return v[0];
			}
			return eval(v);

		}
	}
#pragma endregion

#pragma region mul div addd sub handle
	for (size_t i = 0; i < t.size(); i++)
	{
		if (t[i].op == MUL || t[i].op == DIV )
		{
			std::vector<Token> v;
			for (size_t k = 0; k < i - 1; k++) v.push_back(t[k]);
			Token instuff = calc(t[i - 1], t[i], t[i + 1]);
			v.push_back(instuff);
			for (size_t k = i + 2; k < t.size(); k++) v.push_back(t[k]);
			if (t.size() == 1)
			{
				return v[0];
			}
			return eval(v);
		}
		
	}

	for (size_t i = 0; i < t.size(); i++)
	{
		
		if (t[i].op == ADD || t[i].op == SUB)
		{
			std::vector<Token> v;
			for (size_t k = 0; k < i - 1; k++) v.push_back(t[k]);
			Token instuff = calc(t[i - 1], t[i], t[i + 1]);
			v.push_back(instuff);
			for (size_t k = i + 2; k < t.size(); k++) v.push_back(t[k]);
			if (t.size() == 1)
			{
				return v[0];
			}
			return eval(v);
		}
	}


#pragma endregion

}

Token calc(Token nl, Token op, Token nr) {
	Token ans;
	switch (op.op)
	{
	case ADD:
		if (nl.type == FLOAT_TYPE || nr.type == FLOAT_TYPE)
		{
			ans.type = FLOAT_TYPE;
			ans.f = (nl.type == FLOAT_TYPE ? nl.f : nl.i)+ 
				(nr.type == FLOAT_TYPE ? nr.f : nr.i);
			ans.op = NOOP;
		}
		else
		{
			ans.type = INT_TYPE;
			ans.i = nl.i + nr.i;
			ans.op = NOOP;
		}
		break;
	case SUB:
		if (nl.type == FLOAT_TYPE || nr.type == FLOAT_TYPE)
		{
			ans.type = FLOAT_TYPE;
			ans.f = (nl.type == FLOAT_TYPE ? nl.f : nl.i) -
				(nr.type == FLOAT_TYPE ? nr.f : nr.i);
			ans.op = NOOP;
		}
		else
		{
			ans.type = INT_TYPE;
			ans.i = nl.i - nr.i;
			ans.op = NOOP;
		}
		break;
	case MUL:
		if (nl.type == FLOAT_TYPE || nr.type == FLOAT_TYPE)
		{
			ans.type = FLOAT_TYPE;
			ans.f = (nl.type == FLOAT_TYPE ? nl.f : nl.i) *
				(nr.type == FLOAT_TYPE ? nr.f : nr.i);
			ans.op = NOOP;
		}
		else
		{
			ans.type = INT_TYPE;
			ans.i = nl.i * nr.i;
			ans.op = NOOP;
		}
		break;
	case DIV:
		if (nr.type == FLOAT_TYPE && nr.f == 0)mythrow("divide by zero erorr");
		if (nr.type == INT_TYPE && nr.i == 0) mythrow("divide by zero erorr");
		
		if (nl.type == FLOAT_TYPE || nr.type == FLOAT_TYPE || nl.i % nr.i == 0)
		{
			ans.type = FLOAT_TYPE;
			ans.f = (float)(nl.type == FLOAT_TYPE ? nl.f : nl.i) /
				(float)(nr.type == FLOAT_TYPE ? nr.f : nr.i);
			ans.op = NOOP;

		}
		else
		{
			ans.type = INT_TYPE;
			ans.i = nl.i / nr.i;
			ans.op = NOOP;
		}
		break;

	default:
		break;
	}
	return ans;
}
