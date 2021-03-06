#include "RuntimeLibrary.h"

// Type Declarations
class Lexer_;
class Parser_;
class Source_Text_;
class Syntax_Node_;
class Syntax_Token_;
class Token_Stream_;
class Token_Type_;

// Function Declarations
auto Parse_(::Source_Text_ const *const source_) -> ::Syntax_Node_ const *;
auto EmitCpp_(::Syntax_Node_ const *const syntaxTree_) -> void;
auto Error_(string const message_) -> void;
auto BeginLine_(string const value_) -> void;
auto Write_(string const value_) -> void;
auto EndLine_(string const value_) -> void;
auto WriteLine_(string const value_) -> void;
auto BeginBlock_() -> void;
auto EndBlock_() -> void;
auto IsIdentifierChar_(char const c_) -> bool;
auto IsNumberChar_(char const c_) -> bool;
auto TokenIsIdentifier_() -> bool;
auto Accept_(string const expected_) -> bool;
auto Expect_(string const expected_) -> void;
auto AcceptIdentifier_() -> bool;
auto AcceptString_() -> bool;
auto AcceptCodePoint_() -> bool;
auto AcceptNumber_() -> bool;
auto ExpectIdentifier_() -> string;
auto IsValueType_(string const type_) -> bool;
auto ConvertType_(string const type_) -> string;
auto ConvertType_(bool const mutableBinding_, bool const mutableValue_, string type_) -> string;
auto ParseType_() -> string;
auto ParseAtom_() -> bool;
auto ParseCallArguments_() -> void;
auto ParseExpression_(int const minPrecedence_) -> void;
auto ParseExpression_() -> void;
auto ParseStatement_() -> bool;
auto ParseBlock_() -> void;
auto ParseArgumentsDeclaration_(bool const isMainFunction_, bool const isMethod_) -> string;
auto ParseConstructorArgumentsDeclaration_() -> string;
auto ParseFunctionArgumentsDeclaration_(bool const isMainFunction_) -> string;
auto ParseMethodArgumentsDeclaration_() -> string;
auto ParseClassMember_(string const className_) -> void;
auto ParseDeclaration_() -> void;
auto ParseCompilationUnit_() -> void;
auto EmitPreamble_() -> void;
auto EmitEntryPointAdapter_(::System_::Collections_::List_<::Source_Text_ const *> const *const resources_) -> void;
auto Compile_(::System_::Collections_::List_<::Source_Text_ const *> const *const sources_, ::System_::Collections_::List_<::Source_Text_ const *> const *const resources_) -> string;
auto Main_(::System_::Console_::Console_ *const console_, ::System_::Console_::Arguments_ const *const args_) -> void;
auto ReadSource_(string const path_) -> ::Source_Text_ const *;

// Class Declarations
class Lexer_
{
public:
	auto Analyze_(::Source_Text_ const *const source_) const -> ::Token_Stream_ *;
};
class Parser_
{
public:
	Parser_();
	auto Parse_(::Token_Stream_ const *const tokenStream_) const -> ::Syntax_Node_ const *;
};
class Source_Text_
{
public:
	string Package_;
	string Name_;
	string Text_;
	Source_Text_(string const package_, string const name_, string const text_);
};
class Syntax_Node_
{
public:
};
class Syntax_Token_
{
public:
	::Token_Type_ const * TokenType_;
	::Source_Text_ const * Source_;
	unsigned int Start_;
	unsigned int Length_;
	Syntax_Token_(::Token_Type_ const *const tokenType_, ::Source_Text_ const *const source_, unsigned int const start_, unsigned int const length_);
};
class Token_Stream_
{
public:
	::Source_Text_ const * Source_;
	unsigned int nextToken_;
	Token_Stream_(::Source_Text_ const *const source_);
	auto GetNextToken_() -> string;
	static auto NewToken_(::Token_Type_ const *const type_, unsigned int const end_) -> ::Syntax_Token_ const *;
	static auto NewToken_(::Token_Type_ const *const type_) -> ::Syntax_Token_ const *;
};
class Token_Type_
{
public:
};

// Definitions
auto ::Lexer_::Analyze_(::Source_Text_ const *const source_) const -> ::Token_Stream_ *
{
	return new ::Token_Stream_(source_);
}

::Parser_::Parser_()
{
}

auto ::Parser_::Parse_(::Token_Stream_ const *const tokenStream_) const -> ::Syntax_Node_ const *
{
	return ::None;
}

auto Parse_(::Source_Text_ const *const source_) -> ::Syntax_Node_ const *
{
	::Lexer_ const *const lexer_ = new ::Lexer_();
	::Token_Stream_ *const tokenStream_ = lexer_->Analyze_(source_);
	::Parser_ *const parser_ = new ::Parser_();
	return parser_->Parse_(tokenStream_);
}

auto EmitCpp_(::Syntax_Node_ const *const syntaxTree_) -> void
{
}

::Token_Stream_ * tokenStream_ = ::None;

string Token_ = string("");

::System_::Text_::StringBuilder_ *const TypeDeclarations_ = new ::System_::Text_::StringBuilder_();

::System_::Text_::StringBuilder_ *const FunctionDeclarations_ = new ::System_::Text_::StringBuilder_();

::System_::Text_::StringBuilder_ *const ClassDeclarations_ = new ::System_::Text_::StringBuilder_();

::System_::Text_::StringBuilder_ *const Definitions_ = new ::System_::Text_::StringBuilder_();

int IndentDepth_ = 0;

bool AfterDeclaration_ = false;

string MainFunctionReturnType_ = string("");

bool MainFunctionAcceptsConsole_ = false;

bool MainFunctionAcceptsArgs_ = false;

auto Error_(string const message_) -> void
{
	Definitions_->Append_(string("<$ ") + message_ + string(" $>"));
}

auto BeginLine_(string const value_) -> void
{
	if (AfterDeclaration_)
	{
		Definitions_->AppendLine_();
		AfterDeclaration_ = false;
	}

	Definitions_->Append_(string('\t', IndentDepth_));
	Definitions_->Append_(value_);
}

auto Write_(string const value_) -> void
{
	Definitions_->Append_(value_);
}

auto EndLine_(string const value_) -> void
{
	Definitions_->Append_(value_);
	Definitions_->AppendLine_();
}

auto WriteLine_(string const value_) -> void
{
	if (AfterDeclaration_)
	{
		Definitions_->AppendLine_();
		AfterDeclaration_ = false;
	}

	Definitions_->Append_(string('\t', IndentDepth_));
	Definitions_->Append_(value_);
	Definitions_->AppendLine_();
}

auto BeginBlock_() -> void
{
	WriteLine_(string("{"));
	IndentDepth_ += 1;
}

auto EndBlock_() -> void
{
	AfterDeclaration_ = false;
	IndentDepth_ -= 1;
	WriteLine_(string("}"));
	AfterDeclaration_ = true;
}

auto IsIdentifierChar_(char const c_) -> bool
{
	return (c_ >= 'a' && c_ <= 'z') || (c_ >= 'A' && c_ <= 'Z') || c_ == '_';
}

auto IsNumberChar_(char const c_) -> bool
{
	return c_ >= '0' && c_ <= '9';
}

auto TokenIsIdentifier_() -> bool
{
	if (Token_->Length_ == 0)
	{
		return false;
	}

	for (char const c_ : *(Token_))
	{
		if (!IsIdentifierChar_(c_))
		{
			return false;
		}
	}

	return true;
}

auto Accept_(string const expected_) -> bool
{
	bool const accepted_ = Token_ == expected_;
	if (accepted_)
	{
		Token_ = tokenStream_->GetNextToken_();
	}

	return accepted_;
}

auto Expect_(string const expected_) -> void
{
	if (Token_ != expected_)
	{
		Error_(string("Expected `") + expected_ + string("` but found `") + Token_ + string("`"));
		Token_ = tokenStream_->GetNextToken_();
	}
	else
	{
		Token_ = tokenStream_->GetNextToken_();
	}
}

auto AcceptIdentifier_() -> bool
{
	if (!TokenIsIdentifier_())
	{
		return false;
	}

	Token_ = tokenStream_->GetNextToken_();
	return true;
}

auto AcceptString_() -> bool
{
	if (Token_->Length_ == 0 || Token_[0] != '"')
	{
		return false;
	}

	Token_ = tokenStream_->GetNextToken_();
	return true;
}

auto AcceptCodePoint_() -> bool
{
	if (Token_->Length_ == 0 || Token_[0] != '\'')
	{
		return false;
	}

	Token_ = tokenStream_->GetNextToken_();
	return true;
}

auto AcceptNumber_() -> bool
{
	for (char const c_ : *(Token_))
	{
		if (!IsNumberChar_(c_))
		{
			return false;
		}
	}

	Token_ = tokenStream_->GetNextToken_();
	return true;
}

auto ExpectIdentifier_() -> string
{
	if (!TokenIsIdentifier_())
	{
		Error_(string("Expected identifier, found `") + Token_ + string("`"));
		Token_ = tokenStream_->GetNextToken_();
		return string("<missing>");
	}

	string const identifier_ = Token_;
	Token_ = tokenStream_->GetNextToken_();
	return identifier_;
}

auto IsValueType_(string const type_) -> bool
{
	char const firstChar_ = type_[0];
	return firstChar_ >= 'a' && firstChar_ <= 'z';
}

auto ConvertType_(string const type_) -> string
{
	if (type_ == string("code_point"))
	{
		return string("char");
	}

	if (type_ == string("uint"))
	{
		return string("unsigned int");
	}

	if (type_ == string("int") || type_ == string("bool") || type_ == string("void") || type_ == string("string"))
	{
		return type_;
	}

	string const cppType_ = string("::") + type_->Replace_(string("."), string("_::"))->Replace_(string("<"), string("_<"));
	if (cppType_[cppType_->Length_ - 1] == '>')
	{
		return cppType_;
	}

	return cppType_ + string("_");
}

auto ConvertType_(bool const mutableBinding_, bool const mutableValue_, string type_) -> string
{
	bool const nullable_ = type_[type_->Length_ - 1] == '?';
	if (nullable_)
	{
		type_ = type_->Substring_(0, type_->Length_ - 1);
	}

	bool const isValueType_ = IsValueType_(type_);
	type_ = ConvertType_(type_);
	if (isValueType_)
	{
		if (nullable_)
		{
			type_ = string("::Maybe<") + type_ + string(">");
		}

		if (!mutableBinding_ && !mutableValue_)
		{
			type_ = type_ + string(" const");
		}
	}
	else
	{
		if (!mutableValue_)
		{
			type_ = type_ + string(" const");
		}

		type_ = type_ + string(" *");
		if (!mutableBinding_)
		{
			type_ = type_ + string("const");
		}
	}

	return type_;
}

auto ParseType_() -> string
{
	::System_::Text_::StringBuilder_ *const type_ = new ::System_::Text_::StringBuilder_(ExpectIdentifier_());
	while (Accept_(string(".")))
	{
		type_->Append_(string("."));
		type_->Append_(ExpectIdentifier_());
	}

	if (Accept_(string("<")))
	{
		type_->Append_(string("<"));
		bool const mutableValue_ = Accept_(string("mut"));
		type_->Append_(ConvertType_(true, mutableValue_, ParseType_()));
		Accept_(string(">"));
		type_->Append_(string(">"));
	}

	if (Accept_(string("?")))
	{
		type_->Append_(string("?"));
	}

	return type_->ToString_();
}

auto ParseAtom_() -> bool
{
	if (Accept_(string("new")))
	{
		string type_ = ParseType_();
		if (!IsValueType_(type_))
		{
			Write_(string("new "));
		}

		type_ = ConvertType_(type_);
		Write_(type_);
		Expect_(string("("));
		Write_(string("("));
		ParseCallArguments_();
		Expect_(string(")"));
		Write_(string(")"));
		return true;
	}

	if (Accept_(string("not")))
	{
		Write_(string("!"));
		ParseExpression_();
		return true;
	}

	if (Accept_(string("(")))
	{
		Write_(string("("));
		ParseExpression_();
		Expect_(string(")"));
		Write_(string(")"));
		return true;
	}

	if (Accept_(string("-")))
	{
		Write_(string("-"));
		ParseExpression_(7);
		return true;
	}

	if (Accept_(string("null")))
	{
		Write_(string("::None"));
		return true;
	}

	if (Accept_(string("self")))
	{
		Write_(string("this"));
		return true;
	}

	string const token_ = Token_;
	if (Accept_(string("true")) || Accept_(string("false")) || AcceptNumber_())
	{
		Write_(token_);
		return true;
	}

	if (AcceptIdentifier_())
	{
		Write_(token_ + string("_"));
		return true;
	}

	if (AcceptString_())
	{
		Write_(string("string(") + token_ + string(")"));
		return true;
	}

	if (AcceptCodePoint_())
	{
		Write_(token_);
		return true;
	}

	return false;
}

auto ParseCallArguments_() -> void
{
	bool first_ = true;
	do
	{
		if (first_)
		{
			first_ = false;
		}
		else
		{
			Write_(string(", "));
		}

		ParseExpression_();
	}
	while (Accept_(string(",")));
}

auto ParseExpression_(int const minPrecedence_) -> void
{
	if (!ParseAtom_())
	{
		return;
	}

	for (;;)
	{
		string const token_ = Token_;
		int precedence_;
		bool leftAssociative_;
		bool suffixOperator_ = false;
		if ((token_ == string("=") || token_ == string("+=") || token_ == string("-=")) && minPrecedence_ <= 1)
		{
			precedence_ = 1;
			leftAssociative_ = false;
			Write_(string(" ") + token_ + string(" "));
		}
		else if (token_ == string("or") && minPrecedence_ <= 2)
		{
			precedence_ = 2;
			leftAssociative_ = true;
			Write_(string(" || "));
		}
		else if (token_ == string("and") && minPrecedence_ <= 3)
		{
			precedence_ = 3;
			leftAssociative_ = true;
			Write_(string(" && "));
		}
		else if (token_ == string("==") && minPrecedence_ <= 4)
		{
			precedence_ = 4;
			leftAssociative_ = true;
			Write_(string(" == "));
		}
		else if (token_ == string("<>") && minPrecedence_ <= 4)
		{
			precedence_ = 4;
			leftAssociative_ = true;
			Write_(string(" != "));
		}
		else if ((token_ == string("<") || token_ == string("<=") || token_ == string(">") || token_ == string(">=")) && minPrecedence_ <= 5)
		{
			precedence_ = 5;
			leftAssociative_ = true;
			Write_(string(" ") + token_ + string(" "));
		}
		else if ((token_ == string("+") || token_ == string("-")) && minPrecedence_ <= 6)
		{
			precedence_ = 6;
			leftAssociative_ = true;
			Write_(string(" ") + token_ + string(" "));
		}
		else if (token_ == string("(") && minPrecedence_ <= 8)
		{
			Token_ = tokenStream_->GetNextToken_();
			Write_(string("("));
			ParseCallArguments_();
			if (Token_ != string(")"))
			{
				Error_(string("Expected `)` found `") + Token_ + string("`"));
			}

			Write_(string(")"));
			precedence_ = 8;
			leftAssociative_ = true;
			suffixOperator_ = true;
		}
		else if (token_ == string(".") && minPrecedence_ <= 8)
		{
			precedence_ = 8;
			leftAssociative_ = true;
			Write_(string("->"));
		}
		else if (token_ == string("[") && minPrecedence_ <= 8)
		{
			Token_ = tokenStream_->GetNextToken_();
			Write_(string("["));
			ParseExpression_();
			if (Token_ != string("]"))
			{
				Error_(string("Expected `]` found `") + Token_ + string("`"));
			}

			Write_(string("]"));
			precedence_ = 8;
			leftAssociative_ = true;
			suffixOperator_ = true;
		}
		else
		{
			break;
		}

		Token_ = tokenStream_->GetNextToken_();
		if (!suffixOperator_)
		{
			if (leftAssociative_)
			{
				precedence_ += 1;
			}

			ParseExpression_(precedence_);
		}
	}
}

auto ParseExpression_() -> void
{
	ParseExpression_(1);
}

auto ParseStatement_() -> bool
{
	if (Accept_(string("return")))
	{
		if (Accept_(string(";")))
		{
			WriteLine_(string("return;"));
		}
		else
		{
			BeginLine_(string("return "));
			ParseExpression_();
			Expect_(string(";"));
			EndLine_(string(";"));
		}

		return true;
	}

	if (Accept_(string("loop")))
	{
		WriteLine_(string("for (;;)"));
		ParseBlock_();
		return true;
	}

	if (Accept_(string("while")))
	{
		BeginLine_(string("while ("));
		ParseExpression_();
		EndLine_(string(")"));
		ParseBlock_();
		return true;
	}

	if (Accept_(string("for")))
	{
		BeginLine_(string("for ("));
		string const k_ = Token_;
		if (!Accept_(string("let")) && !Accept_(string("var")))
		{
			Error_(string("Expected `let` or `var` but found `") + Token_ + string("`"));
		}

		string const name_ = ExpectIdentifier_();
		Expect_(string(":"));
		bool const mutableValue_ = Accept_(string("mut"));
		string const type_ = ParseType_();
		Write_(ConvertType_(k_ == string("var"), mutableValue_, type_) + string(" ") + name_ + string("_"));
		Expect_(string("in"));
		Write_(string(" : *("));
		ParseExpression_();
		EndLine_(string("))"));
		ParseBlock_();
		return true;
	}

	if (Accept_(string("do")))
	{
		WriteLine_(string("do"));
		ParseBlock_();
		AfterDeclaration_ = false;
		Expect_(string("while"));
		BeginLine_(string("while ("));
		ParseExpression_();
		Expect_(string(";"));
		EndLine_(string(");"));
		return true;
	}

	if (Accept_(string("if")))
	{
		BeginLine_(string("if ("));
		ParseExpression_();
		EndLine_(string(")"));
		ParseBlock_();
		while (Accept_(string("else")))
		{
			AfterDeclaration_ = false;
			if (Accept_(string("if")))
			{
				BeginLine_(string("else if ("));
				ParseExpression_();
				EndLine_(string(")"));
				ParseBlock_();
			}
			else
			{
				WriteLine_(string("else"));
				ParseBlock_();
				return true;
			}
		}

		return true;
	}

	if (Accept_(string("break")))
	{
		Expect_(string(";"));
		WriteLine_(string("break;"));
		return true;
	}

	if (Accept_(string("continue")))
	{
		Expect_(string(";"));
		WriteLine_(string("continue;"));
		return true;
	}

	string const kind_ = Token_;
	if (Accept_(string("let")) || Accept_(string("var")))
	{
		string const variableName_ = ExpectIdentifier_();
		Expect_(string(":"));
		bool const mutableValue_ = Accept_(string("mut"));
		string variableType_ = ParseType_();
		variableType_ = ConvertType_(kind_ == string("var"), mutableValue_, variableType_);
		BeginLine_(variableType_);
		Write_(string(" ") + variableName_ + string("_"));
		if (Accept_(string("=")))
		{
			Write_(string(" = "));
			ParseExpression_();
		}

		Expect_(string(";"));
		EndLine_(string(";"));
		return true;
	}

	if (Token_ == string("}"))
	{
		return false;
	}

	BeginLine_(string(""));
	ParseExpression_();
	Expect_(string(";"));
	EndLine_(string(";"));
	return true;
}

auto ParseBlock_() -> void
{
	Expect_(string("{"));
	BeginBlock_();
	while (ParseStatement_())
	{
	}

	Expect_(string("}"));
	EndBlock_();
}

auto ParseArgumentsDeclaration_(bool const isMainFunction_, bool const isMethod_) -> string
{
	if (!isMethod_)
	{
		Expect_(string("("));
	}

	if (Accept_(string(")")))
	{
		return string("");
	}

	::System_::Text_::StringBuilder_ *const arguments_ = new ::System_::Text_::StringBuilder_();
	do
	{
		bool const mutableBinding_ = Accept_(string("var"));
		string const name_ = ExpectIdentifier_();
		Expect_(string(":"));
		bool const mutableValue_ = Accept_(string("mut"));
		string const type_ = ParseType_();
		if (isMainFunction_)
		{
			if (type_ == string("System.Console.Console"))
			{
				MainFunctionAcceptsConsole_ = true;
			}

			if (type_ == string("System.Console.Arguments"))
			{
				MainFunctionAcceptsArgs_ = true;
			}
		}

		arguments_->Append_(ConvertType_(mutableBinding_, mutableValue_, type_) + string(" ") + name_ + string("_, "));
	}
	while (Accept_(string(",")));
	Expect_(string(")"));
	string const result_ = arguments_->ToString_();
	return result_->Substring_(0, result_->Length_ - 2);
}

auto ParseConstructorArgumentsDeclaration_() -> string
{
	return ParseArgumentsDeclaration_(false, false);
}

auto ParseFunctionArgumentsDeclaration_(bool const isMainFunction_) -> string
{
	return ParseArgumentsDeclaration_(isMainFunction_, false);
}

auto ParseMethodArgumentsDeclaration_() -> string
{
	return ParseArgumentsDeclaration_(false, true);
}

auto ParseClassMember_(string const className_) -> void
{
	string const accessModifier_ = Token_;
	if (Accept_(string("public")) || Accept_(string("internal")) || Accept_(string("protected")) || Accept_(string("private")))
	{
	}
	else
	{
		Error_(string("Expected access modifier, found `") + accessModifier_ + string("`"));
	}

	if (Accept_(string("new")))
	{
		string const arguments_ = ParseConstructorArgumentsDeclaration_();
		ClassDeclarations_->AppendLine_(string("\t") + className_ + string("_(") + arguments_ + string(");"));
		WriteLine_(string("::") + className_ + string("_::") + className_ + string("_(") + arguments_ + string(")"));
		ParseBlock_();
		return;
	}

	string const kind_ = Token_;
	if (Accept_(string("var")) || Accept_(string("let")))
	{
		string const fieldName_ = ExpectIdentifier_();
		Expect_(string(":"));
		bool const mutableValue_ = Accept_(string("mut"));
		string fieldType_ = ParseType_();
		fieldType_ = ConvertType_(true, mutableValue_, fieldType_);
		Expect_(string(";"));
		ClassDeclarations_->AppendLine_(string("\t") + fieldType_ + string(" ") + fieldName_ + string("_;"));
		return;
	}

	string const methodName_ = ExpectIdentifier_();
	Expect_(string("("));
	bool const mutableSelf_ = Accept_(string("mut"));
	bool isAssociatedFuntion_ = !mutableSelf_;
	if (mutableSelf_)
	{
		Expect_(string("self"));
		if (Token_ != string(")"))
		{
			Expect_(string(","));
		}
	}
	else
	{
		if (Accept_(string("self")))
		{
			isAssociatedFuntion_ = false;
			if (Token_ != string(")"))
			{
				Expect_(string(","));
			}
		}
	}

	string const arguments_ = ParseMethodArgumentsDeclaration_();
	Expect_(string("->"));
	bool const mutableValue_ = Accept_(string("mut"));
	string const returnType_ = ParseType_();
	string const convertedReturnType_ = ConvertType_(true, mutableValue_, returnType_);
	string staticModifier_ = string("");
	if (isAssociatedFuntion_)
	{
		staticModifier_ = string("static ");
	}

	string constModifier_ = string("");
	if (!mutableSelf_ && !isAssociatedFuntion_)
	{
		constModifier_ = string("const ");
	}

	ClassDeclarations_->AppendLine_(string("\t") + staticModifier_ + string("auto ") + methodName_ + string("_(") + arguments_ + string(") ") + constModifier_ + string("-> ") + convertedReturnType_ + string(";"));
	WriteLine_(string("auto ::") + className_ + string("_::") + methodName_ + string("_(") + arguments_ + string(") ") + constModifier_ + string("-> ") + convertedReturnType_);
	ParseBlock_();
}

auto ParseDeclaration_() -> void
{
	string const accessModifier_ = Token_;
	if (Accept_(string("public")) || Accept_(string("internal")))
	{
	}
	else
	{
		Error_(string("Expected access modifier, found `") + accessModifier_ + string("`"));
	}

	string const kind_ = Token_;
	if (Accept_(string("var")) || Accept_(string("let")))
	{
		string const variableName_ = ExpectIdentifier_();
		Expect_(string(":"));
		bool const mutableValue_ = Accept_(string("mut"));
		string variableType_ = ParseType_();
		Expect_(string("="));
		variableType_ = ConvertType_(kind_ == string("var"), mutableValue_, variableType_);
		BeginLine_(variableType_);
		Write_(string(" ") + variableName_ + string("_ = "));
		ParseExpression_();
		Expect_(string(";"));
		EndLine_(string(";"));
		AfterDeclaration_ = true;
		return;
	}

	if (Accept_(string("class")))
	{
		string const className_ = ExpectIdentifier_();
		TypeDeclarations_->AppendLine_(string("class ") + className_ + string("_;"));
		ClassDeclarations_->AppendLine_(string("class ") + className_ + string("_"));
		Expect_(string("{"));
		ClassDeclarations_->AppendLine_(string("{"));
		ClassDeclarations_->AppendLine_(string("public:"));
		while (!Accept_(string("}")))
		{
			ParseClassMember_(className_);
		}

		ClassDeclarations_->AppendLine_(string("};"));
		return;
	}

	if (Accept_(string("enum")))
	{
		Expect_(string("struct"));
		string const enumName_ = ExpectIdentifier_();
		TypeDeclarations_->AppendLine_(string("enum class ") + enumName_ + string("_;"));
		ClassDeclarations_->AppendLine_(string("enum class ") + enumName_ + string("_"));
		Expect_(string("{"));
		ClassDeclarations_->AppendLine_(string("{"));
		do
		{
			string const enumValue_ = ExpectIdentifier_();
			ClassDeclarations_->Append_(string("\t") + enumValue_ + string("_"));
			if (Accept_(string("=")))
			{
				ClassDeclarations_->Append_(string(" = "));
				string const value_ = Token_;
				if (AcceptNumber_())
				{
					ClassDeclarations_->Append_(value_);
				}
				else
				{
					Error_(string("Expected number found `") + value_ + string("`"));
				}
			}

			ClassDeclarations_->AppendLine_(string(","));
		}
		while (Accept_(string(",")));
		Expect_(string("}"));
		ClassDeclarations_->AppendLine_(string("};"));
		return;
	}

	string const name_ = ExpectIdentifier_();
	string const arguments_ = ParseFunctionArgumentsDeclaration_(name_ == string("Main"));
	Expect_(string("->"));
	bool const mutableValue_ = Accept_(string("mut"));
	string const returnType_ = ParseType_();
	string const convertedReturnType_ = ConvertType_(true, mutableValue_, returnType_);
	FunctionDeclarations_->AppendLine_(string("auto ") + name_ + string("_(") + arguments_ + string(") -> ") + convertedReturnType_ + string(";"));
	WriteLine_(string("auto ") + name_ + string("_(") + arguments_ + string(") -> ") + convertedReturnType_);
	if (name_ == string("Main"))
	{
		if (MainFunctionReturnType_ != string(""))
		{
			Error_(string("Multiple declarations of main"));
		}

		MainFunctionReturnType_ = returnType_;
	}

	ParseBlock_();
}

auto ParseCompilationUnit_() -> void
{
	while (TokenIsIdentifier_())
	{
		ParseDeclaration_();
	}
}

auto EmitPreamble_() -> void
{
	TypeDeclarations_->AppendLine_(string("#include \"RuntimeLibrary.h\""));
	TypeDeclarations_->AppendLine_(string(""));
	TypeDeclarations_->AppendLine_(string("// Type Declarations"));
	FunctionDeclarations_->AppendLine_(string(""));
	FunctionDeclarations_->AppendLine_(string("// Function Declarations"));
	ClassDeclarations_->AppendLine_(string(""));
	ClassDeclarations_->AppendLine_(string("// Class Declarations"));
	WriteLine_(string(""));
	WriteLine_(string("// Definitions"));
}

auto EmitEntryPointAdapter_(::System_::Collections_::List_<::Source_Text_ const *> const *const resources_) -> void
{
	WriteLine_(string("// Entry Point Adapter"));
	WriteLine_(string("int main(int argc, char const *const * argv)"));
	BeginBlock_();
	for (::Source_Text_ const *const resource_ : *(resources_))
	{
		BeginLine_(string("resource_manager_->AddResource(::string(\""));
		Write_(resource_->Name_);
		Write_(string("\"), ::string(\""));
		Write_(resource_->Text_->Replace_(string("\\"), string("\\\\"))->Replace_(string("\n"), string("\\n"))->Replace_(string("\r"), string("\\r"))->Replace_(string("\""), string("\\\"")));
		EndLine_(string("\"));"));
	}

	if (resources_->Length_() > 0)
	{
		EndLine_(string(""));
	}

	::System_::Text_::StringBuilder_ *const args_ = new ::System_::Text_::StringBuilder_();
	if (MainFunctionAcceptsConsole_)
	{
		args_->Append_(string("new ::System_::Console_::Console_()"));
	}

	if (MainFunctionAcceptsArgs_)
	{
		if (MainFunctionAcceptsConsole_)
		{
			args_->Append_(string(", "));
		}

		args_->Append_(string("new ::System_::Console_::Arguments_(argc, argv)"));
	}

	if (MainFunctionReturnType_ == string("void"))
	{
		WriteLine_(string("Main_(") + args_->ToString_() + string(");"));
		WriteLine_(string("return 0;"));
	}
	else
	{
		WriteLine_(string("return Main_(") + args_->ToString_() + string(");"));
	}

	EndBlock_();
}

auto Compile_(::System_::Collections_::List_<::Source_Text_ const *> const *const sources_, ::System_::Collections_::List_<::Source_Text_ const *> const *const resources_) -> string
{
	EmitPreamble_();
	::Lexer_ const *const lexer_ = new ::Lexer_();
	for (::Source_Text_ const *const source_ : *(sources_))
	{
		tokenStream_ = lexer_->Analyze_(source_);
		Token_ = tokenStream_->GetNextToken_();
		ParseCompilationUnit_();
	}

	EmitEntryPointAdapter_(resources_);
	return TypeDeclarations_->ToString_() + FunctionDeclarations_->ToString_() + ClassDeclarations_->ToString_() + Definitions_->ToString_();
}

auto Main_(::System_::Console_::Console_ *const console_, ::System_::Console_::Arguments_ const *const args_) -> void
{
	console_->WriteLine_(string("Adamant Compiler v0.1.0"));
	::System_::Collections_::List_<string> *const sourceFilePaths_ = new ::System_::Collections_::List_<string>();
	::System_::Collections_::List_<string> *const resourceFilePaths_ = new ::System_::Collections_::List_<string>();
	string outputFilePath_ = string("");
	int argType_ = 0;
	for (string const arg_ : *(args_))
	{
		if (argType_ == 1)
		{
			resourceFilePaths_->Add_(arg_);
			argType_ = 0;
		}
		else if (argType_ == 2)
		{
			outputFilePath_ = arg_;
			argType_ = 0;
		}
		else
		{
			if (arg_ == string("-r"))
			{
				argType_ = 1;
			}
			else if (arg_ == string("-o"))
			{
				argType_ = 2;
			}
			else
			{
				sourceFilePaths_->Add_(arg_);
			}
		}
	}

	if (sourceFilePaths_->Length_() == 0 || outputFilePath_ == string(""))
	{
		console_->WriteLine_(string("Args: <Input File(s)> -o <OutputFile> -r <Resource File>"));
		return;
	}

	::System_::Collections_::List_<::Source_Text_ const *> *const resources_ = new ::System_::Collections_::List_<::Source_Text_ const *>();
	if (resourceFilePaths_->Length_() > 0)
	{
		console_->WriteLine_(string("Reading Resources:"));
		for (string const resourceFilePath_ : *(resourceFilePaths_))
		{
			console_->WriteLine_(string("  ") + resourceFilePath_);
			resources_->Add_(ReadSource_(resourceFilePath_));
		}
	}

	console_->WriteLine_(string("Compiling:"));
	::System_::Collections_::List_<::Source_Text_ const *> *const sources_ = new ::System_::Collections_::List_<::Source_Text_ const *>();
	for (string const sourceFilePath_ : *(sourceFilePaths_))
	{
		console_->WriteLine_(string("  ") + sourceFilePath_);
		sources_->Add_(ReadSource_(sourceFilePath_));
	}

	string const translated_ = Compile_(sources_, resources_);
	console_->Write_(string("Output: "));
	console_->WriteLine_(outputFilePath_);
	::System_::IO_::FileWriter_ *const outputFile_ = new ::System_::IO_::FileWriter_(outputFilePath_);
	outputFile_->Write_(translated_);
	outputFile_->Close_();
	console_->Write_(string("Outputting RuntimeLibrary to: "));
	string outputDirPath_ = outputFilePath_;
	int index_ = outputDirPath_->LastIndexOf_('/');
	if (index_ != -1)
	{
		outputDirPath_ = outputDirPath_->Substring_(0, index_ + 1);
	}

	index_ = outputDirPath_->LastIndexOf_('\\');
	if (index_ != -1)
	{
		outputDirPath_ = outputDirPath_->Substring_(0, index_ + 1);
	}

	console_->WriteLine_(outputDirPath_);
	::System_::IO_::FileWriter_ * resourceFile_ = new ::System_::IO_::FileWriter_(outputDirPath_ + string("RuntimeLibrary.h"));
	resourceFile_->Write_(resource_manager_->GetString_(string("RuntimeLibrary.h")));
	resourceFile_->Close_();
	resourceFile_ = new ::System_::IO_::FileWriter_(outputDirPath_ + string("RuntimeLibrary.cpp"));
	resourceFile_->Write_(resource_manager_->GetString_(string("RuntimeLibrary.cpp")));
	resourceFile_->Close_();
}

auto ReadSource_(string const path_) -> ::Source_Text_ const *
{
	::System_::IO_::FileReader_ *const file_ = new ::System_::IO_::FileReader_(path_);
	string const contents_ = file_->ReadToEndSync_();
	file_->Close_();
	string name_ = path_;
	int index_ = name_->LastIndexOf_('/');
	if (index_ != -1)
	{
		name_ = name_->Substring_(index_ + 1);
	}

	index_ = name_->LastIndexOf_('\\');
	if (index_ != -1)
	{
		name_ = name_->Substring_(index_ + 1);
	}

	return new ::Source_Text_(string("<default>"), name_, contents_);
}

::Source_Text_::Source_Text_(string const package_, string const name_, string const text_)
{
	Package_ = package_;
	Name_ = name_;
	Text_ = text_;
}

::Syntax_Token_::Syntax_Token_(::Token_Type_ const *const tokenType_, ::Source_Text_ const *const source_, unsigned int const start_, unsigned int const length_)
{
	TokenType_ = tokenType_;
	Source_ = source_;
	Start_ = start_;
	Length_ = length_;
}

::Token_Stream_::Token_Stream_(::Source_Text_ const *const source_)
{
	Source_ = source_;
	nextToken_ = 0;
}

auto ::Token_Stream_::GetNextToken_() -> string
{
	unsigned int position_ = nextToken_;
	unsigned int tokenEnd_ = -1;
	bool escaped_;
	bool done_ = false;
	while (!done_ && position_ < Source_->Text_->Length_)
	{
		char const curChar_ = Source_->Text_[position_];
		if (curChar_ == ' ' || curChar_ == '\t' || curChar_ == '\n' || curChar_ == '\r')
		{
			position_ += 1;
			continue;
		}
		else if (curChar_ == '{' || curChar_ == '}' || curChar_ == '(' || curChar_ == ')' || curChar_ == ';' || curChar_ == ',' || curChar_ == '.' || curChar_ == ':' || curChar_ == '[' || curChar_ == ']' || curChar_ == '?')
		{
			tokenEnd_ = position_ + 1;
			break;
		}
		else if (curChar_ == '=')
		{
			if (position_ + 1 < Source_->Text_->Length_ && Source_->Text_[position_ + 1] == '=')
			{
				tokenEnd_ = position_ + 2;
				break;
			}

			tokenEnd_ = position_ + 1;
			break;
		}
		else if (curChar_ == '+')
		{
			if (position_ + 1 < Source_->Text_->Length_ && Source_->Text_[position_ + 1] == '=')
			{
				tokenEnd_ = position_ + 2;
				break;
			}

			tokenEnd_ = position_ + 1;
			break;
		}
		else if (curChar_ == '-')
		{
			if (position_ + 1 < Source_->Text_->Length_ && Source_->Text_[position_ + 1] == '>')
			{
				tokenEnd_ = position_ + 2;
				break;
			}

			if (position_ + 1 < Source_->Text_->Length_ && Source_->Text_[position_ + 1] == '=')
			{
				tokenEnd_ = position_ + 2;
				break;
			}

			tokenEnd_ = position_ + 1;
			break;
		}
		else if (curChar_ == '/')
		{
			if (position_ + 1 < Source_->Text_->Length_ && Source_->Text_[position_ + 1] == '/')
			{
				while (position_ < Source_->Text_->Length_ && Source_->Text_[position_] != '\r' && Source_->Text_[position_] != '\n')
				{
					position_ += 1;
				}

				continue;
			}

			tokenEnd_ = position_ + 1;
			break;
		}
		else if (curChar_ == '<')
		{
			if (position_ + 1 < Source_->Text_->Length_ && Source_->Text_[position_ + 1] == '>')
			{
				tokenEnd_ = position_ + 2;
				break;
			}

			if (position_ + 1 < Source_->Text_->Length_ && Source_->Text_[position_ + 1] == '=')
			{
				tokenEnd_ = position_ + 2;
				break;
			}

			tokenEnd_ = position_ + 1;
			break;
		}
		else if (curChar_ == '>')
		{
			if (position_ + 1 < Source_->Text_->Length_ && Source_->Text_[position_ + 1] == '=')
			{
				tokenEnd_ = position_ + 2;
				break;
			}

			tokenEnd_ = position_ + 1;
			break;
		}
		else if (curChar_ == '"')
		{
			tokenEnd_ = position_ + 1;
			escaped_ = false;
			while (tokenEnd_ < Source_->Text_->Length_ && (Source_->Text_[tokenEnd_] != '"' || escaped_))
			{
				escaped_ = Source_->Text_[tokenEnd_] == '\\' && !escaped_;
				tokenEnd_ += 1;
			}

			tokenEnd_ += 1;
			break;
		}
		else if (curChar_ == '\'')
		{
			tokenEnd_ = position_ + 1;
			escaped_ = false;
			while (tokenEnd_ < Source_->Text_->Length_ && (Source_->Text_[tokenEnd_] != '\'' || escaped_))
			{
				escaped_ = Source_->Text_[tokenEnd_] == '\\' && !escaped_;
				tokenEnd_ += 1;
			}

			tokenEnd_ += 1;
			break;
		}
		else
		{
			if (IsIdentifierChar_(curChar_))
			{
				tokenEnd_ = position_ + 1;
				while (IsIdentifierChar_(Source_->Text_[tokenEnd_]))
				{
					tokenEnd_ += 1;
				}

				break;
			}

			if ((IsNumberChar_(curChar_)))
			{
				tokenEnd_ = position_ + 1;
				while (IsNumberChar_(Source_->Text_[tokenEnd_]))
				{
					tokenEnd_ += 1;
				}

				break;
			}

			Error_(string("Lexer: Invalid character `") + curChar_ + string("`"));
			position_ += 1;
		}
	}

	string token_;
	if (tokenEnd_ == -1)
	{
		token_ = string("");
		nextToken_ = position_;
	}
	else
	{
		token_ = Source_->Text_->Substring_(position_, tokenEnd_ - position_);
		nextToken_ = tokenEnd_;
	}

	return token_;
}

auto ::Token_Stream_::NewToken_(::Token_Type_ const *const type_, unsigned int const end_) -> ::Syntax_Token_ const *
{
	return ::None;
}

auto ::Token_Stream_::NewToken_(::Token_Type_ const *const type_) -> ::Syntax_Token_ const *
{
	return ::None;
}

// Entry Point Adapter
int main(int argc, char const *const * argv)
{
	resource_manager_->AddResource(::string("RuntimeLibrary.cpp"), ::string("#include \"RuntimeLibrary.h\"\n#include <map>\n\nstring::string()\n	: Length_(0), Buffer(0)\n{\n}\n\nstring::string(char c, int repeat)\n	: Length_(repeat)\n{\n	char* buffer = new char[repeat];\n	for (int i = 0; i < repeat; i++)\n		buffer[i] = c;\n\n	Buffer = buffer;\n}\n\nstring::string(const char* s)\n	: Length_(std::strlen(s)), Buffer(s)\n{\n}\n\nstring::string(int length, const char* s)\n	: Length_(length), Buffer(s)\n{\n}\n\nchar const * string::cstr() const\n{\n	auto buffer = new char[Length_ + 1];\n	std::memcpy(buffer, Buffer, Length_);\n	buffer[Length_] = 0;\n	return buffer;\n}\n\nstring string::Substring_(int start, int length) const\n{\n	return string(length, Buffer + start);\n}\n\nstring string::Replace_(string oldValue, string newValue) const\n{\n	string buffer = \"\";\n	int limit = Length_ - oldValue.Length_ + 1;\n	int lastIndex = 0;\n	for(int i=0; i < limit; i++)\n		if (Substring_(i, oldValue.Length_) == oldValue)\n		{\n			buffer = buffer + Substring_(lastIndex, i-lastIndex) + newValue;\n			i += oldValue.Length_; // skip over the value we just matched\n			lastIndex = i;\n			i--; // we need i-- to offset the i++ that is about to happen\n		}\n\n	buffer = buffer + Substring_(lastIndex, Length_ - lastIndex);\n	return buffer;\n}\n\nint string::LastIndexOf_(char c) const\n{\n	for(int i=Length_-1; i>=0; i--)\n		if(Buffer[i]==c)\n			return i;\n\n	return -1;\n}\n\nchar string::operator[] (const int index) const\n{\n	return Buffer[index];\n}\n\nstring string::operator+(const string& value) const\n{\n	int newLength = Length_ + value.Length_;\n	char* chars = new char[newLength];\n	size_t offset = sizeof(char)*Length_;\n	std::memcpy(chars, Buffer, offset);\n	std::memcpy(chars + offset, value.Buffer, value.Length_);\n	return string(newLength, chars);\n}\n\nstring string::operator+(const char& value) const\n{\n	int newLength = Length_ + 1;\n	char* chars = new char[newLength];\n	size_t offset = sizeof(char)*Length_;\n	std::memcpy(chars, Buffer, offset);\n	chars[Length_] = value;\n	return string(newLength, chars);\n}\n\nbool string::operator==(const string &other) const\n{\n	if (Length_ != other.Length_)\n		return false;\n\n	for (int i = 0; i < Length_; i++)\n		if (Buffer[i] != other.Buffer[i])\n			return false;\n\n	return true;\n}\n\nbool operator < (string const& lhs, string const& rhs)\n{\n    return std::strcmp(lhs.cstr(), rhs.cstr()) < 0;\n}\n\nstd::map<string, string> resourceValues;\n\nstring const & ResourceManager::GetString_(string resourceName)\n{\n	return resourceValues.at(resourceName);\n}\nvoid ResourceManager::AddResource(string name, string value)\n{\n	resourceValues.insert(std::make_pair(name, value));\n}\n\nResourceManager *const resource_manager_ = new ResourceManager();\n\nnamespace System_\n{\n	namespace Console_\n	{\n		void Console_::Write_(string value)\n		{\n			std::printf(\"%.*s\", value.Length_, value.Buffer);\n		}\n\n		void Console_::WriteLine_(string value)\n		{\n			std::printf(\"%.*s\\n\", value.Length_, value.Buffer);\n		}\n\n		void Console_::WriteLine_()\n		{\n			std::printf(\"\\n\");\n		}\n\n		Arguments_::Arguments_(int argc, char const *const * argv)\n			: Count_(argc-1)\n		{\n			args = new string[Count_];\n			for (int i = 0; i < Count_; i++)\n				args[i] = string(argv[i+1]);\n		}\n	}\n\n	namespace IO_\n	{\n		FileReader_::FileReader_(const string& fileName)\n		{\n			std::FILE* foo;\n			auto fname = fileName.cstr();\n			file = std::fopen(fname, \"rb\");\n			delete[] fname;\n		}\n\n		string FileReader_::ReadToEndSync_()\n		{\n			std::fseek(file, 0, SEEK_END);\n			auto length = std::ftell(file);\n			std::fseek(file, 0, SEEK_SET);\n			auto buffer = new char[length];\n			length = std::fread(buffer, sizeof(char), length, file);\n			return string(length, buffer);\n		}\n\n		void FileReader_::Close_()\n		{\n			std::fclose(file);\n		}\n\n		FileWriter_::FileWriter_(const string& fileName)\n		{\n			auto fname = fileName.cstr();\n			file = std::fopen(fname, \"wb\"); // TODO check error\n			delete[] fname;\n		}\n\n		void FileWriter_::Write_(const string& value)\n		{\n			std::fwrite(value.Buffer, sizeof(char), value.Length_, file);\n		}\n\n		void FileWriter_::Close_()\n		{\n			std::fclose(file);\n		}\n	}\n\n	namespace Text_\n	{\n		StringBuilder_::StringBuilder_(string const & value)\n			: buffer(value)\n		{\n		}\n\n		StringBuilder_::StringBuilder_()\n			: buffer(\"\")\n		{\n		}\n\n		void StringBuilder_::Append_(string const & value)\n		{\n			buffer = buffer + value;\n		}\n\n		void StringBuilder_::AppendLine_(string const & value)\n		{\n			buffer = buffer + value + string(\"\\n\");\n		}\n\n		void StringBuilder_::AppendLine_()\n		{\n			buffer = buffer + string(\"\\n\");\n		}\n	}\n}\n"));
	resource_manager_->AddResource(::string("RuntimeLibrary.h"), ::string("// On windows this disables warnings about using fopen_s instead of fopen\r\n// It must be defined before including the headers.  The includes have been moved\r\n// here to avoid leaking this into the program being compiled.  This required the\r\n// use of void* instead of FILE* in some places.\r\n#define _CRT_SECURE_NO_WARNINGS\r\n#include <cstring>\r\n#include <cstdio>\r\n\r\nstruct string\r\n{\r\npublic:\r\n	int Length_;\r\n	char const * Buffer;\r\n\r\n	string();\r\n	string(char c, int repeat);\r\n	string(char const * s);\r\n	string(int length, char const * s);\r\n	char const * cstr() const;\r\n	string Substring_(int start, int length) const;\r\n	string Substring_(int start) const { return Substring_(start, Length_-start); }\r\n	string Replace_(string oldValue, string newValue) const;\r\n	int LastIndexOf_(char c) const;\r\n	char operator[] (int const index) const;\r\n	string operator+(string const & value) const;\r\n	string operator+(char const & value) const;\r\n	string const * operator->() const { return this; }\r\n	string const & operator* () const { return *this; }\r\n	bool operator==(string const & other) const;\r\n	bool operator!=(string const & other) const { return !(*this == other); }\r\n	friend bool operator<(string const & lhs, string const & rhs);\r\n\r\n	typedef char const * const_iterator;\r\n	const_iterator begin() const { return &Buffer[0]; }\r\n	const_iterator end() const { return &Buffer[Length_]; }\r\n};\r\n\r\nclass ResourceManager\r\n{\r\npublic:\r\n	string const & GetString_(string resourceName);\r\n	void AddResource(string name, string value);\r\n};\r\n\r\nextern ResourceManager *const resource_manager_;\r\n\r\nclass NoneType\r\n{\r\npublic:\r\n	template<class T>\r\n	operator T*() const { return static_cast<T*>(0); }\r\n};\r\nstatic const NoneType None = NoneType();\r\n\r\ntemplate<typename T>\r\nstruct Maybe\r\n{\r\nprivate:\r\n	T data;\r\n	bool hasValue;\r\n\r\npublic:\r\n	Maybe(T const & value) : data(value), hasValue(true) {}\r\n	Maybe(::NoneType const & none) : hasValue(false) {}\r\n	T& operator->() { return data; }\r\n	T const & operator->() const\r\n	{\r\n		if(!hasValue) throw \"Access to null Maybe value\";\r\n		return data;\r\n	}\r\n	T  & operator* ()\r\n	{\r\n		if(!hasValue) throw \"Access to null Maybe value\";\r\n		return data;\r\n	}\r\n	T const & operator* () const\r\n	{\r\n		if(!hasValue) throw \"Access to null Maybe value\";\r\n		return data;\r\n	}\r\n	bool operator==(T const & other) const\r\n	{\r\n		return hasValue && data == other;\r\n	}\r\n	bool operator!=(T const & other) const\r\n	{\r\n		return hasValue && data != other;\r\n	}\r\n};\r\n\r\nnamespace System_\r\n{\r\n	namespace Collections_\r\n	{\r\n		template<typename T>\r\n		class List_\r\n		{\r\n		private:\r\n			T* values;\r\n			int length;\r\n			int capacity;\r\n\r\n		public:\r\n			typedef T const * const_iterator;\r\n\r\n			List_() : values(0), length(0), capacity(0) { }\r\n			void Add_(T value);\r\n			int Length_() const { return length; }\r\n			const_iterator begin() const { return values; }\r\n			const_iterator end() const { return &values[length]; }\r\n			T const & Get_(int const index) const { return values[index]; }\r\n		};\r\n\r\n		template<typename T>\r\n		void List_<T>::Add_(T value)\r\n		{\r\n			if(length >= capacity)\r\n			{\r\n				int newCapacity = capacity == 0 ? 16 : capacity * 2;\r\n				T* newValues = new T[newCapacity];\r\n				std::memcpy(newValues, values, length * sizeof(T));\r\n				values = newValues;\r\n				capacity = newCapacity;\r\n			}\r\n			values[length] = value;\r\n			length++;\r\n		}\r\n	}\r\n\r\n	namespace Console_\r\n	{\r\n		class Console_\r\n		{\r\n		public:\r\n			void Write_(string value);\r\n			void WriteLine_(string value);\r\n			void WriteLine_();\r\n		};\r\n\r\n		class Arguments_\r\n		{\r\n		private:\r\n			string* args;\r\n		public:\r\n			typedef string const * const_iterator;\r\n			const int Count_;\r\n\r\n			Arguments_(int argc, char const *const * argv);\r\n			const_iterator begin() const { return &args[0]; }\r\n			const_iterator end() const { return &args[Count_]; }\r\n			string const & Get_(int const index) const { return args[index]; }\r\n		};\r\n	}\r\n\r\n	namespace IO_\r\n	{\r\n		class FileReader_\r\n		{\r\n		private:\r\n			std::FILE* file;\r\n\r\n		public:\r\n			FileReader_(const string& fileName);\r\n			string ReadToEndSync_();\r\n			void Close_();\r\n		};\r\n\r\n		class FileWriter_\r\n		{\r\n		private:\r\n			std::FILE* file;\r\n\r\n		public:\r\n			FileWriter_(const string& fileName);\r\n			void Write_(const string& value);\r\n			void Close_();\r\n		};\r\n	}\r\n\r\n	namespace Text_\r\n	{\r\n		class StringBuilder_\r\n		{\r\n		private:\r\n			string buffer;\r\n		public:\r\n			StringBuilder_();\r\n			StringBuilder_(string const & value);\r\n			void Append_(string const & value);\r\n			void AppendLine_(string const& value);\r\n			void AppendLine_();\r\n			string ToString_() const { return buffer; }\r\n		};\r\n	}\r\n}\r\n"));

	Main_(new ::System_::Console_::Console_(), new ::System_::Console_::Arguments_(argc, argv));
	return 0;
}
