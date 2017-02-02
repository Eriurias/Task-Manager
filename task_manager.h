#ifndef task_manager_h__
#define task_manager_h__
#pragma once

#include <vector>
#include <extdll.h> // rehlds/dlls/extdll.h



#define TASK_FREQUENCY 0.1f

typedef enum
{
	TASK_NORMAL = 0x0,
	TASK_LOOP,
	TASK_REPEAT
} task_types_t;

typedef enum
{
	TASK_DESTROY = 0x0,
	TASK_IGNORED
} task_retvalue_t;

typedef int(*task_handle_t)(const edict_t *, int);

class CTaskManager
{
private:
	
	class CTaskProp
	{
	private:
		float m_flStartTime;
		float m_flCallTime;

		const edict_t *m_pOwner;
		task_handle_t m_pHandle;
		task_types_t m_iType;

		int m_iID;
		int m_iRepeat;
	public:
		
      CTaskProp(const edict_t * owner, task_handle_t handle, float time, task_types_t type = TASK_NORMAL, int tid = 0, int repeat = 0);

		void updateCurrentTime(void);

		bool isExpired();

		bool isFinished()
		{
			return (m_iRepeat < 1);
		}

		int getIndex()
		{
			return m_iID;
		}

		const edict_t * getOwner()
		{
			return m_pOwner;
		}

		task_types_t getType()
		{
			return m_iType;
		}

		int callHandle()
		{
			return m_pHandle(m_pOwner, m_iType == TASK_REPEAT ? (m_iRepeat--) : 0);
		}
	};

	int m_iTaskCount;
	float m_flNextFrame;
   std::vector<CTaskProp *> m_pTaskVector;
public:
   void SetTask(const edict_t *owner, task_handle_t handle, float time, task_types_t type = TASK_NORMAL, int tid = 0, int repeat = 0);
	void RemoveTask(edict_t *owner, int tid = 0);
	void ClearTaskByOwner(edict_t *owner = NULL);
	bool IsExists(edict_t *owner, int tid = 0);
	void StartFrame(float time);
};

extern CTaskManager *g_pTaskManager;
#endif // task_manager_h__