#include <SDL2/SDL.h>
#include<SDL2/SDL_ttf.h> 
#include<chrono>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15;
const int BALL_SPEED = 1.0f; 
const int PADDLE_WIDTH = 15;
const int PADDLE_HEIGHT = 100;
const float PADDLE_SPEED = 1.0f;
const int GOAL_SPACE=100; 

enum Buttons
{ 
	PadOneUp = 0,
	PadOneDown, 
	PadTwoUp,
	PadTwoDown,
};
enum class CollisionType
{
	None,
	Top,
	Middle,
	Bottom,
	Left,
	Right,
};
struct Contact
{
	CollisionType type;
	float penetration;
};

class Vec2
{
	public:
		Vec2() : x(0.0f), y(0.0f) {}

		Vec2(float x, float y) : x(x), y(y) {}

		Vec2 operator+(Vec2 const& rhs) 
		{
			return Vec2(x + rhs.x, y + rhs.y);
		}

		Vec2& operator+=(Vec2 const& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vec2 operator*(float rhs)
		{
			return Vec2(x * rhs, y * rhs);
		}

		float x, y;
};
class Ball
{ 
	public: 
		Ball(Vec2 position, Vec2 velocity) : position(position), velocity(velocity)
		{ 
			rect.x = static_cast<int>(position.x); 
			rect.y = static_cast<int>(position.y); 
			rect.w = BALL_WIDTH; 
			rect.h = BALL_HEIGHT; 
		}
		void Draw(SDL_Renderer* renderer){ 
			rect.x = static_cast<int>(position.x);
			rect.y = static_cast<int>(position.y);
			SDL_RenderFillRect(renderer, &rect);
		}
		void Update(float dt)
		{ 
			position += velocity*dt; 
		}
		void CollideWithPaddle(Contact const& contact)
		{
			position.x += contact.penetration;
			velocity.x = -velocity.x;

			if (contact.type == CollisionType::Top)
			{
				velocity.y = -.75f * BALL_SPEED;
			}
			else if (contact.type == CollisionType::Bottom)
			{
				velocity.y = 0.75f * BALL_SPEED;
			}
		}
		void CollideWithWall(Contact const& contact)
			{
				if ((contact.type == CollisionType::Top)
					|| (contact.type == CollisionType::Bottom))
				{
					position.y += contact.penetration;
					velocity.y = -velocity.y;
				}
				else if (contact.type == CollisionType::Left)
				{
					position.x = WINDOW_WIDTH / 2.0f;
					position.y = WINDOW_HEIGHT / 2.0f;
					velocity.x = BALL_SPEED;
					velocity.y = 0.75f * BALL_SPEED;
				}
				else if (contact.type == CollisionType::Right)
				{
					position.x = WINDOW_WIDTH / 2.0f;
					position.y = WINDOW_HEIGHT / 2.0f;
					velocity.x = -BALL_SPEED;
					velocity.y = 0.75f * BALL_SPEED;
				}
			}
		
		Vec2 position; 
		Vec2 velocity; 
		SDL_Rect rect = {}; 
};
class Paddle
{ 
	public: 
		Paddle(Vec2 position, Vec2 velocity) : position(position), velocity(velocity)
		{ 
			rect.x = static_cast<int>(position.x); 
			rect.y = static_cast<int>(position.y); 
			rect.w = PADDLE_WIDTH; 
			rect.h = PADDLE_HEIGHT; 
		}

		void Draw(SDL_Renderer* renderer)
		{ 
			rect.y = static_cast<int>(position.y);
			SDL_RenderFillRect(renderer, &rect);
		}

		void update(float dt)
		{ 
			position += velocity * dt;
			if (position.y < 0) position.y = 0; 
			else if (position.y > (WINDOW_HEIGHT - PADDLE_HEIGHT)) position.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
		}
		Vec2 position; 
		Vec2 velocity; 
		SDL_Rect rect = {}; 
};
class PlayerScore
{ 
	public:
		PlayerScore(Vec2 position, SDL_Renderer* renderer, TTF_Font* font)
			: renderer(renderer), font(font) 
		{
			surface = TTF_RenderText_Solid(font, "0", {0xFF, 0xFF, 0xFF, 0xFF});
			texture = SDL_CreateTextureFromSurface(renderer, surface); 

			int w, h; 
			SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
			rect.x = static_cast<int>(position.x); 
			rect.y = static_cast<int>(position.y); 
			rect.w = w; 
			rect.h = h; 
		}

		~PlayerScore()
		{ 
			SDL_FreeSurface(surface); 
			SDL_DestroyTexture(texture); 
		}

		void Draw()
		{ 
			SDL_RenderCopy(renderer, texture, nullptr, &rect); 
		}

		void SetScore(int score)
		{
			SDL_FreeSurface(surface);
			SDL_DestroyTexture(texture);

			surface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), {0xFF, 0xFF, 0xFF, 0xFF});
			texture = SDL_CreateTextureFromSurface(renderer, surface);

			int width, height;
			SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
			rect.w = width;
			rect.h = height;
		}
		
		SDL_Renderer* renderer; 
		TTF_Font* font;
		SDL_Surface* surface = {}; 
		SDL_Texture* texture = {}; 
		SDL_Rect rect = {}; 
}; 

Contact CheckPaddleCollision(Ball const& ball, Paddle const& paddle)
{
	// Collision detection is based off of separating axis theorem (SAT)
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + BALL_WIDTH;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + BALL_HEIGHT;

	float paddleLeft = paddle.position.x;
	float paddleRight = paddle.position.x + PADDLE_WIDTH;
	float paddleTop = paddle.position.y;
	float paddleBottom = paddle.position.y + PADDLE_HEIGHT;

	Contact contact{}; 
	if (ballLeft >= paddleRight)
	{
		return contact;
	}

	if (ballRight <= paddleLeft)
	{
		return contact;
	}

	if (ballTop >= paddleBottom)
	{
		return contact;
	}

	if (ballBottom <= paddleTop)
	{
		return contact;
	}

	float paddleRangeUpper = paddleBottom - (2.0f * PADDLE_HEIGHT/3.0f); 
	float paddleRangeMiddle = paddleBottom - (1.0f * PADDLE_HEIGHT/3.0f); 


	if (ball.velocity.x < 0)
	{ 
		contact.penetration = paddleRight - ballLeft; 
	} 
	if (ball.velocity.x > 0)
	{ 
		contact.penetration = paddleLeft - ballRight;
	}

	if ((ballBottom > paddleTop) && (ballBottom < paddleRangeUpper))
	{
		contact.type = CollisionType::Top;
	}
	else if ((ballBottom > paddleRangeUpper) && (ballBottom < paddleRangeMiddle))
	{
		contact.type = CollisionType::Middle;
	}
	else
	{
		contact.type = CollisionType::Bottom;
	}
	return contact;
}
Contact CheckWallCollision(Ball const& ball)
{
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + BALL_WIDTH;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + BALL_HEIGHT;

	Contact contact{};

	if (ballLeft < 0.0f)
	{
		contact.type = CollisionType::Left;
	}
	else if (ballRight > WINDOW_WIDTH)
	{
		contact.type = CollisionType::Right;
	}
	else if (ballTop < 0.0f)
	{
		contact.type = CollisionType::Top;
		contact.penetration = -ballTop;
	}
	else if (ballBottom > WINDOW_HEIGHT)
	{
		contact.type = CollisionType::Bottom;
		contact.penetration = WINDOW_HEIGHT - ballBottom;
	}

	return contact;
}
int main()
{
	// Initialize SDL components
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init(); 

	SDL_Window* window = SDL_CreateWindow("Pong", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	TTF_Font* scoreFont = TTF_OpenFont("dejavu-fonts-ttf-2.37/ttf/DejaVuSansMono.ttf", 40);

	Vec2 p1_score_pos; 
	p1_score_pos.x = static_cast<int>(20); 
	p1_score_pos.y = static_cast<int>(20);
	PlayerScore p1_score(p1_score_pos, renderer, scoreFont); 

	Vec2 p2_score_pos; 
	p2_score_pos.x = static_cast<int>(WINDOW_WIDTH-50); 
	p2_score_pos.y = static_cast<int>(20);
	PlayerScore p2_score(p2_score_pos, renderer, scoreFont); 

	Vec2 ball_position;
	ball_position.x = (WINDOW_WIDTH-BALL_WIDTH)/2.0f; 
	ball_position.y = (WINDOW_HEIGHT-BALL_HEIGHT)/2.0f;
	Ball ball(ball_position, Vec2(BALL_SPEED, 0.0f));

	Vec2 pad1_pos; 
	pad1_pos.x = static_cast<int>(GOAL_SPACE - PADDLE_WIDTH/2.0f); 
	pad1_pos.y = static_cast<int>((WINDOW_HEIGHT - PADDLE_HEIGHT)/2.0f);
	Paddle pad1(pad1_pos, Vec2(0.0f, 0.0f));

	Vec2 pad2_pos; 
	pad2_pos.x = static_cast<int>(WINDOW_WIDTH - GOAL_SPACE - PADDLE_WIDTH/2.0f); 
	pad2_pos.y = static_cast<int>((WINDOW_HEIGHT - PADDLE_HEIGHT)/2.0f); 
	Paddle pad2(pad2_pos, Vec2(0.0f, 0.0f));
	
	// Game logic
	{
		bool running = true;
		float dt = 0.0f; 
		bool buttons[4] = {}; 
		int player1 = 0; 
		int player2 = 0;
		
		// Continue looping and processing events until user exits
		while (running)
		{
			auto startTime = std::chrono::high_resolution_clock::now(); 
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT) running = false;
				else if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
					else if (event.key.keysym.sym == SDLK_w) buttons[Buttons::PadOneUp] = true;					
					else if (event.key.keysym.sym == SDLK_s) buttons[Buttons::PadOneDown] = true;				
					else if (event.key.keysym.sym == SDLK_UP) buttons[Buttons::PadTwoUp] = true;				
					else if (event.key.keysym.sym == SDLK_DOWN) buttons[Buttons::PadTwoDown] = true;
				}
				else if (event.type == SDL_KEYUP)
				{
					if (event.key.keysym.sym == SDLK_w) buttons[Buttons::PadOneUp] = false;					
					else if (event.key.keysym.sym == SDLK_s) buttons[Buttons::PadOneDown] = false;					
					else if (event.key.keysym.sym == SDLK_UP) buttons[Buttons::PadTwoUp] = false;					
					else if (event.key.keysym.sym == SDLK_DOWN) buttons[Buttons::PadTwoDown] = false;
				}
			}

			if (buttons[Buttons::PadOneUp]) pad1.velocity.y = -PADDLE_SPEED;
			else if (buttons[Buttons::PadOneDown]) pad1.velocity.y = PADDLE_SPEED;
			else pad1.velocity.y = 0.0f;

			if (buttons[Buttons::PadTwoUp]) pad2.velocity.y = - PADDLE_SPEED;
			else if (buttons[Buttons::PadTwoDown]) pad2.velocity.y = PADDLE_SPEED;
			else pad2.velocity.y = 0.0f; 


			if (Contact contact = CheckPaddleCollision(ball, pad1); contact.type != CollisionType::None)
			{
				ball.CollideWithPaddle(contact);
			}
			else if (contact = CheckPaddleCollision(ball, pad2); contact.type != CollisionType::None)
			{
				ball.CollideWithPaddle(contact);
			}
			else if (contact = CheckWallCollision(ball); contact.type != CollisionType::None)
			{
				ball.CollideWithWall(contact);
				if (contact.type == CollisionType::Left)
				{ 
					player2++; 
					p2_score.SetScore(player2);
				}
				else if (contact.type == CollisionType::Right)
				{ 
					player1++; 
					p1_score.SetScore(player1);
				}
			}

			pad1.update(dt);
			pad2.update(dt);
			ball.Update(dt);

			// Clear the window to black
			SDL_SetRenderDrawColor(renderer, 0x00, 0x0, 0x0, 0xFF);
			SDL_RenderClear(renderer);

			//
			// Rendering will happen here
			//
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			for (int y = 0; y < WINDOW_HEIGHT; ++y)
			{
				if (y % 10) SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, y);
			}
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x7F, 0xFF, 0xFF);
			ball.Draw(renderer);
			

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			pad1.Draw(renderer);
			pad2.Draw(renderer);

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			p1_score.Draw();
			p2_score.Draw();


			// Present the backbuffer
			SDL_RenderPresent(renderer);


			auto stopTime = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
		}
	}

	// Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(scoreFont);
	TTF_Quit();
	SDL_Quit();
	SDL_Quit();

	return 0;
}