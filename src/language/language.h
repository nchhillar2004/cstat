/*
 language.h
 * */

#ifndef cstat_language_h
#define cstat_language_h

#include <stdbool.h>
#include <stddef.h>

#define MAX_COMMENT_STYLES 2

typedef enum {
	ADA,
	ALGOL,
	ARDUINO,
	ASCII_DOC,
	ASP,
	ASP_NET,
	ASSEMBLY,
	ASSEMBLY_GAS,
	ASTRO,
	AUTOCONF,
	BASH,
	BATCH,
	C,
	C_HEADER,
	CLOJURE,
	CMAKE,
	COBOL,
	CPP,
	CPP_HEADER,
	CPP_MODULE,
	CRYSTAL,
	C_SHARP,
	CSS,
	CUDA,
	CYTHON,
	D,
	DART,
	DEVICE_TREE,
	DOCKERFILE,
	DOT_NET_RES,
	ELISP,
	ELIXIR,
	ELM,
	ERLANG,
	FORTRAN_LEGACY,
	FORTRAN_MODERN,
	F_SHARP,
	GDB,
	GD_SCRIPT,
	GD_SHADER,
	GLEAM,
	GLSL,
	GO,
	GO_HTML,
	GRAPHQL,
	GROOVY,
	HANDLEBARS,
	HASKELL,
	HLSL,
	HOLYC,
	HTML,
	INI,
	JAI,
	JAVA,
	JAVASCRIPT,
	JSON,
	JSP,
	JSX,
	JULIA,
	JUPYTER,
	KOTLIN,
	LEX,
	LINKER_SCRIPT,
	LISP,
	LLVM,
	LUA,
	MAKEFILE,
	MARKDOWN,
	MDX,
	METAL,
	MOJO,
	MS_BUILD,
	MUSTACHE,
	NIM,
	NIX,
	OBJECTIVE_C,
	OBJECTIVE_CPP,
	OCAML,
	OCAML_INTERFACE,
	ODIN,
	OPENCL,
	PASCAL,
	PERL,
	PHP,
	PROLOG,
	PUG,
	PYTHON,
	R,
	RAKEFILE,
	RUBY,
	RUBY_HTML,
	RUST,
	SASS,
	SCALA,
	SCHEME,
	SHELL,
	SML,
	SOLIDITY,
	SQL,
	SVELTE,
	SVG,
	SWIFT,
	SYSTEM_VERILOG,
	TCL,
	TEMPL,
	TEX,
	TEXT,
	TOML,
	TSX,
	TYPESCRIPT,
	TYPST,
	UNREAL_SCRIPT,
	UNREAL_SHADER,
	VB6,
	VB_SCRIPT,
	VERILOG,
	VISUAL_BASIC,
	VIM_SCRIPT,
	WASM,
	WGSL,
	WOLFRAM,
	XML,
	YAML,
	ZIG,
	ZSH,
	LANG_COUNT
} LanguageId;

typedef struct {
	const char *start;
	const char *end;
} BlockComment;

typedef struct {
	const char *line[MAX_COMMENT_STYLES];
	BlockComment block[MAX_COMMENT_STYLES];
	bool nested;
} CommentInfo;

typedef struct {
	const char *name;
	const char *hex;
	CommentInfo comment;
} LanguageInfo;

extern const LanguageInfo LanguageTable[LANG_COUNT];

extern void checkLanguageTable();

#endif
