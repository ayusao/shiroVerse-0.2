#include<algorithm>

#include "game.h"
#include "spriterenderer.h"
#include "resourceManager.h"
#include "gameobject.h"
#include "playerObject.h"
#include "ballObject.h"
#include "particlegenerator.h"
#include "postprocessor.h"
#include <irr/irrKlang.h>
#include <iostream>
using namespace irrklang;

//game related state data
SpriteRenderer* Renderer;
PlayerObject* shiro;  //the dog in spaceship
PlayerObject* swimShiro;

//for breakout
GameObject* Paddle;
BallObject* Ball;
//PlayerObject* shark, *shark2;  //sharks in the ocean
ParticleGenerator* Particles;
PostProcessor* Effects;
ISoundEngine* SoundEngine = createIrrKlangDevice();
float ShakeTime = 0.0f;
bool waitForEnter = false;
//data related to the homepage, positions of the textures
float x_postion = 520.0f;
float y_postion = 200.0f;
float x_width = 140.0f;
float y_width = 70.0f;
float gap = 120.0f;
std::vector<shark> sharks; //vector to hold shark objects

bool executeOnce = false;

Game::Game(unsigned int width, unsigned int height)
    :State(GAME_MENU), Keys(), Width(width), Height(height), Lives(3) {

}

Game::~Game() {
    delete Renderer;
    delete shiro;
    delete Particles;
    delete Effects;
    delete swimShiro;
    delete Ball;
    delete Paddle;

    sharks.clear();

    SoundEngine->drop();
}

void Game::Init() {
    // load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.frag", nullptr, "particle");
    ResourceManager::LoadShader("shaders/postprocessing.vs", "shaders/postprocessing.frag", nullptr, "postprocessing");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

    // load textures
    ResourceManager::LoadTexture("textures/ball.png", true , "ball");
    ResourceManager::LoadTexture("textures/background.png", true, "background");
    ResourceManager::LoadTexture("textures/ocean.png", true, "ocean");
    ResourceManager::LoadTexture("textures/controls.png", true, "helpmenu");
    ResourceManager::LoadTexture("textures/spaceship.png", true, "face");
    ResourceManager::LoadTexture("textures/block.png", false, "block");
    ResourceManager::LoadTexture("textures/astroids.png", true, "block_solid");
    ResourceManager::LoadTexture("textures/particle.png", true, "particle");
    ResourceManager::LoadTexture("textures/passed.png", true, "passed");
    ResourceManager::LoadTexture("textures/wasted.png", true, "wasted");
    ResourceManager::LoadTexture("textures/swim.png", true, "swim");
    ResourceManager::LoadTexture("textures/lvl1purple.png", true, "lvl1p");
    ResourceManager::LoadTexture("textures/lvl2purple.png", true, "lvl2p");
    ResourceManager::LoadTexture("textures/lvl1blue.png", true, "lvl1b");
    ResourceManager::LoadTexture("textures/lvl2blue.png", true, "lvl2b");
    ResourceManager::LoadTexture("textures/helppurple.png", true, "helpp");
    ResourceManager::LoadTexture("textures/helpblue.png", true, "helpb");
    ResourceManager::LoadTexture("textures/exitpurple.png", true, "exitp");
    ResourceManager::LoadTexture("textures/exitblue.png", true, "exitb");
    ResourceManager::LoadTexture("textures/shark.png", true, "sharkright");
    ResourceManager::LoadTexture("textures/sharkleft.png", true, "sharkleft");

    ResourceManager::LoadTexture("textures/block_solid.png", false, "breaksolid");
    ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("textures/powerup_speed.png", true, "powerup_speed");
    ResourceManager::LoadTexture("textures/powerup_sticky.png", true, "powerup_sticky");
    ResourceManager::LoadTexture("textures/powerup_increase.png", true, "powerup_increase");
    ResourceManager::LoadTexture("textures/powerup_confuse.png", true, "powerup_confuse");
    ResourceManager::LoadTexture("textures/powerup_chaos.png", true, "powerup_chaos");
    ResourceManager::LoadTexture("textures/powerup_passthrough.png", true, "powerup_passthrough");
    //setting shark controls
    Shader spriteShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(spriteShader);
    //// Load shark texture
    Texture2D sharkTextureright = ResourceManager::GetTexture("sharkright");
    Texture2D sharkTextureleft = ResourceManager::GetTexture("sharkleft");


    // set render-specific controls
    Shader theShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(theShader);

    Particles = new ParticleGenerator(
        ResourceManager::GetShader("particle"),
        ResourceManager::GetTexture("particle"),
        500);

    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
    // load levels
    GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.8);
    GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.8);
    GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.8);
    GameLevel four; //might be required for exit button

    GameLevel five; five.Load("levels/breakout.lvl", this->Width, this->Height * 0.8);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Levels.push_back(five);
    this->Level = 0;

    glm::vec2 shiroPos = glm::vec2(this->Width / 2.0f - BALL_RADIUS, this->Height - BALL_RADIUS * 2.0f);
    glm::vec2 sharkPos1(1230.0f, 400.0f);
    glm::vec2 sharkPos2(-400.0f, 300.0f);
    glm::vec2 sharkPos3(1210.0f, 230.0f);
    glm::vec2 sharkPos4(-500.0f, 500.0f);
    glm::vec2 sharkPos5(1200.0f, 600.0f);

    shiro = new PlayerObject(shiroPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
    swimShiro = new PlayerObject(shiroPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("swim"));
    sharks.emplace_back(sharkPos1, sharkRadius, sharkVelocityleft, sharkTextureright);
    sharks.emplace_back(sharkPos2, sharkRadius, sharkVelocityright, sharkTextureleft);
    sharks.emplace_back(sharkPos3, sharkRadius, sharkVelocityleft, sharkTextureright);
    sharks.emplace_back(sharkPos4, sharkRadius, sharkVelocityright, sharkTextureleft);
    sharks.emplace_back(sharkPos5, sharkRadius, sharkVelocityleft, sharkTextureright);

    glm::vec2 paddlePos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Paddle = new GameObject(paddlePos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = paddlePos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));
    //audio
    SoundEngine->play2D("audio/breakout.mp3", true,false);
}

void Game::ProcessInput(float dt) {
    if (this->State == GAME_MENU) {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            if (this->levelSelect == 1)
                this->Level = 4;
            else
                this->Level = this->levelSelect;

            if (this->Level != 2)
                this->State = GAME_ACTIVE;
            else
                this->State = HELP_MENU;
            this->KeysProcessed[GLFW_KEY_ENTER] = true;

        }
        if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
        {
            if (this->levelSelect > 0)
                --this->levelSelect;
            else
                this->levelSelect = 3;


            this->KeysProcessed[GLFW_KEY_W] = true;
        }
        if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
        {
            this->levelSelect = (this->levelSelect + 1) % 4;
            this->KeysProcessed[GLFW_KEY_S] = true;
        }
    }

    if (this->State == GAME_WIN || this->State == GAME_OVER) {
        if (!executeOnce) {
            if (this->State == GAME_OVER)
                SoundEngine->play2D("audio/wasted.wav", false);
            Effects->Confuse = true;
            executeOnce = true;
        }
        if (this->Keys[GLFW_KEY_ENTER]) {
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
            Effects->Chaos = false;
            Effects->Confuse = false;
            this->State = GAME_MENU;
        }
    }
    if (this->State == GAME_ACTIVE)
    {
        if (waitForEnter)
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = false;
            if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
            {
                waitForEnter = false;
                this->KeysProcessed[GLFW_KEY_ENTER] = true;
            }

        }
        else
        {
            float velocity = PLAYER_VELOCITY * dt;
            if (this->Level == 1) {
                for (auto& shark : sharks)
                {
                    shark.Position += shark.Velocity * dt;
                    if (shark.Position.x + shark.Radius < 0.0f) {
                        shark.Position.x = this->Width + shark.Radius; // Wrap to the right edge
                    }
                    if (shark.Position.x - shark.Radius > this->Width) {
                        shark.Position.x = -shark.Radius; // Wrap to the left edge
                    }
                }
            }

            // move playerboard
            if (this->Keys[GLFW_KEY_LEFT])
            {
                if (Paddle->Position.x >= 0.0f) {
                    Paddle->Position.x -= velocity;

                    if (Ball->Stuck)
                        Ball->Position.x -= velocity;
                }
                shiro->Position.x -= 1;
                swimShiro->Position.x -= 1;
            }
            if (this->Keys[GLFW_KEY_RIGHT])
            {
                if (Paddle->Position.x <= this->Width - Paddle->Size.x)
                {
                    Paddle->Position.x += velocity;
                    if (Ball->Stuck)
                        Ball->Position.x += velocity;
                }
                shiro->Position.x += 1;
                swimShiro->Position.x += 1;
            }
            if (this->Keys[GLFW_KEY_SPACE])
                Ball->Stuck = false;
            if (this->Keys[GLFW_KEY_UP]) {
                shiro->Position.y -= 1;
                swimShiro->Position.y -= 1;
            }
            if (this->Keys[GLFW_KEY_DOWN]) {
                shiro->Position.y += 1;
                swimShiro->Position.y += 1;
            }
        }
    }
}
void adjustPosition(PlayerObject* obj, float width, float height) {
    if (obj->Position.y > height) {
        obj->Position.y = height - obj->Radius;
    }
    if (obj->Position.y < 0) {
        obj->Position.y = obj->Radius;
    }
    if (obj->Position.x < 0) {
        obj->Position.x = obj->Radius;
    }
    if (obj->Position.x > width) {
        obj->Position.x = width - obj->Radius;
    }
}
void Game::Update(float dt) {
    //update objects

    if (this->Level == 1) {
        this->DoCollisions();
    }
    else if (this->Level == 0) {
        this->DoCollisions();
        //update particles
        Particles->Update(dt, *shiro, 2, glm::vec2(shiro->Radius / 2.0f));
    }
    else if (this->Level == 4) {
        //update ball object
        Ball->Move(dt, this->Width);
        //checks for collisions
        this->DoCollisions();
        //update particles
        Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));
        //update power ups
        this->UpdatePowerUps(dt);
    }

    //reduce shake time
    if (ShakeTime > 0.0f) {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }
    // Adjust positions for shiro
    adjustPosition(shiro, this->Width, this->Height);
    // Adjust positions for swimShiro
    adjustPosition(swimShiro, this->Width, this->Height);

    //check loss condition
    if (Ball->Position.y >= this->Height) //did the ball reach bottom edge
    {
        --this->Lives;
        if (this->Lives == 0) {
            this->State = GAME_OVER;
            this->ResetLevel();
            this->ResetPlayer();
        }
        this->ResetPlayer();
    }
    //win check
    if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
    {
        if (this->Level == 3) {
            glfwTerminate();    // Terminate GLFW
            exit(0);          // Exit the program
        }
        else if (this->Level == 0)
        {
            this->ResetPlayer();
            this->ResetLevel();
            this->Level++;
        }
        else {
            SoundEngine->play2D("audio/missonPassed.mp3", false);
            this->ResetLevel();
            this->ResetPlayer();
            Effects->Chaos = true;
            this->State = GAME_WIN;

        }
    }

    //help menu
    if (this->State == HELP_MENU)
    {
        waitForEnter = true;
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            waitForEnter = false;
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
            this->State = GAME_MENU;
        }
    }
}
bool CheckSharkCollision(PlayerObject& player, shark& theShark, glm::vec2& sharkPosition);
void HandleSharkCollisionAndRender(Game& game, PlayerObject& swimShiro, shark& theShark, glm::vec2& sharkPosition);
void Game::Render() {
    Texture2D theTexture;
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN || this->State == HELP_MENU|| this->State == GAME_OVER)
    {
        Effects->BeginRender();

        if (this->State == GAME_MENU) {
            theTexture = ResourceManager::GetTexture("background");
            Renderer->DrawSprite(theTexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

            if (this->levelSelect == 0) {
                theTexture = ResourceManager::GetTexture("lvl1b");
                Renderer->DrawSprite(theTexture, glm::vec2(x_postion, y_postion), glm::vec2(x_width, y_width), 0.0f);
            }
            else {
                theTexture = ResourceManager::GetTexture("lvl1p");
                Renderer->DrawSprite(theTexture, glm::vec2(x_postion, y_postion), glm::vec2(x_width, y_width), 0.0f);
            }

            if (this->levelSelect == 1) {
                theTexture = ResourceManager::GetTexture("lvl2b");
                Renderer->DrawSprite(theTexture, glm::vec2(x_postion, y_postion + gap), glm::vec2(x_width, y_width), 0.0f);
            }
            else {
                theTexture = ResourceManager::GetTexture("lvl2p");
                Renderer->DrawSprite(theTexture, glm::vec2(x_postion, y_postion + gap), glm::vec2(x_width, y_width), 0.0f);
            }

            if (this->levelSelect == 2) {
                theTexture = ResourceManager::GetTexture("helpb");
                Renderer->DrawSprite(theTexture, glm::vec2(x_postion, y_postion + gap * 2), glm::vec2(x_width, y_width), 0.0f);
            }
            else {
                theTexture = ResourceManager::GetTexture("helpp");
                Renderer->DrawSprite(theTexture, glm::vec2(x_postion, y_postion + gap * 2), glm::vec2(x_width, y_width), 0.0f);
            }
            if (this->levelSelect == 3) {
                theTexture = ResourceManager::GetTexture("exitb");
                Renderer->DrawSprite(theTexture, glm::vec2(x_postion, y_postion + gap * 3), glm::vec2(x_width, y_width), 0.0f);
            }
            else {
                theTexture = ResourceManager::GetTexture("exitp");
                Renderer->DrawSprite(theTexture, glm::vec2(x_postion, y_postion + gap * 3), glm::vec2(x_width, y_width), 0.0f);
            }
        }
        else if (this->State == HELP_MENU)
        {
            //std::cout << "Help meanu aayoo";
            theTexture = ResourceManager::GetTexture("helpmenu");
            Renderer->DrawSprite(theTexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        }
        else {
            if (this->Level == 1) {
                theTexture = ResourceManager::GetTexture("ocean");
                Renderer->DrawSprite(theTexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
                for (auto& shark : sharks) {
                    // Calculate a new position for rendering the shark again
                    glm::vec2 secondPosition = shark.Position + glm::vec2(300.0f, 0.0f); // Adjust as needed

                    // Handle collision and render for the original shark position
                    HandleSharkCollisionAndRender(*this, *swimShiro, shark, shark.Position);
                    // Handle collision and render for the second shark position
                    HandleSharkCollisionAndRender(*this, *swimShiro, shark, secondPosition);
                }
                swimShiro->Draw(*Renderer);
            }
            else if (this->Level == 4) {
                theTexture = ResourceManager::GetTexture("background");
                Renderer->DrawSprite(theTexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
                Paddle->Draw(*Renderer);
                // draw powerups
                for (PowerUp& powerUp : this->PowerUps)
                    if (!powerUp.Destroyed)
                        powerUp.Draw(*Renderer);
                //draw particles
                Particles->Draw();
                //draw ball
                Ball->Draw(*Renderer);
            }
            else {
                theTexture = ResourceManager::GetTexture("background");
                Renderer->DrawSprite(theTexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
                //draw particles
                Particles->Draw();
                //draw ball
                shiro->Draw(*Renderer);
            }
            // draw level
            this->Levels[this->Level].Draw(*Renderer);
        }
        //end rendering to postprocessig framebuffer
        Effects->EndRender();
        //render postprocessing quad
        Effects->Render(glfwGetTime());

        if (this->State == GAME_WIN) {
            theTexture = ResourceManager::GetTexture("passed");
            Renderer->DrawSprite(theTexture, glm::vec2(this->Width / 4.0f, this->Height / 4.0f), glm::vec2(this->Width / 2.0f, this->Height / 2.0f), 0.0f);
        }

        if (this->State == GAME_OVER) {
            theTexture = ResourceManager::GetTexture("wasted");
            Renderer->DrawSprite(theTexture, glm::vec2(this->Width / 4.0f, this->Height / 4.0f), glm::vec2(this->Width / 2.0f, this->Height / 2.0f), 0.0f);
        }
    }
}

void Game::ResetLevel() {
    if (this->Level == 0)
        this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.8);
    else if (this->Level == 1)
        this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.8);
    else if (this->Level == 4)
        this->Levels[4].Load("levels/breakout.lvl", this->Width, this->Height * 0.8);
    this->Lives = 3;
    executeOnce = false;
}

void Game::ResetPlayer() {
    //reset player/ball states
    Paddle->Size = PLAYER_SIZE;
    Paddle->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Paddle->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
    shiro->Reset(glm::vec2(this->Width / 2.0f - BALL_RADIUS, this->Height - BALL_RADIUS * 2), INITIAL_BALL_VELOCITY);
    swimShiro->Reset(glm::vec2(this->Width / 2.0f - BALL_RADIUS, this->Height - BALL_RADIUS * 2), INITIAL_BALL_VELOCITY);

    // also disable all active powerups
    Effects->Chaos = Effects->Confuse = Effects->Shake = false;
    shiro->PassThrough = shiro->Sticky = false;
    Ball->PassThrough = Ball->Sticky = false;

    Paddle->Color = glm::vec3(1.0f);
    Ball->Color = glm::vec3(1.0f);
    shiro->Color = glm::vec3(1.0f);
    swimShiro->Color = glm::vec3(1.0f);
}

// powerups
bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type);

void Game::UpdatePowerUps(float dt)
{
    for (PowerUp& powerUp : this->PowerUps)
    {
        powerUp.Position += powerUp.Velocity * dt;
        if (powerUp.Activated)
        {
            powerUp.Duration -= dt;

            if (powerUp.Duration <= 0.0f)
            {
                // remove powerup from list (will later be removed)
                powerUp.Activated = false;
                // deactivate effects
                if (powerUp.Type == "sticky")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
                    {	// only reset if no other PowerUp of type sticky is active
                        Ball->Sticky = false;
                        Paddle->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "pass-through")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
                    {	// only reset if no other PowerUp of type pass-through is active
                        Ball->PassThrough = false;
                        Ball->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "confuse")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
                    {	// only reset if no other PowerUp of type confuse is active
                        Effects->Confuse = false;
                    }
                }
                else if (powerUp.Type == "chaos")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
                    {	// only reset if no other PowerUp of type chaos is active
                        Effects->Chaos = false;
                    }
                }
            }
        }
    }
    // Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
    // Note we use a lambda expression to remove each PowerUp which is destroyed and not activated
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUp& powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
    ), this->PowerUps.end());
}

bool ShouldSpawn(unsigned int chance)
{
    unsigned int random = rand() % chance;
    return random == 0;
}

void Game::SpawnPowerUps(GameObject& block)
{
    if (ShouldSpawn(75)) // 1 in 75 chance
        this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
    if (ShouldSpawn(75))
        this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
    if (ShouldSpawn(15)) // Negative powerups should spawn more often
        this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
    if (ShouldSpawn(15))
        this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

void ActivatePowerUp(PowerUp& powerUp)
{
    if (powerUp.Type == "speed")
    {
        Ball->Velocity *= 1.2;
    }
    else if (powerUp.Type == "sticky")
    {
        Ball->Sticky = true;
        Paddle->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp.Type == "pass-through")
    {
        Ball->PassThrough = true;
        Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp.Type == "pad-size-increase")
    {
        Paddle->Size.x += 50;
    }
    else if (powerUp.Type == "confuse")
    {
        if (!Effects->Chaos)
            Effects->Confuse = true; // only activate if chaos wasn't already active
    }
    else if (powerUp.Type == "chaos")
    {
        if (!Effects->Confuse)
            Effects->Chaos = true;
    }
}

bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type)
{
    // Check if another PowerUp of the same type is still active
    // in which case we don't disable its effect (yet)
    for (const PowerUp& powerUp : powerUps)
    {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return true;
    }
    return false;
}

//collision detection
bool CheckCollision(GameObject& one, GameObject& two);
Collision CheckCollision(PlayerObject& one, GameObject& two);
Collision CheckCollision(BallObject& one, GameObject& two);
bool CheckSharkCollision(PlayerObject& player, shark& theShark, glm::vec2& sharkPosition);
Direction VectorDirection(glm::vec2 closest);
void Game::DoCollisions() {
    for (GameObject& box : this->Levels[this->Level].Bricks) {
        if (!box.Destroyed) {
            Collision collision = CheckCollision(*shiro, box);
            if (std::get<0>(collision)) {//if collision is true
                //destroyed block if not solid
                if (!box.IsSolid) {
                    box.Destroyed = true;
                    //this->SpawnPowerUps(box);
                    SoundEngine->play2D("audio/bleep.mp3", false);
                }
                else {
                    //if the ball hits the solid block then we enable the shake effect
                    ShakeTime = 0.0045f;
                   // Effects->Shake = true;
                    //SoundEngine->play2D("audio/solid.wav", false);
                }
                //collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (!(shiro->PassThrough && !box.IsSolid)) {
                    if (dir == LEFT || dir == RIGHT) {//horizontal collision
                        shiro->Velocity.x = -shiro->Velocity.x; //reverse horizontal velocity
                        swimShiro->Velocity.x = -swimShiro->Velocity.x;
                        //relocate
                        float penetration = shiro->Radius - std::abs(diff_vector.x);
                        if (dir == LEFT) {
                            shiro->Position.x += penetration; //move ball to right
                            swimShiro->Position.x += penetration;
                        }
                        else {
                            shiro->Position.x -= penetration;    //move ball to left
                            swimShiro->Position.x -= penetration;
                        }
                    }
                    else //vertical collision
                    {
                        shiro->Velocity.y = -shiro->Velocity.y; //reverse vertical velocity
                        swimShiro->Velocity.y = -swimShiro->Velocity.y;

                        //relocate
                        float penetration = shiro->Radius - std::abs(diff_vector.y);
                        if (dir == UP) {
                            shiro->Position.y -= penetration; //move ball back up
                            swimShiro->Position.y -= penetration;
                        }
                        else {
                            shiro->Position.y += penetration; //move ball back down
                            swimShiro->Position.y += penetration;
                        }
                    }
                }
            }
        }
    }

    for (GameObject& box : this->Levels[this->Level].Bricks) {
        if (!box.Destroyed) {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) {//if collision is true
                //destroyed block if not solid
                if (!box.IsSolid) {
                    box.Destroyed = true;
                    this->SpawnPowerUps(box);
                    SoundEngine->play2D("audio/bleep.mp3", false);
                }
                else {
                    //if the ball hits the solid block then we enable the shake effect
                    ShakeTime = 0.001f;
                    Effects->Shake = true;
                    SoundEngine->play2D("audio/solid.wav", false);
                }

                //collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (!(shiro->PassThrough && !box.IsSolid)) {
                    if (dir == LEFT || dir == RIGHT) {//horizontal collision //reverse horizontal velocity
                        Ball->Velocity.x = -Ball->Velocity.x;

                        //relocate
                        float penetration = shiro->Radius - std::abs(diff_vector.x);
                        if (dir == LEFT) {//move ball to right
                            Ball->Position.x += penetration;
                        }
                        else {
                            //move ball to left
                            Ball->Position.x -= penetration;
                        }
                    }
                    else //vertical collision
                    {
                        //reverse vertical velocity
                        Ball->Velocity.y = -Ball->Velocity.y;

                        //relocate
                        float penetration = shiro->Radius - std::abs(diff_vector.y);
                        if (dir == UP) {//move shiro back up
                            swimShiro->Position.y -= penetration;
                        }
                        else {
                            Ball->Position.y += penetration;
                        }
                    }
                }
            }
        }
    }

    // also check collisions on PowerUps and if so, activate them
    for (PowerUp& powerUp : this->PowerUps)
    {
        if (!powerUp.Destroyed)
        {
            // first check if powerup passed bottom edge, if so: keep as inactive and destroy
            if (powerUp.Position.y >= this->Height)
                powerUp.Destroyed = true;

            if (CheckCollision(*Paddle, powerUp))
            {	// collided with player, now activate powerup
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = true;
                powerUp.Activated = true;
                SoundEngine->play2D("audio/powerup.wav", false);
            }
        }
    }
    //check collitions for player pad (unless stuck)
    Collision result = CheckCollision(*Ball, *Paddle);
    if (!Ball->Stuck && std::get<0>(result)) {
        //check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = Paddle->Position.x + Paddle->Size.x / 2.0f;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Paddle->Size.x / 2.0f);

        //then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);// keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);

        // if Sticky powerup is activated, also stick ball to paddle once new velocity vectors were calculated
        Ball->Stuck = Ball->Sticky;
        SoundEngine->play2D("audio/bleep.wav", false);
    }
}



bool CheckCollision(GameObject& one, GameObject& two) { //AABB-AABB collision
    //collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    //collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    //collision only if on both axes
    return collisionX && collisionY;
}

Collision CheckCollision(PlayerObject& one, GameObject& two) {//AABB - Circle collision
    //get center point circle first
    glm::vec2 center(one.Position + one.Radius);
    //calculate AABB info(center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    //get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    //now that we know the clamped values, add this to AABB_center and we get the value of box closet to circle
    glm::vec2 closest = aabb_center + clamped;
    //now retrieve vector between center of the circle and closest point AABB and check if length<radius
    difference = closest - center;

    if (glm::length(difference) < one.Radius) {//no <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage
        return std::make_tuple(true, VectorDirection(difference), difference);
    }
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

Collision CheckCollision(BallObject& one, GameObject& two) {//AABB - Circle collision
    //get center point circle first
    glm::vec2 center(one.Position + one.Radius);
    //calculate AABB info(center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    //get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    //now that we know the clamped values, add this to AABB_center and we get the value of box closet to circle
    glm::vec2 closest = aabb_center + clamped;
    //now retrieve vector between center of the circle and closest point AABB and check if length<radius
    difference = closest - center;

    if (glm::length(difference) < one.Radius) {//no <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage
        return std::make_tuple(true, VectorDirection(difference), difference);
    }
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

bool CheckSharkCollision(PlayerObject& player, shark& theShark, glm::vec2& sharkPosition) {
    // Get the center point of the player and the shark
    glm::vec2 playerCenter = player.Position + glm::vec2(player.Radius);
    glm::vec2 sharkCenter = sharkPosition + glm::vec2(theShark.Radius);

    // Calculate the distance between centers
    float distance = glm::length(playerCenter - sharkCenter);

    // Compare distance with the sum of radii
    if (distance < player.Radius + theShark.Radius) { //circle-circle collision detection
        return true; // Collided
    }
    else {
        return false; // Not collided
    }
}
void HandleSharkCollisionAndRender(Game& game, PlayerObject& swimShiro, shark& theShark, glm::vec2& sharkPosition)
{
    // Check collision with swimShiro
    if (CheckSharkCollision(swimShiro, theShark, sharkPosition)) {
        // Handle collision with swimShiro and theShark
        --game.Lives;
        if (game.Lives == 0) {
            printf("Lives depleted\n");
            game.ResetLevel();
            game.ResetPlayer();
            game.State = GAME_OVER;
        }
        else {
            swimShiro.Position = glm::vec2(game.Width / 2.0f - swimShiro.Radius, game.Height - swimShiro.Radius * 2.0f);
            game.ResetPlayer();

        }
    }
    //    // Render the shark at the given position
    Renderer->DrawSprite(theShark.Sprite, sharkPosition, theShark.Size, theShark.Rotation);
}

//calculate which direction a vector is facing (N,E,S or W)
Direction VectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f,1.0f), //up
        glm::vec2(1.0f,0.0f), //right
        glm::vec2(0.0f,-1.0f), //down
        glm::vec2(-1.0f,0.0f)  //left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++) {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max) {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

