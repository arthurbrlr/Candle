#pragma		once

#include <Candle.h>

	// ------------------ Need abstraction --------------

static bool useOrthoCamera = true;


static std::pair<double, double> GetMouseScreenCoordinates()
{
	double screenWidth = CDL_APP_WIDTH;
	double screenHeight = CDL_APP_HEIGHT;

	std::pair<double, double> mousePixelPos = Candle::Input::GetMousePos();
	
	return { 2 * mousePixelPos.first / screenWidth - 1., 1. - 2 * mousePixelPos.second / screenHeight };
}

	// ----------------------------------------------


	// Later, the renderer won't be part of the ECS so the OnRenderUpdate() will have to be ditched
	// of the system manager, and ECS::Render() should be deleted

class RenderSystem : public Candle::System {

	public:
		RenderSystem()
		{
			_animptr = new Candle::Animation("default", {});
		}


		~RenderSystem()
		{
			delete _animptr;
		}


		void OnRenderUpdate() override
		{
			std::vector<Candle::Shared<Candle::Blueprint>> bps = Candle::BlueprintManager::Get<Candle::SpriteRenderer>();

			if (bps.size() == 0) return;

			Candle::Renderer2D::GetStats()->Reset();
			Candle::Renderer2D::BeginScene();

				/* Sprite Rendering */
			for (auto& bp : bps) {

				Candle::SpriteRenderer& srComp = bp->GetComponent<Candle::SpriteRenderer>();
				
				if (!srComp.IsActive()) continue;

				if (bp->HasComponent<Candle::AnimationController>()) {
					bool animBool = bp->GetComponent<Candle::AnimationController>().GetCurrentAnimation(*_animptr);

					if (animBool) {
						srComp.SetTextureCoordinates(_animptr->keyframes[_animptr->currentKeyframe].textureCoordinates);
					}
				}

				if (bp->HasComponent<Candle::Transform>()) {
					Candle::Renderer2D::GetStats()->DrawSpritesFromSystem++;
					Candle::Renderer2D::DrawSprite(srComp, bp->GetComponent<Candle::Transform>());
				} else {
					Candle::Renderer2D::GetStats()->DrawSpritesFromSystem++;
					Candle::Renderer2D::DrawSprite(srComp);
				}
					
			}

			Candle::Renderer2D::EndScene();
		}


	private:
		Candle::Animation* _animptr;

};