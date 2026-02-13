#pragma once
#include <zyra/util/def.h>
#if not defined(ZYRA_PUBLIC)
#include <string>
#include <utility>
#include <format>
#include "str_util.h"
#endif // ZYRA_PUBLIC


zyra_begin_

namespace log {
#if not defined(ZYRA_PUBLIC)
	extern void print_impl(const std::string& text);

	template <typename... Args>
	void println(std::format_string<Args...> fmt, Args&&... args) {
		print_impl(std::format(fmt, std::forward<Args>(args)...) + "\n");
	}
	template <typename... Args>
	void print_debug(std::format_string<Args...> fmt, Args&&... args) {
		print_impl("[Debug]: " + std::format(fmt, std::forward<Args>(args)...) + "\n");
	}
	template <typename... Args>
	void print_warning(std::format_string<Args...> fmt, Args&&... args) {
		print_impl("[Warning]: " + std::format(fmt, std::forward<Args>(args)...) + "\n");
	}
	template <typename... Args>
	void print_error(std::format_string<Args...> fmt, Args&&... args) {
		print_impl("[Error]: " + std::format(fmt, std::forward<Args>(args)...) + "\n");
	}

#else // ZYRA_PUBLIC
	template <typename... Args>
	void print_ln(Args&&... args) {}
	template <typename... Args>
	void print_debug(Args&&... args) {}
	template <typename... Args>
	void print_warning(Args&&... args) {}
	template <typename... Args>
	void print_error(Args&&... args) {}
#endif // ZYRA_PUBLIC
}

zyra_end_