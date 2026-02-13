#pragma once


#define zyra_remove_copy_constructor(_class) \
_class(const _class&) = delete; \
_class& operator=(const _class&) = delete

#define zyra_remove_move_constructor(_class) \
_class(_class&&) = delete; \
_class& operator=(_class&&) = delete

#define zyra_remove_default_constructor(_class) \
_class() = delete

#define zyra_add_default_constructor(_class) \
_class() = default