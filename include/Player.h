#pragma once
#include "Paddle.h"

class Player : public Paddle
{
public:
	void Update(float deltaT) override;
};