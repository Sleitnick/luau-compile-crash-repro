#include <lua.h>
#include <lualib.h>
#include <luacode.h>
#include <Luau/CodeGen.h>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <memory>
#include <string_view>

static const char* kUserdataTypes[] = {"Vector2", nullptr};

// Example 1: A function takes 'v' and prints it. No type annotations. (This one works)
const char* EXAMPLE_1 = "function test(v: Vector2) print(v) end test(Vector2.new(0, 0))";

// Example 2: Same as above, but with no type annotation for 'v'. (This crashes)
const char* EXAMPLE_2 = "function test(v) print(v) end test(Vector2.new(0, 0))";

int compile_test(lua_State* L, const char* name, const char* source)
{
	printf("compiling %s...\n", name);

	lua_CompileOptions opts{};
	opts.userdataTypes = kUserdataTypes; // This causes a crash with EXAMPLE_2
	opts.typeInfoLevel = 1;
	opts.debugLevel = 2;
	opts.coverageLevel = 0;
	opts.optimizationLevel = 2;

	size_t data_size;
	char* data = luau_compile(source, strlen(source), &opts, &data_size);

	// Crash occurs on `luau_load`:
	int load_res = luau_load(L, name, data, data_size, 0);

	if (load_res != 0)
	{
		size_t len;
		const char* msg = lua_tolstring(L, -1, &len);
		lua_pop(L, 1);
		printf("%s\n", msg);
		free(data);
		return 1;
	}
	printf("%s success\n", name);
	free(data);
	lua_pop(L, 1); // discard the loaded code
	return 0;
}

int main(int argc, char* argv[])
{
	std::unique_ptr<lua_State, void (*)(lua_State*)> state(luaL_newstate(), lua_close);
	lua_State* L = state.get();

	if (Luau::CodeGen::isSupported())
	{
		Luau::CodeGen::create(L);
		Luau::CodeGen::setUserdataRemapper(L, kUserdataTypes, [](void *ctx, const char *str, size_t len) -> uint8_t
			{
				// Taken from the conformance code test. The crash occurs before this function
				// is called, so the contents here don't matter. But a function must be set.
				const char** types = (const char**)ctx;
				uint8_t index = 0;
				std::string_view sv{str, len};
				for (; *types; ++types)
				{
					if (sv == *types)
					{
						return index;
					}
					index++;
				}
				return 0xff;
			}
		);
	}

	luaL_openlibs(L);
	luaL_sandbox(L);

	int e1 = compile_test(L, "=Example1", EXAMPLE_1);
	int e2 = compile_test(L, "=Example2", EXAMPLE_2);

	printf("success\n");
	return 0;
}
