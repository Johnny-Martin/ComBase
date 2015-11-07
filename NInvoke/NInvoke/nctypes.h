/*
 *    nctypes.h:
 *    
 *    Copyright (C) 2014 By limhiaoing <blog.poxiao.me> All Rights Reserved.
 *
 */

#ifndef NINVOKE_CTYPES_H_
#define NINVOKE_CTYPES_H_
#include <cstddef>
#include <string>
#include <sstream>

#include <lua.hpp>

enum ctypes {
    type_void = 0,
    type_pointer,
    type_char,
    type_signed_char,
    type_wchar_t,
    type_unsigned_char,
    type_short,
    type_unsigned_short,
    type_int,
    type_unsigned_int,
    type_long,
    type_unsigned_long,
    type_long_long,
    type_unsigned_long_long,
    type_end
};

ctypes type_name_to_ctype(const char* type_name);

template<class T, class U>
struct is_same {
    static const bool value = false;
};

template<class T>
struct is_same<T, T> {
    static const bool value = true;
};

template<typename NativeType>
class scalar_type {
    typedef scalar_type<NativeType> this_type;
public:
    static const char* name;
    typedef NativeType native_type;
private:
    native_type _value;
public:
    // for lua
    static int create(lua_State* L) {
        native_type value = static_cast<native_type>(luaL_checknumber(L, 1));
        this_type::push(L, value);
        return 1;
    }

    static int type_size(lua_State* L) {
        lua_pushnumber(L, sizeof(native_type));
        return 1;
    }

    static int type_name(lua_State* L) {
        lua_pushstring(L, name);
        return 1;
    }

    static int max_value(lua_State* L) {
#ifdef max
#undef max
#endif
        this_type::push(L, std::numeric_limits<native_type>::max());
        return 1;
    }

    static int min_value(lua_State* L) {
#ifdef min
#undef min
#endif
        this_type::push(L, std::numeric_limits<native_type>::min());
        return 1;
    }

    static int eq_operation(lua_State* L) {
        lua_pushboolean(L, reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name))->_value == reinterpret_cast<this_type*>(luaL_checkudata(L, 2, name))->_value ? 1 : 0);
        return 1;
    }

    static int lt_operation(lua_State* L) {
        lua_pushboolean(L, reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name))->_value < reinterpret_cast<this_type*>(luaL_checkudata(L, 2, name))->_value ? 1 : 0);
        return 1;
    }

    static int le_operation(lua_State* L) {
        lua_pushboolean(L, reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name))->_value <= reinterpret_cast<this_type*>(luaL_checkudata(L, 2, name))->_value ? 1 : 0);
        return 1;
    }

    static int to_number(lua_State* L) {
        this_type* self = reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name));
        lua_pushnumber(L, static_cast<lua_Number>(self->_value));
        return 1;
    }

    static int to_string(lua_State* L) {
        this_type* self = reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name));
        std::string str;
        try {
            std::stringstream ss;
            if(is_same<native_type, wchar_t>::value) {
                ss << static_cast<unsigned short>(self->_value);
            }
            else {
                ss << self->_value;
            }
            ss >> str;
        }
        catch(std::bad_alloc&) {
            lua_pushliteral(L, "out of memory");
            lua_error(L);
        }
        lua_pushlstring(L, str.data(), str.size());
        return 1;
    }

    static int get_pointer(lua_State* L) {
        this_type* self = reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name));
        ctype_pointer::push(L, &self->_value);
        return 1;
    }
public:
    // for c
    static void push(lua_State* L, native_type value) {
        this_type* self = reinterpret_cast<this_type*>(lua_newuserdata(L, sizeof(this_type)));
        self->_value = value;
        luaL_getmetatable(L, name);
        lua_setmetatable(L, -2);
    }

    static bool registe(lua_State* L, const char* short_name) {
        int top = lua_gettop(L);
        lua_newtable(L);
        lua_setfield(L, -2, short_name);
        lua_getfield(L, -1, short_name);
        lua_pushcfunction(L, &this_type::create);
        lua_setfield(L, -2, "new");
        lua_pushcfunction(L, &this_type::type_size);
        lua_setfield(L, -2, "type_size");
        lua_pushcfunction(L, &this_type::max_value);
        lua_setfield(L, -2, "max_value");
        lua_pushcfunction(L, &this_type::min_value);
        lua_setfield(L, -2, "min_value");
        luaL_newmetatable(L, name);
        lua_pushvalue(L, -1);
        lua_pushcfunction(L, &this_type::to_number);
        lua_setfield(L, -2, "to_number");
        lua_pushcfunction(L, &this_type::to_string);
        lua_setfield(L, -2, "__tostring");
        lua_pushcfunction(L, &this_type::get_pointer);
        lua_setfield(L, -2, "get_pointer");
        lua_pushcfunction(L, &this_type::type_name);
        lua_setfield(L, -2, "type_name");

        lua_setfield(L, -2, "__index");
        lua_pushcfunction(L, &this_type::eq_operation);
        lua_setfield(L, -2, "__eq");
        lua_pushcfunction(L, &this_type::lt_operation);
        lua_setfield(L, -2, "__lt");
        lua_pushcfunction(L, &this_type::le_operation);
        lua_setfield(L, -2, "__le");
        lua_setmetatable(L, -2);
        lua_settop(L, top);
        return true;
    }

    static std::size_t native_type_size() {
        return sizeof(native_type);
    }

    native_type get_value() const {
        return this->_value;
    }
};

// c build-in typedef
typedef scalar_type<char> ctype_char;
typedef scalar_type<signed char> ctype_signed_char;
typedef scalar_type<unsigned char> ctype_unsigned_char;
typedef scalar_type<wchar_t> ctype_wchar_t;
typedef scalar_type<short> ctype_short;
typedef scalar_type<unsigned short> ctype_unsigned_short;
typedef scalar_type<int> ctype_int;
typedef scalar_type<unsigned int> ctype_unsigned_int;
typedef scalar_type<long> ctype_long;
typedef scalar_type<unsigned long> ctype_unsigned_long;
typedef scalar_type<long long> ctype_long_long;
typedef scalar_type<unsigned long long> ctype_unsigned_long_long;

// pointer
class ctype_pointer {
    typedef ctype_pointer this_type;
public:
    typedef void* native_type;
    static const char* name;
private:
    native_type _value;
public:
    // for lua
    static int create(lua_State* L);
    static int type_size(lua_State* L);
    static int type_name(lua_State* L);
    static int to_number(lua_State* L);
    static int to_string(lua_State* L);
    static int get_pointer(lua_State* L);

    static int fetch_increase(lua_State* L);
    static int fetch_decrease(lua_State* L);

    static int eq_operation(lua_State* L);
    static int lt_operation(lua_State* L);
    static int le_operation(lua_State* L);

    template<typename Type>
    static int deref_as(lua_State* L) {
        this_type* self = reinterpret_cast<this_type*>(luaL_checkudata(L, 1, name));
        Type::push(L, *reinterpret_cast<typename Type::native_type*>(self->_value));
        return 1;
    }
public:
    // for c
    static void push(lua_State* L, native_type value);
    static std::size_t native_type_size();
    static bool registe(lua_State* L);

    native_type get_value() const;
};

template <ctypes Type>
struct type_enum_to_ctype {
    typedef void ctype;
};

template<>
struct type_enum_to_ctype<type_pointer> {
    typedef ctype_pointer ctype;
};

template<>
struct type_enum_to_ctype<type_char> {
    typedef ctype_char ctype;
};

template<>
struct type_enum_to_ctype<type_signed_char> {
    typedef ctype_signed_char ctype;
};

template<>
struct type_enum_to_ctype<type_unsigned_char> {
    typedef ctype_unsigned_char ctype;
};

template<>
struct type_enum_to_ctype<type_wchar_t> {
    typedef ctype_wchar_t ctype;
};

template<>
struct type_enum_to_ctype<type_short> {
    typedef ctype_short ctype;
};

template<>
struct type_enum_to_ctype<type_unsigned_short> {
    typedef ctype_unsigned_short ctype;
};

template<>
struct type_enum_to_ctype<type_int> {
    typedef ctype_int ctype;
};

template<>
struct type_enum_to_ctype<type_unsigned_int> {
    typedef ctype_unsigned_int ctype;
};

template<>
struct type_enum_to_ctype<type_long> {
    typedef ctype_long ctype;
};

template<>
struct type_enum_to_ctype<type_unsigned_long> {
    typedef ctype_unsigned_long ctype;
};

template<>
struct type_enum_to_ctype<type_long_long> {
    typedef ctype_long_long ctype;
};

template<>
struct type_enum_to_ctype<type_unsigned_long_long> {
    typedef ctype_unsigned_long_long ctype;
};

#ifdef  _WIN64
    typedef ctype_unsigned_long_long ctype_size_t;
#else
    typedef ctype_unsigned_int ctype_size_t;
#endif

#endif
