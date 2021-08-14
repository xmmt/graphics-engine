#pragma once

#include <vector>
#include <string>
#include <iostream>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace engine {
namespace script_system {

class lua_script {
public:
    lua_script(std::string const& filename);
    ~lua_script();

public:
    void print_error(std::string const& variable_name, std::string const& reason);

    template <typename T>
    T get(std::string& const variable_name) {
        if (not lua_state_) {
            print_error(variable_name, "Script is not loaded");
            return lua_get_default<T>();
        }

        T result;
        if (lua_get_to_stack(variable_name)) {
            result = lua_get<T>(variable_name);
        } else {
            result = lua_get_default<T>();
        }

        lua_pop(lua_state_, level + 1);
        return result;
    }

    bool lua_get_to_stack(std::string const& variable_name);

    template <typename T>
    T lua_get(std::string const& variable_name) {
        return 0;
    }

    template <typename T>
    T lua_get_default() {
        return 0;
    }

private:
    lua_State* lua_state_;
    int32_t level_;
};

} // namespace script_system
} // namespace engine