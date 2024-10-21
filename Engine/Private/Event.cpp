#include "Event.h"
#include "GameObject.h"
//#include "Scene.h"
//#include "Export_Utility.h"


//Dead=======================================================

//
////SceneCHange======================================
//CChangeSceneEvent::CChangeSceneEvent(CScene* _pScene)
//	:m_pScene(_pScene)
//{
//	m_pScene->AddRef();
//	m_eEventID = EVENT_ID::SCENE_CHANGE;
//}
//
//CChangeSceneEvent* CChangeSceneEvent::Create(CScene* _pScene)
//{
//
//	return  new CChangeSceneEvent(_pScene);
//}
//
//void CChangeSceneEvent::Exec()
//{
//	Engine::Set_Scene(m_pScene);
//}
//
//void CChangeSceneEvent::Free()
//{
//	Safe_Release(m_pScene);
//}
//
////Create=========================================================
//CCreateObjEvent::CCreateObjEvent(CLayer* _pLayer, const _tchar* pObjTag, CGameObject* _pNewObj)
//	: m_pLayer(_pLayer),m_pNewObj(_pNewObj), m_pObjTag(pObjTag)
//{
//	m_pLayer->AddRef();
//	m_pNewObj->AddRef();
//	m_eEventID = EVENT_ID::CREATE_OBJ;
//}
//
//CCreateObjEvent* CCreateObjEvent::Create(CLayer* _pLayer, const _tchar* pObjTag, CGameObject* _pNewObj)
//{
//	return new CCreateObjEvent(_pLayer,pObjTag, _pNewObj);
//}
//
//void CCreateObjEvent::Exec()
//{
//
//	m_pLayer->Add_GameObject(m_pObjTag, m_pNewObj);
//
//}
//
//void CCreateObjEvent::Free()
//{
//	Safe_Release(m_pLayer);
//	Safe_Release(m_pNewObj);
//}
