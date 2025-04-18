#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"


// you can declare prototypes of parser functions below


FILE* f;

void error(int w, Token t);
void classDelcar();
void memberDeclar();
void classVarDeclar();
void type();
void subroutineDeclar();
void paramList();
void subroutineBody();
void statement();
void varDeclarStatement();
void letStatement();
void ifStatement();
void whileStatement();
void doStatement();
void subroutineCall();
void expressionList();
void returnStatement();
void expression();
void relationalExpression();
void ArithmeticExpression();
void term();
void factor();
void operand();



void error(int w, Token t){
	exit(0);
}

void classDeclar(){
	Token next_token = PeekNextToken();

	if(strcmp(next_token.lx,"class") == 0){
		error(classExpected,next_token);
		return;
	}

	next_token = PeekNextToken();
	if(next_token.tp != ID){
		error(idExpected,next_token);
	}

	next_token = GetNextToken();

	if(strcmp(next_token.lx,"{") != 0){
		error(openBraceExpected,next_token);
	}

	next_token = PeekNextToken();

	while(strcmp(next_token.lx,"}") != 0){
		memberDeclar();
		next_token = PeekNextToken();
	}

	next_token = GetNextToken();

	if(strcmp(next_token.lx , "}") != 0){
		error(openBraceExpected,next_token);
	}

}

void memberDeclar(){
	Token t = PeekNextToken();

	if(strcmp(t.lx, "class") == 0){
		classDeclar();
	}else if(strcmp(t.lx, "methods") == 0 || strcmp(t.lx, "functions") == 0 || strcmp(t.lx, "constructors" ) == 0){
		subroutineDeclar();
	}else{
		error(classExpected,t);
		return;
	}

}

void classVarDeclar(){
	Token t = GetNextToken();

	if(strcmp(t.lx, "static") == 0){
		type();
		t = GetNextToken();
		
		if(t.tp == ID){
			;
		}

		t = GetNextToken();

		while(strcmp(t.lx, ",")==0){
			t = GetNextToken();
			if(t.tp != ID){
				error(idExpected,t);
			}
		}

	}else if(strcmp(t.lx, "field") == 0){
		type();
		t = PeekNextToken();
		
		if(t.tp ==ID){
			;
		}

		t = GetNextToken();

		while(strcmp(t.lx, ",") == 0){
			t = GetNextToken();
			if(t.tp != ID){
				error(idExpected,t);
				return;
			}
		}
	}else{
		error(classVarErr,t);
		return;
	}
}

void type(){
	Token t = GetNextToken();

	if (strcmp(t.lx, "int") == 0){
		;
	}else if(strcmp(t.lx, "char") == 0){
		;
	}else if(strcmp(t.lx, "boolean") == 0){
		;
	}else if(t.tp != ID){
		error(idExpected,t);
	}else{
		error(illegalType,t);
	}
}

void subroutineDeclar(){
	Token t = GetNextToken();
	if(strcmp(t.lx, "constructor") == 0){
		;
	}else if(strcmp(t.lx, "function") == 0){
		;
	}else if(strcmp(t.lx, "method") == 0){
		;
	}else{
		error(subroutineDeclarErr,t);
	}

	t = PeekNextToken();
	if(strcmp(t.lx, "void") == 0){

	}else{
		type();
	}

	t = GetNextToken();
	if(t.tp != ID){
		error(idExpected,t);
		return;
	}

	t = GetNextToken();
	if(strcmp(t.lx, "(") != 0){
		error(openParenExpected,t);
		return;
	}
	
	paramList();

	if(strcmp(t.lx, ")") != 0){
		error(closeParenExpected,t);
		return;
	}

	subroutineBody();

}

void paramList(){
	Token t = PeekNextToken();

	if(strcmp(t.lx, ")") == 0){
		;
	}

	type();
	t = GetNextToken();
	if(t.tp != ID){
		error(idExpected,t);
	}

	t = PeekNextToken();
	while(strcmp(t.lx, ",") == 0){
		GetNextToken();
		type();

		t = GetNextToken();
		if(t.tp != ID){
			error(idExpected,t);
			return;
		}
	}

}

void subroutineBody(){
	Token t = GetNextToken();

	if(strcmp(t.lx, "{") != 0){
		error(openBraceExpected,t);
		return;
	}

	t = PeekNextToken();
	while(strcmp(t.lx, "}") != 0){
		statement();
		t = PeekNextToken();
	}

	t = GetNextToken();
	if(strcmp(t.lx, "}") != 0){
		error(closeBraceExpected,t);
		return;
	}

}

// Statements
void statement(){
	Token t = GetNextToken();
	if(strcmp(t.lx, "var") == 0){
		varDeclarStatement();
	}else if(strcmp(t.lx, "let") == 0){
		letStatement();
	}else if(strcmp(t.lx, "if") == 0){
		ifStatement();
	}else if(strcmp(t.lx, "while") == 0){
		whileStatement();
	}else if(strcmp(t.lx, "do") == 0){
		doStatement();
	}else if(strcmp(t.lx, "return") == 0){
		returnStatement();
	}
}

void varDeclarStatement(){
	Token t = GetNextToken();
	if(strcmp(t.lx, "var") != 0){
		return;
	}

	type();

	t = GetNextToken();

	if(t.tp != ID){
		error(idExpected,t);
		return;
	}

	t = GetNextToken();

	while(strcmp(t.lx, ",") == 0){
		t = GetNextToken();
		if(t.tp != ID){
			error(idExpected,t);
			return;
		}
	}

}

void letStatement(){
	Token t = GetNextToken();
	if(strcmp(t.lx, "let") == 0){
		t = GetNextToken();
		if(t.tp != ID){
			error(idExpected,t);
			return;
		}

		t = PeekNextToken();
		if(strcmp(t.lx, "[") == 0){
			GetNextToken();
			t = PeekNextToken();

			if(strcmp(t.lx, "[") == 0){
				GetNextToken();
				expression();
			}
			
			t = GetNextToken();

			if(strcmp(t.lx, "]") != 0){
				error(closeBracketExpected,t);
			}

			t = GetNextToken();
			if(strcmp(t.lx, "]") != 0){
				error(closeBracketExpected,t);
			}

		}

		t = GetNextToken();

		if(strcmp(t.lx, "=") != 0){
			error(equalExpected,t);
			return;
		}

		expression();

		t = GetNextToken();

		if(strcmp(t.lx, ";") != 0){
			error(semicolonExpected,t);
			return;
		}
		
	}
}

void ifStatement(){
	Token t = GetNextToken();

	if(strcmp(t.lx, "let") == 0){
		;
	}
	
	t = GetNextToken();

	if(strcmp(t.lx, "(") == 0){
		;
	}

	t = GetNextToken();

	if(t.tp != ID){
		error(idExpected,t);
		return;
	}

	t = GetNextToken();

	if(strcmp(t.lx, ")") == 0){
		;
	}

	t = GetNextToken();

	if(strcmp(t.lx, "{") != 0){
		error(openBraceExpected,t);
		return;
	}

	t = PeekNextToken();

	while(strcmp(t.lx, "}") != 0){
		GetNextToken();
		statement();
		t = PeekNextToken();
	}

	t = GetNextToken();

	if(strcmp(t.lx, "}") != 0){
		error(idExpected, t);
		return;
	}

	t = GetNextToken();
	if(strcmp(t.lx, "else") == 0){
		GetNextToken();
		if(strcmp(t.lx, "{") != 0){
			error(idExpected,t);
			return;
		}

		t = PeekNextToken();
		while(strcmp(t.lx, "}") == 0){
			GetNextToken();
			statement();
			t = PeekNextToken();
		}

		t = GetNextToken();
		if(strcmp(t.lx, "]") != 0){
			error(closeBracketExpected,t);
			return;
		}
	}


}

void whileStatement(){
	Token t = GetNextToken();

	if(strcmp(t.lx, "while") == 0){
		t = GetNextToken();

		if(strcmp(t.lx, "(") != 0){
			error(openParenExpected,t);
		}

		expression();

		t = GetNextToken();

		if(strcmp(t.lx, ")") != 0){
			error(closeParenExpected,t);
			return;
		}

		t = GetNextToken();

		if(strcmp(t.lx, "{") == 0){
			error(openBraceExpected,t);
			return;
		}

		while(strcmp(t.lx, "}") != 0){
			statement();
		}

		t = GetNextToken();

		if(strcmp(t.lx, "}") != 0){
			error(closeBraceExpected,t);
			return;
		}
		
	}

}

void doStatement(){
	Token t = GetNextToken();

	if(strcmp(t.lx, "do") == 0){
		subroutineCall();
	}

}

void subroutineCall(){
	Token t = GetNextToken();

	if(t.tp != ID){
		error(idExpected,t);
	}

	t = PeekNextToken();
	if(strcmp(t.lx, "[") == 0){
		GetNextToken();
		t = GetNextToken();
		if(strcmp(t.lx, ".") == 0){
			t = GetNextToken();
			if(t.tp != ID){
				error(idExpected,t);
				return;
			}
			t = GetNextToken();
			if(strcmp(t.lx, "int") != 0){
				error(closeBraceExpected,t);
				return;
			}
			t = GetNextToken();
			if(strcmp(t.lx, "(") == 0){
				expressionList();
			}
			t = GetNextToken();
			if(strcmp(t.lx, ")") == 0){
				;
			}

		}

	}

}

void expressionList(){
	
	expression();

	Token t = GetNextToken();
	if(strcmp(t.lx, "{") == 0){
		t = GetNextToken();
		while(strcmp(t.lx, ",") != 0){
			expression();
			t=GetNextToken();
		}
		t = GetNextToken();
		if(strcmp(t.lx, "}") != 0){
			error(closeBraceExpected,t);
		}

	}else{
		;
	}

}

void returnStatement(){
	Token t = GetNextToken();
	if(strcmp(t.lx, "return") == 0){
		t = PeekNextToken();
		if(strcmp(t.lx, "[") == 0){
			GetNextToken();
			expression();
		}
		t = GetNextToken();
		if(strcmp(t.lx, "]") != 0){
			error(closeBracketExpected,t);
			return;
		}
		 
		t = GetNextToken();
		if(strcmp(t.lx, ";") != 0){
			error(semicolonExpected,t);
		}
	}

}

//expression


void expression(){
	relationalExpression();

	Token t = PeekNextToken();

	while(strcmp(t.lx, "&") == 0 || strcmp(t.lx, "|") == 0){
		GetNextToken();
		relationalExpression();

	}

}

void relationalExpression(){
	ArithmeticExpression();

	Token t = PeekNextToken();
	while(strcmp(t.lx, "=") == 0 || strcmp(t.lx, ">") == 0 || strcmp(t.lx, "<") == 0){
		GetNextToken();
		ArithmeticExpression();
	}

}

void ArithmeticExpression(){
	term();

	Token t = PeekNextToken();
	while(strcmp(t.lx, "+") == 0 || strcmp(t.lx, "-") == 0){
		GetNextToken();
		term();
	}

}

void term(){
	factor();
	Token t = PeekNextToken();
	while(strcmp(t.lx, "*") == 0 || strcmp(t.lx, "/") == 0){
		GetNextToken();
		factor();
	}

}

void factor(){
	Token t = GetNextToken();
	if(strcmp(t.lx, "-") == 0 || strcmp(t.lx, "~") == 0 || strcmp(t.lx, "") == 0){
		operand();
	}
}

void operand(){
	Token t = GetNextToken();
	
	if(strcmp(t.lx, "integerConstrant") == 0){
		;
	}else if(strcmp(t.lx, "identifier") != 0){
		error(idExpected,t);

		t = PeekNextToken();
		if(strcmp(t.lx, "[") == 0){
			GetNextToken();
			t = GetNextToken();
			
			if(strcmp(t.lx, ".") == 0){
				t = GetNextToken();
				if(strcmp(t.lx, "identifier") != 0){
					error(idExpected,t);
				}
				t = GetNextToken();
				if(strcmp(t.lx, "]") != 0){
					error(closeBracketExpected,t);
				}

				t = PeekNextToken();
				if(strcmp(t.lx, "[") == 0){
					GetNextToken();
					t = GetNextToken();
					if(strcmp(t.lx, "[") == 0){
						expression();
						t = GetNextToken();
						if(strcmp(t.lx, "]") != 0){
							error(closeBracketExpected,t);
						}
					}else if(strcmp(t.lx, "(") == 0){
						expressionList();
						t = GetNextToken();
						if(strcmp(t.lx, ")") != 0){
							error(closeParenExpected,t);
						}
					}
					t = GetNextToken();
					if(strcmp(t.lx, "]") != 0){
						error(closeBracketExpected,t);
					}
				}
			}
		}

	}else if(strcmp(t.lx, "(") == 0){
		expression();
		t = GetNextToken();
		if(strcmp(t.lx, ")") != 0){
			error(closeParenExpected,t);
		}

	}else if(strcmp(t.lx, "stringLiteral")==0){
		
	}else if(strcmp(t.lx, "true")==0){
		;
	}else if(strcmp(t.lx, "false")==0){
		;
	}else if(strcmp(t.lx, "null")==0){
		;
	}else if(strcmp(t.lx, "this")==0){
		;
	}
}




int InitParser (char* file_name)
{
	f = fopen(file_name,"r");
	if(f == 0){
		return 0;
	}else{
		return 1;
	}

}

ParserInfo Parse ()
{
	ParserInfo pi;

	// implement the function

	classDeclar();
	
	pi.er = none;

	classDeclar();

	return pi;
}


int StopParser ()
{
	return 1;
}

#ifndef TEST_PARSER
int main ()
{
    if(!InitParser("Ball.jack")){
        printf("file open error\n");
        return 1;
    }

    ParserInfo result = Parse();

    if (result.er == none) {
        printf("Parsing succeeded.\n");
    } else {
        printf("Parsing failed with error code: %d\n", result.er);
    }

    StopParser();

    return 0;
}

#endif
