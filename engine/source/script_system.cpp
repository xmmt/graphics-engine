#include <engine/script_system.hpp>

using namespace engine;
using namespace script_system;

lua_script::lua_script(std::string const& filename) {
    lua_state_ = luaL_newstate();
    if (luaL_loadfile(lua_state_, filename.c_str()) or lua_pcall(lua_state_, 0, 0, 0)) {
        std::cout << "Error: script not loaded (" << filename << ")" << std::endl;
        lua_state_ = 0;
    }
}

lua_script::~lua_script() {
    if (lua_state_) {
        lua_close(lua_state_);
    }
}

void lua_script::print_error(std::string const& variable_name, std::string const& reason) {
    std::cout << "Error, can't get [" << variable_name << "]. " << reason << std::endl;
}

template <>
inline std::string lua_script::lua_get_default<std::string>() {
    return "null";
}

bool lua_script::lua_get_to_stack(std::string const& variable_name) {
    level_ = 0;
    std::string var;
    for (auto i = 0; i < variable_name.size(); ++i) {
        if (variable_name.at(i) == '.') {
            if (level_ == 0) {
                lua_getglobal(lua_state_, var.c_str());
            } else {
                lua_getfield(lua_state_, -1, var.c_str());
            }

            if (lua_isnil(lua_state_, -1)) {
                print_error(variable_name, var + " is not defined");
                return false;
            } else {
                var = "";
                level_ += 1;
            }
        } else {
            var += variable_name.at(i);
        }
    }

    if (level_ == 0) {
        lua_getglobal(lua_state_, var.c_str());
    } else {
        lua_getfield(lua_state_, -1, var.c_str());
    }

    if (lua_isnil(lua_state_, -1)) {
        print_error(variable_name, var + " is not defined");
        return false;
    }

    return true;
}

template<>
inline bool lua_script::lua_get<bool>(std::string const& variable_name) {
    return static_cast<bool>(lua_toboolean(lua_state_, -1));
}

template <>
inline float lua_script::lua_get<float>(std::string const& variable_name) {
    if (not lua_isnumber(lua_state_, -1)) {
        print_error(variable_name, "Not a number");
    }
    return static_cast<float>(lua_tonumber(lua_state_, -1));
}

template <>
inline int32_t lua_script::lua_get<int32_t>(std::string const& variable_name) {
    if (not lua_isnumber(lua_state_, -1)) {
        print_error(variable_name, "Not a number");
    }
    return static_cast<int32_t>(lua_tonumber(lua_state_, -1));
}

template <>
inline std::string lua_script::lua_get<std::string>(std::string const& variable_name) {
    std::string s{ "null" };
    if (lua_isstring(lua_state_, -1)) {
        s = std::string(lua_tostring(lua_state_, -1));
    } else {
        print_error(variable_name, "Not a string");
    }
    return s;
}