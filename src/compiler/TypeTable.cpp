#include "TypeTable.h"

using namespace std;

const std::set<std::string> CppTypes{"void",
                                     "bool",
                                     "char",
                                     "wchar_t",
                                     "char8_t",
                                     "char16_t",
                                     "char32_t",
                                     "float",
                                     "double",
                                     "nullptr_t",
                                     "short",
                                     "int",
                                     "long",
                                     "signed",
                                     "unsigned",
                                     "size_t",
                                     "array",
                                     "atomic",
                                     "bitset",
                                     "ptrdiff_t",
                                     "day",
                                     "duration",
                                     "hh_mm_ss",
                                     "month",
                                     "month_day",
                                     "month_day_last",
                                     "month_weekday",
                                     "set",
                                     "month_weekday_last",
                                     "time_point",
                                     "weekday",
                                     "weekday_last",
                                     "weekday_indexed",
                                     "year",
                                     "year_month",
                                     "year_month_day",
                                     "year_month_weekday",
                                     "year_month_weekday_last",
                                     "file_clock",
                                     "gps_clock",
                                     "high_resolution_clock",
                                     "local_t",
                                     "common_type",
                                     "steady_clock",
                                     "system_clock",
                                     "tai_clock",
                                     "utc_clock",
                                     "time_zone",
                                     "time_zone_link",
                                     "codecvt_utf8",
                                     "codecvt_utf8_utf16",
                                     "codecvt_utf16",
                                     "complex",
                                     "condition_variable",
                                     "condition_variable_any",
                                     "deque",
                                     "exception",
                                     "bad_exception",
                                     "directory_entry",
                                     "directory_iterator",
                                     "file_status",
                                     "filesystem_error",
                                     "path",
                                     "recursive_directory_iterator",
                                     "space_info",
                                     "forward_list",
                                     "filebuf",
                                     "wfilebuf",
                                     "basic_filebuf",
                                     "fstream",
                                     "basic_fstream",
                                     "ifstream",
                                     "wifstream",
                                     "basic_ifstream",
                                     "ofstream",
                                     "wofstream",
                                     "basic_ofstream",
                                     "iostream",
                                     "wiostream",
                                     "basic_iostream",
                                     "istream",
                                     "wistream",
                                     "basic_istream",
                                     "ostream",
                                     "wostream",
                                     "basic_ostream",
                                     "function",
                                     "future",
                                     "packaged_task",
                                     "promise",
                                     "shared_future",
                                     "hash_map",
                                     "hash_multimap",
                                     "hash_set",
                                     "hash_multiset",
                                     "initializer_list",
                                     "iterator",
                                     "reverse_iterator",
                                     "list",
                                     "codecvt",
                                     "codecvt_base",
                                     "collate",
                                     "ctype",
                                     "locale",
                                     "messages",
                                     "map",
                                     "multimap",
                                     "allocator",
                                     "auto_ptr",
                                     "bad_weak_ptr",
                                     "shared_ptr",
                                     "unique_ptr",
                                     "weak_ptr",
                                     "memory_resource",
                                     "mutex",
                                     "shared_mutex",
                                     "shared_timed_mutex",
                                     "shared_lock",
                                     "adpot_lock_t",
                                     "defer_lock_t",
                                     "lock_guard",
                                     "unique_lock",
                                     "timed_mutex",
                                     "bad_alloc",
                                     "nothrow_t",
                                     "queue",
                                     "priority_queue",
                                     "span",
                                     "istringstream",
                                     "ostringstream",
                                     "stringbuf",
                                     "stringstream",
                                     "wistringstream",
                                     "wostringstream",
                                     "wstringbuf",
                                     "wstringstream",
                                     "basic_stringbuf",
                                     "basic_istringstream",
                                     "basic_ostringstream",
                                     "basic_stringstream",
                                     "stack",
                                     "string",
                                     "basic_string",
                                     "basic_string_view",
                                     "istrstream",
                                     "ostrstream",
                                     "strstream",
                                     "thread",
                                     "hash",
                                     "tuple",
                                     "tuple_element",
                                     "tuple_size",
                                     "type_index",
                                     "unordered_map",
                                     "unordered_multimap",
                                     "unordered_set",
                                     "unordered_multiset",
                                     "pair",
                                     "identity",
                                     "valarray",
                                     "vector"};

namespace htf
{

TypeTable::TypeTable() : _types{}
{}

bool TypeTable::exist(const std::string& type) const
{
    return _types.count(type) || CppTypes.count(type);
}

void TypeTable::add(const std::string& type)
{
    if (!CppTypes.count(type)) _types.insert(type);
}

}   // namespace htf
