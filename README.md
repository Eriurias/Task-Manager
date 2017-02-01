# Task Manager

## What is this?
* This is ready solution for call functions at specified times

## How can use it?
* The first thing we have to do - is allocate storage space (memory)  for task object and then deallocate storage
```c++
void OnMetaAttach()//or amxx attach
{
	g_pTaskManager = new CTaskManager;
}

void OnMetaDetach()//or amxx detach
{
	delete g_pTaskManager;
}
```
* After turning on the server clear all tasks
```c++
void ServerActivate_Post(edict_t *pEdictList, int edictCount, int clientMax)
{
	if (g_pTaskManager != NULL)
		g_pTaskManager->ClearTaskByOwner();
    
	SET_META_RESULT(MRES_IGNORED);
}
```
* Then use functions on necessity.
### Example
```c++
int ShowPlayerGreeting_TaskHandle(edict_t *pPlayer);

void ClientPutInServer_Post(edict_t *pPlayer)
{
	if (g_pTaskManager != NULL)
		g_pTaskManager->ClearTaskByOwner(pPlayer);
    
	g_pTaskManager->SetTask(pPlayer, (task_handle_t)ShowPlayerGreeting_TaskHandle, 5.0, TASK_NORMAL);

	SET_META_RESULT(MRES_IGNORED);
}

int ShowPlayerGreeting_TaskHandle(edict_t *pPlayer)
{
	UTIL_ClientPrint(pPlayer, print_center, "Hello, %s!", pPlayer->v.netname);
  
	return TASK_IGNORED;
}
```
