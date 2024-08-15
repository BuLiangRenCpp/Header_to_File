#include "ExcepPath.h"
#include "output.h"

namespace htf
{
namespace path
{

ExcepPath::ExcepPath(const std::string& what) : ExcepBase{ "path-error: " + what }
{}

ExcepPath::ExcepPath(const ErrorCode& error_num, const FS::path& path)
{
	_msg = "path-error: ";
    switch (error_num) {
	case ErrorCode::not_exist_file:
		_msg += "No such file";
		break;
	case ErrorCode::not_exist_directory:
		_msg += "No such directory";
		break;
	case ErrorCode::not_exist:
		_msg += "No such path";
		break;
	case ErrorCode::not_create_file:
		_msg += "Not create file";
		break;;
	case ErrorCode::not_create_directory:
		_msg += "Not create directory";
		break;
	case ErrorCode::not_create:
		_msg += "Not creat path";
		break;
	case ErrorCode::not_remove_file:
		_msg += "Not remove file";
		break;
	case ErrorCode::not_remove_directory:
		_msg += "Not remove directory";
		break;
	case ErrorCode::not_remove: 
		_msg += "Not remove path";
		break;
	default:
		break;
    }
    _msg += ":" + mark_path(path);
}

}   // namespace path
}   // namespace htf
