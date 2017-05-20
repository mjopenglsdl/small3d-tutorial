/*
 *  GameLogic.cpp
 *
 *  Created on: 2014/11/09
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */


#define MAX_Z -1.0f
#define MIN_Z -24.0f
#define MAX_X 24.0f
#define MIN_X -24.0f

#define GROUND_Y -1.0f
#define FULL_ROTATION 6.28f // More or less 360 degrees in radians

#define BUG_TILT_SPEED 0.03f
#define BUG_ROTATION_SPEED 0.06f
#define BUG_SPEED 0.08f
#define BUG_START_ALTITUDE 2.6f

#define GOAT_ROTATION_SPEED 0.1f
#define GOAT_SPEED 0.05f


#include <memory>
#include <small3d/MathFunctions.hpp>
#include <small3d/Exception.hpp>
#include "GameLogic.hpp"


using namespace small3d;

namespace ChaseTheGoat3D {
  
  GameLogic::GameLogic() :
  goat("goat", "resources/models/Goat/goatAnim",
       19, "resources/models/Goat/Goat.png",
       "resources/models/GoatBB/GoatBB.obj"),
  bug("bug", "resources/models/Bug/bugAnim", 9){
    
    renderer = &Renderer::getInstance("Chase the Goat 3D", 0, 0, 1.2f);
    sound = &SoundPlayer::getInstance();
    
    bug.adjustRotation(glm::vec3(0.0f, 1.57f, 0.0f));
    goat.adjustRotation(glm::vec3(0.0f, 1.57f, 0.0f));
    
    Image startScreenTexture("resources/images/startScreen.png");
    
    renderer->generateTexture("startScreen", startScreenTexture.getData(), startScreenTexture.getWidth(),
                             startScreenTexture.getHeight());
    
    Image groundTexture("resources/images/grass.png");
    renderer->generateTexture("ground", groundTexture.getData(), groundTexture.getWidth(), groundTexture.getHeight());
    
    Image skyTexture("resources/images/sky.png");
    renderer->generateTexture("sky", skyTexture.getData(), skyTexture.getWidth(), skyTexture.getHeight());
    
    
    bug.colour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    bug.setFrameDelay(2);
    
    gameState = START_SCREEN;
    
    sound->load("resources/sounds/bah.ogg", "bah");
    
    seconds = 0;
    
    lightModifier = -0.01f;
    
    goatState = WALKING_STRAIGHT;
  }
  
  void GameLogic::initGame() {
    goat.offset = glm::vec3(-1.2f, GROUND_Y, -4.0f);
    bug.offset = glm::vec3(3.6f, GROUND_Y + BUG_START_ALTITUDE, -5.0f);
    bug.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    
    
    bug.startAnimating();
    goat.startAnimating();
    
    
    startSeconds = glfwGetTime();
    
  }
  
  void GameLogic::moveGoat() {
    goatState = TURNING;
    
    float xDistance = bug.offset.x - goat.offset.x;
    float zDistance = bug.offset.z - goat.offset.z;
    float distance = ROUND_2_DECIMAL(sqrt(xDistance * xDistance + zDistance * zDistance));
    
    float goatRelX = ROUND_2_DECIMAL(xDistance / distance);
    float goatRelZ = ROUND_2_DECIMAL(zDistance / distance);
    
    float goatDirectionX = -sin(goat.rotation.y);
    float goatDirectionZ = cos(goat.rotation.y);
    
    float dotPosDir = goatRelX * goatDirectionX + goatRelZ * goatDirectionZ; // dot product
    
    if (dotPosDir > 0.98f) {
      goatState = TURNING;
    } else
      goatState = WALKING_STRAIGHT;
    
    if (goatState == TURNING) {
      goat.rotation.y -= GOAT_ROTATION_SPEED;
      
    }
    
    if (goat.offset.z > MAX_Z) {
      goat.offset.z = MAX_Z;
      goatState = TURNING;
    }
    if (goat.offset.z < MIN_Z) {
      goat.offset.z = MIN_Z;
      goatState = TURNING;
    }
    if (goat.offset.x > MAX_X) {
      goat.offset.x = MAX_X;
      goatState = TURNING;
    }
    if (goat.offset.x < MIN_X) {
      goat.offset.x = MIN_X;
      goatState = TURNING;
    }
    
    goat.offset.x += sin(goat.rotation.y) * GOAT_SPEED;
    goat.offset.z -= cos(goat.rotation.y) * GOAT_SPEED;
    goat.offset.y -= sin(goat.rotation.x) * GOAT_SPEED;
    
    goat.animate();
    
  }
  
  void GameLogic::moveBug(const KeyInput &keyInput) {
    
    if (keyInput.left) {
      bug.rotation.y -= BUG_ROTATION_SPEED;
      
      if (bug.rotation.y < -FULL_ROTATION)
        bug.rotation.y = 0.0f;
      
      
    } else if (keyInput.right) {
      bug.rotation.y += BUG_ROTATION_SPEED;
      
      if (bug.rotation.y > FULL_ROTATION)
        bug.rotation.y = 0.0f;
    }
    
    if (keyInput.down) {
      
      bug.rotation.x -= BUG_TILT_SPEED;
      
      if (bug.rotation.x < -0.75f)
        bug.rotation.x = -0.75f;
      
      
    } else if (keyInput.up) {
      
      bug.rotation.x += BUG_TILT_SPEED;
      
      if (bug.rotation.x > 0.75f)
        bug.rotation.x = 0.75f;
    }
    
    if (keyInput.space) {
      bug.offset.x += sin(bug.rotation.y) * BUG_SPEED;
      bug.offset.z -= cos(bug.rotation.y) * BUG_SPEED;
      bug.offset.y -= sin(bug.rotation.x) * BUG_SPEED;
    }
    
    if (bug.offset.y < GROUND_Y + 0.5f)
      bug.offset.y = GROUND_Y + 0.5f;
    
    if (bug.offset.z > MAX_Z)
      bug.offset.z = MAX_Z;
    if (bug.offset.z < MIN_Z)
      bug.offset.z = MIN_Z;
    if (bug.offset.x > MAX_X)
      bug.offset.x = MAX_X;
    if (bug.offset.x < MIN_X)
      bug.offset.x = MIN_X;
    
    
    // Bug chase camera
    renderer->cameraPosition = bug.offset;
    renderer->cameraPosition.x -= sin(bug.rotation.y) * 1.7f;
    renderer->cameraPosition.z += cos(bug.rotation.y) * 1.7f;
    renderer->cameraPosition.y += sin(bug.rotation.x) * 1.7f;
    renderer->cameraRotation = bug.rotation;
    if (renderer->cameraPosition.y < GROUND_Y + 1.0f)
      renderer->cameraPosition.y = GROUND_Y + 1.0f;
    
    bug.animate();
    
    if (goat.collidesWith(bug.offset)) {
      gameState = START_SCREEN;
      sound->play("bah", "goat");
      seconds = (glfwGetTime() - startSeconds);
    }
  }
  
  void GameLogic::processGame(const KeyInput &keyInput) {
    moveBug(keyInput);
    moveGoat();
  }
  
  void GameLogic::processStartScreen(const KeyInput &keyInput) {
    if (keyInput.enter) {
      initGame();
      gameState = PLAYING;
    }
  }
  
  void GameLogic::process(const KeyInput &keyInput) {
    switch (gameState) {
      case START_SCREEN:
        processStartScreen(keyInput);
        break;
      case PLAYING:
        processGame(keyInput);
        break;
      default:
        throw Exception("Urecognised game state");
    }
  }
  
  void GameLogic::render() {
    renderer->clearScreen();
    
    if (gameState == START_SCREEN) {
      
      renderer->renderTexture("startScreen", glm::vec3(-1.0f, -1.0f, 1.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f));
      
      if (seconds != 0) {
        renderer->write("Goat not bitten for " + intToStr(seconds) + " seconds",
                       glm::vec3(1.0f, 0.5f, 0.0f), glm::vec2(-0.95f, -0.8f), glm::vec2(0.0f, -0.6f));
      }
      
    } else {
      
      renderer->renderTexture("sky", glm::vec3(-1.0f, -1.0f, 1.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f));
      
      // Draw the background
      
      renderer->renderTexture("ground", glm::vec3(-25.0f, GROUND_Y, MAX_Z),
                             glm::vec3(25.0f, GROUND_Y, MIN_Z), true);
      
      renderer->render(goat);
      renderer->render(bug);
      
    }
    renderer->swapBuffers();
  }
  
}
