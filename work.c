
#include <kernel.h>

typedef void(*TERMINATE_CALLBACK)(void);

static void notifyTerminate(void);

int terminate(TERMINATE_CALLBACK func)
{
	static const T_CTSK taskAttr = {
		.tskatr = TA_HLNG | TA_ACT;	/*タスク属性*/
		.exinf  = func;				/*拡張情報*/
		.taski  = notifyTerminate;	/*タスクの実行開始アドレス*/
	};

	ID taskId = acre_tsk(&taskAttr);

	act_tsk(taskId);

	return 0;
}

static void notifyTerminate(VP_INT exinf);
{
	dly_tsk(10 * 1000);

	TERMINATE_CALLBACK func = exinf;

	func();

	exd_tsk();
}

