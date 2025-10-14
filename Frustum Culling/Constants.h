#pragma once
#include<DirectXMath.h>

//Header holding all the constants

constexpr int ENTITY_COUNT = 6000;

constexpr float LEVEL_HEIGHT = 30.0f;
constexpr int LEVEL_WIDTH = 1000;
constexpr int LEVEL_DEPTH = 1000;

constexpr float CAMERA_SPEED = 0.96f;
constexpr float ROTATION_SPEED = 0.096f;

#define FORWARD_KEY 'W'
#define BACKWARD_KEY 'S'
#define LEFT_KEY 'A'
#define RIGHT_KEY 'D'
#define UP_KEY VK_UP
#define DOWN_KEY VK_DOWN

#define ROTATION_LEFT	VK_LEFT
#define ROTATION_RIGHT	VK_RIGHT

constexpr float FOV = 90.0f;
constexpr float WINDOW_WIDTH = 800.0f;
constexpr float WINDOW_HEIGHT = 600.0f;
constexpr float Z_NEAR = 1.0f;
constexpr float Z_FAR = 1000.0f;


const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = { 0, 0, 1 };
const DirectX::XMVECTOR DEFAULT_BACKWARD_VECTOR = { 0, 0, -1 };
const DirectX::XMVECTOR DEFAULT_UP_VECTOR = { 0, 1, 0 };
const DirectX::XMVECTOR DEFAULT_DOWN_VECTOR = { 0, -1, 0 };
const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR = { -1, 0, 0 };
const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = { 1, 0, 0 };