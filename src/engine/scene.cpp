// COMP710 GP Framework 2022

#include "scene.h"

#include "renderer.h"
#include "entity.h"
#include "scenemanager.h"

#include <cassert>
#ifdef _DEBUG
#include <imgui/imgui_impl_sdl.h>
#endif _DEBUG

Scene::Scene() 
	: m_isLoaded(false)
#ifdef _DEBUG
	, dm_iDisplayEntityIndex(0)
#endif // _DEBUG
{

}

Scene::~Scene()
{

}

bool Scene::Initialise(Renderer& renderer, SceneManager& sceneManager, FMOD::System& fmodSystem)
{
	m_isLoaded = true;
	m_pSceneManager = &sceneManager;
	m_pRenderer = &renderer;
	m_pFMODsystem = &fmodSystem;

	return true;
}


#ifdef _DEBUG
void
Scene::BeginDebugDraw()
{
	ImGui::Begin(dm_name);
	if (dm_pEntityDebugQueue.size() > 0)
	{
		ImGui::SliderInt("Select Entity", &dm_iDisplayEntityIndex, 0, dm_pEntityDebugQueue.size() - 1);
		ImGui::Text("Selected Entity: %s", dm_pEntityDebugQueue[dm_iDisplayEntityIndex]->GetIdAsString().c_str());
		ImGui::BeginChild(dm_pEntityDebugQueue[dm_iDisplayEntityIndex]->GetIdAsString().c_str(), ImVec2(ImGui::GetWindowContentRegionWidth(), 400), true, ImGuiWindowFlags_HorizontalScrollbar);
		dm_pEntityDebugQueue[dm_iDisplayEntityIndex]->DebugDraw();
		ImGui::EndChild();
	}
}

void
Scene::AddEntityToDebugQueue(Entity* entity)
{
	dm_pEntityDebugQueue.push_back(entity);
}

void
Scene::RemoveEntityFromDebugQueue(Entity* rmEntity)
{
	for (EntityVectorIterator entity = dm_pEntityDebugQueue.begin(); entity != dm_pEntityDebugQueue.end(); ++entity)
	{
		if (rmEntity->GetId() == (*entity)->GetId())
		{
			delete *entity;
			entity = dm_pEntityDebugQueue.erase(entity);
			return;
		}
	}
}
#endif // _DEBUG