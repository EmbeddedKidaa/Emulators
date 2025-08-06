#include <cstdint>
#include <SDL2/SDL.h>

// SDL is used to render and receive input in a multi-Graphics way
class Graphics {
    public:
        // Constructor
        Graphics(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);

        // Destructor
        ~Graphics();

        // Functions
        void Update(void const* buffer, int pitch);
        bool ProcessInput(uint8_t* keys);

    private:
        SDL_Window* window{};
        SDL_Renderer* renderer{};
	    SDL_Texture* texture{};
};