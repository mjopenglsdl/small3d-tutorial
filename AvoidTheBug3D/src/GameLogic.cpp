/*
 *  GameLogic.cpp
 *
 *  Created on: 2014/11/09
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */


#define MAX_Z -1.0f
#define MIN_Z -24.0f

#define GROUND_Y -1.0f
#define FULL_ROTATION 6.28f // More or less 360 degrees in radians

#define BUG_ROTATION_SPEED 0.12f
#define BUG_DIVE_TILT 0.8f
#define BUG_SPEED 0.08f
#define BUG_DIVE_DURATION 30
#define BUG_START_DIVE_DISTANCE 0.6f
#define BUG_FLIGHT_HEIGHT 1.4f

#define GOAT_ROTATION_SPEED 0.1f
#define GOAT_SPEED 0.05f

#include <memory>
#include <small3d/MathFunctions.hpp>
#include <stdexcept>
#include <cmath>
#include "GameLogic.hpp"


using namespace small3d;

namespace AvoidTheBug3D {
  
  GameLogic::GameLogic() :
    goat("goat", "resources/models/Goat/goatAnim",
	 19, "resources/models/GoatBB/GoatBB.obj"),
    bug("bug", "resources/models/Bug/bugAnim", 9),
    tree("tree", "resources/models/Tree/tree.obj",
	 1, "resources/models/TreeBB/TreeBB.obj"),
    bahSound("resources/sounds/bah.ogg"){
    
    renderer = &Renderer::getInstance("Avoid the Bug 3D", 854, 480);
    renderer->cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    Image goatTexture("resources/models/Goat/goat.png");
    renderer->generateTexture("goatTexture", goatTexture);

    Image treeTexture("resources/models/Tree/tree.png");
    renderer->generateTexture("treeTexture", treeTexture);
    
    Image startScreenTexture("resources/images/startScreen.png");
    renderer->generateTexture("startScreen", startScreenTexture);
    
    bug.colour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    bug.setFrameDelay(2);
    bugVerticalSpeed = ROUND_2_DECIMAL(BUG_FLIGHT_HEIGHT / BUG_DIVE_DURATION);
    
    tree.offset = glm::vec3(2.6f, GROUND_Y, -8.0f);
    tree.rotation = glm::vec3(0.0f, -0.5f, 0.0f);
    
    gameState = START_SCREEN;
    
    startSeconds = 0;
    seconds = 0;
    
    lightModifier = -0.01f;
  }
  
  void GameLogic::initGame() {
    goat.offset = glm::vec3(-1.2f, GROUND_Y, -4.0f);
    bug.offset = glm::vec3(0.5f, GROUND_Y + BUG_FLIGHT_HEIGHT, -18.0f);
    
    bug.startAnimating();
    
    bugState = FLYING_STRAIGHT;
    bugPreviousState = FLYING_STRAIGHT;
    bugFramesInCurrentState = 1;
    
    startSeconds = glfwGetTime();
  }
  
  void GameLogic::moveGoat(const KeyInput &keyInput) {
    goat.stopAnimating();
    
    if (keyInput.left) {
      goat.rotation.y -= GOAT_ROTATION_SPEED;
      
      while (goat.collidesWith(tree)) {
        goat.rotation.y += GOAT_ROTATION_SPEED;
      }
      
      if (goat.rotation.y < -FULL_ROTATION)
        goat.rotation.y = 0.0f;
      goat.startAnimating();
      
    } else if (keyInput.right) {
      goat.rotation.y += GOAT_ROTATION_SPEED;
      while (goat.collidesWith(tree)) {
        goat.rotation.y -= GOAT_ROTATION_SPEED;
      }
      
      
      if (goat.rotation.y > FULL_ROTATION)
        goat.rotation.y = 0.0f;
      goat.startAnimating();
    }
    
    if (keyInput.up) {
      
      goat.offset.x += sin(goat.rotation.y) * GOAT_SPEED;
      goat.offset.z -= cos(goat.rotation.y) * GOAT_SPEED;
      
      while (goat.collidesWith(tree)) {
        goat.offset.x -= sin(goat.rotation.y) * GOAT_SPEED;
        goat.offset.z += cos(goat.rotation.y) * GOAT_SPEED;
      }
      
      goat.startAnimating();
      
    } else if (keyInput.down) {
      goat.offset.x -= sin(goat.rotation.y) * GOAT_SPEED;
      goat.offset.z += cos(goat.rotation.y) * GOAT_SPEED;
      
      while (goat.collidesWith(tree)) {
        goat.offset.x += sin(goat.rotation.y) * GOAT_SPEED;
        goat.offset.z -= cos(goat.rotation.y) * GOAT_SPEED;
      }
      
      goat.startAnimating();
    }
    
    if (goat.offset.z < MIN_Z + 1.0f)
      goat.offset.z = MIN_Z + 1.0f;
    if (goat.offset.z > MAX_Z - 1.0f)
      goat.offset.z = MAX_Z - 1.0f;
    
    if (goat.offset.x < goat.offset.z)
      goat.offset.x = goat.offset.z;
    if (goat.offset.x > -(goat.offset.z))
      goat.offset.x = -(goat.offset.z);
    
    goat.animate();
    
    // Uncomment to see the goat's view of the world
    // renderer->cameraPosition = goat.offset;
    // renderer->cameraPosition.y += 1.0f;
    // renderer->cameraRotation = goat.rotation;
    
  }
  
  void GameLogic::moveBug() {
    
    float xDistance = bug.offset.x - goat.offset.x;
    float zDistance = bug.offset.z - goat.offset.z;
    float distance = ROUND_2_DECIMAL(sqrt(xDistance * xDistance + zDistance * zDistance));
    
    float goatRelX = ROUND_2_DECIMAL(xDistance / distance);
    float goatRelZ = ROUND_2_DECIMAL(zDistance / distance);
    
    float bugDirectionX = -sin(bug.rotation.y);
    float bugDirectionZ = cos(bug.rotation.y);
    
    float dotPosDir = goatRelX * bugDirectionX + goatRelZ * bugDirectionZ; // dot product
    
    // Bug state: decide
    if (bugState == bugPreviousState) {
      ++bugFramesInCurrentState;
    } else {
      bugFramesInCurrentState = 1;
    }
    
    bugPreviousState = bugState;
    
    if (bugState == DIVING_DOWN) {
      if (goat.collidesWith(bug.offset)) {
        bahSound.play();
        seconds = static_cast<int>(glfwGetTime() - startSeconds);
        gameState = START_SCREEN;
      }
      
      if (bugFramesInCurrentState > BUG_DIVE_DURATION / 2) {
        bugState = DIVING_UP;
      }
    } else if (bugState == DIVING_UP) {
      if (goat.collidesWith(bug.offset)) {
        gameState = START_SCREEN;
      }
      
      if (bugFramesInCurrentState > BUG_DIVE_DURATION / 2) {
        bugState = FLYING_STRAIGHT;
        bug.offset.y = GROUND_Y + BUG_FLIGHT_HEIGHT; // Correction of possible rounding errors
      }
    } else {
      
      if (distance > BUG_START_DIVE_DISTANCE) {
        if (dotPosDir < 0.98f) {
          bugState = TURNING;
        } else {
          bugState = FLYING_STRAIGHT;
        }
      } else {
        bugState = DIVING_DOWN;
      }
      
    }
    
    // Bug state: represent
    
    bug.rotation.x = 0;
    
    if (bugState == TURNING) {
      
      bug.rotation.y -= BUG_ROTATION_SPEED;
      
    } else if (bugState == DIVING_DOWN) {
      bug.rotation.x = BUG_DIVE_TILT;
      bug.offset.y -= bugVerticalSpeed;
    } else if (bugState == DIVING_UP) {
      bug.rotation.x = -BUG_DIVE_TILT;
      bug.offset.y += bugVerticalSpeed;
    }
    
    if (bug.rotation.y < -FULL_ROTATION)
      bug.rotation.y = 0.0f;
    
    
    bug.offset.x += sin(bug.rotation.y) * BUG_SPEED;
    bug.offset.z -= cos(bug.rotation.y) * BUG_SPEED;
    
    if (bug.offset.z < MIN_Z)
      bug.offset.z = MIN_Z;
    if (bug.offset.z > MAX_Z)
      bug.offset.z = MAX_Z;
    
    if (bug.offset.x < bug.offset.z)
      bug.offset.x = bug.offset.z;
    if (bug.offset.x > -(bug.offset.z))
      bug.offset.x = -(bug.offset.z);
    
    // Uncomment to see the bug's view of the world
    // renderer->cameraPosition = bug.offset;
    // renderer->cameraRotation = bug.rotation;
    
    bug.animate();
  }
  
  void GameLogic::processGame(const KeyInput &keyInput) {
    moveBug();
    moveGoat(keyInput);
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
      throw std::runtime_error("Urecognised game state");
      break;
    }
  }
  
  void GameLogic::render() {
    renderer->clearScreen();
    
    //Uncomment for groovy nightfall effect :)
    /* renderer->lightIntensity += lightModifier;
     
       if (renderer->lightIntensity < 0)
       {
       renderer->lightIntensity = 0.0f;
       lightModifier = 0.01f;
       }
     
       if (renderer->lightIntensity > 1.0f)
       {
       renderer->lightIntensity = 1.0f;
       lightModifier = -0.01f;
       } */
    
    if (gameState == START_SCREEN) {
      
      renderer->renderTexture("startScreen", glm::vec3(-1.0f, 1.0f, 1.0f),
                              glm::vec3(1.0f, -1.0f, 1.0f));
      
      if (seconds != 0) {
        renderer->write("Goat not bitten for " + intToStr(seconds) + " seconds",
                        glm::vec3(1.0f, 0.5f, 0.0f), glm::vec2(-0.95f, -0.6f), glm::vec2(0.0f, -0.8f));
      }
      
    } else {
      
      // Draw the sky
      renderer->renderSurface(glm::vec3(0.4f, 0.37f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f),
                              glm::vec3(1.0f, -1.0f, 1.0f));
      
      // Draw the ground
      renderer->renderSurface(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-25.0f, GROUND_Y, MIN_Z),
                              glm::vec3(25.0f, GROUND_Y, MAX_Z), true);
      
      renderer->render(goat.getModel(), goat.offset, goat.rotation, "goatTexture");
      renderer->render(bug.getModel(), bug.offset, bug.rotation, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
      renderer->render(tree.getModel(), tree.offset, tree.rotation, "treeTexture");
      
    }
    renderer->swapBuffers();
  }
  
}
