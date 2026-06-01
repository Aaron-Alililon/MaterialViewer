#include "pch.h"
#include "InputManager.h"

namespace rcore {

	InputManager* InputManager::instance = nullptr;

	InputManager& InputManager::get() {
		if (instance == nullptr) {
			instance = new InputManager();
		}

		return *instance;
	}

	InputManager::InputManager() {
		for (int i = 0; i < 256; i++) {
			m_keys[i] = false;
		}
	}

	void InputManager::keyDown(unsigned int input) {
		m_keys[input] = true;
		return;
	}


	void InputManager::keyUp(unsigned int input) {
		m_keys[input] = false;
		return;
	}


	bool InputManager::isKeyDown(unsigned int key) const {
		return m_keys[key];
	}

}