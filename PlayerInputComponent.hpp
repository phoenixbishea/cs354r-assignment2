#ifndef PlayerInputComponent_hpp
#define PlayerInputComponent_hpp

class PlayerInputComponent : public InputComponent
{
public:
	virtual void update(GameObject& obj, OIS::Keyboard* keyboard, World& world)
	{
		bool forward = keyboard->isKeyDown(OIS::KC_W) || 
					   keyboard->isKeyDown(OIS::KC_COMMA) || 
					   keyboard->isKeyDown(OIS::KC_UP);

		bool down = keyboard->isKeyDown (OIS::KC_S) || 
					keyboard->isKeyDown(OIS::KC_O) || 
					keyboard->isKeyDown(OIS::KC_DOWN);

		bool left = keyboard->isKeyDown (OIS::KC_A) || 
					keyboard->isKeyDown(OIS::KC_LEFT);

		bool right = keyboard->isKeyDown(OIS::KC_D) || 
					 keyboard->isKeyDown(OIS::KC_E) || 
					 keyboard->isKeyDown(OIS::KC_RIGHT);

		if (forward)
		{
			// velocity should be a btVector3 that can be handled by bullet
			obj.velocity = obj.orientation * Vector3(0, 0, -WALK_SPEED);
		}
		else if (down)
		{
			obj.velocity = obj.orientation * Vector3(0, 0, WALK_SPEED);
		}
		else if (left)
		{
			Quaternion rotation = Quaternion(0, ROTATION_SPEED * world->elapsedTime, 0);
			obj.orientation = rotation * orientation;
		}
		else if (right)
		{
			Quaternion rotation = Quaternion(0, -ROTATION_SPEED * world->elapsedTime, 0);
			obj.orientation = rotation * orientation;
		}
	}

private:
	const float DEAD_ZONE = 0.1f;
	const float ROTATION_SPEED = 2.0f;
	const float WALK_SPEED = 500.0f;
};

#endif
