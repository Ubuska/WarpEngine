#include "Geometry.h"
#include "SceneNodes.h"
#include "Events/Events.h"

class CameraNode;
class SkyNode;
class LightNode;
class LightManager;
class IRenderer;

typedef std::map<ActorId, shared_ptr<ISceneNode>> SceneActorMap;

class Scene
{
protected:
	shared_ptr<SceneNode> m_Root;
	shared_ptr<CameraNode> m_Camera;
	shared_ptr<IRenderer> m_Renderer;

	Mat4x4 m_MatrixStack;
	SceneActorMap m_ActorMap;
	LightManager* m_LightManager;

	void RenderAlphaPass();

public:
	Scene(shared_ptr<IRenderer> _Renderer);
	virtual ~Scene();

	bool OnRender();
	bool OnRestore();
	bool OnLostDevice();
	bool OnUpdate(const int _DeltaMilliseconds);

	shared_ptr<ISceneNode> FindActor(ActorId _Id);
	bool AddChild(ActorId _Id, shared_ptr<ISceneNode> _Child);
	bool RemoveChild(ActorId _Id);
	 // Event Delegates
    void NewRenderComponentDelegate(IEventDataPtr _pEventData);
   // void ModifiedRenderComponentDelegate(IEventDataPtr pEventData);			// added post-press!
    void DestroyActorDelegate(IEventDataPtr _pEventData);
    //void MoveActorDelegate(IEventDataPtr pEventData);

	void SetCamera(shared_ptr<CameraNode> _Camera) { m_Camera = _Camera; }
	const shared_ptr<CameraNode> GetCamera() const { return m_Camera; }

	LightManager *GetLightManager() { return m_LightManager; }

	void PushAndSetMatrix(const Mat4x4 &_ToWorld);
	void PopMatrix();
	void PushMatrix();
	const Mat4x4 GetTopMatrix();

};