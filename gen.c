/*
   generate Language enum, array to store all languages data - C code using the data
   from languages.toml
 * */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char *data;
size_t len;

uint16_t line;
uint16_t col;
char *start;
char *current;

bool logs;

typedef enum {
	TIER_SYSTEMS,
	TIER_APPLICATION,
	TIER_SCRIPTING,
	TIER_WEB
} Tier;

typedef enum {
	PARADIGM_IMPERATIVE,
	PARADIGM_OOP,
	PARADIGM_FUNCTIONAL,
	PARADIGM_CONCURRENT,
	PARADIGM_LOGIC,
	PARADIGM_DECLARATIVE
} Paradigm;

typedef enum {
	TYPE_PROGRAMMING,
	TYPE_ASSEMBLY,
	TYPE_TEXT,
	TYPE_MARKUP,
	TYPE_QUERY,
	TYPE_SHADING,
	TYPE_STYLESHEET,
	TYPE_CONFIG,
	TYPE_SHELL,
	TYPE_TEMPLATE,
	TYPE_BUILD,
	TYPE_DSL
} Type;

typedef enum {
	FAM_C,
	FAM_CPP,
	FAM_ML,
	FAM_LISP,
	FAM_PYTHON,
	FAM_ALGOL,
	FAM_PASCAL,
	FAM_BASIC,
	FAM_SQL,
	FAM_ERLANG,
	FAM_JAVA,
	FAM_RUBY
} Family;

typedef struct {
	const char *start;
	const char *end;
} BlockComment;

typedef uint8_t TierMask;
typedef uint8_t ParadigmMask;

typedef struct {
	uint8_t id;
	const char *name, *filename, *hex;
	const char *const *line_comments;
	const BlockComment *block_comments;
	const char *const *extensions;
	TierMask tier;
	ParadigmMask paradigm;
	uint8_t type;
	uint8_t family;
	uint8_t nested;
	uint8_t line_comments_len;
	uint8_t block_comments_len;
	uint8_t extensions_len;
} Language;

typedef enum {
	BLOCK_COMMENTS,
	EXTENSIONS,
	FAMILY,
	FILENAME,
	HEX,
	LINE_COMMENTS,
	NAME,
	NESTED,
	PARADIGMS,
	TIER,
	TYPE,
	LANG,
	UNKNOWN
} KeyType;

typedef enum {
	OP_DOT, // .
	OP_EQ, // =
	OP_COMMA, // ,
	OP_QUOTE, // "
	OP_BRACKET_L, // [
	OP_BRACKET_R, // ]
	OP_BRACKET_BRACKET_L, // [[
	OP_BRACKET_BRACKET_R, // ]]
	OP_UNKNOWN
} Op;

static char *tier_name(Tier tier)
{
	switch (tier) {
		case TIER_SYSTEMS:
			return "systems";
		case TIER_APPLICATION:
			return "application";
		case TIER_SCRIPTING:
			return "scripting";
		case TIER_WEB:
			return "web";
	}
}

static char *paradigm_name(Paradigm paradigm)
{
	switch (paradigm) {
		case PARADIGM_IMPERATIVE:
			return "imperative";
		case PARADIGM_OOP:
			return "oop";
		case PARADIGM_CONCURRENT:
			return "concurrent";
		case PARADIGM_LOGIC:
			return "logic";
		case PARADIGM_FUNCTIONAL:
			return "functional";
		case PARADIGM_DECLARATIVE:
			return "declarative";
	}
}

static char *type_name(Type type)
{
	switch (type) {
		case TYPE_ASSEMBLY:
			return "assembly";
		case TYPE_BUILD:
			return "build";
		case TYPE_CONFIG:
			return "config";
		case TYPE_DSL:
			return "dsl";
		case TYPE_MARKUP:
			return "markup";
		case TYPE_PROGRAMMING:
			return "programming";
		case TYPE_QUERY:
			return "query";
		case TYPE_SHADING:
			return "shading";
		case TYPE_SHELL:
			return "shell";
		case TYPE_STYLESHEET:
			return "stylesheet";
		case TYPE_TEMPLATE:
			return "template";
		case TYPE_TEXT:
			return "text";
	}
}

static char *family_name(Family family)
{
	switch (family) {
		case FAM_C:
			return "C";
		case FAM_CPP:
			return "C++";
		case FAM_ALGOL:
			return "ALGOL";
		case FAM_BASIC:
			return "BASIC";
		case FAM_ERLANG:
			return "Erlang";
		case FAM_JAVA:
			return "Java";
		case FAM_LISP:
			return "Lisp";
		case FAM_ML:
			return "ML";
		case FAM_PASCAL:
			return "Pascal";
		case FAM_PYTHON:
			return "Python";
		case FAM_RUBY:
			return "Ruby";
		case FAM_SQL:
			return "SQL";
		default:
			return "NULL";
	}
}

static bool is_at_end()
{
	return current >= data + len;
}

static char peek()
{
	if (is_at_end()) return '\0';
	return *current;
}

static char peek_next()
{
	if (current + 1 >= data + len) return '\0';
	return *current;
}

static void advance()
{
	current++;
	if (peek() == '\n') {
		line++;
		col = 1;
	} else {
		col++;
	}
}

static bool check_key(int length, const char *key)
{
	return current - start == length && memcmp(start, key, len) == 0;
}

static Op get_operator()
{
	if (peek() == '.') return OP_DOT;
	if (peek() == '=') return OP_EQ;
	if (peek() == ',') return OP_COMMA;
	if (peek() == '"') return OP_QUOTE;
	if (peek() == '[') return OP_BRACKET_BRACKET_L;
	if (peek() == ']') return OP_BRACKET_R;

	return OP_UNKNOWN;
}

static KeyType get_key_type()
{
	if (check_key(3, "hex")) return HEX;
	if (check_key(4, "tier")) return TIER;
	if (check_key(4, "name")) return NAME;
	if (check_key(4, "type")) return TYPE;
	if (check_key(6, "family")) return FAMILY;
	if (check_key(6, "nested")) return NESTED;
	if (check_key(8, "filename")) return FILENAME;
	if (check_key(8, "language")) return LANG;
	if (check_key(9, "paradigms")) return PARADIGMS;
	if (check_key(10, "extensions")) return EXTENSIONS;
	if (check_key(13, "line_comments")) return LINE_COMMENTS;
	if (check_key(14, "block_comments")) return BLOCK_COMMENTS;
	return UNKNOWN;
}

static uint32_t hash(const char *str)
{
	uint32_t h = 0;

	while (*str) {
		h = h * 31 + (unsigned char)*str;
		str++;
	}

	return h;
}

static void parse()
{
	while (!is_at_end()) {
		start = current;
		advance();
	}
}

static int read_data(const char *filename)
{
	FILE *fptr = fopen(filename, "rb");
	if (fptr == NULL) {
		fprintf(stderr, "unable to open file \"%s\".\n", filename);
		return -1;
	}

	if (fseek(fptr, 0L, SEEK_END) != 0) goto fail;
	size_t file_size = ftell(fptr);
	if (file_size < 0) goto fail;
	rewind(fptr);

	char *buffer = malloc(file_size + 1);
	if (buffer == NULL) goto fail;

	size_t bytes_read = fread(buffer, sizeof(char), file_size, fptr);
	if (bytes_read != file_size) goto fail;

	buffer[bytes_read] = '\0';

	fclose(fptr);
	data = buffer;
	len = bytes_read;
	return 0;

fail:
	fprintf(stderr, "unable to read file \"%s\".\n", filename);
	fclose(fptr);
	free(buffer);
	return -1;
}

int main()
{
	const char *filename = "languages.toml";
	if (read_data(filename) != 0) EXIT_FAILURE;

	start = data;
	current = data;
	line = 1;
	col = 1;
	logs = false;

	//parse();
	printf("%zu\n", sizeof(Language));

	return 0;
}
