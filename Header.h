#pragma once
#include <iostream>
#include <string>
#include <vector>

enum token_type {
	INT_TYPE, FLOAT_TYPE, OP_TYPE
};

enum op_t {
	ADD, SUB, MUL, DIV, PARO, PARC,  MINUS, NOOP
};

struct Token
{
	int i = 0;
	float f = 0.0f;
	op_t op;
	token_type type;
};

void mythrow(std::string s) {
	std::cout << s << std::endl;
	exit(0);
}

Token calc(Token, Token, Token);

bool is_sign(const std::string&, int);

std::vector<Token> tokenize(const std::string&);

bool valid(const std::vector<Token>&);

Token eval(const std::vector<Token>&);

int is_int_digit_in_string(const std::string& s, int &i);

float is_float_digit_in_string(const std::string& s, int& i);
