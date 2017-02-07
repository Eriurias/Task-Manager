#include "task_manager.h"

CTaskManager *g_pTaskManager = NULL;

CTaskManager::CTaskProp::CTaskProp(edict_t * owner, task_handle_t handle, float time, task_types_t type, int tid, int repeat)
{
	m_pOwner = owner;
	m_pHandle = handle;
	m_flCallTime = time;
	m_flStartTime = gpGlobals->time;
	m_iType = type;
	m_iID = tid;
	m_iRepeat = repeat;
}

void CTaskManager::SetTask(edict_t *owner, task_handle_t handle, float time, task_types_t type, int tid, int repeat)
{
	//get unique index
	tid += (owner ? ENTINDEX(owner) : 0);

	auto pThisTask = new CTaskProp
	(
		owner,
		handle,
		time,
		type,
		tid,
		repeat
	);

	m_pTaskVector.push_back(pThisTask);
}

void CTaskManager::RemoveTask(edict_t *owner, int tid)
{
	//get unique index
	tid += (owner ? ENTINDEX(owner) : 0);

	auto it = m_pTaskVector.begin();

	while (it != m_pTaskVector.end())
	{
		auto pThisTask = (*it);

		if (pThisTask->getIndex() == tid)
		{
			delete pThisTask;
			m_pTaskVector.erase(it);
			break;
		}

		it++;
	}
}

bool CTaskManager::IsExists(edict_t *owner, int tid)
{
	//get unique index
	tid += (owner ? ENTINDEX(owner) : 0);

	for (auto it : m_pTaskVector)
	{
		if (it->getIndex() == tid)
			return true;
	}

	return false;
}

void CTaskManager::ClearTaskByOwner(edict_t *owner)
{
	auto it = m_pTaskVector.begin();

	while (it != m_pTaskVector.end())
	{
		auto pThisTask = (*it);

		if (pThisTask->getOwner() == owner)
		{
			delete pThisTask;
			m_pTaskVector.erase(it);
			break;
		}

		it++;
	}
}

void CTaskManager::StartFrame(float time)
{
	if (m_pTaskVector.empty() || m_flNextFrame > gpGlobals->time)
	{
		return;
	}

	m_flNextFrame = gpGlobals->time + time;

	auto it = m_pTaskVector.begin();

	while (it != m_pTaskVector.end())
	{
		auto pThisTask = (*it);

		if (!pThisTask->isExpired())
		{
			it++;
			continue;
		}

		switch (pThisTask->getType())
		{
		case TASK_NORMAL:
			pThisTask->callHandle();

			delete pThisTask;
			it = m_pTaskVector.erase(it);

			break;
		case TASK_LOOP:
			if (pThisTask->callHandle() == TASK_DESTROY)
			{
				delete pThisTask;
				it = m_pTaskVector.erase(it);
			}
			else
			{
				pThisTask->updateCurrentTime();
				it++;
			}
			break;
		case TASK_REPEAT:
			if (!pThisTask->isFinished())
			{
				pThisTask->callHandle();
				pThisTask->updateCurrentTime();
				it++;
			}
			else
			{
				delete pThisTask;
				it = m_pTaskVector.erase(it);
			}
			break;
		}
	}
}
