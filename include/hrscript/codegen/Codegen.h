/*
 * Copyright (C) 2015  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef hrscript_codegen_h
#define hrscript_codegen_h
#include <hrscript/codegen/Backend.h>

namespace hrscript {
class Codegen {
public:
	Codegen(Backend& backend);
	virtual ~Codegen() = default;
};
} // namespace hrscript
#endif//hrscript_codegen_h
