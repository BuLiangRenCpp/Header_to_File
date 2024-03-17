#pragma once
#include "Ins.h"
#include "htf.h"


// 解析指令，执行对应操作
class Resolve {
public:
	Resolve();

	void resolve(const Ins&);
	bool is_run() const;

private:
	bool _is_run = true;		// 记录程序是否运行

	// 单指令
	void _run_end_ins();
    // 输出默认目录 (cwd)
	void _list_default_ins();

	// 多指令
	void _htf_ins(const std::vector<Ins_arg>&);
};

