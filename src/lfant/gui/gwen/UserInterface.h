
#include "lfant/UserInterface.h"

//#include <ft2build.h>
//#include FT_FREETYPE_H



namespace Gwen {
namespace Renderer {
class SFML2;
class OpenGL;
class Base;
}
namespace Skin {
class TexturedBase;
}
namespace Controls {
class Canvas;
class Base;
}
namespace Input {
class SFML;
class GLFW;
}
}

namespace lfant {
namespace gui {
namespace gwen {

class UserInterface : public lfant::UserInterface
{
public:
	UserInterface(Game* game);
	~UserInterface();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Deinit();

	virtual void* GetRoot();

	virtual void OnKey(uint16 key, int mode, int mods);
	virtual void OnChar(char key);
	virtual void OnMouseButton(uint16 btn, int mode, int mods);
	virtual void OnMouseMove(ivec2 pos);
	virtual void OnWindowResize(int width, int height);

	void* freetype;

//private:
	ptr<Gwen::Renderer::Base> renderer;
//	ptr<Gwen::Renderer::SFML2> renderer;
	ptr<Gwen::Skin::TexturedBase> skin;
	ptr<Gwen::Controls::Canvas> canvas;
//	ptr<Gwen::Input::SFML> input;
//	ptr<Gwen::Input::GLFW> input;

	ivec2 prevMousePos;

};

}
}
}

